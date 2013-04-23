/***************************************************************************************
* AUTHOR : Lo
* DATE   : 2010-2-21
* MODULE : dkrnln.C
* 
* Command: 
*	Source of IOCTRL Sample Driver
*
* Description:
*		Demonstrates communications between USER and KERNEL.
*
****************************************************************************************
* Copyright (C) 2010 Lo.
****************************************************************************************/

#include "dkrnln.h"
#include "struct.h"
#include "lib2.h"
#include "fnshare.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if __cplusplus
extern “C” { 
#endif
int _afxForceUSRDLL;
#if __cplusplus
}
#endif


EXTERN_C void WINAPIV ReportAllocMemError(SIZE_T dwBytes)
{

}

EXTERN_C INT WINAPI krnl_ProcessNotifyLib (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
	return 0;
}


EXTERN_C void WINAPIV krnl_MOtherHelp()
{
}

EXTERN_C void WINAPIV krnl_MMessageLoop()
{
}

EXTERN_C void WINAPIV krnl_MReportError(UINT uErrCode)
{
};

EXTERN_C void WINAPIV krnl_MCallDllCmd()
{
};

EXTERN_C __declspec(naked) void WINAPIV krnl_MCallLibCmd()
{
	__asm
	{
		lea		edx, dword ptr [esp + 0x8]
		sub		esp, 0xC
		push		edx
		push		dword ptr [esp + 0x14]
		mov		dword ptr [esp + 0x8], 0
		mov		dword ptr [esp + 0xC], 0
		mov		dword ptr [esp + 0x10], 0
		lea		edx, dword ptr [esp + 0x8]
		push		edx
		call		ebx
		mov		eax, dword ptr [esp + 0xC]
		mov		edx, dword ptr [esp + 0x10]
		mov		ecx, dword ptr [esp + 0x14]
		add		esp, 0x18
			retn
	}
};

EXTERN_C __declspec(naked) void WINAPIV krnl_MCallKrnlLibCmd()
{
	__asm
	{
		lea		eax, dword ptr [esp + 0x8]
		sub		esp, 0xC
		push		eax
		push		dword ptr [esp + 0x14]
		xor		eax, eax
		mov		dword ptr [esp + 0x8], eax
		mov		dword ptr [esp + 0xC], eax
		mov		dword ptr [esp + 0x10], eax
		lea		edx, dword ptr [esp + 0x8]
		push		edx
		call		ebx
		mov		eax, dword ptr [esp + 0xC]
		mov		edx, dword ptr [esp + 0x10]
		mov		ecx, dword ptr [esp + 0x14]
		add		esp, 0x18
		retn
	}
};

EXTERN_C INT WINAPIV krnl_MReadProperty()
{
	return 0;
};

EXTERN_C INT WINAPIV krnl_MWriteProperty()
{
	return 0;
};

const DWORD HEAD_AUTO_MEMORY = 0xABCDEF77;
typedef struct _MEM_HEAD 
{
	DWORD check;
	DWORD size;
}*MEM_NODE_PTR;

MEM_NODE_PTR GetMemoryHead(void* lpMem)
{
	if (lpMem)
	{
		MEM_NODE_PTR p = (MEM_NODE_PTR)( ((ULONG)lpMem) -sizeof(struct _MEM_HEAD) );
		if (p->check == HEAD_AUTO_MEMORY)
		{
			return p;
		}
	}
	return NULL;
}

EXTERN_C PVOID WINAPIV krnl_MMalloc(DWORD dwBytes)
{
	MEM_NODE_PTR lpMem = (MEM_NODE_PTR)ExAllocatePool(PagedPool, sizeof(struct _MEM_HEAD) + dwBytes );
	if ( lpMem == NULL )
	{
		ReportAllocMemError(dwBytes);
		return NULL;
	}
	lpMem->check = HEAD_AUTO_MEMORY;
	lpMem->size = dwBytes;
	return &lpMem[1];
};


