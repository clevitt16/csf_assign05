#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"

#define LINEBUF_SIZE 1024

int main(int argc, char **argv) {
	/* TODO: implement this program */
	if (argc != 2) {
		printf("Server program needs a single argument specifying port");
		return 1;
	}
	int port = atoi(argv[1]);
	if (port == 0) {
		printf("Argument should be a port number greater than or equal to 1024");
	}

	int listeningFD = open_listenfd(argv[1]);
	struct Calc * calc = calc_create();
	while(1) {
		struct sockaddr_in clientaddr;
		int clientFD = accept(listeningFD, NULL, NULL);
		int chatStatus = chat_with_client(calc, clientFD, clientFD);
		close(clientFD);
		printf("Connection closed by foreign host.");
		if (chatStatus == 1) {
			break;
		}
	}

	return 0;
}



int chat_with_client(struct Calc *calc, int infd, int outfd) {
	rio_t in;
	char linebuf[LINEBUF_SIZE];

	/* wrap standard input (which is file descriptor 0) */
	rio_readinitb(&in, infd);

	/*
	 * Read lines of input, evaluate them as calculator expressions,
	 * and (if evaluation was successful) print the result of each
	 * expression.  Quit when "quit" command is received.
	 */
	int done = 0;
	while (!done) {
		ssize_t n = rio_readlineb(&in, linebuf, LINEBUF_SIZE);
		if (n <= 0) {
			/* error or end of input */
			done = 1;
		} else if (strcmp(linebuf, "quit\n") == 0 || strcmp(linebuf, "quit\r\n") == 0) {
			/* quit command */
			done = 1;
		} else if (strcmp(linebuf, "shutdown\n") == 0 || strcmp(linebuf, "shutdown\r\n") == 0) {
			/* quit command */
			return 1;
		} else {
			/* process input line */
			int result;
			if (calc_eval(calc, linebuf, &result) == 0) {
				/* expression couldn't be evaluated */
				rio_writen(outfd, "Error\n", 6);
			} else {
				/* output result */
				int len = snprintf(linebuf, LINEBUF_SIZE, "%d\n", result);
				if (len < LINEBUF_SIZE) {
					rio_writen(outfd, linebuf, len);
				}
			}
		}
	}
	return 0;
}
