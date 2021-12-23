#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int cs531_system(char *comm);

void handler_SIGSTOP(int sigNum)
{
    signal(SIGTSTP, handler_SIGSTOP);
    printf("received signal stop\n");
}
void handler_SIGINT(int sigNum)
{
    printf("received signal\n");
    signal(SIGINT, SIG_DFL);
}
void handler_SIGQUIT(int sigNum)
{
    signal(SIGQUIT, handler_SIGQUIT);
    printf("received signal quit\n");
}
void handler_SIGKILL(int sigNum)
{
    signal(SIGKILL, handler_SIGKILL);
    printf("received signal Kill\n");
}

int cs531_system(char *comm)
{
    signal(SIGINT, handler_SIGINT);
    signal(SIGQUIT, handler_SIGQUIT);
    signal(SIGTSTP, handler_SIGSTOP);
    signal(SIGKILL, handler_SIGKILL);
    //store the characters in to array
    char commandInput[2][200];
    //count the tokens
    int i = 0;
    char *commandChar = strtok(comm, " ");
    while (commandChar)
    {
        strcpy(commandInput[i++], commandChar);
        commandChar = strtok(NULL, " ");
    }
    //create a  child process
    wait(NULL);
    int pid = fork();
    int res;
    int res1;
    if (pid == 0)
    {
        //open the terminal
        int terminal = open("/dev/tty", O_RDWR);
        close(1);
        dup(terminal);
        close(2);
        dup(terminal);
        close(terminal);
        //if no commands was passed as argument
        if (i == 0)
        {
            printf("no arguments passed\n");
        }
        //if command is passed without args
        if (i == 1)
        {
            res = execlp(commandInput[0], commandInput[0], (char *)NULL);
        }
        else
        {
            res = execlp(commandInput[0], commandInput[0], commandInput[1], (char *)NULL);
        }
        if (res == -1)
        {
            printf("failed to execute the command%s\n", commandInput[0]);
        }
    }
    else
    {
        int status;
        pid_t processes = pid;
        int flags = WNOHANG | WUNTRACED | WCONTINUED;
        do
        {
            pid = waitpid(processes, &status, flags);
            if (pid == -1)
            {
                perror("waitpid error: ");
            } 
            else if (pid == 0)
            {
            }
            else
            {
                if (WIFSTOPPED(status))
                {
                    pid = 0;
                    printf("stopped by signal %d\n", WEXITSTATUS(status));
                    res1 = WEXITSTATUS(status);
                }
                else if (WIFCONTINUED(status))
                {
                    pid = 0;
                    printf("continued by signal\n");
                }
                else if (WIFSIGNALED(status))
                {

                    printf("terminated by a signal %d\n", WTERMSIG(status));
                }
                else if (WIFEXITED(status))
                {
                    res1 = WEXITSTATUS(status);
                    printf("Exit status is: %d\n", res1);
                }
                res1 = WEXITSTATUS(status);
            }
        } while (pid == 0);
    }
    return res1;
}

int main(int c, char *argv[])
{
    int status = cs531_system(argv[1]);
    printf("Parent process is sleeping.\n");
    sleep(10);
    return status;
    return 0;
}