#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>

namespace echo {
	struct Args {
		bool help{false};
		bool newline{true};
		std::string text;
	};

	[[noreturn]] void panic(std::string_view msg) {
		std::string out = "error: ";
		out += msg;
		out += " (";
		out += std::strerror(errno);
		out += ")\n";

		std::ignore = write(2, out.c_str(), out.size());
		_exit(EXIT_FAILURE);
	}

	Args parse_args(int argc, char* argv[]) {
		Args args;

		std::vector<std::string_view> tokens(argv + 1, argv + argc);

		for (size_t i = 0; i < tokens.size(); ++i) {
			const auto t = tokens[i];

			if (t == "-h" || t == "--help") {
				args.help = true;
			} 
			else if (t == "-n") {
				args.newline = false;
			} 
			else if (!t.empty() && t[0] == '-') {
				throw std::runtime_error("unknown flag: " + std::string(t));
			} 
			else {
				if (!args.text.empty()) {
					args.text += ' ';  // fix: preserve spacing
				}
				args.text += t;
			}
		}
		return args;
	}

	void print(std::string_view s) {
		if (write(1, s.data(), s.size()) < 0) {
			panic("write failed");
		}
	}
} // namespace echo

int main(int argc, char* argv[]) {
    using namespace echo;

    try {
        const auto args = parse_args(argc, argv);

        if (args.help) {
            print("Usage: app [-h] [-n] text\n");
            return 0;
        }

		print(args.text);
		if (args.newline) {
			print("\n");
		}
    } catch (const std::exception& e) {
        std::string msg = std::string("error: ") + e.what() + "\n";
		std::ignore = write(2, msg.c_str(), msg.size());
        return 1;
    }
}
