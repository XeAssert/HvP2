#ifndef __KEXEXINFO_H
#define __KEXEXINFO_H

// flags for XexLoadImage/XexLoadModule
#define XEX_MODULE_FLAG_TITLE_PROCESS   0x0001
#define XEX_MODULE_FLAG_TITLE_IMPORTS   0x0002
#define XEX_MODULE_FLAG_DEBUGGER        0x0004
#define XEX_MODULE_FLAG_DLL             0x0008
#define XEX_MODULE_FLAG_PATCH           0x0010
#define XEX_MODULE_FLAG_PATCH_FULL      0x0020
#define XEX_MODULE_FLAG_PATCH_DELTA     0x0040
#define XEX_MODULE_FLAG_USER_MODE		0x0080
#define XEX_MODULE_TYPE_TITLE 			(XEX_MODULE_FLAG_TITLE_PROCESS)
#define XEX_MODULE_TYPE_TITLE_DLL 		(XEX_MODULE_FLAG_TITLE_PROCESS | XEX_MODULE_FLAG_DLL)
#define XEX_MODULE_TYPE_SYSTEM_APP 		(XEX_MODULE_FLAG_DLL)
#define XEX_MODULE_TYPE_SYSTEM_DLL 		(XEX_MODULE_FLAG_DLL | XEX_MODULE_FLAG_TITLE_IMPORTS)

#define XEX_MODULE_FLAG_BOUND_PATH      0x40000000
#define XEX_MODULE_FLAG_SILENT_LOAD     0x80000000

typedef enum XEXPRIVS{
	XEX_PRIVILEGE_NO_FORCE_REBOOT					= 0,
	XEX_PRIVILEGE_FOREGROUND_TASKS					= 1,
	XEX_PRIVILEGE_NO_ODD_MAPPING					= 2,
	XEX_PRIVILEGE_HANDLE_MCE_INPUT					= 3,
	XEX_PRIVILEGE_RESTRICT_HUD_FEATURES				= 4,
	XEX_PRIVILEGE_HANDLE_GAMEPAD_DISCONNECT			= 5,
	XEX_PRIVILEGE_INSECURE_SOCKETS					= 6,
	XEX_PRIVILEGE_RESERVED_7						= 7, /*(Xbox1 XSP interop)*/
	XEX_PRIVILEGE_SET_DASH_CONTEXT					= 8,
	XEX_PRIVILEGE_TITLE_USES_GAME_VOICE_CHANNEL		= 9,
	XEX_PRIVILEGE_TITLE_PAL50_INCOMPATIBLE			= 10,
	XEX_PRIVILEGE_TITLE_INSECURE_UTILITYDRIVE		= 11,
	XEX_PRIVILEGE_TITLE_XAM_HOOKS					= 12,
	XEX_PRIVILEGE_TITLE_PII							= 13,
	XEX_PRIVILEGE_CROSSPLATFORM_SYSTEM_LINK			= 14,
	XEX_PRIVILEGE_MULTIDISC_SWAP					= 15,
	XEX_PRIVILEGE_MULTIDISC_INSECURE_MEDIA			= 16,
	XEX_PRIVILEGE_AUTHENTICATION_EX_REQUIRED		= 17,/*(AP25 Media)*/
	XEX_PRIVILEGE_NO_CONFIRM_EXIT					= 18,
	XEX_PRIVILEGE_ALLOW_BACKGROUND_DOWNLOAD			= 19,
	XEX_PRIVILEGE_CREATE_PERSISTABLE_RAMDRIVE		= 20,
	XEX_PRIVILEGE_INHERIT_PERSISTED_RAMDRIVE		= 21,
	XEX_PRIVILEGE_ALLOW_HUD_VIBRATION				= 22,
	XEX_PRIVILEGE_TITLE_BOTH_UTILITY_PARTITIONS		= 23,
	XEX_PRIVILEGE_HANDLE_IPTV_INPUT					= 24,
	XEX_PRIVILEGE_PREFER_BIGBUTTON_INPUT			= 25,
	XEX_PRIVILEGE_RESERVED_26						= 26,
	XEX_PRIVILEGE_MULTIDISC_CROSS_TITLE				= 27,
	XEX_PRIVILEGE_TITLE_INSTALL_INCOMPATIBLE		= 28,
	XEX_PRIVILEGE_ALLOW_AVATAR_GET_METADATA_BY_XUID	= 29,
	XEX_PRIVILEGE_ALLOW_CONTROLLER_SWAPPING			= 30,
	XEX_PRIVILEGE_DASH_EXTENSIBILITY_MODULE			= 31,

	XEX_PRIVILEGE_ALLOW_NETWORK_READ_CANCEL			= 32,
	XEX_PRIVILEGE_UNINTERRUPTABLE_READS				= 33,
	XEX_PRIVILEGE_REQUIRE_EXPERIENCE_FULL			= 34,
	XEX_PRIVILEGE_GAMEVOICE_REQUIRED_UI 			= 35,
	XEX_PRIVILEGE_TITLE_SET_PRESENCE_STRING 		= 36,
	XEX_PRIVILEGE_NATAL_TILT_CONTROL 				= 37,
	XEX_PRIVILEGE_TITLE_REQUIRES_SKELETAL_TRACKING	= 38,
	XEX_PRIVILEGE_TITLE_SUPPORTS_SKELETAL_TRACKING	= 39,
	XEX_PRIVILEGE_USE_LARGE_HDS_FILE_CACHE          = 40,
	XEX_PRIVILEGE_TITLE_SUPPORTS_DEEP_LINK          = 41,
	XEX_PRIVILEGE_TITLE_BODY_PROFILE                = 42,
	XEX_PRIVILEGE_TITLE_WINUSB                      = 43,
	XEX_PRIVILEGE_TITLE_SUPPORTS_DEEP_LINK_REFRESH  = 44,
	XEX_PRIVILEGE_LOCAL_ONLY_SOCKETS                = 45,
	XEX_PRIVILEGE_TITLE_CONTENT_ACQUIRE_AND_DOWNLOAD= 46,

};

