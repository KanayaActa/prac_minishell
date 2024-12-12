#include "minishell.h"

int main(int ac, char **av, char **envp)
{
    (void)ac;(void)av;
    t_shell shell;
    shell.envp=env_copy(envp);
    shell.last_status=0;
    setup_signals();
    rl_catch_signals = 0;

    shell_loop(&shell);

    // free envp
    for(int i=0;shell.envp[i];i++)xfree(shell.envp[i]);
    xfree(shell.envp);
    return shell.last_status;
}

void shell_loop(t_shell *shell)
{
    char *line;
    while (1)
    {
        line = readline("minishell> ");
        if (!line)
        {
            // ctrl-D
            printf("exit\n");
            break;
        }

        if (g_received_signal == SIGINT) {
            // ここでプロンプト整形
            shell->last_status = 130;
            g_received_signal = 0;
            rl_on_new_line();
            rl_replace_line("", 0);
            rl_redisplay();
            xfree(line);
            continue;
        }

        if (*line)
            add_history(line);
        t_command *cmd = parse_line(shell, line);
        xfree(line);

        if (!cmd) continue; // parse error

        if (cmd->next == NULL && cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
        {
            int old_stdin = dup(0);
            int old_stdout = dup(1);
            if (open_redirs(cmd) < 0) {
                fprintf(stderr, "redirection error\n");
                dup2(old_stdin, 0);
                dup2(old_stdout, 1);
                close(old_stdin);
                close(old_stdout);
                free_command_list(cmd);
                continue;
            }
            shell->last_status = run_builtin(shell, cmd->argv);
            dup2(old_stdin, 0);
            dup2(old_stdout, 1);
            close(old_stdin);
            close(old_stdout);
        }
        else
        {
            shell->last_status = execute_pipeline(shell, cmd);
        }
        free_command_list(cmd);
    }
}
