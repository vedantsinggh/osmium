#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <cstring>

constexpr const char* BIN_PATH = "/bin";

void print(const std::string& text) {
	std::ignore = write(STDOUT_FILENO, text.c_str(), text.size());
}

std::string read_input() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

std::vector<std::string> split_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<char*> to_c_args(std::vector<std::string>& args) {
    std::vector<char*> c_args;
    for (auto& arg : args) {
        c_args.push_back(arg.data());
    }
    c_args.push_back(nullptr);
    return c_args;
}

bool binary_exists(const std::string& cmd) {
    DIR* dp = opendir(BIN_PATH);
    if (!dp) return false;

    struct dirent* entry;
    while ((entry = readdir(dp)) != nullptr) {
        if (cmd == entry->d_name) {
            closedir(dp);
            return true;
        }
    }

    closedir(dp);
    return false;
}

int launch(std::vector<std::string>& args) {
    if (args.empty()) return 1;

    if (!binary_exists(args[0])) {
        print("command not found!\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0) {
        // child provess
        auto c_args = to_c_args(args);

        if (execvp(c_args[0], c_args.data()) == -1) {
            perror("exec failed");
            _exit(EXIT_FAILURE);
        }
    } 
    else if (pid < 0) {
        perror("fork failed");
    } 
    else {
        // parent process
        int status;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int main() {
	// TODO: load config files here!

    while (true) {
        print("> ");

        std::string line = read_input();
        if (line.empty()) continue;

        auto args = split_line(line);

        if (args[0] == "exit") break;

        launch(args);
    }

    return 0;
}
