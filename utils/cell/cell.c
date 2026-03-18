#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(){
	//TODO: load config files here!

	char* line;
	char* args;

	do{
		print("> ");
		line = read_input();
		if(strcmp(line, "") != 0){
			print(line);
			print("\n");
		}

	}while(1);

	//cleanup

	return 0;
}
