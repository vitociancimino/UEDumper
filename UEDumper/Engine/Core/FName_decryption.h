#pragma once
#include <cstdint>

#include "Memory/memory.h"

/*
 * DO NOT INCLUDE THIS FILE ANYWHERE OR ADD FUNCTIONS HERE
 *
 * This function is used in the EngineCore::FNameToString function and
 * should decrypt the name buffer, as some games encrypt the fnames
 *
 * you shouldnt have to change anything in the params.
 * The function expects you to write the data back into the input buffer.
 * In case your decryption routine somehow writes the decrypted data into a second buffer,
 * just use memcpy to copy it into the input buffer
*/

//you can add here any global variables in case the games decryption function uses any global variables
//you can (or should) mark the global variables as "static"

// example:
// static uint64_t dword_4B64088 = 0;

//use the Memory::read function for reading any memory.
static void fname_decrypt(char* inputBuf, int namelength)
{
	unsigned int v13 = namelength; // er9
	__int64 v14 = 0; // rdx

	if (namelength)
	{
		do
		{
			inputBuf[v14] ^= 0x8Bu;
			v14 = (unsigned int)(v14 + 1);
		} while ((unsigned int)v14 < v13);
	}

	/// Example:
	///	char* v2 = inputBuf; // rdi
	///	int v4 = namelength; // ebx
	///	int v5 = 0;
	///	uint64_t result = 38i64;
	///	if (v4)
	///	{
	///		do
	///		{
	///			uint64_t v7 = v5 | result;
	///			++v2;
	///			++v5;
	///			BYTE v8 = ~(BYTE)v7;
	///			result = (unsigned int)(2 * v7);
	///			*(BYTE*)(v2 - 1) ^= v8;
	///		} while (v5 < v4);
	///	}
	///

	/// Another example
	/// if (dword_4CF88E4 == 0)
	/// 	dword_4CF88E4 = Memory::read<uint64_t>(Memory::getBaseAddress() + 0x4CF88E4);
	/// 
	/// if (dword_4BF84A4 == 0)
	/// 	dword_4BF84A4 = Memory::read<uint64_t>(Memory::getBaseAddress() + 0x4BF84A4);
	/// 
	/// const int v4 = dword_4BF84A4 ^ 0x9C677CC5;
	/// unsigned int v5 = dword_4CF88E4 + v4;
	/// char result = v5 ^ *inputBuf;
	/// *(BYTE*)outputBuff = result;
	/// const char* v7;
	/// __int64 v8;
	/// if (result)
	/// {
	/// 	v7 = &inputBuf[-outputBuff];
	/// 	v8 = -outputBuff;
	/// 	do
	/// 	{
	/// 		v5 += dword_4CF88E4 + v8 + ++outputBuff;
	/// 		result = v5 ^ v7[outputBuff];
	/// 		*(BYTE*)outputBuff = result;
	/// 	} while (result);
	/// }



	//memcpy here if needed, also inputBuf should have the same size as decname otherwise buffer overflow
	//memcpy(inputBuf, decname, sizeof(decname));
}

static void fname_decrypt2(char* inputBuf, int namelength)
{
	unsigned int v10 = namelength; // er9
	__int64 v11 = 0; // rdx

	if (namelength)
	{
		v11 = 0i64;
		do
		{
			inputBuf[v11] ^= 0x8Bu;
			v11 = (unsigned int)(v11 + 2);
		} while ((unsigned int)v11 < v10);
	}

	/// Example:
	///	char* v2 = inputBuf; // rdi
	///	int v4 = namelength; // ebx
	///	int v5 = 0;
	///	uint64_t result = 38i64;
	///	if (v4)
	///	{
	///		do
	///		{
	///			uint64_t v7 = v5 | result;
	///			++v2;
	///			++v5;
	///			BYTE v8 = ~(BYTE)v7;
	///			result = (unsigned int)(2 * v7);
	///			*(BYTE*)(v2 - 1) ^= v8;
	///		} while (v5 < v4);
	///	}
	///

	/// Another example
	/// if (dword_4CF88E4 == 0)
	/// 	dword_4CF88E4 = Memory::read<uint64_t>(Memory::getBaseAddress() + 0x4CF88E4);
	/// 
	/// if (dword_4BF84A4 == 0)
	/// 	dword_4BF84A4 = Memory::read<uint64_t>(Memory::getBaseAddress() + 0x4BF84A4);
	/// 
	/// const int v4 = dword_4BF84A4 ^ 0x9C677CC5;
	/// unsigned int v5 = dword_4CF88E4 + v4;
	/// char result = v5 ^ *inputBuf;
	/// *(BYTE*)outputBuff = result;
	/// const char* v7;
	/// __int64 v8;
	/// if (result)
	/// {
	/// 	v7 = &inputBuf[-outputBuff];
	/// 	v8 = -outputBuff;
	/// 	do
	/// 	{
	/// 		v5 += dword_4CF88E4 + v8 + ++outputBuff;
	/// 		result = v5 ^ v7[outputBuff];
	/// 		*(BYTE*)outputBuff = result;
	/// 	} while (result);
	/// }



	//memcpy here if needed, also inputBuf should have the same size as decname otherwise buffer overflow
	//memcpy(inputBuf, decname, sizeof(decname));
}