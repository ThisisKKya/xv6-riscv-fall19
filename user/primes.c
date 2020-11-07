#include "kernel/types.h"
#include "user/user.h"

void primes()
{
	int baseNum, pointNum;
	int fd[2];
	pipe(fd);
	if (read(0, &baseNum, sizeof(int)) <= 0)
	{
		close(1);
		exit();
	}
	printf("prime %d\n", baseNum);
	if (fork() == 0)
	{
		close(0);
		dup(fd[0]);
		close(fd[0]);
		close(fd[1]);
		primes();
	}
	else
	{
		close(1);
		dup(fd[1]);
		close(fd[0]);
		close(fd[1]);
		while (read(0, &pointNum, sizeof(int)) > 0)
		{
			if (pointNum % baseNum != 0)
			{
				/* code */
				write(1, &pointNum, sizeof(int));
			}
		}
		close(1);
		wait();
		exit();
	}
}
int main()
{
	int i = 2;
	int fd[2];
	pipe(fd);

	if (fork() == 0)
	{
		close(0);
		dup(fd[0]);
		close(fd[0]);
		close(fd[1]);
		primes();
	}
	else
	{
		close(1);
		dup(fd[1]);
		close(fd[0]);
		close(fd[1]);
		for (; i < 36; i++)
		{
			write(1, &i, sizeof(int));
		}
		close(1);
		wait();
	}
	exit();
}