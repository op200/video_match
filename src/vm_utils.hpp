#pragma once

#include <string>

namespace vm_utils {

std::string ff_err_to_str(int errorCode);
std::string ansi_to_utf8(const char *ansi_str);

} // namespace vm_utils
