#ifndef __HVREADWRITE_H_
#define __HVREADWRITE_H_

#pragma once
#include <xtl.h>

typedef unsigned __int64 QWORD;

NTSYSAPI
PVOID
NTAPI
MmGetPhysicalAddress(
	IN		PVOID Address
);

DWORD magic = 0x72627472;
__int64 addr;

__int64 __declspec(naked) HvxGetVersion(DWORD key, __int64 type, __int64 SourceAddress, __int64 DestAddress, __int64 lenInBytes)
{
	__asm
	{
		li r0, 0x0
		sc
		blr
	}
}

BYTE* HvxPeekBytes(ULONGLONG Address, DWORD length) {
	BYTE * data = (BYTE*)XPhysicalAlloc(length, MAXULONG_PTR, NULL, PAGE_READWRITE); //make sure to free this memory when you are done with it!
	addr = 0x8000000000000000ULL + (DWORD)MmGetPhysicalAddress((DWORD*)data);
	HvxGetVersion(magic, 5, Address, addr, length);
	return data;
}

void HvxPokeBytes(ULONGLONG Address, BYTE *Data, DWORD length)
{
	__int64 src = 0x8000000000000000ULL + (DWORD)MmGetPhysicalAddress(Data);
	HvxGetVersion(magic, 5, src, Address, length);
}

void HvxPokeDWORD(ULONGLONG Address, DWORD dwDword)
{
	DWORD dwData = dwDword;
	HvxPokeBytes(Address, (PBYTE)&dwData, 4);
}

void HvxPokeQWORD(ULONGLONG Address, QWORD qwDword)
{
	QWORD dwData = qwDword;
	HvxPokeBytes(Address, (PBYTE)&dwData, 8);
}

DWORD HvxPeekDWORD(ULONGLONG Address)
{
	DWORD dwData;
	BYTE * data = HvxPeekBytes(Address, 4);
	memcpy(&dwData, data, 4);
	XPhysicalFree(data);
	return dwData;
}

QWORD HvxPeekQWORD(ULONGLONG Address)
{
	QWORD qwData;
	BYTE * data = HvxPeekBytes(Address, 8);
	memcpy(&qwData, data, 8);
	XPhysicalFree(data);
	return qwData;
}
#endif