typedef struct _CV_INFO_PDB70 {
	DWORD      CvSignature; 
	GUID       Guid;       // unique identifier 
	DWORD      Age;             // an always-incrementing value 
	BYTE       PdbFileName[1];  // zero terminated string with the name of the PDB file 
} CV_INFO_PDB70, *PCV_INFO_PDB70;

typedef struct _PAGE_DESCIPTION {
	DWORD	Size : 28;
	DWORD	Info : 4;
} PAGE_DESCIPTION, *PPAGE_DESCIPTION;
C_ASSERT(sizeof(PAGE_DESCIPTION) == 0x4);

typedef struct _HV_PAGE_INFO { 
	PAGE_DESCIPTION PageDescription; // 0x0 sz:0x4
	BYTE DataDigest[0x14]; // 0x4 sz:0x14
} HV_PAGE_INFO, *PHV_PAGE_INFO; // size 24
C_ASSERT(sizeof(HV_PAGE_INFO) == 0x18);

typedef struct _HV_IMAGE_EXPORT_TABLE { 
	DWORD Magic[3]; // 0x0 sz:0xC
	DWORD ModuleNumber[2]; // 0xC sz:0x8
	DWORD Version[3]; // 0x14 sz:0xC
	DWORD ImageBaseAddress; // 0x20 sz:0x4
	DWORD Count; // 0x24 sz:0x4
	DWORD Base; // 0x28 sz:0x4
} HV_IMAGE_EXPORT_TABLE, *PHV_IMAGE_EXPORT_TABLE; // size 44
C_ASSERT(sizeof(HV_IMAGE_EXPORT_TABLE) == 0x2C);

