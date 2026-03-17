#include <unistd.h>
#include <string.h>
#include "banner.h"
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>

void clear_screen() {
	write(1, "\033[2J\033[H", 7);
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

void print_banner() {
	write(1, banner, strlen(banner));
}

int main() {
	mount_kernel_fs();
	setup_console();


	clear_screen();
	print_banner();
	write(1, "\n>", 2);
	while (1);

}
