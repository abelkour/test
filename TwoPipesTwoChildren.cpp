// file: onepipe.cpp
// author: M. Amine Belkoura
// date: 03/04/2015
// purpose: CS3376
// description:
// this program executes "ls -ltr | grep 3376", by dividing the two command among the child and parent process
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
	int status;
	int child1pid;
	int child2pid;

	char *cat_args[] = {"ls", "-ltr", NULL}; 
	char *grep_args[] = {"grep", "3376", NULL};
	char *wc_args[] = {"wc", "-l", NULL};

	// create one pipe to send the output of "ls" process to "grep" process 
	int pipesA[2];
	int pipesB[2];

	pipe(pipesA);
	pipe(pipesB);

	// fork the first child (to execute cat) 
	if((child1pid = fork()) == -1){
		perror("Error creating a child process"); 
		exit(1);
	}
	if (child1pid == 0) {
		// replace cat's stdout with write part of 1st pipe
		dup2(pipesA[1], 1);
		// close all pipes (very important!); end we're using was safely copied
		close(pipesA[0]);
		close(pipesA[1]); 

		execvp(*cat_args, cat_args); 
		exit(0);
	}
	if ((child2pid = fork()) == -1) {
		perror("Error creating a child process"); 
		exit(1);
	}
	if (child2pid == 0) {
		dup2(pipesA[0], 0);
		dup2(pipesB[1], 1);

		
		close(pipesA[0]);
		close(pipesA[1]); 
		close(pipesB[0]);
		close(pipesB[1]); 
		execvp(*grep_args, grep_args);
		exit(0);

	}
	else {
		// replace grep's stdin with read end of 1st pipe 
		dup2(pipesB[0], 0);
		close(pipesA[0]); 
		close(pipesA[1]);
		close(pipesB[0]);
		close(pipesB[1]); 
		execvp(*wc_args, wc_args); 
	}
	return(0); 
}