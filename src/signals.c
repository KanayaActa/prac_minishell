#include "minishell.h"

volatile sig_atomic_t g_received_signal = 0;

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        // とりあえず改行だけだす。rl系関数はここでは呼ばない。
        write(STDERR_FILENO, "\n", 1);
        g_received_signal = SIGINT;
    }
    else if (sig == SIGQUIT)
    {
        // ctrl-\ はbash同様、親プロセス中で特に何もしない
        g_received_signal = SIGQUIT;
    }
}



void setup_signals(void)
{
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGTSTP, SIG_IGN);
}
