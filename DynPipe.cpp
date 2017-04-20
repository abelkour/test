#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv) {

	cout << "Received " << argc - 1 << " arguments." << endl;
	int pipeA[2];
    int pipeB[2];
    int childpid;
	
	for (int i = 1; i < argc; i++) {

		vector<char> temp_array;
		char *token = strtok(argv[i], " ");
		int j = 0;

	    while (token != NULL) {
	        cout << token << '\n';
	        temp_array.push_back(*token);
	        token = strtok(NULL, " ");
	        j++;
	    }
	    temp_array.push_back(*token); //add on NULL terminator

	    char *args[temp_array.size()];
	    for (int k = 0; k < temp_array.size(); k++) {
	    	args[k] = &temp_array[k];
	    }
	   
	    if (i == 1) { // only move the write end of the pipe
	    	pipe(pipeA);

	    	if ((childpid = fork()) == -1) {
		    	perror("Error creating a child process"); 
				exit(1);
		    }
		    if (childpid == 0) {
		    	dup2(pipeA[1], 1);
		    	close(pipeA[0]);
				close(pipeA[1]); 

				execvp(*args, args);
				exit(0);
		    }
	    } else if (i == argc - 1) {
	    	dup2(pipeB[0], 0);
			close(pipeA[0]); 
			close(pipeA[1]);
			close(pipeB[0]);
			close(pipeB[1]); 
			execvp(*args, args); 
			exit(0);
	    } else {
	    	pipe(pipeA);
	    	pipe(pipeB);

	    	if ((childpid = fork()) == -1) {
		    	perror("Error creating a child process"); 
				exit(1);
		    }
		    if (childpid == 0) {
		    	dup2(pipeA[0], 0); //move pipeA to read end
		    	dup2(pipeB[1], 1); //move pipeB to write end
		    	close(pipeA[0]);   
				close(pipeA[1]); 

				execvp(*args, args);
				exit(0);
		    }
	    }	    
	}

	//move read end of the pipe
	

	return 0;
}