/**
 * int    picoshell(char **cmds[]);
 * 
 * The goal of this function is to execute a pipeline. It must execute each
 * commands of cmds and connect the output of one to the input of the
 * next command (just like a shell).
 * e
 * Cmds contains a null-terminated list of valid commands. Each rows
 * of cmds are an argv array directly usable for a call to execvp. The first
 * arguments of each command is the command name or path and can be passed
 * directly as the first argument of execvp.
 * 
 * If any error occur, The function must return 1 (you must of course
 * close all the open fds before). otherwise the function must wait all child
 * processes and return 0. You will find in this directory a file main.c which
 * contain something to help you test your function.
 * 
 * 
 * Examples:
 * ./picoshell /bin/ls "|" /usr/bin/grep picoshell
 * picoshell
 * ./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
 * squblblb
 */

// Allowed functions: close, fork, wait, exit, execvp, dup2, pipe

#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>

int	picoshell(char **cmds[]) {

	typedef int t_fd;
	enum { ERR = 1, NICE = 0 }; 
	enum { READ = 0, WRITE = 1 };

	if ( !cmds || !cmds[0] )
		return NICE;

	pid_t	pid = 0;
	t_fd	pipefd[2];
	t_fd	lastpipe = STDIN_FILENO;
	bool	dup_failure = false;

	for( int i = 0; cmds[i]; i++) {
			
		if ( pipe(pipefd) )
			return ERR;

		pid = fork();

		if ( pid == -1 ) {
			close(pipefd[READ]);
			close(pipefd[WRITE]);
			if ( lastpipe != STDIN_FILENO )
				close(lastpipe);
			return ERR;
		}

		// CHILD
		if ( pid == 0 ) {
			if ( i > 0 ) {
				dup_failure = dup2(lastpipe, STDIN_FILENO) == -1;
			if ( cmds[i + 1] )
				dup_failure = dup2(pipefd[WRITE], STDOUT_FILENO) == -1 || dup_failure;
			close(pipefd[READ]);
			close(pipefd[WRITE]);
			if ( lastpipe != STDIN_FILENO )
				close(lastpipe);
			if (dup_failure)
				exit(EXIT_FAILURE);
			execvp(cmds[i][0], cmds[i]);
			exit(EXIT_FAILURE);
		}

		// PARENT
		close(pipefd[WRITE]);
		if ( i > 0 )
			close(lastpipe);
		if ( cmds[i + 1] )
			lastpipe = pipefd[READ];
		else
			close(pipefd[READ]);
	}
	while( wait(NULL) > 0 );

	return NICE;

}
