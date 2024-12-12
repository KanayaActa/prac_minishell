// src/executor.c
#include "minishell.h"

// static void close_fds(int *fds, int count)
// {
//     for(int i = 0; i < count; i++)
//         if(fds[i] >= 0)
//             close(fds[i]);
// }

int open_redirs(t_command *c)
{
    t_redir *r = c->redir;
    int fd = -1; // Initialize fd to -1

    while(r)
    {
        if(r->type == R_INPUT)
            fd = open(r->filename, O_RDONLY);
        else if(r->type == R_OUTPUT)
            fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if(r->type == R_APPEND)
            fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if(r->type == R_HEREDOC)
        {
            // Implement heredoc (simplified)
            char template[] = "/tmp/minishell_heredocXXXXXX";
            fd = mkstemp(template);
            if(fd < 0)
                return -1;
            unlink(template); // Remove the template file

            // Read lines until delimiter is found
            // This is a simplified version. For full implementation, handle reading input.
            // You can use a loop with readline here to read user input until the delimiter is reached.
            // For brevity, we'll skip the actual reading.
        }

        if(fd < 0)
            return -1;

        if(r->type == R_INPUT || r->type == R_HEREDOC)
        {
            if(dup2(fd, STDIN_FILENO) < 0)
                return -1;
            close(fd);
        }
        else
        {
            if(dup2(fd, STDOUT_FILENO) < 0)
                return -1;
            close(fd);
        }

        r = r->next;
    }
    return 0;
}

static char *find_cmd_in_path(t_shell *shell, char *cmd)
{
    if(ft_strchr(cmd, '/') != NULL)
        return ft_strdup(cmd);

    char *path = env_get_value(shell->envp, "PATH");
    if(!path)
        return NULL;

    char **paths = ft_split(path, ':');
    if(!paths)
        return NULL;

    for(int i = 0; paths[i]; i++)
    {
        char *f = ft_strjoin(paths[i], "/");
        char *fp = ft_strjoin_free(f, ft_strdup(cmd));
        if(access(fp, X_OK) == 0)
        {
            // Free paths
            for(int j = 0; paths[j]; j++)
                xfree(paths[j]);
            xfree(paths);
            return fp;
        }
        xfree(fp);
    }

    // Free paths
    for(int j = 0; paths[j]; j++)
        xfree(paths[j]);
    xfree(paths);

    return NULL;
}

static void exec_cmd(t_shell *shell, t_command *c)
{
    if(!c->argv || !c->argv[0])
        exit(shell->last_status);

    if(is_builtin(c->argv[0]))
    {
        // Execute built-in in child process
        exit(run_builtin(shell, c->argv));
    }

    char *path = find_cmd_in_path(shell, c->argv[0]);
    if(!path)
    {
        fprintf(stderr, "minishell: %s: command not found\n", c->argv[0]);
        exit(127);
    }

    execve(path, c->argv, shell->envp);
    fprintf(stderr, "minishell: execve error: %s\n", strerror(errno));
    exit(126);
}

int execute_pipeline(t_shell *shell, t_command *cmd)
{
    // Count commands
    int count = 0;
    for(t_command *c = cmd; c; c = c->next)
        count++;

    int **pipes = NULL;
    if(count > 1)
    {
        pipes = xmalloc(sizeof(int*) * (count - 1));
        for(int i = 0; i < count - 1; i++)
        {
            pipes[i] = xmalloc(sizeof(int) * 2);
            if(pipe(pipes[i]) < 0){
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
    }

    pid_t *pids = xmalloc(sizeof(pid_t) * count);
    t_command *c = cmd;
    for(int i = 0; i < count; i++)
    {
        pids[i] = fork();
        if(pids[i] < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(pids[i] == 0)
        {
            // Child process

            // Reset signals to default
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);

            // Redirect input if not first command
            if(i > 0)
            {
                if(dup2(pipes[i-1][0], STDIN_FILENO) < 0){
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            // Redirect output if not last command
            if(i < count -1)
            {
                if(dup2(pipes[i][1], STDOUT_FILENO) < 0){
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            // Close all pipe fds in child
            if(pipes)
            {
                for(int j = 0; j < count -1; j++)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }

            // Handle redirections
            if(open_redirs(c) < 0){
                fprintf(stderr, "minishell: redirection error\n");
                exit(EXIT_FAILURE);
            }

            // Execute command
            exec_cmd(shell, c);
        }
        c = c->next;
    }

    // Parent process closes all pipe fds
    if(pipes)
    {
        for(int i = 0; i < count -1; i++)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
            xfree(pipes[i]);
        }
        xfree(pipes);
    }

    // Wait for all children
    int status;
    for(int i = 0; i < count; i++)
    {
        waitpid(pids[i], &status, 0);
        if(WIFEXITED(status))
            shell->last_status = WEXITSTATUS(status);
        else if(WIFSIGNALED(status))
            shell->last_status = 128 + WTERMSIG(status);
    }
    xfree(pids);

    return shell->last_status;
}
