#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

#define RL_BUFSIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

char *lsh_read_line(void) {
    int bufsize = RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    while (1) {
        // Read a character
        c = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
      		buffer[position] = c;
   		}
    	position++;

    	// If we have exceeded the buffer, reallocate.
    	if (position >= bufsize) {
     		bufsize += RL_BUFSIZE;
     	 	buffer = realloc(buffer, bufsize);
     	 	if (!buffer) {
        		fprintf(stderr, "lsh: allocation error\n");
        		exit(EXIT_FAILURE);
      		}
    	}
  	}
}

char ** lsh_split_line(char *line) {
  	int bufsize = TOK_BUFSIZE, position = 0;
  	char **tokens = malloc(bufsize * sizeof(char*));
  	char *token;

  	if (!tokens) {
    	fprintf(stderr, "lsh: allocation error\n");
    	exit(EXIT_FAILURE);
  	}

  	token = strtok(line, TOK_DELIM);
  	while (token != NULL) {
    	tokens[position] = token;
    	position++;

    	if (position >= bufsize) {
      		bufsize += TOK_BUFSIZE;
      		tokens = realloc(tokens, bufsize * sizeof(char*));
      		if (!tokens) {
        		fprintf(stderr, "lsh: allocation error\n");
        		exit(EXIT_FAILURE);
      		}
    	}

    	token = strtok(NULL, TOK_DELIM);
  	}
  	tokens[position] = NULL;
  	return tokens;
}
