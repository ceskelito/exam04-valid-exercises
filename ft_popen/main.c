#include <stdio.h>
#include <unistd.h>

int	ft_popen(const char *file, char *const argv[], char type);

int	main(void)
{
	int     fd = ft_popen("ls", (char *const []){"ls", "-l", NULL}, 'r');
	dup2(fd, 0);

	close(fd);

	fd = ft_popen("grep", (char *const []){"grep", "popen", NULL}, 'r');

	FILE    *stream = fdopen(fd, "r");
	char    buffer[1024];

	if (!stream)
		return (1);

	while (fgets(buffer, sizeof(buffer), stream))
		printf("%s", buffer);

	fclose(stream);
	close(fd);
	return (0);
}
