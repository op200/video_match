#include "vm_output.hpp"

#include "vm_log.hpp"
#include "vm_match.hpp"
#include "vm_option.hpp"

#include <format>
#include <fstream>

namespace vm_output {
void vm_output() {
  if (vm_option::output_type == vm_option::output_type_enum::framenum)
    for (fnum i = 0; i < vm_option::frame_count_1; ++i)
      vm_log::output(std::format(
          R"({0}->{1})", i,
          vm_match::match_frame_list[i] == -1
              ? static_cast<int64_t>(-1)
              : static_cast<int64_t>(vm_match::match_frame_list[i])));

  if (!vm_option::log_path.empty()) {
    std::ofstream log_file(vm_option::log_path, std::ios::out);
    if (log_file.is_open()) {
      for (fnum i = 0; i < vm_option::frame_count_1; ++i)
        log_file << i << "->"
                 << (vm_match::match_frame_list[i] == -1
                         ? static_cast<int64_t>(-1)
                         : static_cast<int64_t>(
                               vm_match::match_frame_list[i]))
                 << '\n';
      log_file.close();
    } else
      vm_log::error("unable to open log file");
  }
}
} // namespace vm_output