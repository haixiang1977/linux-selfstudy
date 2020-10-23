#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

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
                while (1)
                {
                        sleep(1); // we should see child process becomes zombie
                }
                printf("Exit from parent\n");
        }

        return 0;
}
