#include "vm_log.h"
#include "vm_match.h"
#include "vm_option.h"
#include "vm_output.h"
#include "vm_utils.h"

#include <Windows.h>
#include <chrono>
#include <ranges>
#include <string>

int main(int argc, char *argv[]) {

  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);

  // get options
  std::vector<std::string> args = std::views::counted(argv, argc) |
                                  std::views::transform([](const char *arg) {
                                    return vm_utils::ansi_to_utf8(arg);
                                  }) |
                                  std::ranges::to<std::vector>();

  if (args.size() == 1)
    args.push_back("-h");

  bool *benchmark = nullptr;

  benchmark = &vm_option::benchmark;

  vm_option::get_option(args);

  // benchmark start
  std::chrono::steady_clock::time_point start_time;
  if (*benchmark)
    start_time = std::chrono::high_resolution_clock::now();

  // match
  vm_match::do_match();

  // output
  vm_output::vm_output();

  // benchmark end
  if (*benchmark)
    vm_log::info(std::format(
        "benchmark {0}",
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start_time)));
}