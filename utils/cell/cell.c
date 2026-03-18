#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define TOK_DELIM " \t\r\n\a"

void print(char* text){
	write(1, text, strlen(text));
}

char* read_input(void){
	const int max_buffer_size = 1024;
	int buffer_size = max_buffer_size;
	int position = 0;
	char* buffer = (char* )malloc(sizeof(char) * buffer_size);
	int c;

	if(!buffer) {
		print("PANIK! cant allocate memory!");
		exit(-1);
	}

	while(1){
		c = getchar();

		if(c == EOF || c == '\n'){
			buffer[position] = '\0';
			return buffer;
		}else{
			buffer[position] = c;
		}
		position++;

		if(position >= buffer_size){
			buffer_size += max_buffer_size;
			buffer =  (char*)realloc(buffer, buffer_size);

			if(!buffer) {
				print("PANIK! cant allocate memory!");
				exit(-1);
			}
		}
	}
}

char** split_line(char* line){
	const int max_buffer_size = 64;
	int buffer_size = max_buffer_size;

	char** tokens = malloc(buffer_size * sizeof(char*));
	char* token;
	int position = 0;

	if (!tokens){
		print("PANIK! can't allocate memory!");
		exit(-1);
	}

	token = strtok(line, TOK_DELIM);
	while(token != NULL){
		tokens[position] = token;
		position++;

		if (position >= buffer_size){
			buffer_size += max_buffer_size;
			tokens = realloc(tokens, buffer_size * sizeof(char*));
			if(!token){
				print("PANIK! can't allocate memory! 2");
				exit(-1);
			}
			char** temp = realloc(tokens, buffer_size * sizeof(char*));
			if (!temp){
				print("PANIK! can't allocate memory!");
				exit(-1);
			}
			tokens = temp;
		}

		token = strtok(NULL, TOK_DELIM);
	}

	tokens[position] = NULL;
	return tokens;
}


int launch(char** args){
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0){
		if (execvp(args[0], args) == -1){
			print("PANIK!");
		}
		exit(-1);
	} else if (pid < 0){
		print("PANIK!");
	}else {
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int main(){
	//TODO: load config files here!

	char* line;
	char** args;
	int status;

	do{
		print("> ");
		line = read_input();
		if(strcmp(line, "") != 0){
			args = split_line(line);
			status = launch(args);
		}

	}while(status);

	//cleanup

	return 0;
}
