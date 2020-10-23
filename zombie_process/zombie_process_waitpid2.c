#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t child_pid;

int wait_pid_check()
{
	int status;

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
		return 0;
	}
	return -1;
}

int main()
{
	child_pid = fork();

	if (child_pid == 0)
	{
		printf("Hello from child\n");
		sleep(2);
		printf("Exit from child\n");
	}
	else
	{
		printf("Hello from parent\n");
		printf("wait for child terminate\n");
		wait_pid_check(); // should be fail
		sleep(5);
		kill(child_pid, 9);
		wait_pid_check();
		while (1)
		{
			sleep(1); // we should see child process becomes zombie
		}
		printf("Exit from parent\n");
	}

	return 0;
}

