#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

pid_t child_pid;

void sig_handler()
{
	// do nothong here
	return;
}

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

void sleep_check(int sec)
{
	int ret = 0;
	time_t time_start, time_stop;

	time(&time_start);
	ret = sleep(sec);
	time(&time_stop);

	if (ret == 0)
	{
		printf("sleep timeout diff %d\n", (time_stop - time_start));
	}
	else if (errno == EINTR)
	{
		printf("sleep interrupted %d left diff %d\n", ret, (time_stop - time_start));
	}
	else
	{
		printf("sleep unknown %d left\n", ret);
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
		sleep(10);
		printf("Exit from child\n");
	}
	else
	{
		printf("Hello from parent\n");
		printf("1st wait for child exit and should fail\n");
		wait_pid_check();
		sleep_check(1);
		printf("2nd wait for child exit and should fail\n");
		wait_pid_check();
		// try to kill process with signal SIGKILL
		kill(child_pid, 9);
		printf("3rd wait for child exit and maybe succeed\n");
		if (wait_pid_check() == -1)
		{
			sleep_check(5);
			printf("4th wait for child exit and should succeed\n");
			wait_pid_check();
		}
		while (1)
		{
			sleep(1); // we should see child process becomes zombie
		}
		printf("Exit from parent\n");
	}

	return 0;
}

