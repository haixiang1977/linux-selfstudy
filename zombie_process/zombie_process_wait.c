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
                        pid_t pid = wait(&status);
                        if (pid == -1)
                        {
                                printf("error wait");
                        }
                        else
                        {
                                printf("%d waited status %d\n", pid, status);
                        }
                }
                while (1)
                {
                        sleep(1); // we should see child process becomes zombie
                }
                printf("Exit from parent\n");
        }

        return 0;
}
