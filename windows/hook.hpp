#pragma once

#include <cstdint>
#include <string>

bool get_section_info(std::string_view name, uintptr_t& start, uintptr_t& end);
void write_jmp(const uintptr_t from, const uintptr_t to);
uintptr_t sig_scan(const uintptr_t start, const uintptr_t end, std::string_view pattern);
uint64_t hook_is_feature_available(uintptr_t user, const char* feature);
void hook();