typedef struct _HV_IMAGE_IMPORT_TABLE { 
	BYTE NextImportDigest[0x14]; // 0x0 sz:0x14
	DWORD ModuleNumber; // 0x14 sz:0x4
	DWORD Version[2]; // 0x18 sz:0x8
	BYTE Unused; // 0x20 sz:0x1
	BYTE ModuleIndex; // 0x21 sz:0x1
	WORD ImportCount; // 0x22 sz:0x2
} HV_IMAGE_IMPORT_TABLE, *PHV_IMAGE_IMPORT_TABLE; // size 36
C_ASSERT(sizeof(HV_IMAGE_IMPORT_TABLE) == 0x24);

typedef struct _XEX_EXECUTION_ID { 
	DWORD MediaID; // 0x0 sz:0x4
	union {
		DWORD Version; // 0x4 sz:0x4
		struct {
			BYTE Pad0; // 0x4 sz:0x1
			BYTE Pad1; // 0x5 sz:0x1
			BYTE TitleUpdateNum; // 0x6 sz:0x1
			BYTE Pad3;// 0x7 sz:0x1
		};
	};
	DWORD BaseVersion; // 0x8 sz:0x4
	union {
		DWORD TitleID; // 0xC sz:0x4
		struct {
			WORD PublisherID; // 0xC sz:0x2
			WORD GameID; // 0xE sz:0x2
		};
	};
	BYTE Platform; // 0x10 sz:0x1
	BYTE ExecutableType; // 0x11 sz:0x1
	BYTE DiscNum; // 0x12 sz:0x1
	BYTE DiscsInSet; // 0x13 sz:0x1
	DWORD SaveGameID; // 0x14 sz:0x4
} XEX_EXECUTION_ID, *PXEX_EXECUTION_ID; // size 24
C_ASSERT(sizeof(XEX_EXECUTION_ID) == 0x18);

typedef struct _XEX_IMPORT_TABLE_ENT {
	DWORD ImportDestAddr;
	DWORD ImportStubAddr;
} XEX_IMPORT_TABLE_ENT, *PXEX_IMPORT_TABLE_ENT;

typedef struct _XEX_IMPORT_TABLE { 
	DWORD TableSize;
	BYTE NextImportDigest[20];
	DWORD ModuleNumber;
	DWORD Version[2];
	BYTE Unused;
	BYTE ModuleIndex;
	WORD ImportCount;
	DWORD ImportStubAddr[1];
} XEX_IMPORT_TABLE, *PXEX_IMPORT_TABLE;

typedef struct _XEX_IMPORT_TABLE_ORG { 
	DWORD TableSize; // 0x0 sz:0x4
	HV_IMAGE_IMPORT_TABLE ImportTable; // 0x4 sz:0x24
} XEX_IMPORT_TABLE_ORG, *PXEX_IMPORT_TABLE_ORG; // size 40
C_ASSERT(sizeof(XEX_IMPORT_TABLE_ORG) == 0x28);

typedef struct _XEX_IMPORT_DESCRIPTOR { 
	DWORD Size; // 0x0 sz:0x4
	DWORD NameTableSize; // 0x4 sz:0x4
	DWORD ModuleCount; // 0x8 sz:0x4
	// nametable is here of nametable size
	// followed by modulecount number of xex import tables
} XEX_IMPORT_DESCRIPTOR, *PXEX_IMPORT_DESCRIPTOR; // size 12
C_ASSERT(sizeof(XEX_IMPORT_DESCRIPTOR) == 0xC);

typedef struct _IMAGE_EXPORT_ADDRESS_TABLE { 
	DWORD Magic[3]; // 48 00 00 00 00 48 56 45 48 00 00 00
	DWORD ModuleNumber[2];
	DWORD Version[3];
	DWORD ImageBaseAddress; // must be <<16 to be accurate
	DWORD Count;
	DWORD Base;
	DWORD ordOffset[1]; // ordOffset[0]+ (ImageBaseAddress<<8) = function offset of ordinal 1
} IMAGE_EXPORT_ADDRESS_TABLE, *PIMAGE_EXPORT_ADDRESS_TABLE;

