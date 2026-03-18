#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include "banner.h"

void print(char* text){
	write(1, text, strlen(text));
}

void clear_screen() {
	print("\033[2J\033[H");
}

void mount_kernel_fs() {
	mkdir("/proc", 0555);
	mkdir("/sys", 0555);

	mount("proc", "/proc", "proc", 0, 0);
	mount("sysfs", "/sys", "sysfs", 0, 0);
}

void setup_console() {
	int fd = open("/dev/console", O_RDWR);
	if (fd < 0)
		return;
	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);
	if (fd > 2)
		close(fd);
}

void start_shell(){
	int pid = fork();

	if(pid == 0){
		char *argv[] = {"/bin/cell", NULL};
		execve("/bin/cell", argv, NULL);

		print("Failed to start shell");
		while(1);
	}

	while(1){
		wait(NULL);
	}
}

int main() {
	mount_kernel_fs();
	setup_console();

	clear_screen();
	print(banner);
	
	start_shell();

	while (1);
}
