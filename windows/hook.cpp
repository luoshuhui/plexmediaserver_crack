#include "hook.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <vector>
#include <dbghelp.h>
#include <iostream>

uintptr_t _is_feature_available;

bool get_section_info(std::string_view name, uintptr_t& start, uintptr_t& end)
{
	char filename[MAX_PATH];
	GetModuleFileNameA(NULL, filename, sizeof(filename));

	const auto cur_handle = GetModuleHandleA(filename);
	IMAGE_NT_HEADERS* nt_hdr = ImageNtHeader(cur_handle);
	IMAGE_SECTION_HEADER* section_hdr = reinterpret_cast<IMAGE_SECTION_HEADER*>(nt_hdr + 1);
	const uintptr_t image_base = reinterpret_cast<uintptr_t>(cur_handle);

	for(int i = 0; i < nt_hdr->FileHeader.NumberOfSections; i++, section_hdr++)
	{
		const auto section_header_name = reinterpret_cast<char*>(section_hdr->Name);

		if(name == section_header_name)
		{
			const uintptr_t base_module = image_base + section_hdr->VirtualAddress;
			start = base_module;
			end = base_module + section_hdr->Misc.VirtualSize - 1;

			return true;
		}
	}

	return false;
}

void write_jmp(const uintptr_t from, const uintptr_t to)
{
	uint8_t shellcode[] =
	{
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp [rip+0x06]
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // ?
	};

	*reinterpret_cast<uintptr_t*>(&shellcode[6]) = to;

	DWORD old_prot;
	VirtualProtect(reinterpret_cast<void*>(from), sizeof(shellcode), PAGE_EXECUTE_READWRITE, &old_prot);
	memcpy(reinterpret_cast<void*>(from), shellcode, sizeof(shellcode));
	VirtualProtect(reinterpret_cast<void*>(from), sizeof(shellcode), old_prot, &old_prot);
}

uintptr_t sig_scan(const uintptr_t start, const uintptr_t end, std::string_view pattern)
{
	constexpr const uint16_t WILDCARD = 0xFFFF;
	std::vector<uint16_t> pattern_vec;
	
	for(uintptr_t i = 0; i < pattern.length(); i++)
	{
		if(pattern[i] == ' ')
		{
			continue;
		}

		if(pattern[i] == '?')
		{
			if(pattern[i + 1] == '?')
			{
				i++;
			}

			pattern_vec.push_back(WILDCARD);

			continue;
		}

		pattern_vec.push_back(static_cast<uint16_t>(std::strtol(&pattern[i], nullptr, 16)));
		i++;
	}

	uintptr_t end_address = start;
	MEMORY_BASIC_INFORMATION mbi{};
	
	while(end_address < end && VirtualQuery(reinterpret_cast<void*>(end_address), &mbi, sizeof(mbi)))
	{
		if((mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) == 0 ||
			(mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) > 0)
		{
			break;
		}

		end_address = std::min(end, static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize));
	}

	const auto vec_length = pattern_vec.size();

	for(uintptr_t i = start; i <= end_address - vec_length; i++)
	{
		bool mismatch = false;

		for(uintptr_t x = 0; x < vec_length; x++)
		{
			const auto mem = *reinterpret_cast<uint8_t*>(i + x);

			if(pattern_vec[x] != WILDCARD && mem != pattern_vec[x])
			{
				mismatch = true;

				break;
			}
		}

		if(!mismatch)
		{
			return i;
		}
	}

	return 0;
}

uint64_t hook_is_feature_available([[maybe_unused]] uintptr_t user, [[maybe_unused]] const char* feature)
{
	// `feature` is a GUID. You can use it to enable certain features rather than Godmode (everything); but there's no reason to limit ourselves.. is there?

	return true;
}

void hook()
{
	uintptr_t dottext_start;
	uintptr_t dottext_end;

	if(!get_section_info(".text", dottext_start, dottext_end))
	{
		std::cerr << "[ERR] [plexmediaserver_crack] .text section not found; aborting.\n";

		return;
	}

	_is_feature_available = sig_scan(dottext_start, dottext_end, "41 54 41 56 41 57 48 83 EC 20 4C 8B F9 4C 8B F2");

	if(_is_feature_available == 0)
	{
		std::cerr << "[ERR] [plexmediaserver_crack] Couldn't find is_feature_available; aborting.\n";

		return;
	}

	write_jmp(_is_feature_available, reinterpret_cast<uintptr_t>(&hook_is_feature_available));
}