typedef struct _HV_IMAGE_INFO { 
	BYTE Signature[0x100]; // 0x0 sz:0x100
	DWORD InfoSize; // 0x100 sz:0x4
	DWORD ImageFlags; // 0x104 sz:0x4
	DWORD LoadAddress; // 0x108 sz:0x4
	BYTE ImageHash[0x14]; // 0x10C sz:0x14
	DWORD ImportTableCount; // 0x120 sz:0x4
	BYTE ImportDigest[0x14]; // 0x124 sz:0x14
	BYTE MediaID[0x10]; // 0x138 sz:0x10
	BYTE ImageKey[0x10]; // 0x148 sz:0x10
	DWORD ExportTableAddress; // 0x158 sz:0x4
	BYTE HeaderHash[0x14]; // 0x15C sz:0x14
	DWORD GameRegion; // 0x170 sz:0x4
} HV_IMAGE_INFO, *PHV_IMAGE_INFO; // size 372
C_ASSERT(sizeof(HV_IMAGE_INFO) == 0x174);

typedef struct _XEX_SECURITY_INFO { 
	DWORD Size; // 0x0 sz:0x4
	DWORD ImageSize; // 0x4 sz:0x4
	HV_IMAGE_INFO ImageInfo; // 0x8 sz:0x174
	DWORD AllowedMediaTypes; // 0x17C sz:0x4
	DWORD PageDescriptorCount; // 0x180 sz:0x4
} XEX_SECURITY_INFO, *PXEX_SECURITY_INFO; // size 388
C_ASSERT(sizeof(XEX_SECURITY_INFO) == 0x184);

typedef struct _XEX_HEADER_STRING { 
	DWORD Size; // 0x0 sz:0x4
	BYTE Data[1]; // 0x4 sz:0x1
} XEX_HEADER_STRING, *PXEX_HEADER_STRING; // size 8
C_ASSERT(sizeof(XEX_HEADER_STRING) == 0x8);

typedef struct _IMAGE_XEX_HEADER { 
	DWORD Magic; // 0x0 sz:0x4
	DWORD ModuleFlags; // 0x4 sz:0x4
	DWORD SizeOfHeaders; // 0x8 sz:0x4
	DWORD SizeOfDiscardableHeaders; // 0xC sz:0x4
	DWORD SecurityInfo; // 0x10 sz:0x4
	DWORD HeaderDirectoryEntryCount; // 0x14 sz:0x4
} IMAGE_XEX_HEADER, *PIMAGE_XEX_HEADER; // size 24
C_ASSERT(sizeof(IMAGE_XEX_HEADER) == 0x18);

