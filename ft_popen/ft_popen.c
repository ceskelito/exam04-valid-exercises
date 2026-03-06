#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>


// pipe, fork, dup2, execvp, close, exit


/**
 *	The function must launch the executable file with the arguments argv (using execvp).
 *	If type is 'r' the function must return a file descriptor connected to the output of the command.
 *	If type is 'w' the function must return a file descriptor connected to the input of the command.
 *	In case of error or invalid parameter the function must return -1.
 */

int	ft_popen(const char *file, char *const argv[], char type) {

	enum { ERR = -1 };
	typedef enum { READ = 0, WRITE = 1} e_pipe_end;

	int			pipefd[2] = {0, 0};
	e_pipe_end	ret_pipe_end;
	e_pipe_end	child_pipe_end;
	int			child_fd;
	pid_t		pid;

	if ( type == 'r' ) {
		ret_pipe_end = READ;
		child_pipe_end = WRITE;
		child_fd = STDOUT_FILENO;
	}
	else if ( type == 'w' ) {
		ret_pipe_end = WRITE;
		child_pipe_end = READ;
		child_fd = STDIN_FILENO;
	}
	else
		return ERR;

	if ( pipe(pipefd) == -1 )
		return ERR;

	pid = fork();
	if ( pid == -1) {
		close(pipefd[READ]);
		close(pipefd[WRITE]);
		return ERR;
	}

	if ( !pid ) { //child
		int dup_exit = dup2(pipefd[child_pipe_end], child_fd);
		close(pipefd[READ]);
		close(pipefd[WRITE]);
		if ( dup_exit == -1 )
			exit(EXIT_FAILURE);
		execvp(file, argv);
		exit(EXIT_FAILURE);
	}
	close(pipefd[child_pipe_end]);
	return pipefd[ret_pipe_end];
}
