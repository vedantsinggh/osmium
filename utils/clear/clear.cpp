#include <unistd.h>

int main(void){
	char* command = "\033[2J\033[H";
	write(1, command, sizeof(command));
}
