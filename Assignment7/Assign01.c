#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

int main()
{
	pid_t pid;
	int sig;

	printf("Enter pid:");
	scanf("%d", &pid);

	printf("Enter signal number:");
	scanf("%d", &sig);

	int ret = kill(pid, sig);

	if(ret == -1)
	{
		printf("Kill command failed\n");
		exit(1);
	}
	return 0;
}
