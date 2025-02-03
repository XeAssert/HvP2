// HvPatcher.cpp : Defines the entry point for the application.
//
// Made by Xx jAmes t xX
// Patches resolves of 'xbdm.xex' to 'HvP2.xex' so you don't get unresolved links when trying to load debug builds

#include "stdafx.h"
#include "Detour.h"
#include "HvReadWrite.h"


// to remove debug prints about game loading error codes
#define DBGSTR //DbgPrint


// some vars for unloading/unhooking
DWORD dwUnloadXEXState;
BOOL fMessageBoxThread;
BOOL fIsReadingMessageBox;
BOOL fIsLoadingDebugBuild;
BOOL fIsOrignalXboxGame;

// our module handle
HANDLE hModule;

// kernel detours
Detour<int> XexpCompleteLoadDetour;
Detour<int> XexpResolveImageImportsDetour;
Detour<int> MmAllocatePhysicalMemoryExDetour;

// XexMenu 1.1 / 1.2 detours
Detour<int> XexMenuGetFilesDetour;
Detour<int> XexMenuStrStrDetour;


#pragma region Exports

int ExportSub0() { DBGSTR("[HvP2] Called 0\n"); return 0x002da0000; }
// DmAllocatePool
int ExportSub1(int Size) { DBGSTR("[HvP2] Called DmAllocatePool\n"); return (int)ExAllocatePoolWithTag(Size, 'xbdm'); }
int ExportSub2() { DBGSTR("[HvP2] Called 2\n"); return 0x002da0000; }
int ExportSub3() { DBGSTR("[HvP2] Called 3\n"); return 0x002da0000; }
int ExportSub4() { DBGSTR("[HvP2] Called 4\n"); return 0x002da0000; }
int ExportSub5() { DBGSTR("[HvP2] Called 5\n"); return 0x002da0000; }
int ExportSub6() { DBGSTR("[HvP2] Called 6\n"); return 0x002da0000; }
int ExportSub7() { DBGSTR("[HvP2] Called 7\n"); return 0x002da0000; }
int ExportSub8() { DBGSTR("[HvP2] Called 8\n"); return 0x002da0000; }
void ExportSub9(PVOID ptr) { DBGSTR("[HvP2] Called DmFreePool\n"); ExFreePool(ptr); }
int ExportSub10() { DBGSTR("[HvP2] Called 10\n"); return 0x002da0000; }
int ExportSub11() { DBGSTR("[HvP2] Called 11\n"); return 0x002da0000; }
int ExportSub12() { DBGSTR("[HvP2] Called 12\n"); return 0x002da0000; }
int ExportSub13() { DBGSTR("[HvP2] Called 13\n"); return 0x002da0000; }
int ExportSub14() { DBGSTR("[HvP2] Called 14\n"); return 0x002da0000; }
int ExportSub15() { DBGSTR("[HvP2] Called 15\n"); return 0x002da0000; }

typedef struct _DM_XBE {
	char LaunchPath[MAX_PATH + 1];
	DWORD TimeStamp;
	DWORD CheckSum;
	DWORD StackSize;
} DM_XBE, *PDM_XBE;

