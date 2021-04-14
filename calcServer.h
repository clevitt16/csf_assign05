/*
 * Calculator Server function definitions
 * CSF Assignment 5
 * C. Levitt
 * clevitt1@jh.edu
 * T. Karani
 * tkarani1@jh.edu
*/

#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"


typedef struct {
	int fileDescriptor;
	struct Calc * calc;
} Client;

int chat_with_client(struct Calc *calc, int infd, int outfd);