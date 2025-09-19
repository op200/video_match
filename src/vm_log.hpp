#pragma once

#include <string_view>

namespace vm_log {
void output(const std::string_view &info);

void change_title(const std::string_view &info);

void error(const std::string_view &info);

void errore(const std::string_view &info);

void warning(const std::string_view &info);

void info(const std::string_view &info);
} // namespace vm_log