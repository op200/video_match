#include "vm_log.hpp"

#include <Windows.h>
#include <cstdlib>
#include <format>
#include <print>

#include "vm_version.hpp"

namespace vm_log {

void output(const std::string_view &msg) { std::println("{}", msg); }

void change_title(const std::string_view &msg) {
  std::string output = std::format("{0} {1} v{2}", msg, PROGRAM_NAME, VERSION);
  // std::cout << "\033]0;" << output << "\007";
  SetConsoleTitle(output.c_str());
}

void error(const std::string_view &msg) {
  std::println(stderr, "\033[31m[{} ERROR]\033[0m {}", PROGRAM_NAME, msg);
}

void errore(const std::string_view &msg) {
  error(msg);
  std::exit(EXIT_SUCCESS);
}

void warning(const std::string_view &msg) {
  std::println(stderr, "\033[33m[{} WARNING]\033[0m {}", PROGRAM_NAME, msg);
}

void info(const std::string_view &msg) {
  std::println(stderr, "\033[35m[{} INFO]\033[0m {}", PROGRAM_NAME, msg);
}

} // namespace vm_log