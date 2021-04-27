/*
 * Calculator Server implementation
 * CSF Assignment 5
 * C. Levitt
 * clevitt1@jh.edu
 * T. Karani
 * tkarani1@jh.edu
 */

#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"
#include "calcServer.h"
#include <pthread.h>

#define LINEBUF_SIZE 1024
void fatal(char* s);
int main(int argc, char **argv) {
	// command-line input error handling
	if (argc != 2) {
		printf("Server program needs a single argument specifying port\n");
		return 1;
	}
	int port = atoi(argv[1]);
	if (port < 1024) {
		printf("Argument should be a port number greater than or equal to 1024\n");
		return 1;
	} 
	// create server socket and Calc object
	int listeningFD = Open_listenfd(argv[1]);
	struct Calc * calc = calc_create();
	while(1) {    // loop will continue listening and accepting connections until shutdown called
		int clientFD = Accept(listeningFD, NULL, NULL);
		if (clientFD < 0) {
			printf("Error accepting client connection"); //?? what to do on failure?
		}
		Client * clientInfo = malloc(sizeof(Client));
		clientInfo->fd = clientFD;
		clientInfo->calc = calc; 
		pthread_t thr_id;
		if (pthread_create(&thr_id, NULL, worker, clientInfo) != 0) {
			printf("pthread_create failed"); // ??
		}
	}
	calc_destroy(calc);
	return 0;
}


// added functionality for shutdown command
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


void *worker(void *arg) {
	Client * clientInfo = arg;
	pthread_detach(pthread_self());
	//int chatStatus = chat_with_client(clientInfo->calc, clientInfo->fd, clientInfo->fd);
	chat_with_client(clientInfo->calc, clientInfo->fd, clientInfo->fd);
	close(clientInfo->fd);
	printf("Connection closed by foreign host.\n");
	free(clientInfo);
	return NULL;
}