int ExportSub16(LPCSTR szName, PDM_XBE pdxbe) {
	DBGSTR("[HvP2] Called DmGetXbeInfo\n"); 

	//return ((int(*)(...))0x91007B70)(szName, pdxbe);



	PLDR_DATA_TABLE_ENTRY hModule;

	XexPcToFileHeader((PVOID)0x82000000, &hModule);

	char szBuffer[MAX_PATH];
	WideCharToMultiByte(CP_UTF8, NULL, (wchar_t*)hModule->FullDllName.Buffer, -1, (char*)szBuffer, MAX_PATH, NULL, NULL);

	for (int i = strlen(szBuffer) - 1; i > 0; i--)
	{
		if (szBuffer[i] == '\\')
		{
			szBuffer[i] = 0;
			break;
		}
	}

	strcpy(pdxbe->LaunchPath, szBuffer);

	pdxbe->CheckSum = 0;
	pdxbe->StackSize = 0;
	pdxbe->TimeStamp = 0;

	return 0x02DA0000;

}
int ExportSub17(char* szName, int ccName) {
	DBGSTR("[HvP2] Called DmGetXboxName\n");

	strcpy_s(szName, ccName, "James & Chr0m3");

	return 0x02DA0000;
}
int ExportSub18() { DBGSTR("[HvP2] Called 18\n"); return 0; }
int ExportSub19() { DBGSTR("[HvP2] Called 19\n"); return 0; }
int ExportSub20() { DBGSTR("[HvP2] Called 20\n"); return 0; }
// DmIsDebuggerPresent
int ExportSub21() 
{
	//DBGSTR("[HvP2] Called DmIsDebuggerPresent\n");
	return 0x2DA0000;
}
int ExportSub22() { DBGSTR("[HvP2] Called 22\n"); return 0; }
int ExportSub23() { DBGSTR("[HvP2] Called 23\n"); return 0; }
int ExportSub24() { DBGSTR("[HvP2] Called 24\n"); return 0; }
int ExportSub25() { DBGSTR("[HvP2] Called 25\n"); return 0; }
int ExportSub26() { DBGSTR("[HvP2] Called 26\n"); return 0; }
int ExportSub27() { DBGSTR("[HvP2] Called 27\n"); return 0; }
int ExportSub28() { DBGSTR("[HvP2] Called 28\n"); return 0; }
int ExportSub29() { DBGSTR("[HvP2] Called 29\n"); return 0; }
int ExportSub30() { DBGSTR("[HvP2] Called DmRegisterCommandProcessor\n"); return 0x2DA0000; }
int ExportSub31() { DBGSTR("[HvP2] Called 31\n"); return 0; }
int ExportSub32() { DBGSTR("[HvP2] Called 32\n"); return 0; }
int ExportSub33() { DBGSTR("[HvP2] Called 33\n"); return 0; }
int ExportSub34() { DBGSTR("[HvP2] Called 34\n"); return 0; }
int ExportSub35() { DBGSTR("[HvP2] Called 35\n"); return 0; }
int ExportSub36() { DBGSTR("[HvP2] Called 36\n"); return 0; }
int ExportSub37() { DBGSTR("[HvP2] Called 37\n"); return 0; }
int ExportSub38() { DBGSTR("[HvP2] Called 38\n"); return 0; }
int ExportSub39() { DBGSTR("[HvP2] Called 39\n"); return 0; }
int ExportSub40() { DBGSTR("[HvP2] Called 40\n"); return 0; }
int ExportSub41() { DBGSTR("[HvP2] Called 41\n"); return 0; }
int ExportSub42() { DBGSTR("[HvP2] Called 42\n"); return 0; }
int ExportSub43() { DBGSTR("[HvP2] Called 43\n"); return 0; }
int ExportSub44() { DBGSTR("[HvP2] Called 44\n"); return 0; }
int ExportSub45() { DBGSTR("[HvP2] Called 45\n"); return 0; }
int ExportSub46() { DBGSTR("[HvP2] Called 46\n"); return 0; }
int ExportSub47() { DBGSTR("[HvP2] Called 47\n"); return 0; }
int ExportSub48() { DBGSTR("[HvP2] Called 48\n"); return 0; }
int ExportSub49() { DBGSTR("[HvP2] Called 49\n"); return 0; }
int ExportSub50() { DBGSTR("[HvP2] Called 50\n"); return 0; }
int ExportSub51() { DBGSTR("[HvP2] Called 51\n"); return 0; }
int ExportSub52() { DBGSTR("[HvP2] Called 52\n"); return 0; }
int ExportSub53() { DBGSTR("[HvP2] Called 53\n"); return 0; }
int ExportSub54() { DBGSTR("[HvP2] Called 54\n"); return 0; }
int ExportSub55() { DBGSTR("[HvP2] Called 55\n"); return 0; }
int ExportSub56() { DBGSTR("[HvP2] Called 56\n"); return 0; }
int ExportSub57() { DBGSTR("[HvP2] Called 57\n"); return 0; }
int ExportSub58() { DBGSTR("[HvP2] Called 58\n"); return 0; }
int ExportSub59() { DBGSTR("[HvP2] Called 59\n"); return 0; }
int ExportSub60() { DBGSTR("[HvP2] Called 60\n"); return 0; }
int ExportSub61() { DBGSTR("[HvP2] Called 61\n"); return 0; }
int ExportSub62() { DBGSTR("[HvP2] Called 62\n"); return 0; }
int ExportSub63() { DBGSTR("[HvP2] Called 63\n"); return 0; }
int ExportSub64() { DBGSTR("[HvP2] Called 64\n"); return 0; }
int ExportSub65() { DBGSTR("[HvP2] Called 65\n"); return 0; }
int ExportSub66() { DBGSTR("[HvP2] Called 66\n"); return 0; }
int ExportSub67() { DBGSTR("[HvP2] Called 67\n"); return 0; }
int ExportSub68() { DBGSTR("[HvP2] Called 68\n"); return 0; }
int ExportSub69() { DBGSTR("[HvP2] Called 69\n"); return 0; }
int ExportSub70() { DBGSTR("[HvP2] Called 70\n"); return 0; }
int ExportSub71() { DBGSTR("[HvP2] Called 71\n"); return 0; }
int ExportSub72() { DBGSTR("[HvP2] Called 72\n"); return 0; }
int ExportSub73() { DBGSTR("[HvP2] Called 73\n"); return 0; }
int ExportSub74() { DBGSTR("[HvP2] Called 74\n"); return 0; }
int ExportSub75() { DBGSTR("[HvP2] Called 75\n"); return 0; }
int ExportSub76() { DBGSTR("[HvP2] Called 76\n"); return 0; }
int ExportSub77() { DBGSTR("[HvP2] Called 77\n"); return 0; }
int ExportSub78() { DBGSTR("[HvP2] Called 78\n"); return 0; }
int ExportSub79() { DBGSTR("[HvP2] Called 79\n"); return 0; }
int ExportSub80() { DBGSTR("[HvP2] Called 80\n"); return 0; }
int ExportSub81() { DBGSTR("[HvP2] Called 81\n"); return 0; }
int ExportSub82() { DBGSTR("[HvP2] Called DmCaptureStackBackTrace\n"); return 0x2DA0000; }// DmCaptureStackBackTrace return XBDM_NOERR
int ExportSub83() { DBGSTR("[HvP2] Called 83\n"); return 0; }
int ExportSub84() { DBGSTR("[HvP2] Called 84\n"); return 0; }
int ExportSub85() { DBGSTR("[HvP2] Called 85\n"); return 0; }
int ExportSub86() { DBGSTR("[HvP2] Called 86\n"); return 0; }
int ExportSub87() { DBGSTR("[HvP2] Called 87\n"); return 0; }
int ExportSub88() { DBGSTR("[HvP2] Called 88\n"); return 0; }
int ExportSub89() { DBGSTR("[HvP2] Called 89\n"); return 0; }
int ExportSub90() { DBGSTR("[HvP2] Called 90\n"); return 0; }
int ExportSub91() { DBGSTR("[HvP2] Called 91\n"); return 0; }
int ExportSub92() { DBGSTR("[HvP2] Called 92\n"); return 0; }
int ExportSub93() { DBGSTR("[HvP2] Called 93\n"); return 0; }
int ExportSub94() { DBGSTR("[HvP2] Called 94\n"); return 0; }
int ExportSub95() { DBGSTR("[HvP2] Called 95\n"); return 0; }
int ExportSub96() { DBGSTR("[HvP2] Called 96\n"); return 0; }
int ExportSub97() { DBGSTR("[HvP2] Called 97\n"); return 0; }
int ExportSub98() { DBGSTR("[HvP2] Called 98\n"); return 0; }
int ExportSub99() { DBGSTR("[HvP2] Called 99\n"); return 0; }
int ExportSub100() { DBGSTR("[HvP2] Called 100\n"); return 0; }
int ExportSub101() { DBGSTR("[HvP2] Called 101\n"); return 0; }
int ExportSub102() { DBGSTR("[HvP2] Called 102\n"); return 0; }
int ExportSub103() { DBGSTR("[HvP2] Called 103\n"); return 0; }
int ExportSub104() { DBGSTR("[HvP2] Called 104\n"); return 0; }
int ExportSub105() { DBGSTR("[HvP2] Called 105\n"); return 0; }
int ExportSub106() { DBGSTR("[HvP2] Called 106\n"); return 0; }
int ExportSub107() { DBGSTR("[HvP2] Called 107\n"); return 0; }
int ExportSub108() { DBGSTR("[HvP2] Called 108\n"); return 0; }
int ExportSub109() { DBGSTR("[HvP2] Called 109\n"); return 0; }
int ExportSub110() { DBGSTR("[HvP2] Called 110\n"); return 0; }
int ExportSub111() { DBGSTR("[HvP2] Called 111\n"); return 0; }
int ExportSub112() { DBGSTR("[HvP2] Called 112\n"); return 0; }
int ExportSub113() { DBGSTR("[HvP2] Called 113\n"); return 0; }
int ExportSub114() { DBGSTR("[HvP2] Called 114\n"); return 0; }
int ExportSub115() { DBGSTR("[HvP2] Called 115\n"); return 0; }
int ExportSub116() { DBGSTR("[HvP2] Called 116\n"); return 0; }
int ExportSub117() { DBGSTR("[HvP2] Called 117\n"); return 0; }
int ExportSub118() { DBGSTR("[HvP2] Called 118\n"); return 0; }
int ExportSub119() { DBGSTR("[HvP2] Called 119\n"); return 0; }
int ExportSub120() { DBGSTR("[HvP2] Called 120\n"); return 0; }
int ExportSub121() { DBGSTR("[HvP2] Called 121\n"); return 0; }
int ExportSub122() { DBGSTR("[HvP2] Called 122\n"); return 0; }
int ExportSub123() { DBGSTR("[HvP2] Called 123\n"); return 0; }
int ExportSub124() { DBGSTR("[HvP2] Called 124\n"); return 0; }
int ExportSub125() { DBGSTR("[HvP2] Called 125\n"); return 0; }
int ExportSub126() { DBGSTR("[HvP2] Called 126\n"); return 0; }
int ExportSub127() { DBGSTR("[HvP2] Called 127\n"); return 0; }
int ExportSub128() { DBGSTR("[HvP2] Called 128\n"); return 0; }
int ExportSub129() { DBGSTR("[HvP2] Called 129\n"); return 0; }
int ExportSub130() { DBGSTR("[HvP2] Called 130\n"); return 0; }
int ExportSub131() { DBGSTR("[HvP2] Called 131\n"); return 0; }
int ExportSub132() { DBGSTR("[HvP2] Called 132\n"); return 0; }
int ExportSub133() { DBGSTR("[HvP2] Called 133\n"); return 0; }
int ExportSub134() { DBGSTR("[HvP2] Called 134\n"); return 0; }
int ExportSub135() { DBGSTR("[HvP2] Called 135\n"); return 0; }
int ExportSub136() { DBGSTR("[HvP2] Called 136\n"); return 0; }
int ExportSub137() { DBGSTR("[HvP2] Called 137\n"); return 0; }
int ExportSub138() { DBGSTR("[HvP2] Called 138\n"); return 0; }
int ExportSub139() { DBGSTR("[HvP2] Called 139\n"); return 0; }
int ExportSub140() { DBGSTR("[HvP2] Called 140\n"); return 0; }


LPCSTR ESysSymbolicLinkName = "\\system??\\E:";
LPCSTR ESymbolicLinkName = "\\??\\E:";
LPCSTR DEVKITSymbolicLinkName = "\\??\\DEVKIT:";
LPCSTR DEVKITSysSymbolicLinkName = "\\system??\\DEVKIT:";
LPCSTR DevkitDeviceName = "\\Device\\Harddisk0\\Partition1\\DEVKIT";
// DmMapDevkitDrive

