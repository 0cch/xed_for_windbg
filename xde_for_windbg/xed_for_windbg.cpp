// xed_for_windbg.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "XEDParse.h"

#ifdef _WIN64
#pragma comment(lib, "lib/xdeparse/XEDParse64.lib")
#elif _WIN32
#pragma comment(lib, "lib/xdeparse/XEDParse32.lib")
#endif

class EXT_CLASS : public ExtExtension
{
public:
	EXT_COMMAND_METHOD(a64);

};

EXT_DECLARE_GLOBALS();

bool XEDPARSE_CALL UnknownCode(const char* text, ULONGLONG* value)
{
	return false;
}

EXT_COMMAND(a64,
	"Input x64 asm code.",
	"{;ed,r;Address;Begin address}")
{
	ULONG64 base_address = GetUnnamedArgU64(0);
	CHAR buffer[0x1000] = {0};
	ULONG input_length = 0;

	Out("%p  ", base_address);

	while (SUCCEEDED(m_Control4->Input(buffer, 0x1000, &input_length))) {
		if (input_length == 0 || buffer[0] == 0) {
			break;
		}

		CStringA backspace;
		for (ULONG i = 0; i < input_length; i++) {
			backspace.Append("\b");
		}
		Out(backspace.GetString());

		XEDPARSE xed = {0};
		xed.x64 = true;
		xed.cip = base_address;
		xed.cbUnknown = UnknownCode;
		strcpy_s(xed.instr, buffer);

		if (XEDParseAssemble(&xed) != XEDPARSE_OK) {
			Err("Cannot assemble this code.\r\n");
			break;
		}

		Out("%s = ", buffer);
		
		for (unsigned int i = 0; i < xed.dest_size; i++) {
			Out("%02x ", xed.dest[i]);
		}

		Out("\r\n\r\n");

		ULONG tmp = 0;
		m_Data->WriteVirtualUncached(base_address, xed.dest, xed.dest_size, &tmp);
		if (tmp != xed.dest_size) {
			Err("Cannot write to the memory space.\r\n");
			break;
		}

		base_address += xed.dest_size;
		Out("%p  ", base_address);
		
		buffer[0] = 0;
		input_length = 0;
	}
}