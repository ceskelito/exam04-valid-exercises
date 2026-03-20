#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>

enum { ERR = -1 };

int ft_popen(const char *file, char *const argv[], char type) {

	enum { READ = 0, WRITE = 1 };

	int pipefd[2];
	int cpid = 0;

	int ret_pe = 0; // Pipe End to return
	int child_pe = 0; // Pipe End to dup in the child
	int todup_fd = 0; // STDx_FILENO to overwrite (with dup2) in the child

	if (!file || !argv || (type != 'r' && type != 'w'))
		return ERR;

	if ( pipe(pipefd) == -1 )
		return ERR;

	// Set up the file descriptors.
	if (type == 'r')
	{
		// Child will write in the pipe.
		// Return the READ end of the pipe.
		ret_pe = pipefd[READ];
		child_pe = pipefd[WRITE];
		todup_fd = STDOUT_FILENO;
	}
	else {
		// Child will wait for an input from the pipe.
		// Returns the WRITE pipe's end to the caller.
		//
		// The child will wait for the caller to write in the pipe.
		ret_pe = pipefd[WRITE];
		child_pe = pipefd[READ];
		todup_fd = STDIN_FILENO;
	}

	cpid = fork();
	if ( cpid == -1 )
	{
		close(pipefd[READ]);
		close(pipefd[WRITE]);
		return ERR;
	}
	else if ( cpid == 0 ) { //CHILD
		int dup_exit = dup2(child_pe, todup_fd);
		close(pipefd[READ]);
		close(pipefd[WRITE]);
		if ( dup_exit == -1 )
			exit(EXIT_FAILURE);
		execvp(file, argv);
		exit(EXIT_FAILURE);
	}
	// Only close the file descriptors which we'll not return
	if ( type == 'r' )
		close(pipefd[WRITE]);
	else
		close(pipefd[READ]);
	return ret_pe;
}

// int main(void) {
//     int fd;
//
//     // Writing example
//     fd = ft_popen("cat", (char *const []){"cat", NULL}, 'w');
//     write(fd, "Hello World!\n", 13);
//     close(fd);
//
//     // Reading example
//     fd = ft_popen("ls", (char *const []){"ls", "-l", NULL}, 'r');
//     char buf[1024];
//     ssize_t n;
//     while ((n = read(fd, buf, sizeof(buf))) > 0)
//         write(STDOUT_FILENO, buf, n);
//     close(fd);
//
//     return 0;
// }