int ExportSub141() {
	DBGSTR("[HvP2] Called DmMapDevkitDrive\n");

	ANSI_STRING symName, devName;
	NTSTATUS st;

	DWORD b = KeGetCurrentProcessType();

	if (b == 2)
		RtlInitAnsiString(&symName, ESysSymbolicLinkName);
	else
		RtlInitAnsiString(&symName, ESymbolicLinkName);

	RtlInitAnsiString(&devName, DevkitDeviceName);

	st = ObCreateSymbolicLink(&symName, &devName);

	if (st < ((NTSTATUS)0xC0000035L) && st < 0)
		return 0x80070000;

	if (b == 2)
		RtlInitAnsiString(&symName, DEVKITSysSymbolicLinkName);
	else
		RtlInitAnsiString(&symName, DEVKITSymbolicLinkName);

	st = ObCreateSymbolicLink(&symName, &devName);

	if (st < ((NTSTATUS)0xC0000035L) && st < 0)
		return 0x80070000;

	return S_OK;
}
int ExportSub142() { DBGSTR("[HvP2] Called 142\n"); return 0; }
int ExportSub143() { DBGSTR("[HvP2] Called 143\n"); return 0; }
// DmGetMouseChanges
int ExportSub144() { /*DBGSTR("[HvP2] Called DmGetMouseChanges\n");*/ return ERROR_DEVICE_NOT_CONNECTED; }
int ExportSub145() { DBGSTR("[HvP2] Called 145\n"); return 0; }
int ExportSub146() { DBGSTR("[HvP2] Called 146\n"); return 0; }
int ExportSub147() { DBGSTR("[HvP2] Called 147\n"); return 0; }
int ExportSub148() { DBGSTR("[HvP2] Called 148\n"); return 0; }
int ExportSub149() { DBGSTR("[HvP2] Called 149\n"); return 0; }
int ExportSub150() { DBGSTR("[HvP2] Called 150\n"); return 0; }
int ExportSub151() { DBGSTR("[HvP2] Called 151\n"); return 0; }
int ExportSub152() { DBGSTR("[HvP2] Called 152\n"); return 0; }
int ExportSub153() { DBGSTR("[HvP2] Called 153\n"); return 0; }
int ExportSub154() { DBGSTR("[HvP2] Called 154\n"); return 0; }
int ExportSub155() { DBGSTR("[HvP2] Called 155\n"); return 0; }
int ExportSub156() { DBGSTR("[HvP2] Called 156\n"); return 0; }
int ExportSub157() { DBGSTR("[HvP2] Called 157\n"); return 0; }
int ExportSub158() { DBGSTR("[HvP2] Called 158\n"); return 0; }
int ExportSub159() { DBGSTR("[HvP2] Called 159\n"); return 0; }
int ExportSub160() { DBGSTR("[HvP2] Called 160\n"); return 0; }
// DmGetSystemInfo
byte bSystemInfo[] = { 0x00, 0x00, 0x00, 0x20, 0x00, 0x02, 0x00, 0x00, 0x07, 0x60, 0x00, 0x00, 0x00, 0x02, 0x00,
0x00, 0x44, 0x97, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x53, 0x08, 0x00, 0x00, 0x40, 0x00, 0x00, 0x23 };
int ExportSub161(int Ptr) {
	DBGSTR("[HvP2] Called DmGetSystemInfo\n");

	if (!Ptr || *(int *)Ptr != 0x20)
		return 0x82DA0017;

	// just copy what I dumped off my xbox on rgloader xD
	memcpy((PVOID)Ptr, bSystemInfo, 0x20);
	
	return 0x02da0000;
}
int ExportSub162() { DBGSTR("[HvP2] Called 162\n"); return 0; }
int ExportSub163() { DBGSTR("[HvP2] Called 163\n"); return 0; }
int ExportSub164() { DBGSTR("[HvP2] Called 164\n"); return 0; }
// DmQueryTitleMemoryStatistics
byte bSystemMemoryInfo[] = { 0x00, 0x00, 0x00, 0x20, 0x00, 0x02, 0x00, 0x00,
0x07, 0x60, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x44, 0x97, 0x00, 0x00, 0x00,
0x02, 0x00, 0x00, 0x53, 0x08, 0x00, 0x00, 0x40, 0x00, 0x00, 0x23, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
int ExportSub165(int Ptr) {
	//DBGSTR("[HvP2] Called DmQueryTitleMemoryStatistics\n");

	if (!Ptr)
		return 0x82DA0017;

	// just copy what I dumped off my xbox on rgloader xD
	memcpy((PVOID)Ptr, bSystemMemoryInfo, 0x30);

	return 0x02DA0000;
}
int ExportSub166() { DBGSTR("[HvP2] Called 166\n"); return 0; }
int ExportSub167() { DBGSTR("[HvP2] Called 167\n"); return 0; }
int ExportSub168() { DBGSTR("[HvP2] Called 168\n"); return 0; }
int ExportSub169() { DBGSTR("[HvP2] Called 169\n"); return 0; }
int ExportSub170() { DBGSTR("[HvP2] Called 170\n"); return 0; }
int ExportSub171() { DBGSTR("[HvP2] Called 171\n"); return 0; }
int ExportSub172() { DBGSTR("[HvP2] Called 172\n"); return 0; }
int ExportSub173() { DBGSTR("[HvP2] Called 173\n"); return 0; }
int ExportSub174() { DBGSTR("[HvP2] Called 174\n"); return 0; }
int ExportSub175() { DBGSTR("[HvP2] Called 175\n"); return 0; }
int ExportSub176() { DBGSTR("[HvP2] Called 176\n"); return 0; }
int ExportSub177() { DBGSTR("[HvP2] Called 177\n"); return 0; }
int ExportSub178() { DBGSTR("[HvP2] Called 178\n"); return 0; }
int ExportSub179() { DBGSTR("[HvP2] Called 179\n"); return 0; }
int ExportSub180() { DBGSTR("[HvP2] Called 180\n"); return 0; }
int ExportSub181() { DBGSTR("[HvP2] Called 181\n"); return 0; }
int ExportSub182() { DBGSTR("[HvP2] Called 182\n"); return 0; }
int ExportSub183() { DBGSTR("[HvP2] Called 183\n"); return 0; }
int ExportSub184() { DBGSTR("[HvP2] Called 184\n"); return 0; }
int ExportSub185() { DBGSTR("[HvP2] Called 185\n"); return 0; }
int ExportSub186() { DBGSTR("[HvP2] Called 186\n"); return 0; }
int ExportSub187() { DBGSTR("[HvP2] Called 187\n"); return 0; }
int ExportSub188() { DBGSTR("[HvP2] Called 188\n"); return 0; }
int ExportSub189() { DBGSTR("[HvP2] Called 189\n"); return 0; }
int ExportSub190() { DBGSTR("[HvP2] Called 190\n"); return 0; }
int ExportSub191() { DBGSTR("[HvP2] Called 191\n"); return 0; }
int ExportSub192() { DBGSTR("[HvP2] Called 192\n"); return 0; }
int ExportSub193() { DBGSTR("[HvP2] Called 193\n"); return 0; }
int ExportSub194() { DBGSTR("[HvP2] Called 194\n"); return 0; }
int ExportSub195() { DBGSTR("[HvP2] Called 195\n"); return 0; }
int ExportSub196() { DBGSTR("[HvP2] Called 196\n"); return 0; }
int ExportSub197() { DBGSTR("[HvP2] Called 197\n"); return 0; }
int ExportSub198() { DBGSTR("[HvP2] Called 198\n"); return 0; }
int ExportSub199() { DBGSTR("[HvP2] Called 199\n"); return 0; }
int ExportSub200() { DBGSTR("[HvP2] Called 200\n"); return 0; }
int ExportSub201() { DBGSTR("[HvP2] Called 201\n"); return 0; }
int ExportSub202() { DBGSTR("[HvP2] Called 202\n"); return 0; }
int ExportSub203() { DBGSTR("[HvP2] Called 203\n"); return 0; }
int ExportSub204() { DBGSTR("[HvP2] Called 204\n"); return 0; }
int ExportSub205() { DBGSTR("[HvP2] Called 205\n"); return 0; }
int ExportSub206() { DBGSTR("[HvP2] Called 206\n"); return 0; }
int ExportSub207() { DBGSTR("[HvP2] Called 207\n"); return 0; }
int ExportSub208() { DBGSTR("[HvP2] Called 208\n"); return 0; }
int ExportSub209() { DBGSTR("[HvP2] Called 209\n"); return 0; }
int ExportSub210() { DBGSTR("[HvP2] Called 210\n"); return 0; }
int ExportSub211() { DBGSTR("[HvP2] Called 211\n"); return 0; }
int ExportSub212() { DBGSTR("[HvP2] Called 212\n"); return 0; }
int ExportSub213() { DBGSTR("[HvP2] Called 213\n"); return 0; }
// DmGetConsoleDebugMemoryStatus
//#include <xbdm.h>
int ExportSub214(int* pdwConsoleMemConfig)
{ 
	DBGSTR("[HvP2] Called DmGetConsoleDebugMemoryStatus\n");
	*pdwConsoleMemConfig = 0;
	return 0x02DA0000;
}
int ExportSub215() { DBGSTR("[HvP2] Called 215\n"); return 0; }
int ExportSub216() { DBGSTR("[HvP2] Called 216\n"); return 0; }
int ExportSub217() { DBGSTR("[HvP2] Called 217\n"); return 0; }
int ExportSub218() { DBGSTR("[HvP2] Called 218\n"); return 0; }
int ExportSub219() { DBGSTR("[HvP2] Called 219\n"); return 0; }
int ExportSub220() { DBGSTR("[HvP2] Called 220\n"); return 0; }
int ExportSub221() { DBGSTR("[HvP2] Called 221\n"); return 0; }
int ExportSub222() { DBGSTR("[HvP2] Called 222\n"); return 0; }
int ExportSub223() { DBGSTR("[HvP2] Called 223\n"); return 0; }
int ExportSub224() { DBGSTR("[HvP2] Called 224\n"); return 0; }
int ExportSub225() { DBGSTR("[HvP2] Called 225\n"); return 0; }
int ExportSub226() { DBGSTR("[HvP2] Called 226\n"); return 0; }
int ExportSub227() { DBGSTR("[HvP2] Called 227\n"); return 0; }
int ExportSub228() { DBGSTR("[HvP2] Called 228\n"); return 0; }
int ExportSub229() { DBGSTR("[HvP2] Called 229\n"); return 0; }
int ExportSub230() { DBGSTR("[HvP2] Called 230\n"); return 0; }
int ExportSub231() { DBGSTR("[HvP2] Called 231\n"); return 0; }
int ExportSub232() { DBGSTR("[HvP2] Called 232\n"); return 0; }
int ExportSub233() { DBGSTR("[HvP2] Called 233\n"); return 0; }
int ExportSub234() { DBGSTR("[HvP2] Called 234\n"); return 0; }
int ExportSub235() { DBGSTR("[HvP2] Called 235\n"); return 0; }
int ExportSub236() { DBGSTR("[HvP2] Called 236\n"); return 0; }
int ExportSub237() { DBGSTR("[HvP2] Called 237\n"); return 0; }
int ExportSub238() { DBGSTR("[HvP2] Called 238\n"); return 0; }
int ExportSub239() { DBGSTR("[HvP2] Called 239\n"); return 0; }
int ExportSub240() { DBGSTR("[HvP2] Called 240\n"); return 0; }
int ExportSub241() { DBGSTR("[HvP2] Called 241\n"); return 0; }
int ExportSub242() { DBGSTR("[HvP2] Called 242\n"); return 0; }
int ExportSub243() { DBGSTR("[HvP2] Called 243\n"); return 0; }
int ExportSub244() { DBGSTR("[HvP2] Called 244\n"); return 0; }
int ExportSub245() { DBGSTR("[HvP2] Called 245\n"); return 0; }
int ExportSub246() { DBGSTR("[HvP2] Called 246\n"); return 0; }
int ExportSub247() { DBGSTR("[HvP2] Called 247\n"); return 0; }
int ExportSub248() { DBGSTR("[HvP2] Called 248\n"); return 0; }
int ExportSub249() { DBGSTR("[HvP2] Called 249\n"); return 0; }
int ExportSub250() { DBGSTR("[HvP2] Called 250\n"); return 0; }
int ExportSub251() { DBGSTR("[HvP2] Called 251\n"); return 0; }
int ExportSub252() { DBGSTR("[HvP2] Called 252\n"); return 0; }
int ExportSub253() { DBGSTR("[HvP2] Called 253\n"); return 0; }
int ExportSub254() { DBGSTR("[HvP2] Called 254\n"); return 0; }
int ExportSub255() { DBGSTR("[HvP2] Called 255\n"); return 0; }
int ExportSub256() { DBGSTR("[HvP2] Called 256\n"); return 0; }
int ExportSub257() { DBGSTR("[HvP2] Called 257\n"); return 0; }
int ExportSub258() { DBGSTR("[HvP2] Called 258\n"); return 0; }
int ExportSub259() { DBGSTR("[HvP2] Called 259\n"); return 0; }
int ExportSub260() { DBGSTR("[HvP2] Called 260\n"); return 0; }
int ExportSub261() { DBGSTR("[HvP2] Called 261\n"); return 0; }
int ExportSub262() { DBGSTR("[HvP2] Called 262\n"); return 0; }
int ExportSub263() { DBGSTR("[HvP2] Called 263\n"); return 0; }
int ExportSub264() { DBGSTR("[HvP2] Called 264\n"); return 0; }
int ExportSub265() { DBGSTR("[HvP2] Called 265\n"); return 0; }
int ExportSub266() { DBGSTR("[HvP2] Called 266\n"); return 0; }
int ExportSub267() { DBGSTR("[HvP2] Called 267\n"); return 0; }
int ExportSub268() { DBGSTR("[HvP2] Called 268\n"); return 0; }
int ExportSub269() { DBGSTR("[HvP2] Called 269\n"); return 0; }
int ExportSub270() { DBGSTR("[HvP2] Called 270\n"); return 0; }
int ExportSub271() { DBGSTR("[HvP2] Called 271\n"); return 0; }
int ExportSub272() { DBGSTR("[HvP2] Called 272\n"); return 0; }
int ExportSub273() { DBGSTR("[HvP2] Called 273\n"); return 0; }
int ExportSub274() { DBGSTR("[HvP2] Called 274\n"); return 0; }
int ExportSub275() { DBGSTR("[HvP2] Called 275\n"); return 0; }
int ExportSub276() { DBGSTR("[HvP2] Called 276\n"); return 0; }
int ExportSub277() { DBGSTR("[HvP2] Called 277\n"); return 0; }
int ExportSub278() { DBGSTR("[HvP2] Called 278\n"); return 0; }
int ExportSub279() { DBGSTR("[HvP2] Called 279\n"); return 0; }
int ExportSub280() { DBGSTR("[HvP2] Called 280\n"); return 0; }
int ExportSub281() { DBGSTR("[HvP2] Called 281\n"); return 0; }
int ExportSub282() { DBGSTR("[HvP2] Called 282\n"); return 0; }
int ExportSub283() { DBGSTR("[HvP2] Called 283\n"); return 0; }
int ExportSub284() { DBGSTR("[HvP2] Called 284\n"); return 0; }
int ExportSub285() { DBGSTR("[HvP2] Called 285\n"); return 0; }
int ExportSub286() { DBGSTR("[HvP2] Called 286\n"); return 0; }
int ExportSub287() { DBGSTR("[HvP2] Called 287\n"); return 0; }
int ExportSub288() { DBGSTR("[HvP2] Called 288\n"); return 0; }
int ExportSub289() { DBGSTR("[HvP2] Called 289\n"); return 0; }
int ExportSub290() { DBGSTR("[HvP2] Called 290\n"); return 0; }
int ExportSub291() { DBGSTR("[HvP2] Called 291\n"); return 0; }
int ExportSub292() { DBGSTR("[HvP2] Called 292\n"); return 0; }
int ExportSub293() { DBGSTR("[HvP2] Called 293\n"); return 0; }
int ExportSub294() { DBGSTR("[HvP2] Called 294\n"); return 0; }
int ExportSub295() { DBGSTR("[HvP2] Called 295\n"); return 0; }
int ExportSub296() { DBGSTR("[HvP2] Called 296\n"); return 0; }
int ExportSub297() { DBGSTR("[HvP2] Called 297\n"); return 0; }
int ExportSub298() { DBGSTR("[HvP2] Called 298\n"); return 0; }
int ExportSub299() { DBGSTR("[HvP2] Called 299\n"); return 0; }
int ExportSub300() { DBGSTR("[HvP2] Called 300\n"); return 0; }
int ExportSub301() { DBGSTR("[HvP2] Called 301\n"); return 0; }
int ExportSub302() { DBGSTR("[HvP2] Called 302\n"); return 0; }
int ExportSub303() { DBGSTR("[HvP2] Called 303\n"); return 0; }
int ExportSub304() { DBGSTR("[HvP2] Called 304\n"); return 0; }
int ExportSub305() { DBGSTR("[HvP2] Called 305\n"); return 0; }
int ExportSub306() { DBGSTR("[HvP2] Called 306\n"); return 0; }
int ExportSub307() { DBGSTR("[HvP2] Called 307\n"); return 0; }
int ExportSub308() { DBGSTR("[HvP2] Called 308\n"); return 0; }
int ExportSub309() { DBGSTR("[HvP2] Called 309\n"); return 0; }
int ExportSub310() { DBGSTR("[HvP2] Called 310\n"); return 0; }
int ExportSub311() { DBGSTR("[HvP2] Called 311\n"); return 0; }
int ExportSub312() { DBGSTR("[HvP2] Called 312\n"); return 0; }
int ExportSub313() { DBGSTR("[HvP2] Called 313\n"); return 0; }
int ExportSub314() { DBGSTR("[HvP2] Called 314\n"); return 0; }
int ExportSub315() { DBGSTR("[HvP2] Called 315\n"); return 0; }
int ExportSub316() { DBGSTR("[HvP2] Called 316\n"); return 0; }
int ExportSub317() { DBGSTR("[HvP2] Called 317\n"); return 0; }
int ExportSub318() { DBGSTR("[HvP2] Called 318\n"); return 0; }
int ExportSub319() { DBGSTR("[HvP2] Called 319\n"); return 0; }
int ExportSub320() { DBGSTR("[HvP2] Called 320\n"); return 0; }
int ExportSub321() { DBGSTR("[HvP2] Called 321\n"); return 0; }
int ExportSub322() { DBGSTR("[HvP2] Called 322\n"); return 0; }
int ExportSub323() { DBGSTR("[HvP2] Called 323\n"); return 0; }
int ExportSub324() { DBGSTR("[HvP2] Called 324\n"); return 0; }
int ExportSub325() { DBGSTR("[HvP2] Called 325\n"); return 0; }
int ExportSub326() { DBGSTR("[HvP2] Called 326\n"); return 0; }
int ExportSub327() { DBGSTR("[HvP2] Called 327\n"); return 0; }
int ExportSub328() { DBGSTR("[HvP2] Called 328\n"); return 0; }
int ExportSub329() { DBGSTR("[HvP2] Called 329\n"); return 0; }
int ExportSub330() { DBGSTR("[HvP2] Called 330\n"); return 0; }
int ExportSub331() { DBGSTR("[HvP2] Called 331\n"); return 0; }
int ExportSub332() { DBGSTR("[HvP2] Called 332\n"); return 0; }
int ExportSub333() { DBGSTR("[HvP2] Called 333\n"); return 0; }
int ExportSub334() { DBGSTR("[HvP2] Called 334\n"); return 0; }
int ExportSub335() { DBGSTR("[HvP2] Called 335\n"); return 0; }
int ExportSub336() { DBGSTR("[HvP2] Called 336\n"); return 0; }
int ExportSub337() { DBGSTR("[HvP2] Called 337\n"); return 0; }
int ExportSub338() { DBGSTR("[HvP2] Called 338\n"); return 0; }
int ExportSub339() { DBGSTR("[HvP2] Called 339\n"); return 0; }
int ExportSub340() { DBGSTR("[HvP2] Called 340\n"); return 0; }
int ExportSub341() { DBGSTR("[HvP2] Called 341\n"); return 0; }
int ExportSub342() { DBGSTR("[HvP2] Called 342\n"); return 0; }
int ExportSub343() { DBGSTR("[HvP2] Called 343\n"); return 0; }
int ExportSub344() { DBGSTR("[HvP2] Called 344\n"); return 0; }
int ExportSub345() { DBGSTR("[HvP2] Called 345\n"); return 0; }
int ExportSub346() { DBGSTR("[HvP2] Called 346\n"); return 0; }
int ExportSub347() { DBGSTR("[HvP2] Called 347\n"); return 0; }
int ExportSub348() { DBGSTR("[HvP2] Called 348\n"); return 0; }
int ExportSub349() { DBGSTR("[HvP2] Called 349\n"); return 0; }
int ExportSub350() { DBGSTR("[HvP2] Called 350\n"); return 0; }
int ExportSub351() { DBGSTR("[HvP2] Called 351\n"); return 0; }
int ExportSub352() { DBGSTR("[HvP2] Called 352\n"); return 0; }
int ExportSub353() { DBGSTR("[HvP2] Called 353\n"); return 0; }
int ExportSub354() { DBGSTR("[HvP2] Called 354\n"); return 0; }
int ExportSub355() { DBGSTR("[HvP2] Called 355\n"); return 0; }
int ExportSub356() { DBGSTR("[HvP2] Called 356\n"); return 0; }
int ExportSub357() { DBGSTR("[HvP2] Called 357\n"); return 0; }
int ExportSub358() { DBGSTR("[HvP2] Called 358\n"); return 0; }
int ExportSub359() { DBGSTR("[HvP2] Called 359\n"); return 0; }
int ExportSub360() { DBGSTR("[HvP2] Called 360\n"); return 0; }
int ExportSub361() { DBGSTR("[HvP2] Called 361\n"); return 0; }
int ExportSub362() { DBGSTR("[HvP2] Called 362\n"); return 0; }
int ExportSub363() { DBGSTR("[HvP2] Called 363\n"); return 0; }
int ExportSub364() { DBGSTR("[HvP2] Called 364\n"); return 0; }
int ExportSub365() { DBGSTR("[HvP2] Called 365\n"); return 0; }
int ExportSub366() { DBGSTR("[HvP2] Called 366\n"); return 0; }
int ExportSub367() { DBGSTR("[HvP2] Called 367\n"); return 0; }
int ExportSub368() { DBGSTR("[HvP2] Called 368\n"); return 0; }
int ExportSub369() { DBGSTR("[HvP2] Called 369\n"); return 0; }
int ExportSub370() { DBGSTR("[HvP2] Called 370\n"); return 0; }
int ExportSub371() { DBGSTR("[HvP2] Called 371\n"); return 0; }
int ExportSub372() { DBGSTR("[HvP2] Called 372\n"); return 0; }
int ExportSub373() { DBGSTR("[HvP2] Called 373\n"); return 0; }
int ExportSub374() { DBGSTR("[HvP2] Called 374\n"); return 0; }
int ExportSub375() { DBGSTR("[HvP2] Called 375\n"); return 0; }
int ExportSub376() { DBGSTR("[HvP2] Called 376\n"); return 0; }
int ExportSub377() { DBGSTR("[HvP2] Called 377\n"); return 0; }
int ExportSub378() { DBGSTR("[HvP2] Called 378\n"); return 0; }
int ExportSub379() { DBGSTR("[HvP2] Called 379\n"); return 0; }
int ExportSub380() { DBGSTR("[HvP2] Called 380\n"); return 0; }
int ExportSub381() { DBGSTR("[HvP2] Called 381\n"); return 0; }
int ExportSub382() { DBGSTR("[HvP2] Called 382\n"); return 0; }
int ExportSub383() { DBGSTR("[HvP2] Called 383\n"); return 0; }
int ExportSub384() { DBGSTR("[HvP2] Called 384\n"); return 0; }
int ExportSub385() { DBGSTR("[HvP2] Called 385\n"); return 0; }
int ExportSub386() { DBGSTR("[HvP2] Called 386\n"); return 0; }
int ExportSub387() { DBGSTR("[HvP2] Called 387\n"); return 0; }
int ExportSub388() { DBGSTR("[HvP2] Called 388\n"); return 0; }
int ExportSub389() { DBGSTR("[HvP2] Called 389\n"); return 0; }
int ExportSub390() { DBGSTR("[HvP2] Called 390\n"); return 0; }
int ExportSub391() { DBGSTR("[HvP2] Called 391\n"); return 0; }
int ExportSub392() { DBGSTR("[HvP2] Called 392\n"); return 0; }
int ExportSub393() { DBGSTR("[HvP2] Called 393\n"); return 0; }
int ExportSub394() { DBGSTR("[HvP2] Called 394\n"); return 0; }
int ExportSub395() { DBGSTR("[HvP2] Called 395\n"); return 0; }
int ExportSub396() { DBGSTR("[HvP2] Called 396\n"); return 0; }
int ExportSub397() { DBGSTR("[HvP2] Called 397\n"); return 0; }
int ExportSub398() { DBGSTR("[HvP2] Called 398\n"); return 0; }
int ExportSub399() { DBGSTR("[HvP2] Called 399\n"); return 0; }
int ExportSub400() { DBGSTR("[HvP2] Called 400\n"); return 0; }
int ExportSub401() { DBGSTR("[HvP2] Called 401\n"); return 0; }
int ExportSub402() { DBGSTR("[HvP2] Called 402\n"); return 0; }
int ExportSub403() { DBGSTR("[HvP2] Called 403\n"); return 0; }
int ExportSub404() { DBGSTR("[HvP2] Called 404\n"); return 0; }
int ExportSub405() { DBGSTR("[HvP2] Called 405\n"); return 0; }
int ExportSub406() { DBGSTR("[HvP2] Called 406\n"); return 0; }
int ExportSub407() { DBGSTR("[HvP2] Called 407\n"); return 0; }
int ExportSub408() { DBGSTR("[HvP2] Called 408\n"); return 0; }
int ExportSub409() { DBGSTR("[HvP2] Called 409\n"); return 0; }
int ExportSub410() { DBGSTR("[HvP2] Called 410\n"); return 0; }
int ExportSub411() { DBGSTR("[HvP2] Called 411\n"); return 0; }
int ExportSub412() { DBGSTR("[HvP2] Called 412\n"); return 0; }
int ExportSub413() { DBGSTR("[HvP2] Called 413\n"); return 0; }
int ExportSub414() { DBGSTR("[HvP2] Called 414\n"); return 0; }
int ExportSub415() { DBGSTR("[HvP2] Called 415\n"); return 0; }
int ExportSub416() { DBGSTR("[HvP2] Called 416\n"); return 0; }
int ExportSub417() { DBGSTR("[HvP2] Called 417\n"); return 0; }
int ExportSub418() { DBGSTR("[HvP2] Called 418\n"); return 0; }
int ExportSub419() { DBGSTR("[HvP2] Called 419\n"); return 0; }
int ExportSub420() { DBGSTR("[HvP2] Called 420\n"); return 0; }
int ExportSub421() { DBGSTR("[HvP2] Called 421\n"); return 0; }
int ExportSub422() { DBGSTR("[HvP2] Called 422\n"); return 0; }
int ExportSub423() { DBGSTR("[HvP2] Called 423\n"); return 0; }
int ExportSub424() { DBGSTR("[HvP2] Called 424\n"); return 0; }
int ExportSub425() { DBGSTR("[HvP2] Called 425\n"); return 0; }
int ExportSub426() { DBGSTR("[HvP2] Called 426\n"); return 0; }
int ExportSub427() { DBGSTR("[HvP2] Called 427\n"); return 0; }
int ExportSub428() { DBGSTR("[HvP2] Called 428\n"); return 0; }
int ExportSub429() { DBGSTR("[HvP2] Called 429\n"); return 0; }
int ExportSub430() { DBGSTR("[HvP2] Called 430\n"); return 0; }
int ExportSub431() { DBGSTR("[HvP2] Called 431\n"); return 0; }
int ExportSub432() { DBGSTR("[HvP2] Called 432\n"); return 0; }
int ExportSub433() { DBGSTR("[HvP2] Called 433\n"); return 0; }
int ExportSub434() { DBGSTR("[HvP2] Called 434\n"); return 0; }
int ExportSub435() { DBGSTR("[HvP2] Called 435\n"); return 0; }
int ExportSub436() { DBGSTR("[HvP2] Called 436\n"); return 0; }
int ExportSub437() { DBGSTR("[HvP2] Called 437\n"); return 0; }
int ExportSub438() { DBGSTR("[HvP2] Called 438\n"); return 0; }
int ExportSub439() { DBGSTR("[HvP2] Called 439\n"); return 0; }
int ExportSub440() { DBGSTR("[HvP2] Called 440\n"); return 0; }
int ExportSub441() { DBGSTR("[HvP2] Called 441\n"); return 0; }
int ExportSub442() { DBGSTR("[HvP2] Called 442\n"); return 0; }
int ExportSub443() { DBGSTR("[HvP2] Called 443\n"); return 0; }
int ExportSub444() { DBGSTR("[HvP2] Called 444\n"); return 0; }
int ExportSub445() { DBGSTR("[HvP2] Called 445\n"); return 0; }
int ExportSub446() { DBGSTR("[HvP2] Called 446\n"); return 0; }
int ExportSub447() { DBGSTR("[HvP2] Called 447\n"); return 0; }
int ExportSub448() { DBGSTR("[HvP2] Called 448\n"); return 0; }
int ExportSub449() { DBGSTR("[HvP2] Called 449\n"); return 0; }
int ExportSub450() { DBGSTR("[HvP2] Called 450\n"); return 0; }
int ExportSub451() { DBGSTR("[HvP2] Called 451\n"); return 0; }
int ExportSub452() { DBGSTR("[HvP2] Called 452\n"); return 0; }
int ExportSub453() { DBGSTR("[HvP2] Called 453\n"); return 0; }
int ExportSub454() { DBGSTR("[HvP2] Called 454\n"); return 0; }
int ExportSub455() { DBGSTR("[HvP2] Called 455\n"); return 0; }
int ExportSub456() { DBGSTR("[HvP2] Called 456\n"); return 0; }
int ExportSub457() { DBGSTR("[HvP2] Called 457\n"); return 0; }
int ExportSub458() { DBGSTR("[HvP2] Called 458\n"); return 0; }
int ExportSub459() { DBGSTR("[HvP2] Called 459\n"); return 0; }
int ExportSub460() { DBGSTR("[HvP2] Called 460\n"); return 0; }
int ExportSub461() { DBGSTR("[HvP2] Called 461\n"); return 0; }
int ExportSub462() { DBGSTR("[HvP2] Called 462\n"); return 0; }
int ExportSub463() { DBGSTR("[HvP2] Called 463\n"); return 0; }
int ExportSub464() { DBGSTR("[HvP2] Called 464\n"); return 0; }
int ExportSub465() { DBGSTR("[HvP2] Called 465\n"); return 0; }
int ExportSub466() { DBGSTR("[HvP2] Called 466\n"); return 0; }
int ExportSub467() { DBGSTR("[HvP2] Called 467\n"); return 0; }
int ExportSub468() { DBGSTR("[HvP2] Called 468\n"); return 0; }
int ExportSub469() { DBGSTR("[HvP2] Called 469\n"); return 0; }
int ExportSub470() { DBGSTR("[HvP2] Called 470\n"); return 0; }
int ExportSub471() { DBGSTR("[HvP2] Called 471\n"); return 0; }
int ExportSub472() { DBGSTR("[HvP2] Called 472\n"); return 0; }
int ExportSub473() { DBGSTR("[HvP2] Called 473\n"); return 0; }
int ExportSub474() { DBGSTR("[HvP2] Called 474\n"); return 0; }
int ExportSub475() { DBGSTR("[HvP2] Called 475\n"); return 0; }
int ExportSub476() { DBGSTR("[HvP2] Called 476\n"); return 0; }
int ExportSub477() { DBGSTR("[HvP2] Called 477\n"); return 0; }
int ExportSub478() { DBGSTR("[HvP2] Called 478\n"); return 0; }
int ExportSub479() { DBGSTR("[HvP2] Called 479\n"); return 0; }
int ExportSub480() { DBGSTR("[HvP2] Called 480\n"); return 0; }
int ExportSub481() { DBGSTR("[HvP2] Called 481\n"); return 0; }
int ExportSub482() { DBGSTR("[HvP2] Called 482\n"); return 0; }
int ExportSub483() { DBGSTR("[HvP2] Called 483\n"); return 0; }
int ExportSub484() { DBGSTR("[HvP2] Called 484\n"); return 0; }
int ExportSub485() { DBGSTR("[HvP2] Called 485\n"); return 0; }
int ExportSub486() { DBGSTR("[HvP2] Called 486\n"); return 0; }
int ExportSub487() { DBGSTR("[HvP2] Called 487\n"); return 0; }
int ExportSub488() { DBGSTR("[HvP2] Called 488\n"); return 0; }
int ExportSub489() { DBGSTR("[HvP2] Called 489\n"); return 0; }
int ExportSub490() { DBGSTR("[HvP2] Called 490\n"); return 0; }
int ExportSub491() { DBGSTR("[HvP2] Called 491\n"); return 0; }
int ExportSub492() { DBGSTR("[HvP2] Called 492\n"); return 0; }
int ExportSub493() { DBGSTR("[HvP2] Called 493\n"); return 0; }
int ExportSub494() { DBGSTR("[HvP2] Called 494\n"); return 0; }
int ExportSub495() { DBGSTR("[HvP2] Called 495\n"); return 0; }
int ExportSub496() { DBGSTR("[HvP2] Called 496\n"); return 0; }
int ExportSub497() { DBGSTR("[HvP2] Called 497\n"); return 0; }
int ExportSub498() { DBGSTR("[HvP2] Called 498\n"); return 0; }
int ExportSub499() { DBGSTR("[HvP2] Called 499\n"); return 0; }
int ExportSub500() { DBGSTR("[HvP2] Called 500\n"); return 0; }
int ExportSub501() { DBGSTR("[HvP2] Called 501\n"); return 0; }
int ExportSub502() { DBGSTR("[HvP2] Called 502\n"); return 0; }
int ExportSub503() { DBGSTR("[HvP2] Called 503\n"); return 0; }
int ExportSub504() { DBGSTR("[HvP2] Called 504\n"); return 0; }
int ExportSub505() { DBGSTR("[HvP2] Called 505\n"); return 0; }
int ExportSub506() { DBGSTR("[HvP2] Called 506\n"); return 0; }
int ExportSub507() { DBGSTR("[HvP2] Called 507\n"); return 0; }
int ExportSub508() { DBGSTR("[HvP2] Called 508\n"); return 0; }
int ExportSub509() { DBGSTR("[HvP2] Called 509\n"); return 0; }
int ExportSub510() { DBGSTR("[HvP2] Called 510\n"); return 0; }
int ExportSub511() { DBGSTR("[HvP2] Called 511\n"); return 0; }
int ExportSub512() { DBGSTR("[HvP2] Called 512\n"); return 0; }


#pragma endregion


//-------------------------------------------------------------------------------------
// Name: MmAllocatePhysicalMemoryExHook
// Desc: Hooks the debug builds MmAllocatePhysicalMemoryEx and if it succeeds then zero
//		 the memory.
//		 (Some builds just read into data and if it's not null it will treat it as valid data)
//-------------------------------------------------------------------------------------
int MmAllocatePhysicalMemoryExHook(int r3, int Size, int r5, int r6, int r7, int r8)
{
	int Result = MmAllocatePhysicalMemoryExDetour.CallOriginal(r3, Size, r5, r6, r7, r8);

	if (Result != 0) ZeroMemory((PVOID)Result, Size);

	return Result;
}


#define XexMenuSearchResultMax 30
char szSearchResults[XexMenuSearchResultMax][50];
//-------------------------------------------------------------------------------------
// Name: XexMenuGetFilesHook
// Desc: Hooks xexmenu's GetFiles function and if it's looking for xex's from a RS click
//		 then check for all exe and xbe to add to the list too. 
//-------------------------------------------------------------------------------------
int XexMenuGetFilesHook(int r3, int OutData, int r5, bool bFilterXEX)
{
	int NumOfResults,
		NumOfResults2,
		i;

	// "\\*.xex" xex part of the search string
	int *pSearchFilter = (int*)0x8200376B;

	// if we aren't filtering then just return
	if (bFilterXEX == false)
		return XexMenuGetFilesDetour.CallOriginal(r3, OutData, r5, bFilterXEX);

	// clear all the strings
	ZeroMemory(szSearchResults, XexMenuSearchResultMax * 50);

	// set the search extention to exe
	*pSearchFilter = 'exe\0';

	// find all the 'EXE' files
	NumOfResults = XexMenuGetFilesDetour.CallOriginal(r3, OutData, r5, bFilterXEX);

	// copy the file names to our buffer
	for (i = 0; i < NumOfResults; i++) 
		strcpy(szSearchResults[i], (char*)(OutData + (i * 0xA04) + 4));

	// set the search extention to xbe
	*pSearchFilter = 'xbe\0';
	NumOfResults2 = XexMenuGetFilesDetour.CallOriginal(r3, OutData, r5, bFilterXEX);

	// copy the file names to our buffer
	for (i = 0; i < NumOfResults2; i++)
		strcpy(szSearchResults[NumOfResults + i], (char*)(OutData + (i * 0xA04) + 4));


	// add the sizes together
	NumOfResults2 += NumOfResults;

	// reset the search extention to xex
	*pSearchFilter = 'xex\0';

	// call the orignal function to find the xex files
	NumOfResults = XexMenuGetFilesDetour.CallOriginal(r3, OutData, r5, bFilterXEX);

	// copy the exe & xbe results to the end result
	for(i = 0; i < NumOfResults2; i++)
		strcpy((char*)(OutData + ((NumOfResults+i) * 0xA04) + 4), szSearchResults[i]);

	NumOfResults += NumOfResults2;

	// if we don't have enough space
	if (NumOfResults > XexMenuSearchResultMax)
		return XexMenuSearchResultMax;

	// add all our strings to our buffer
	for(i = 0; i < NumOfResults; i++)
		strcpy(szSearchResults[i], (char*)(OutData + (i * 0xA04) + 4));

	// copy all the strings back to the return buffer
	for (i = 0; i < NumOfResults; i++)
	{
		// say it's a .xex type
		*(int *)(OutData + (i * 0xA04)) = 3;
		// copy the names to the buffer
		strcpy((char*)(OutData + (i * 0xA04) + 4), szSearchResults[i]);
		strcpy((char*)(OutData + (i * 0xA04) + 4 + 0x400), szSearchResults[i]);
	}

	// return all the sizes
	return NumOfResults;
}

//-------------------------------------------------------------------------------------
// Name: XexMenuStrStrHook
// Desc: Hooks xexmenu's strstr function and if it's checking the '.xex' extension then 
//		 check for an '.exe'. if it doesn't find a '.exe' then just do what we hooked.
//		 This will allow the .exe to be launched as a game.
//		 
//		 NOTE: there is a bug that if a file contains '.xex' in it's name it will think
//			   it's an xex even if the extension is '.txt'
//-------------------------------------------------------------------------------------
int XexMenuStrStrHook(char* Str, const char * SubStr)
{

	// Check if xexmenu is doing a 'xex' compair on a filename ( only the search function does this )
	if (strstr(SubStr, ".xex"))
	{

		// Check if the file is an exe and return 1 for a success
		if (strstr(Str, ".exe") || strstr(Str, ".EXE"))
			return 1;
	}

	// If it's not an 'exe' just do a normal functoin call
	return (int)strstr(Str, SubStr);
}

//-------------------------------------------------------------------------------------
// Name: MessageBoxThread()
// Desc: Display a Messagebox Error depending on 'lpArg'
//-------------------------------------------------------------------------------------
DWORD WINAPI MessageBoxThread(LPVOID lpArg)
{
	DWORD dwArg = (DWORD)lpArg;

	fMessageBoxThread = TRUE;

	Sleep(4000);

	MESSAGEBOX_RESULT result;
	XOVERLAPPED over;

	LPCWSTR XShowMessageBoxUIButtons[] = { L"OK" };
	LPCWSTR wtext = L"";

	// invalid Dashbord!
	if (dwArg == 0)
		wtext = L"Your current dashbord is not supported!\nPlease Update to dash 17559.\n\nIf you are already on 17559 Please contact us so we can fix this bug.\n\nMessage Chr0m3x on discord or send a message in the #hvp-plugin-support channel on Obscure Gamers discord!";
	if (dwArg == 1)
		wtext = L"Your console HV is not supported\n\nIf you are on a JTAG/RGH on dash 17559 please contact us so we can fix this bug.\n\nMessage Chr0m3x on discord or send a message in the #hvp-plugin-support channel on Obscure Gamers discord!";

	while (XShowMessageBoxUI(0, L"HvP Error!", wtext, 1, XShowMessageBoxUIButtons, 0, 0, &result, &over) == ERROR_ACCESS_DENIED) Sleep(1000);

	while (!XHasOverlappedIoCompleted(&over)) Sleep(200);

	//so we can unload safely
	fMessageBoxThread = FALSE;

	// unload
	XexUnloadImageAndExitThread(::hModule, 0);

	return TRUE;
}

//-------------------------------------------------------------------------------------
// Name: TryAndHookXexMenu()
// Desc: Was orignally just in XexpCompleteLoadHook but now if we have to re hook XexpCompleteLoad
//		 we might have XexMenu already loaded without our hooks. so try and hook them.
//-------------------------------------------------------------------------------------
void TryAndHookXexMenu()
{
	// xexmenu 1.2 strstr address
	if (MmIsAddressValid((PVOID)0x821AD9D0) && *(QWORD *)0x821AD9D0 == 0x89640000280B0000)
	{
		DBGSTR("[HvP2][XexpCompleteLoadHook]: Hooking XexMenu so we can launch 'EXE' Files!\n");
		XexMenuStrStrDetour.SetupDetour(0x821AD9D0, XexMenuStrStrHook);
		XexMenuGetFilesDetour.SetupDetour(0x820A4E28, XexMenuGetFilesHook);
	}
}


//-------------------------------------------------------------------------------------
// Name: XexpCompleteLoadHook()
// Desc: Was originally only hooked to display the error codes of failed to launch games.
//		 Now we check if 'xbox.xex' is being loaded and then unload if it is.
//		 Also used to hook games / XexMenu as soon as it loads up.
//-------------------------------------------------------------------------------------
int XexpCompleteLoadHook(PHANDLE *pHandle, DWORD dwVersion)
{
	char szBuffer[MAX_PATH];

	// copy the Loading modules name to our buffer
	WideCharToMultiByte(CP_UTF8, NULL, (wchar_t*)((PLDR_DATA_TABLE_ENTRY)*pHandle)->BaseDllName.Buffer, -1, (char*)szBuffer, MAX_PATH, NULL, NULL);

	DBGSTR("[HvP2][XexpCompleteLoadHook]: Loading '%s'\n", szBuffer);

	// unload our hooks because we are trying to load an OG xbox game
	if (strstr(szBuffer, "xbox.xex"))
	{
		DBGSTR("[HvP2][XexpCompleteLoadHook]: Original Xbox game detected! removing kernel hooks!\n");

		XexpCompleteLoadDetour.TakeDownDetour();
		XexpResolveImageImportsDetour.TakeDownDetour();

		fIsOrignalXboxGame = TRUE;

		// use this so dashlaunch will try to load again without our hooks and we won't crash!
		return 0xC0000102; //NT_STATUS_FILE_CORRUPT_ERROR
	}

	int Result = XexpCompleteLoadDetour.CallOriginal(pHandle, dwVersion);

	if(Result == 0xC0000142)
		DBGSTR("[HvP2][XexpCompleteLoadHook]: 0xC0000142 (game might of manually aborted!)\n");
	else
		DBGSTR("[HvP2][XexpCompleteLoadHook]: 0x%08X\n", Result);

	// if the result is 0 and the module is a title then setup some hooks
	if (Result == 0 && (int)((PLDR_DATA_TABLE_ENTRY)*pHandle)->ImageBase == 0x82000000)
	{

		// see if XexMenu needs to be hooked
		TryAndHookXexMenu();

		// if we are loading a debug build
		if (fIsLoadingDebugBuild)
		{
			fIsLoadingDebugBuild = FALSE;

			MmAllocatePhysicalMemoryExDetour.SetupDetour(*pHandle, "xboxkrnl.exe", 0xBA, MmAllocatePhysicalMemoryExHook);
		}


		// fix some skate debug builds from crashing!
		// sk82_na_zd
		if (MmIsAddressValid((PVOID)0x82740818) && *(__int64 *)0x82740818 == 0x419A00F43D400014)
		{
			*(short *)0x82740818 = 0x4800;
			*(short *)0x82740940 = 0x4800;
		}

		// sk82_na_r
		if (MmIsAddressValid((PVOID)0x82557498) && *(__int64 *)0x82557498 == 0x419A00F43D400014)
		{
			*(short *)0x82557498 = 0x4800;
			*(short *)0x825575C0 = 0x4800;
		}
	}

	return Result;
}


//-------------------------------------------------------------------------------------
// Name: XexpResolveImageImportsHook
// Desc: Checks if the module has any imports to 'xbdm' and if it does then replace it 
//		 with 'HvP2'.
//		 All of our exports are just 'fake' xbdm imports
//-------------------------------------------------------------------------------------
int XexpResolveImageImportsHook(PLDR_DATA_TABLE_ENTRY hModule, int importTable, int r5)
{
	int NumOfStrings = *(int*)(importTable + 8);
	char* Ptr = (char*)importTable + 0x0C;


	for (int i = 0; i < NumOfStrings; i++)
	{
		int Strlen = strlen(Ptr);
		if (GetModuleHandle(Ptr) == 0 || !strcmp(Ptr, "xbdm.xex"))
		{
			DBGSTR("[HvP2][XexpResolveImageImportsHook]: Patching Module %s\n", Ptr);
			ZeroMemory(Ptr, Strlen);
			strcpy(Ptr, "HvP2.xex");
			fIsLoadingDebugBuild = TRUE;
		}

		if (i < (NumOfStrings - 1))
		{
			Ptr += Strlen;
			Ptr = (char*)(((int)Ptr + 4) & 0xFFFFFFFC);
		}
	}
	int Result = XexpResolveImageImportsDetour.CallOriginal(hModule, importTable, r5);
	DBGSTR("[HvP2][XexpResolveImageImportsHook]: 0x%08X\n", Result);
	return Result;
}


//-------------------------------------------------------------------------------------
// Name: SetupHooks()
// Desc: Sets up HV / kernel hooks
//-------------------------------------------------------------------------------------
DWORD WINAPI SetupHooks(LPVOID)
{
	// qwHv is just the first 4 bytes of the HV 
	DWORD dwHvHeader, dwHv = 0x4E4E4497;
	DWORD dwPtr;

	// 0x2AA58 in HV
	QWORD qwHvAddress = 0x800001040002AA58;

	// reset the state
	dwUnloadXEXState = 0;

	// wait 500ms
	Sleep(500);

	// if we have less then 4 letters in our handle name just set the buffer
	if (((PLDR_DATA_TABLE_ENTRY)hModule)->BaseDllName.Length < 16)
	{
		// Patch our module to be called HvP2.xex so xbdm imports resolve to us!
		((PLDR_DATA_TABLE_ENTRY)hModule)->BaseDllName.Length = 16;
		((PLDR_DATA_TABLE_ENTRY)hModule)->BaseDllName.Buffer = L"HvP2.xex";
	}
	else
	{

		// Patch our module to be named HvP2.xex so xbdm imports resolve to us!
		wcscpy((wchar_t*)((PLDR_DATA_TABLE_ENTRY)hModule)->BaseDllName.Buffer, L"HvP2.xex");
		((PLDR_DATA_TABLE_ENTRY)hModule)->BaseDllName.Length = 16;
	}


	DBGSTR("[HvP2]: Reading 4 bytes of HV... ");

	// Read the first 4 bytes of the HV
	dwHvHeader = HvxPeekDWORD(0);

	DBGSTR("Done!\n");

	DBGSTR("[HvP2]: Checking Header... ");


	// check if the 4 bytes are what we are expecting!
	if( dwHvHeader != dwHv )
	{
		DbgPrint("[HvP2]: Unsuported Dash/HV version! (0x%08X != %08X)\n", dwHvHeader, dwHv);
		CreateThread(0, 0, MessageBoxThread, 0, 0, 0);
		Sleep(100);
		return TRUE;
	}

	DBGSTR("Dash Version supported!\n");

	DBGSTR("[HvP2]: Checking HV Addresses!... ");

	// read 0x2AA48 and see if the bytes match up, just to double check RGH3 addresses are right & that we aren't on a devkit with spoofed results
	if (HvxPeekDWORD(qwHvAddress - 0x10) != 0x9161006C)
	{
		DbgPrint("[HvP2]: Unsuported HV!\n", dwHvHeader, dwHv);
		CreateThread(0, 0, MessageBoxThread, (LPVOID)1, 0, 0);
		Sleep(100);
		return TRUE;
	}

	DBGSTR("HV Addresses supported!\n");

	DBGSTR("[HvP2]: Setting up HV Patches!... ");


	// 0x2AA58 in HV
	HvxPokeDWORD(qwHvAddress, 0x60000000); // remove failed 0xC0000225 load reponse in 'HvxResolveImports'


	DBGSTR("Done!\n");

	DBGSTR("[HvP2]: Setting up Kernel Patches!... ");


	// XexpCompleteImageLoad make '0xC0000225' failed to a li r3, 0; blr

	/*
	8007AB78  lis r30,-32762		; 8006h
	8007AB7C  ori r30,r30,5292		; 14ACh
	8007AB80  b 8007A2F4
	*/

	dwPtr = 0x8007AB78;
	*(short *)(dwPtr + 2) = (short)0x8006; //0x800614AC
	*(short *)(dwPtr + 4 + 2) = 0x14AC; // 0x800614AC


	MakeBranchTo(dwPtr + 8, dwPtr - 0xE4); // 0x8007AB80 = b 0x8007AA94

SetupKrnlHooks:

	XexpCompleteLoadDetour.SetupDetour(0x8007D528, XexpCompleteLoadHook);
	XexpResolveImageImportsDetour.SetupDetour(0x80079D48, XexpResolveImageImportsHook);

	// try and hook XexMenu incase we have loaded it.
	TryAndHookXexMenu();

	DBGSTR("Done!\n");

	DBGSTR("\n\n\n");
	DbgPrint("[HvP2]: Made by 'Xx jAmes t xX' for dash 17559\n");
	DbgPrint("[HvP2]: Special thanks to 'Chr0m3 x MoDz' for testing and making me update this app again!\n");

	// so we know we are running!
	dwUnloadXEXState = 1;

	// just a loop incase we need to setup our hooks again
	for (;;)
	{
		char szBuffer[MAX_PATH];
		PLDR_DATA_TABLE_ENTRY hModule;


		// stop this thread because we are unloading!
		if (dwUnloadXEXState == 2)
		{
			// tell the DllMain we have exited!
			dwUnloadXEXState = 3;
			return TRUE;
		}

		// if we are playing an original xbox game check if we have stopped to setup out hooks again!
		if (fIsOrignalXboxGame)
		{
			// get the title module handle
			if (XexPcToFileHeader((PVOID)0x82000000, &hModule))
			{
				// copy the name to our buffer
				WideCharToMultiByte(CP_UTF8, NULL, hModule->BaseDllName.Buffer, -1, (char*)szBuffer, MAX_PATH, NULL, NULL);
				
				// if we aren't playing and orignal xbox1 game then setup hooks!
				if (strstr(szBuffer, "xefu") == 0 && strstr(szBuffer, "xbox.xex") == 0)
				{
					fIsOrignalXboxGame = FALSE;
					DBGSTR("[HvP2]: Setting up kernel hooks again!\n");
					goto SetupKrnlHooks;
				}
			}
		}

		Sleep(500);
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name: DllMain()
// Desc: The function which is called at initialization/termination of the process
//       and thread and whenever LoadLibrary() or FreeLibrary() are called.
//       The DllMain routine should not be used to load another module or call a
//       routine that may block.
//-----------------------------------------------------------------------------
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_DETACH)
	{
		DBGSTR("[HvP2]: DLL_PROCESS_DETACH\n");


		// if we have the main thread running then stop it
		if (dwUnloadXEXState == 1)
		{
			// state 2 is to tell it to exit
			dwUnloadXEXState = 2;

			// wait till the state changes
			while (dwUnloadXEXState == 2)
				Sleep(100);
		}


		// if the messagebox thread is running just wait
		while (fMessageBoxThread == TRUE)
			Sleep(100);

		// take down our hooks!
		XexpCompleteLoadDetour.TakeDownDetour();
		XexpResolveImageImportsDetour.TakeDownDetour();
		MmAllocatePhysicalMemoryExDetour.TakeDownDetour();
		XexMenuStrStrDetour.TakeDownDetour();
		XexMenuGetFilesDetour.TakeDownDetour();

		Sleep(20);
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DBGSTR("[HvP2]: DLL_PROCESS_ATTACH\n");

		// save our module handle
		::hModule = hModule;

		// set the reference count to 1 so we can unload if we want to
		((PLDR_DATA_TABLE_ENTRY)hModule)->LoadCount = 1;

		// start our thread for hooking/unhooking while loading orignal xbox games
		ExCreateThread(0, 0, 0, 0, SetupHooks, 0, EX_CREATE_FLAG_CORE5 | EX_CREATE_FLAG_SYSTEM);
	}
	
	return TRUE;
}