typedef struct _LDR_DATA_TABLE_ENTRY { 
	LIST_ENTRY InLoadOrderLinks;  // 0x0 sz:0x8
	LIST_ENTRY InClosureOrderLinks;  // 0x8 sz:0x8
	LIST_ENTRY InInitializationOrderLinks; // 0x10 sz:0x8
	PVOID NtHeadersBase; // 0x18 sz:0x4
	PVOID ImageBase; // 0x1C sz:0x4
	DWORD SizeOfNtImage; // 0x20 sz:0x4
	UNICODE_STRING FullDllName; // 0x24 sz:0x8
	UNICODE_STRING BaseDllName; // 0x2C sz:0x8
	DWORD Flags; // 0x34 sz:0x4
	DWORD SizeOfFullImage; // 0x38 sz:0x4
	PVOID EntryPoint; // 0x3C sz:0x4
	WORD LoadCount; // 0x40 sz:0x2
	WORD ModuleIndex; // 0x42 sz:0x2
	PVOID DllBaseOriginal; // 0x44 sz:0x4
	DWORD CheckSum; // 0x48 sz:0x4
	DWORD ModuleLoadFlags; // 0x4C sz:0x4
	DWORD TimeDateStamp; // 0x50 sz:0x4
	PVOID LoadedImports; // 0x54 sz:0x4
	PVOID XexHeaderBase; // 0x58 sz:0x4
	union{
		STRING LoadFileName; // 0x5C sz:0x8
		struct {
			PVOID ClosureRoot; // 0x5C sz:0x4 LDR_DATA_TABLE_ENTRY
			PVOID TraversalParent; // 0x60 sz:0x4 LDR_DATA_TABLE_ENTRY
		} asEntry;
	} inf;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY; // size 100
C_ASSERT(sizeof(LDR_DATA_TABLE_ENTRY) == 0x64);

typedef struct _XEX_GAME_RATINGS {
	BYTE	Ratings[0x40];
} XEX_GAME_RATINGS, *PXEX_GAME_RATINGS;

typedef struct _XEX_LAN_KEY {
	BYTE	Key[0x10];
} XEX_LAN_KEY, *PXEX_LAN_KEY;

typedef struct _IMAGE_XEX_DIRECTORY_ENTRY { 
	DWORD Key; // 0x0 sz:0x4
	DWORD Value; // 0x4 sz:0x4
} IMAGE_XEX_DIRECTORY_ENTRY, *PIMAGE_XEX_DIRECTORY_ENTRY; // size 8
C_ASSERT(sizeof(IMAGE_XEX_DIRECTORY_ENTRY) == 0x8);

typedef struct _XEX_SECTION_HEADER { 
	char SectionName[0x8]; // 0x0 sz:0x8
	DWORD VirtualAddress; // 0x8 sz:0x4
	DWORD VirtualSize; // 0xC sz:0x4
} XEX_SECTION_HEADER, *PXEX_SECTION_HEADER; // size 16
C_ASSERT(sizeof(XEX_SECTION_HEADER) == 0x10);

typedef struct _XEX_SECTION_INFO {
	DWORD			   Size;
	XEX_SECTION_HEADER Section[1];
} XEX_SECTION_INFO, *PXEX_SECTION_INFO;

typedef struct _XEX_DATA_DESCRIPTOR { 
	DWORD Size; // 0x0 sz:0x4
	BYTE DataDigest[0x14]; // 0x4 sz:0x14
} XEX_DATA_DESCRIPTOR, *PXEX_DATA_DESCRIPTOR; // size 24
C_ASSERT(sizeof(XEX_DATA_DESCRIPTOR) == 0x18);

typedef struct _XEX_COMPRESSED_DATA_DESCRIPTOR { 
	DWORD WindowSize; // 0x0 sz:0x4
	XEX_DATA_DESCRIPTOR FirstDescriptor; // 0x4 sz:0x18
} XEX_COMPRESSED_DATA_DESCRIPTOR, *PXEX_COMPRESSED_DATA_DESCRIPTOR; // size 28
C_ASSERT(sizeof(XEX_COMPRESSED_DATA_DESCRIPTOR) == 0x1C);

typedef struct _XEX_RAW_DATA_DESCRIPTOR { 
	DWORD DataSize; // 0x0 sz:0x4
	DWORD ZeroSize; // 0x4 sz:0x4
} XEX_RAW_DATA_DESCRIPTOR, *PXEX_RAW_DATA_DESCRIPTOR; // size 8
C_ASSERT(sizeof(XEX_RAW_DATA_DESCRIPTOR) == 0x8);

#define XEX_DATA_FLAG_ENCRYPTED             0x0001
#define XEX_DATA_FORMAT_NONE                0x0000
#define XEX_DATA_FORMAT_RAW                 0x0001
#define XEX_DATA_FORMAT_COMPRESSED          0x0002
#define XEX_DATA_FORMAT_DELTA_COMPRESSED    0x0003

typedef struct _XEX_FILE_DATA_DESCRIPTOR { 
	DWORD Size; // 0x0 sz:0x4
	WORD Flags; // 0x4 sz:0x2
	WORD Format; // 0x6 sz:0x2
	// union {
		// XEX_DATA_DESCRIPTOR data;
		// XEX_RAW_DATA_DESCRIPTOR raw;
		// XEX_COMPRESSED_DATA_DESCRIPTOR compressed;
	// } fmt;
} XEX_FILE_DATA_DESCRIPTOR, *PXEX_FILE_DATA_DESCRIPTOR; // size 8
C_ASSERT(sizeof(XEX_FILE_DATA_DESCRIPTOR) == 0x8);

typedef struct _XEX_DELTA_PATCH_DESCRIPTOR { 
	DWORD Size; // 0x0 sz:0x4
	DWORD TargetVersion; // 0x4 sz:0x4
	DWORD SourceVersion; // 0x8 sz:0x4
	BYTE DigestSource[0x14]; // 0xC sz:0x14
	BYTE ImageKeySource[0x10]; // 0x20 sz:0x10
	DWORD SizeOfTargetHeaders; // 0x30 sz:0x4
	DWORD DeltaHeadersSourceOffset; // 0x34 sz:0x4
	DWORD DeltaHeadersSourceSize; // 0x38 sz:0x4
	DWORD DeltaHeadersTargetOffset; // 0x3C sz:0x4
	DWORD DeltaImageSourceOffset; // 0x40 sz:0x4
	DWORD DeltaImageSourceSize; // 0x44 sz:0x4
	DWORD DeltaImageTargetOffset; // 0x48 sz:0x4
} XEX_DELTA_PATCH_DESCRIPTOR, *PXEX_DELTA_PATCH_DESCRIPTOR; // size 76
C_ASSERT(sizeof(XEX_DELTA_PATCH_DESCRIPTOR) == 0x4C);

typedef struct _XEX_VITAL_STATS { 
	DWORD CheckSum; // 0x0 sz:0x4
	DWORD TimeDateStamp; // 0x4 sz:0x4
} XEX_VITAL_STATS, *PXEX_VITAL_STATS; // size 8
C_ASSERT(sizeof(XEX_VITAL_STATS) == 0x8);

typedef struct _XEX_CALLCAP_IMPORTS { 
	DWORD BeginFunctionThunkAddress; // 0x0 sz:0x4
	DWORD EndFunctionThunkAddress; // 0x4 sz:0x4
} XEX_CALLCAP_IMPORTS, *PXEX_CALLCAP_IMPORTS; // size 8
C_ASSERT(sizeof(XEX_CALLCAP_IMPORTS) == 0x8);

// #if defined(_M_PPCBE)
// #pragma bitfield_order(push, lsb_to_msb)
// #endif

typedef struct _XEXIMAGE_LIBRARY_VERSION { 
	BYTE LibraryName[0x8]; // 0x0 sz:0x8
	WORD MajorVersion; // 0x8 sz:0x2
	WORD MinorVersion; // 0xA sz:0x2
	WORD BuildVersion; // 0xC sz:0x2
	// union{
		// WORD AsWord;
		// struct {
			WORD DebugBuild : 1; // 0xE bfo:0x15
			WORD ApprovedLibrary : 2; // 0xE bfo:0x13
			WORD XEXVersion : 1; // 0xE bfo:0x12
			WORD Unused2 : 1; // 0xE bfo:0x11
			WORD ToolVersion : 1; // 0xE bfo:0x10
			WORD Unused1 : 2; // 0xE bfo:0x8
			WORD QFEVersion : 8; // 0xE bfo:0x0
		// } AsBits;
	// } bf;
} XEXIMAGE_LIBRARY_VERSION, *PXEXIMAGE_LIBRARY_VERSION; // size 16
C_ASSERT(sizeof(XEXIMAGE_LIBRARY_VERSION) == 0x10);

// #if defined(_M_PPCBE)
// #pragma bitfield_order(pop)
// #endif

typedef struct _XEX_TLS_DATA { 
	DWORD TlsSlotCount; // 0x0 sz:0x4
	PVOID AddressOfRawData; // 0x4 sz:0x4
	DWORD SizeOfRawData; // 0x8 sz:0x4
	DWORD SizeOfTlsData; // 0xC sz:0x4
} XEX_TLS_DATA, *PXEX_TLS_DATA; // size 16
C_ASSERT(sizeof(XEX_TLS_DATA) == 0x10);

typedef struct _XEX_SERVICE_ID_LIST { 
	DWORD Size; // 0x0 sz:0x4
	DWORD CustomServiceIDs[0x4]; // 0x4 sz:0x10
} XEX_SERVICE_ID_LIST, *PXEX_SERVICE_ID_LIST; // size 20
C_ASSERT(sizeof(XEX_SERVICE_ID_LIST) == 0x14);

typedef struct _XEX_KEY_VAULT_PRIVILEGES { 
	QWORD Mask; // 0x0 sz:0x8
	QWORD Match; // 0x8 sz:0x8
} XEX_KEY_VAULT_PRIVILEGES, *PXEX_KEY_VAULT_PRIVILEGES; // size 16
C_ASSERT(sizeof(XEX_KEY_VAULT_PRIVILEGES) == 0x10);

typedef struct _XEX_SYSTEM_TIME_RANGE { 
	QWORD Start; // 0x0 sz:0x8
	QWORD End; // 0x8 sz:0x8
} XEX_SYSTEM_TIME_RANGE, *PXEX_SYSTEM_TIME_RANGE; // size 16
C_ASSERT(sizeof(XEX_SYSTEM_TIME_RANGE) == 0x10);

typedef struct _XEX_PAGE_HEAP_OPTIONS { 
	DWORD Size; // 0x0 sz:0x4
	DWORD Flags; // 0x4 sz:0x4
} XEX_PAGE_HEAP_OPTIONS, *PXEX_PAGE_HEAP_OPTIONS; // size 8
C_ASSERT(sizeof(XEX_PAGE_HEAP_OPTIONS) == 0x8);

typedef struct _XEX_CONSOLE_ID_TABLE { 
	DWORD Size; // 0x0 sz:0x4
	// console IDs here, this is just a regular sized struct
	//BYTE abConsoleId[0x5][(size/5)];
} XEX_CONSOLE_ID_TABLE, *PXEX_CONSOLE_ID_TABLE; // size 4
C_ASSERT(sizeof(XEX_CONSOLE_ID_TABLE) == 0x4);


#define XEX_HEADER_STRUCT(key, struct)	(((key) << 8) | (sizeof (struct) >> 2))
#define XEX_HEADER_FIXED_SIZE(key, size)(((key) << 8) | ((size) >> 2))
#define XEX_HEADER_ULONG(key) 			(((key) << 8) | 1)
#define XEX_HEADER_FLAG(key) 			((key) << 8)
#define XEX_HEADER_SIZEDSTRUCT(key)		(((key) << 8) | 0xFF)
#define XEX_HEADER_STRING_FIELD(key)	XEX_HEADER_SIZEDSTRUCT(key)


// for RtlImageXexHeaderField, RtlImageXexHeaderString
#define XEX_HEADER_SECTION_TABLE			XEX_HEADER_SIZEDSTRUCT(0x0002)
#define XEX_FILE_DATA_DESCRIPTOR_HEADER		XEX_HEADER_SIZEDSTRUCT(0x0003)
#define XEX_PATCH_FILE_BASE_REFERENCE		XEX_HEADER_FIXED_SIZE(0x0004, 0x14)
#define XEX_HEADER_DELTA_PATCH_DESCRIPTOR	XEX_HEADER_SIZEDSTRUCT(5)
#define XEX_HEADER_KEY_VAULT_PRIVS			XEX_HEADER_STRUCT(0x0040, XEX_KEY_VAULT_PRIVILEGES)
#define XEX_HEADER_TIME_RANGE				XEX_HEADER_STRUCT(0x0041, XEX_SYSTEM_TIME_RANGE)
#define XEX_HEADER_CONSOLE_ID_TABLE			XEX_HEADER_SIZEDSTRUCT(0x0042) // lists disallowed console IDs
#define XEX_HEADER_BOUND_PATH				XEX_HEADER_STRING_FIELD(0x0080)
#define XEX_HEADER_DEVICE_ID				XEX_HEADER_FIXED_SIZE(0x0081, 0x14)
#define XEX_HEADER_ORIGINAL_BASE_ADDRESS	XEX_HEADER_ULONG(0x0100)
#define XEX_HEADER_ENTRY_POINT				XEX_HEADER_FLAG(0x0101)
#define XEX_HEADER_PE_BASE					XEX_HEADER_ULONG(0x0102)
#define XEX_HEADER_IMPORTS					XEX_HEADER_SIZEDSTRUCT(0x0103)
#define XEX_HEADER_PE_EXPORTS				XEX_HEADER_STRUCT(0xE104, IMAGE_DATA_DIRECTORY)
#define XEX_HEADER_VITAL_STATS				XEX_HEADER_STRUCT(0x0180, XEX_VITAL_STATS)
#define XEX_HEADER_CALLCAP_IMPORTS			XEX_HEADER_STRUCT(0x0181, XEX_CALLCAP_IMPORTS)
#define XEX_HEADER_FASTCAP_ENABLED			XEX_HEADER_FLAG(0x0182)
#define XEX_HEADER_PE_MODULE_NAME			XEX_HEADER_STRING_FIELD(0x0183)
#define XEX_HEADER_BUILD_VERSIONS			XEX_HEADER_SIZEDSTRUCT(0x0200)
#define XEX_HEADER_TLS_DATA					XEX_HEADER_STRUCT(0x201, XEX_TLS_DATA)
#define XEX_HEADER_STACK_SIZE				XEX_HEADER_FLAG(0x0202)
#define XEX_HEADER_FSCACHE_SIZE				XEX_HEADER_ULONG(0x203)
#define XEX_HEADER_XAPI_HEAP_SIZE			XEX_HEADER_ULONG(0x0204)
#define XEX_HEADER_PAGE_HEAP_SIZE_FLAGS		XEX_HEADER_STRUCT(0x0280, XEX_PAGE_HEAP_OPTIONS)
#define XEX_HEADER_PRIVILEGE(priv)			(XEX_HEADER_FLAG(0x0300)+((priv&~0x1f)<<3))
#define XEX_HEADER_EXECUTION_ID				XEX_HEADER_STRUCT(0x400, XEX_EXECUTION_ID)
#define XEX_HEADER_SERVICE_ID_LIST			XEX_HEADER_SIZEDSTRUCT(0x401)
#define XEX_HEADER_WORKSPACE_SIZE			XEX_HEADER_ULONG(0x0402)
#define XEX_HEADER_GAME_RATINGS				XEX_HEADER_FIXED_SIZE(0x403, XEX_NUMBER_GAME_RATING_SYSTEMS)
#define XEX_HEADER_LAN_KEY					XEX_HEADER_FIXED_SIZE(0x404, XEX_LAN_KEY_SIZE)
#define XEX_HEADER_MSLOGO					XEX_HEADER_SIZEDSTRUCT(0x0405)
#define XEX_HEADER_MULTIDISK_MEDIA_IDS		XEX_HEADER_SIZEDSTRUCT(0x0406)
#define XEX_HEADER_ALTERNATE_TITLE_IDS		XEX_HEADER_SIZEDSTRUCT(0x0407)
#define XEX_HEADER_ADDITIONAL_TITLE_MEM		XEX_HEADER_ULONG(0x0408)


#endif // __KEXEXINFO_H
