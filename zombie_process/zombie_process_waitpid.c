#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
        pid_t child_pid;

        child_pid = fork();

        if (child_pid == 0)
        {
                printf("Hello from child\n");
                sleep(5);
                printf("Exit from child\n");
        }
        else
        {
                printf("Hello from parent\n");
                printf("wait for child terminate\n");
                {
                        int status;

                        // should block here for ~ 5 seconds
                        while (1)
                        {
                                pid_t pid = waitpid(child_pid, &status, WNOHANG);
                                if (pid == -1)
                                {
                                        printf("error waitpid\n");
                                }
                                else if (pid == 0)
                                {
                                        printf("child state not change\n");
                                }
                                else
                                {
                                        printf("%d waited status %d\n", pid, status);
                                }
                                if (pid == child_pid)
                                {
                                        printf("child exited\n");
                                        break;
                                }
                                sleep(1);
                        }
                }
                while (1)
                {
                        sleep(1); // we should see not child process becomes zombie because waitpid reap it
                }
                printf("Exit from parent\n");
        }

        return 0;
}
