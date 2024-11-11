#pragma once
#include <windows.h>

#define	OP_HELLO					0x0001
#define	OP_GETPID					0x0002
#define	OP_GETBASE					0x0003
#define	OP_GETSIZE					0x0004
#define	OP_GETPEB					0x0005
#define	OP_READ						0x0006
#define	OP_WHRITE					0x0007
#define	OP_MALLOC					0x0008
#define	OP_FREE						0x0009
#define	OP_CHANGE_MEMORY_PAGE_FLAG  0x0010
#define OP_CREATE_REMOTE_THREAD     0x0011
#define	OP_UNLINK_TAGWND_NODE       0x0012
#define	OP_MOUSE_EVENT				0x0013
//
#define MOUSE_LEFT_BUTTON_DOWN		0x0001  
#define MOUSE_LEFT_BUTTON_UP		0x0002  
#define MOUSE_RIGHT_BUTTON_DOWN		0x0004 
#define MOUSE_RIGHT_BUTTON_UP		0x0008  
#define MOUSE_MIDDLE_BUTTON_DOWN	0x0010  
#define MOUSE_MIDDLE_BUTTON_UP		0x0020

namespace driver {

	inline PVOID control_function;
	inline HANDLE target_process;

	typedef struct _IO {

		byte opCode;
		void* param1;
		void* param2;
		void* param3;
		void* output;

	} IO, * PIO;

	inline uintptr_t call_hook(PIO m)
	{
		if (!control_function)
		{
			HMODULE user32 = LoadLibraryA("user32.dll");
			HMODULE win32u = LoadLibraryA("win32u.dll");

			if (win32u)
				control_function = GetProcAddress(win32u, "NtOpenCompositionSurfaceSectionInfo");
			else
				return 0;
		}

		const auto control = static_cast<uintptr_t(__stdcall*)(PIO)>(control_function);
		return control(m);
	}

	inline void mouseEvent(short x, short y, unsigned short button_flags = 0) {

		IO m{};
		m.opCode = OP_MOUSE_EVENT;
		m.param1 = (void*)x;
		m.param2 = (void*)y;
		m.param3 = (void*)button_flags;
		call_hook(&m);
	}

	inline int moveNodeFromTagWnd(HWND nodeFrom, HWND nodeTo)
	{
		IO m{};

		m.opCode = OP_UNLINK_TAGWND_NODE;
		m.param1 = nodeFrom;
		m.param2 = nodeTo;

		call_hook(&m);
		return (int)m.output;
	}

	inline uintptr_t get_peb() {

		IO m{};

		m.opCode = OP_GETPEB;
		m.param1 = target_process;

		call_hook(&m);
		return (uintptr_t)m.output;
	}

	inline uintptr_t get_module_base_address(const char* module_name) {

		IO m{};

		m.opCode = OP_GETBASE;
		m.param1 = target_process;
		m.param2 = (void*)module_name;

		call_hook(&m);
		return (uintptr_t)m.output;
	}

	inline size_t get_module_size(const char* module_name) {

		IO m{};

		m.opCode = OP_GETSIZE;
		m.param1 = target_process;
		m.param2 = (void*)module_name;

		call_hook(&m);
		return (size_t)m.output;
	}

	inline uintptr_t malloc(size_t size, ULONG protection) {

		IO m{};

		m.opCode = OP_MALLOC;
		m.param1 = target_process;
		m.param2 = (void*)size;
		m.param3 = (void*)protection;

		call_hook(&m);
		return (uintptr_t)m.output;
	}

	inline bool free(ULONG64 address, size_t size) {

		IO m{};

		m.opCode = OP_FREE;
		m.param1 = target_process;
		m.param2 = (void*)address;
		m.param3 = (void*)size;

		call_hook(&m);
		return (bool)m.output;
	}

	inline bool changeMemoryFlag(ULONG64 address, ULONG protection) {

		IO m{};

		m.opCode = OP_CHANGE_MEMORY_PAGE_FLAG;
		m.param1 = target_process;
		m.param2 = (void*)address;
		m.param3 = (void*)protection;

		call_hook(&m);
		return (bool)m.output;
	}

	inline bool createRemoteThread(ULONG64 address) {

		IO m{};
		m.opCode = OP_CREATE_REMOTE_THREAD;
		m.param1 = target_process;
		m.param2 = (void*)address;

		call_hook(&m);
		return (bool)m.output;
	}

	inline void read_size(ULONG64 address, PVOID buffer, size_t size) {

		IO m{};

		m.opCode = OP_READ;
		m.param1 = target_process;
		m.param2 = (void*)address;
		m.param3 = (void*)size;
		m.output = buffer;

		call_hook(&m);
	}

	inline void write_size(ULONG64 address, PVOID buffer, size_t size) {

		IO m{};

		m.opCode = OP_WHRITE;
		m.param1 = target_process;
		m.param2 = (void*)address;
		m.param3 = (void*)size;
		m.output = buffer;

		call_hook(&m);
	}

	template <typename type>
	inline type read(ULONG64 address) {
		type buffer{};

		IO m{};

		m.opCode = OP_READ;
		m.param1 = target_process;
		m.param2 = (void*)address;
		m.param3 = (void*)sizeof(type);
		m.output = &buffer;

		call_hook(&m);
		return buffer;
	}

	inline const char* readString(ULONG64 address, size_t length = 36) {

		char* buffer = new char[length];
		IO m{};
		m.opCode = OP_READ;
		m.param1 = target_process;
		m.param2 = (void*)address;
		m.param3 = (void*)length;
		m.output = buffer;

		call_hook(&m);

		return buffer;
	}

	template <typename type>
	inline void write(ULONG64 address, type value)
	{
		IO m{};

		m.opCode = OP_WHRITE;
		m.param1 = target_process;
		m.param2 = (void*)address;
		m.param3 = (void*)sizeof(type);
		m.output = &value;

		call_hook(&m);
	}

	inline HANDLE attach(const char* process_name)
	{
		IO m{};

		m.opCode = OP_GETPID;
		m.param1 = (void*)(process_name);

		call_hook(&m);
		target_process = (HANDLE)m.output;
		return target_process;
	}

	inline HANDLE process_exists(const char* process_name)
	{
		IO m{};

		m.opCode = OP_GETPID;
		m.param1 = (void*)process_name;

		call_hook(&m);
		return (HANDLE)m.output;
	}

	inline bool ready()
	{
		bool output = true;

		IO m{};
		m.opCode = OP_HELLO;
		call_hook(&m);

		return (int)m.output == 1991;
	}
}