#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t child_pid;

void sig_handler()
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
	}
	return;
}

int main()
{
	signal (SIGCHLD, sig_handler);

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
		printf("wait for child exit\n");
		while (1)
		{
			sleep(1); // we should see child process becomes zombie
		}
		printf("Exit from parent\n");
	}

	return 0;
}
