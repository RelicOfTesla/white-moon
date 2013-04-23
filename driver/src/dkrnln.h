#pragma once 

#include <ntddk.h>
#include <devioctl.h>
#include "common.h"

EXTERN_C NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString);
EXTERN_C VOID WINAPIV EStartup (VOID);

//DriverUnLoad并释放资源
EXTERN_C __declspec(naked) __stdcall DriverUnLoader(IN PDRIVER_OBJECT pDriverObj)		
{
	
	__asm
	{
			push ebp
			mov  ebp,esp
			push ebx
			push esi
			push edi
			cmp DriverUnLoad,0h						//判断DriverUnLoad是否为0
			jz L01			
			mov eax,pDriverObj
			push eax
			call DriverUnLoad						//调用用户设置的DriverUnLoad
L01:
			mov eax,ReleaseAddr
			test eax,eax							//判断释放资源的地址是否为0
			jz L02
			call eax								//释放掉易申请的内存资源				
L02:
			xor eax,eax
			pop edi
			pop esi
			pop ebx
			mov esp,ebp
			pop ebp
			retn 04h
	}
}

//Link链接到的驱动入口
EXTERN_C __declspec(naked) __stdcall DriverLoader(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString)
{
	__asm
	{
			push ebp
			mov  ebp,esp
			push ebx
			push esi
			push edi
			mov ecx,dword ptr ss:[pDriverObj]
			mov edx,dword ptr ds:[ecx + 0x14]							//(PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection
			mov ecx,dword ptr ds:[edx + 0x18]							//(PLDR_DATA_TABLE_ENTRY)->DllBase
			mov DriverBase,ecx											//保存自身驱动基地址
			call EStartup												//调用易的初始化函数
			mov eax,pRegistryString
			push eax
			mov ecx,pDriverObj
			push ecx
			call DriverEntry											//调用易的驱动入口函数
			test eax,eax
			jnz L01
			mov ecx,dword ptr ss:[pDriverObj]
			lea ebx,dword ptr ds:[ecx + 0x34]							//取得DriverEntry设置的DriverUnLoad地址
			mov ecx,dword ptr ds:[ebx]
			test ecx,ecx												//判断是否设置了DriverUnload
			jz L01
			mov DriverUnLoad,ecx										//保存DriverUnLoad地址在DriverUnLoader进行调用
			mov ecx,DriverUnLoader
			mov dword ptr ds:[ebx],ecx									//设置新的DriverUnLoad接口
L01:
			pop edi
			pop esi
			pop ebx
			mov esp,ebp
			pop ebp
			retn 08h
	}
}