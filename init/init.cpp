#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <cerrno>
#include <cstring>
#include <string_view>
#include <fstream>
#include <string>

#include "banner.h"

namespace osmium {
	void log(std::string_view msg) noexcept {
		std::cout << msg;
	}

	[[noreturn]] void panic(const std::string_view msg) noexcept {
		std::cerr << "[PANIC] " << msg << " (" << std::strerror(errno) << ")\n";
		_exit(EXIT_FAILURE);
	}

	void clear_screen() noexcept {
		log("\033[2J\033[H");
	}

	void safe_mkdir(const char* path, mode_t mode){
		if (mkdir(path, mode) == -1 && errno != EEXIST){
			panic(std::string("mkdir failed") + path);
		}
	}

	void safe_mount(const char* source, const char* target, const char* fs_type, unsigned long flags = 0) {
		if (mount(source, target, fs_type, flags, nullptr) == -1) {
			panic(std::string("mount failed: ") + target);
		}
	}

	void mount_kernel_fs() {
		safe_mkdir("/proc", 0555);
		safe_mkdir("/sys", 0555);

		safe_mount("proc", "/proc", "proc", 0);
		safe_mount("sysfs", "/sys", "sysfs", 0);

		struct stat st{};
		if (stat("/proc/version", &st) != 0) {
			panic("proc not mounted correctly");
		}
	}

	void log_info(const std::string param) {
		std::ifstream file("/proc/" + param);
		if (!file) {
			panic("failed to open /proc/");
		}

		std::string line;
		std::getline(file, line);

		if (line.empty()) {
			panic("/proc/ is empty");
		}

		log("[KERNEL] " + line + "\n");
	}

	void log_all_info() noexcept {
		log_info("meminfo");
		log_info("cpuinfo");
		log_info("version");
		log_info("uptime");
		log_info("mounts");
	}

	void setup_console() {
		int fd = open("/dev/console", O_RDWR);
		if (fd < 0){
			panic("failed to open /dev/console");
		}

		for (int i = 0; i < 3; ++i) {
			if (dup2(fd, i) < 0) {
				panic("dup2 failed");
			}
		}

		if (fd > 2){
			close(fd);
		}
	}

	[[noreturn]] void start_shell(){
		constexpr char shell_path[] = "/bin/cell";
		char* const argv[] = {const_cast<char*>(shell_path), nullptr};
		
		while(true){
			pid_t pid = fork();

			if (pid < 0){
				panic("fork failed!");
			}

			if (pid == 0){
				execve(shell_path, argv, nullptr);
				panic("execve failed");
			}

			int status = 0;
			if(waitpid(pid, &status,0 ) < 0){
				panic("waitpid failed");
			}

			log("shell exited. Restarting..");
		}
	}
} // osmium

int main(void) {
	using namespace osmium;

	mount_kernel_fs();
	setup_console();

	clear_screen();
	log(banner);
	log_all_info();
	
	start_shell();
}