#include <PshPack1.h>
#define SystemModuleInformation 11
typedef struct _SYSTEM_MODULE_INFORMATION {
	ULONG Reserved[2];
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT Unknow;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	char ImageName[256];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef struct _SYSTEM_MODULE_INFORMATION_LIST
{
	ULONG Count;
	SYSTEM_MODULE_INFORMATION ModuleList[1];
}SYSTEM_MODULE_INFORMATION_LIST, *PSYSTEM_MODULE_INFORMATION_LIST;
#include <PopPack.h>

BOOL GetInstanceAnsSize(PVOID* pInstance, int* pSize )
{
	ULONG size = 0;
	PSYSTEM_MODULE_INFORMATION pModule;
	void* pdata;
	ZwQuerySystemInformation(SystemModuleInformation, &size, 0, &size);
	if (size)
	{
		pdata = ExAllocatePool(NonPagedPool, size);
		if (pdata)
		{
			if( ZwQuerySystemInformation(SystemModuleInformation, pdata, size, &size) == 0)
			{
				PSYSTEM_MODULE_INFORMATION_LIST pList = (PSYSTEM_MODULE_INFORMATION_LIST)pdata;
				ULONG paddr = (ULONG)&GetInstanceAnsSize;
				ULONG i;
				ULONG Count = pList->Count;
				for (i = 0; i < Count; i++)
				{
					ULONG ImageBase = (ULONG)(pList->ModuleList[i].Base);
					ULONG ImageSize = (ULONG)(pList->ModuleList[i].Size);
					if ( paddr >= ImageBase && paddr <= ImageBase+ImageSize )
					{
						*pInstance = (void*)ImageBase;
						*pSize = ImageSize;
						ExFreePool(pdata);
						return TRUE;
					}
				}
			}
			ExFreePool(pdata);
		}
	}
	return FALSE;
}
BOOL IsInImage(void* pMem)
{
	static void* hInst = 0;
	static long ImageSize = 0;
	ULONG ptr = (ULONG)pMem;
	if (hInst == 0 || ImageSize == 0)
	{
		GetInstanceAnsSize(&hInst, &ImageSize);
	}
	if ( ptr >= (ULONG)(hInst) && ptr <= (ULONG)(hInst)+(ULONG)(ImageSize) )
	{
		return TRUE;
	}
	return FALSE;
}

EXTERN_C void WINAPIV krnl_MFree(PVOID lpMem)
{
	MEM_NODE_PTR hdr = GetMemoryHead(lpMem);
	if (hdr)
	{
		lpMem = hdr;
	}
	if ( lpMem )
	{
		if (IsInImage(lpMem))
		{
			return;
		}
		else
		{
			ExFreePool(lpMem);
		}
	}
};


EXTERN_C PVOID WINAPIV krnl_MRealloc(PVOID lpMem, DWORD dwBytes)
{
	MEM_NODE_PTR ptr = GetMemoryHead(lpMem);
	if (ptr)
	{
		if (dwBytes > ptr->size)
		{
			void* p = krnl_MMalloc(dwBytes);
			if (p == NULL)
			{
				ReportAllocMemError(dwBytes);
				return NULL;
			}
			memcpy(p, lpMem, ptr->size);
			krnl_MFree(lpMem);
			return p;
		}
		return lpMem;
	}
	return NULL;
};


EXTERN_C void WINAPIV krnl_MExitProcess(UINT uExitCode)
{
};

EXTERN_C void WINAPIV krnl_MLoadBeginWin()
{

};
//////////////////////////////////////////////////////////////////////////

#define KRNLN_FUNC(name)	EXTERN_C void WINAPIV krnln_##name(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)

typedef ULONGLONG ARG_VAL;

ARG_VAL ToNumberArg(MDATA_INF* pv)
{
	ARG_VAL result = 0;
	switch( pv->m_dtDataType )
	{
	case SDT_BYTE:
		result = (ARG_VAL)(pv->m_byte);
		break;
	case SDT_SHORT:
		result = (ARG_VAL)(pv->m_short);
		break;
	case SDT_INT:
		result = (ARG_VAL)(pv->m_int);
		break;
	case SDT_INT64:
		result = (ARG_VAL)(pv->m_int64);
		break;
	case SDT_BOOL:
		result = (ARG_VAL)(pv->m_bool);
		break;
	case SDT_DATE_TIME:
		break;
	case SDT_SUB_PTR:
		result = (ARG_VAL)(pv->m_pdwSubCodeAdr);
		break;
	case SDT_TEXT:
	case SDT_FLOAT:
	case SDT_DOUBLE:
	case SDT_BIN:
		break;
	default:
		break;
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
/*
//	到整数
EXTERN_C void WINAPIV krnln_fnToInt(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	switch ( pArgInf[0].m_dtDataType )
	{
	case SDT_BYTE:
		pRetData->m_int = pArgInf[0].m_byte;
		break;
	case SDT_SHORT:
		pRetData->m_int = pArgInf[0].m_short;
		break;
	case SDT_INT:
		pRetData->m_int = pArgInf[0].m_int;
		break;
	case SDT_INT64:
		pRetData->m_int = (INT)pArgInf[0].m_int64;
		break;
	case SDT_FLOAT:
		//pRetData->m_int = (INT)pArgInf[0].m_float;
		break;
	case SDT_DOUBLE:
		//pRetData->m_int = (INT)pArgInf[0].m_double;
		break;
	case SDT_BOOL:
		pRetData->m_int = pArgInf[0].m_bool;
		break;
	case SDT_DATE_TIME:
		break;
	case SDT_TEXT:
		if ( pArgInf[0].m_pText )
		{
			//pRetData->m_int = strtol(pArgInf[0].m_pText, NULL, 10);
		}
		break;
	case SDT_BIN:
		break;
	case SDT_SUB_PTR:
		pRetData->m_int = (INT)pArgInf[0].m_pdwSubCodeAdr;
		break;
	}
}
*/

//	到整数
KRNLN_FUNC(fnToInt)
{
	pRetData->m_int64 = ToNumberArg(&pArgInf[0]);
}

// 位取反
KRNLN_FUNC(fnBNot)
{
	pRetData->m_int64 = ~ToNumberArg(&pArgInf[0]);
}
// 位或
KRNLN_FUNC(fnBOr)
{
	int i;

	pRetData->m_int64 = 0;
	for (i = 0; i < nArgCount; i++)
	{
		pRetData->m_int64 |= ToNumberArg(&pArgInf[i]);
	}
}

// 位与
KRNLN_FUNC(fnBAnd)
{
	int i;
	
	pRetData->m_int64 = -1;
	for (i = 0; i < nArgCount; i++)
	{
		pRetData->m_int64 &= ToNumberArg(&pArgInf[i]);
	}
}

// 位异或
KRNLN_FUNC(fnBXor)
{
	int i;
	
	pRetData->m_int64 = 0;
	for (i = 0; i < nArgCount; i++)
	{
		pRetData->m_int64 ^= ToNumberArg(&pArgInf[i]);
	}
}
