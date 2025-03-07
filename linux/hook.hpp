#pragma once

#include <cstdint>
#include <string>

bool get_dottext_info(uintptr_t& start, uintptr_t& end);
void write_jmp(const uintptr_t from, const uintptr_t to);
uintptr_t sig_scan(const uintptr_t start, const uintptr_t end, std::string_view pattern);
uintptr_t follow_call_rel32(const uintptr_t address);
uint64_t hook_is_feature_available(uintptr_t user, const char* feature);
void hook();
