#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	pid_t	pid;
	int	pfd[2];
	char s[100];
	size_t rd;

	if(pipe(pfd) == -1)
		return (perror("pipe"), 1);
	//dup2(pfd[0],STDIN_FILENO);
	pid = fork();
	if (pid == -1)
		return(perror("fork"), 1);
	if (pid != 0)
		write(pfd[1], "testing\n", 8);
	if (pid == 0)
	{
		rd = read(pfd[0], s, 99);
		if (rd > 0)
			s[rd] = 0;
		
		printf("%s", s);
	}
	return (0);
}
