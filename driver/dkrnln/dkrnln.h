/***************************************************************************************
* AUTHOR : Lo
* DATE   : 2010-2-21
* MODULE : dkrnln.H
*
* IOCTRL Sample Driver
*
* Description:
*		Demonstrates communications between USER and KERNEL.
*
****************************************************************************************
* Copyright (C) 2010 Lo.
****************************************************************************************/
#pragma once 

#include <ntddk.h>
#include <devioctl.h>
#include "common.h"

NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString);

