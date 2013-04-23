#include <ntddk.h>

DWORD ReleaseAddr = 0;				//易语言释放资源的函数地址,驱动DriverUnLoad时调用它释放内存
DWORD DriverUnLoad= 0;				//易语言在DriverEntry里面设置的pDriverObj类型中的的DriverUnLoad地址
DWORD DriverBase = 0;				//易语言编译驱动程序加载后的自身驱动基地址
//////////////////////////////////////////////////////////////////////////
MEM_NODE_PTR GetMemoryHead(void* lpMem)
{
	if (lpMem)
	{
		MEM_NODE_PTR p = (MEM_NODE_PTR)( ((ULONG)lpMem) -sizeof(struct _MEM_HEAD) );
		if (p->Check == HEAD_AUTO_MEMORY)
		{
			return p;
		}
	}
	return NULL;
}

EXTERN_C PVOID WINAPIV krnl_MMalloc(DWORD dwBytes)
{
	MEM_NODE_PTR lpMem = NULL;
	if (KeGetCurrentIrql() < DISPATCH_LEVEL)
	{
		lpMem = (MEM_NODE_PTR)ExAllocatePool(PagedPool, sizeof(struct _MEM_HEAD) + dwBytes ); 
	}
	if (lpMem == NULL)
	{
		lpMem = (MEM_NODE_PTR)ExAllocatePool(NonPagedPool, sizeof(struct _MEM_HEAD) + dwBytes );
		if (lpMem == NULL)
		{
			return NULL;
		}
	}
	lpMem->Check = HEAD_AUTO_MEMORY;
	lpMem->Size = dwBytes;
	return &lpMem[1];				//返回 MEM_NODE_PTR 结构后面的地址
}

EXTERN_C void WINAPIV krnl_MFree(PVOID lpMem)
{
	if ( MmIsAddressValid( lpMem))
	{
		MEM_NODE_PTR MemHead = GetMemoryHead(lpMem);
		if (MemHead && MemHead->Size > 0)
		{
			ExFreePool(MemHead);
		}
	}
}

EXTERN_C PVOID WINAPIV krnl_MRealloc(PVOID lpMem, DWORD dwBytes)
{
	if (MmIsAddressValid( lpMem))
	{
		MEM_NODE_PTR MemHead = GetMemoryHead(lpMem);		//取得申请内存的标志
		if (MemHead)
		{
			if (dwBytes > MemHead->Size)
			{
				void* p = krnl_MMalloc(dwBytes);
				if (p)
				{
					memcpy(p, lpMem, MemHead->Size);
					ExFreePool(MemHead);
					return p;
				}
				
			}
			return lpMem;
		}
	}
	return NULL;
}

EXTERN_C __declspec(naked) void WINAPIV krnl_MOtherHelp()
{
	__asm
	{
			PUSH EBX
			PUSH ESI
			PUSH EDI
			CMP EAX,0x3					//判断信息号是否为3
			JNZ L011
			MOV EAX,DWORD PTR SS:[ESP+0x10]
			MOV ReleaseAddr,EAX				//将释放资源的地址赋值给变量
			JMP L004015B4
L011:
			CMP EAX,0x2					//判断信息号是否为2
			JNZ L004015B4
			MOV EDI,01h
			MOV EBX,DWORD PTR SS:[ESP+014h]
			MOV EAX,DriverBase
			MOV ESI,DWORD PTR DS:[EBX]
			CMP EAX,ESI
			JA SHORT L004015B4
			MOV ECX,DWORD PTR DS:[EAX+03Ch]
			MOV ECX,DWORD PTR DS:[ECX+EAX+050h]		//映像大小
			ADD ECX,EAX			
			CMP ESI,ECX
			JA SHORT L004015B4
			MOV EDX,DWORD PTR DS:[ESI]
			LEA ECX,DWORD PTR DS:[ESI+04h]
			TEST EDX,EDX
			MOV EAX,01h
			JLE SHORT L0040158A
			PUSH EBP
			MOV EDI,EDX
L0040157E:
			MOV EBP,DWORD PTR DS:[ECX]
			ADD ECX,04h
			IMUL EAX,EBP
			DEC EDI
			JNZ SHORT L0040157E
			POP EBP
L0040158A:
			IMUL EAX,DWORD PTR SS:[ESP+010h]
			LEA EDI,DWORD PTR DS:[EAX+EDX*04h+04h]
			PUSH EDI
			CALL krnl_MMalloc
			MOV ECX,EDI
			MOV EDI,EAX
			MOV EDX,ECX
			ADD ESP,04h
			SHR ECX,02h
			REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
			MOV ECX,EDX
			AND ECX,03h
			REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
			POP EDI
			MOV DWORD PTR DS:[EBX],EAX
			POP ESI
			POP EBX
			RETN
L004015B4:
			POP EDI
			POP ESI
			POP EBX
			RETN
	}
}


EXTERN_C __declspec(naked) void WINAPIV krnl_MCallKrnlLibCmd()			//调用静态支持库接口
{
	__asm jmp ebx						//EBX为支持库命令地址.这里不要接口,直接调用函数,因为支持库函数声明已改变
}

EXTERN_C __declspec(naked) void WINAPIV krnl_MCallLibCmd()				//调用静态库接口
{
	__asm
	{
			lea		edx, dword ptr [esp + 0x8]
			sub		esp, 0xC
			push	edx
			push	dword ptr [esp + 0x14]
			mov		dword ptr [esp + 0x8], 0
			mov		dword ptr [esp + 0xC], 0
			mov		dword ptr [esp + 0x10], 0
			lea		edx, dword ptr [esp + 0x8]
			push	edx
			call	ebx
			mov		eax, dword ptr [esp + 0xC]
			mov		edx, dword ptr [esp + 0x10]
			mov		ecx, dword ptr [esp + 0x14]
			add		esp, 0x18
			retn;
	}
}

EXTERN_C __declspec(naked) WINAPIV AnyRet(){__asm xor eax,eax __asm retn};

EXTERN_C __declspec(naked) WINAPIV ReportAllocMemError(SIZE_T dwBytes)
{
		__asm jmp AnyRet
}
EXTERN_C __declspec(naked) WINAPIV krnl_MMessageLoop()
{
		__asm jmp AnyRet
}

EXTERN_C __declspec(naked) WINAPIV krnl_MReportError(UINT uErrCode)
{
		__asm jmp AnyRet
}

EXTERN_C __declspec(naked) WINAPIV krnl_MCallDllCmd()				//调用Dll命令
{
		__asm jmp AnyRet
}

EXTERN_C __declspec(naked) WINAPIV krnl_MReadProperty()
{
		__asm jmp AnyRet
}

EXTERN_C __declspec(naked) WINAPIV krnl_MWriteProperty()
{
		__asm jmp AnyRet
}
EXTERN_C __declspec(naked) void WINAPIV krnl_MExitProcess(UINT uExitCode)
{
		__asm jmp AnyRet
}

EXTERN_C __declspec(naked) void WINAPIV krnl_MLoadBeginWin()
{
		__asm jmp AnyRet
}

EXTERN_C __declspec(naked) WINAPI krnl_ProcessNotifyLib (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
		__asm jmp AnyRet
}

/*
EXTERN_C __declspec(naked) void WINAPIV Old_krnl_MCallKrnlLibCmd()		//旧的调用静态支持库转接口
{
	__asm
	{
		lea		eax, dword ptr [esp + 0x8]
			sub		esp, 0xC
			push	eax
			push	dword ptr [esp + 0x14]
			xor		eax, eax
			mov		dword ptr [esp + 0x8], eax
			mov		dword ptr [esp + 0xC], eax
			mov		dword ptr [esp + 0x10], eax
			lea		edx, dword ptr [esp + 0x8]
			push	edx
			call	ebx
			mov		eax, dword ptr [esp + 0xC]
			mov		edx, dword ptr [esp + 0x10]
			mov		ecx, dword ptr [esp + 0x14]
			add		esp, 0x18
			retn
	}
}
*/

BOOL Half2All(const BYTE *pBuf, int in_size, BYTE *pOut, int *out_size) 
{ 
	BYTE   *pBt1={0} , *pBt2={0} , *pBt3={0};
	int		i = 0;
	if(!pBuf || !pOut) 
	{ 
		return FALSE; 
	} 	
	if(*out_size < (in_size * 2 + 1)) 
	{ 
		*out_size = in_size * 2 + 1; 
		return   FALSE; 
	} 
	
	pBt1 = (BYTE*) krnl_MMalloc(in_size + 1);
	if (!pBt1)
	{
		return FALSE;
	}

	pBt2 = pOut; 
	pBt3 = pBt1;

	memset(pBt1, 0 , in_size+1); 
	memcpy(pBt1, pBuf , in_size); 

	for(; i <= in_size ; i++) 
	{ 
		if(*pBt1 == 0x00) 
		{ 
			*pBt2++ = *pBt1++; 
		} 
		else   if(*pBt1 == 0xa3) 
		{ 
			*pBt2++ = *pBt1++; 
			*pBt2++ = *pBt1++; 
		} 
		else   if(*pBt1 < 0xa3) 
		{ 
			*pBt2++ = 0xa3; 
			*pBt2++ = 0x80 + *pBt1++; 
		} 
		else 
		{ 
			*pBt2++ = *pBt1++; 
		} 
	} 
	krnl_MFree(pBt3);
	return   TRUE; 
} 

BOOL All2Half(const BYTE *pBuf, int in_size, BYTE *pOut, int *out_size) 
{ 
	BYTE  *pBt1={0} , *pBt2={0}, *pBt3={0};
	int	   i = 0;
	if(!pBuf || !pOut) 
	{ 
		return  FALSE; 
	} 	
	if(*out_size < (in_size + 1)) 
	{ 
		return  FALSE; 
	} 

	pBt1 = (BYTE*) krnl_MMalloc(in_size + 1); 

	if (!pBt1)
	{
		return FALSE;
	}
	pBt2 = pOut; 
	pBt3 = pBt1;

	memset(pBt1 , 0, in_size + 1); 
	memcpy(pBt1 , pBuf, in_size); 

	for(; i < in_size ; i++) 
	{ 
		if(*pBt1 == 0x00) 
		{ 
			*pBt2++ = *pBt1++; 
		} 
		else   if(*pBt1 == 0xa3) 
		{ 
			 pBt1++; 
			*pBt2++ = *pBt1++ - 0x80; 
		} 
		else 
		{ 
			*pBt2++ = *pBt1++; 
		} 
	} 
	krnl_MFree(pBt3);
	return TRUE; 
}

DOUBLE _pow (DOUBLE Base , DOUBLE Exponent )
{
	DOUBLE deRetn = 0;
	if (Base < 0)
	{
		Base = -Base;
	}
	__asm
	{
			FLD Base
			FLD Exponent
			FYL2X
			FLD ST
			FRNDINT
			FSUBR ST(1),ST
			FXCH ST(1)
			FCHS
			F2XM1
			FLD1
			FADDP ST(1),ST
			FSCALE
			FSTP ST(1)
			FSTP deRetn
	}
	if (Base < 0)
	{
		deRetn = - deRetn;
	}
	return deRetn;
}

double WINAPIV _atof(const char *nptr)
{
    const char *p;
    double dResult = 0;
    //iSymbol记录该浮点数的符号，expSym记录科学计数法的符号，dotSymbol记录访问小数点，sciSymbol
    //记录访问科学计数法，dotNumber记录当前是小数点后第几位，expResult记录科学计数法的幂值

    int iSymbol = 1, expSymbol = 1, dotSymbol = 0, sciSymbol = 0, dotNumber = 0, expResult = 0;

    p = nptr;

    //忽略最前面部分的空格符和制表符
    while(p)
    {
        if(*p == ' ' || *p == '\t')
            continue;
        break;
    }

    //浮点数符号
    if(*p == '+')
    {
        iSymbol = 1;
        ++p;
    }
    else if(*p == '-')
    {
        iSymbol = -1;
        ++p;
    }
    //整数部分处理
    while(p&& dotSymbol == 0 && sciSymbol == 0)
    {
        if(isdigit(*p))
            dResult = dResult*10+(*p-'0');
        else if(*p == '.')
            dotSymbol = 1;
        else if(*p == 'e' || *p == 'E')
            sciSymbol = 1;
        else
            return dResult;
        ++p;
    }
    //小数点形式的double部分
    while(p && dotSymbol == 1 && sciSymbol == 0)
    {
        ++dotNumber;
        if(isdigit(*p))
        {
            int numTemp = dotNumber;
            double dTemp = *p-'0';
            while(numTemp--)
                dTemp /= 10;
            dResult += dTemp;
        }
        else if(*p == 'e' || *p == 'E')
            sciSymbol = 1;
        else
            return dResult;

        ++p;
    }
    //科学技术形式
    if(sciSymbol == 1)
    {
        if(*p == '+')
        {
            ++p;
            expSymbol = 1;
        }
        else if(*p == '-')
        {
            ++p;
            expSymbol = -1;
        }
        while(*p != ' ')
        {
            if(isdigit(*p))
                expResult = expResult*10+(*p-'0');
            else
                break;
            ++p;
        }
    }
    //综合结果
    if(expSymbol == 1)

        while(expResult--)
            dResult *= 10;
    else if(expSymbol == -1)
        while(expResult--)
            dResult /= 10;
    return (double)dResult*iSymbol;
}

double WINAPIV _strtod(const char *nptr, char **endptr)
{
	return _atof(nptr);
}

void WINAPIV reverse(char *s)
{
	int i , j;
	int len = strlen(s);
	for( i = 0 , j = len - 1 ; i < j ; i++ , j--)
		s[i]^=s[j]^=s[i]^=s[j];
}

void WINAPIV _ftoa(char *s, double n, int digits)				//digits 为精度长度,保留小数点后面多少位
{
	int i = 0 , p = 0 , cur_d = 0;
	const double eps = 1e-12;
	BOOL sign = TRUE;
	long long t = 0;
	memset(s , 0 , sizeof(s));
	if( n < 0 ) 
	{ 
		sign = FALSE;
		n = -n;
	}
	t = (long long)n;
	n = n - t;
	if( t == 0)
	{
		s[i++] = '0';
	}
	else
	{
		while( t != 0 )
		{
			p =(INT) t % 10;
			t /= 10;
			s[i++] = p + '0';
		}
		if(!sign) s[i++] = '-';
		reverse(s);
	}
	if( fabs( n - 0.0 ) > eps )
	{
		s[i++] = '.';
		cur_d = 0;
		while( cur_d < digits)
		{
			cur_d++;
			n *= 10.0;
			p =(INT)(long long)n;
			n = n - p;
			s[i++] = p + '0';
		}
	}
	s[i] = '\0';
 }

__declspec(naked) INT WINAPIV _ftol(float f)
{
	__asm
	{
			PUSH EBP
			MOV EBP,ESP
			ADD ESP,-0Ch
			WAIT
			FSTCW WORD PTR SS:[EBP-02h]
			WAIT
			MOV AX,WORD PTR SS:[EBP-02h]
			OR AH,0Ch
			MOV WORD PTR SS:[EBP-04h],AX
			FLDCW WORD PTR SS:[EBP-04h]
			FISTP QWORD PTR SS:[EBP-0Ch]
			FLDCW WORD PTR SS:[EBP-02h]
			MOV EAX,DWORD PTR SS:[EBP-0Ch]
			MOV EDX,DWORD PTR SS:[EBP-08h]
			LEAVE
			RETN
	}
}

double WINAPIV _modf(double x, double *iptr) 

{
	double ret = fmod(x,1.0);
	*iptr = x - ret;
	return ret;
}

double f = 1.157407407407407E-05;
__declspec(naked) double WINAPIV TimeToInt64(signed int Year, int Month, signed int Day, int Hour, int Minute, signed int Second)
{
	__asm
	{
		SUB ESP,030h
		MOV EAX,DWORD PTR SS:[ESP+040h]
		PUSH EBX
		MOV EBX,DWORD PTR SS:[ESP+038h]
		PUSH EBP
		LEA EAX,DWORD PTR DS:[EAX+EAX*04h]
		MOV EBP,DWORD PTR SS:[ESP+044h]
		PUSH ESI
		PUSH EDI
		LEA EAX,DWORD PTR DS:[EAX+EAX*04h]
		MOV EDX,01Eh
		LEA EAX,DWORD PTR DS:[EAX+EAX*08h]
		SHL EAX,04h
		MOV DWORD PTR SS:[ESP+050h],EAX
		MOV EAX,DWORD PTR SS:[ESP+054h]
		FILD DWORD PTR SS:[ESP+050h]
		MOV ECX,EAX
		SHL ECX,04h
		SUB ECX,EAX
		SHL ECX,02h
		MOV DWORD PTR SS:[ESP+050h],ECX
		CMP EBX,076Bh
		FILD DWORD PTR SS:[ESP+050h]
		FADDP ST(1),ST
		FIADD DWORD PTR SS:[ESP+058h]
		FMUL f
		JNZ SHORT L004012DB
		CMP DWORD PTR SS:[ESP+048h],0Ch
		JNZ SHORT L004012D6
		CMP EBP,EDX
		JL SHORT L004012D6
		ADD EBP,-01Eh
		POP EDI
		MOV DWORD PTR SS:[ESP+044h],EBP
		POP ESI
		FILD DWORD PTR SS:[ESP+040h]
		POP EBP
		POP EBX
		FADDP ST(1),ST
		ADD ESP,030h
		RETN
L004012D6:
		OR EDI,0FFFFFFFFh
		JMP SHORT L004012ED
L004012DB:
		XOR EAX,EAX
		CMP EBX,076Bh
		SETLE AL
		DEC EAX
		AND EAX,02h
		DEC EAX
		MOV EDI,EAX
L004012ED:
		MOV ECX,EDI
		XOR ESI,ESI
		DEC ECX
		MOV DWORD PTR SS:[ESP+050h],EDI
		NEG ECX
		SBB ECX,ECX
		ADD ECX,076Ch
L00401300:
		CMP ECX,EBX
		JE SHORT L0040134E
		MOV EAX,ECX
		MOV EBP,0190h
		CDQ
		IDIV EBP
		MOV EDI,016Dh
		TEST EDX,EDX
		JE SHORT L00401336
		MOV EAX,ECX
		MOV EBP,064h
		CDQ
		IDIV EBP
		TEST EDX,EDX
		JE SHORT L0040133B
		MOV EDX,ECX
		AND EDX,080000003h
		JNS SHORT L00401334
		DEC EDX
		OR EDX,0FFFFFFFCh
		INC EDX
L00401334:
		JNZ SHORT L0040133B
L00401336:
		MOV EDI,016Eh
L0040133B:
		MOV EBP,DWORD PTR SS:[ESP+04Ch]
		ADD ESI,EDI
		MOV EDI,DWORD PTR SS:[ESP+050h]
		MOV EDX,01Eh
		ADD ECX,EDI
		JMP SHORT L00401300
L0040134E:
		MOV EAX,01Fh
		MOV DWORD PTR SS:[ESP+01Ch],EDX
		MOV DWORD PTR SS:[ESP+010h],EAX
		MOV DWORD PTR SS:[ESP+018h],EAX
		MOV DWORD PTR SS:[ESP+020h],EAX
		MOV DWORD PTR SS:[ESP+028h],EAX
		MOV DWORD PTR SS:[ESP+02Ch],EAX
		MOV DWORD PTR SS:[ESP+034h],EAX
		MOV DWORD PTR SS:[ESP+03Ch],EAX
		MOV EAX,EBX
		MOV DWORD PTR SS:[ESP+024h],EDX
		MOV DWORD PTR SS:[ESP+030h],EDX
		MOV DWORD PTR SS:[ESP+038h],EDX
		MOV ECX,0190h
		CDQ
		IDIV ECX
		MOV DWORD PTR SS:[ESP+014h],01Ch
		TEST EDX,EDX
		JE SHORT L004013B2
		MOV EAX,EBX
		MOV ECX,064h
		CDQ
		IDIV ECX
		TEST EDX,EDX
		JE SHORT L004013BA
		AND EBX,080000003h
		JNS SHORT L004013B0
		DEC EBX
		OR EBX,0FFFFFFFCh
		INC EBX
L004013B0:
		JNZ SHORT L004013BA
L004013B2:
		MOV DWORD PTR SS:[ESP+014h],01Dh
L004013BA:
		CMP EDI,01h
		JNZ SHORT L00401403
		MOV EDX,DWORD PTR SS:[ESP+048h]
		XOR EAX,EAX
		DEC EDX
		LEA ECX,DWORD PTR SS:[ESP+010h]
L004013CA:
		CMP EAX,EDX
		JE SHORT L004013EE
		MOV EBX,DWORD PTR DS:[ECX]
		ADD ECX,04h
		ADD ESI,EBX
		INC EAX
		CMP EAX,0Ch
		JL SHORT L004013CA
		INC ESI
		POP EDI
		MOV DWORD PTR SS:[ESP+044h],ESI
		POP ESI
		FILD DWORD PTR SS:[ESP+040h]
		POP EBP
		POP EBX
		FADDP ST(1),ST
		ADD ESP,030h
		RETN
L004013EE:
		ADD ESI,EBP
		POP EDI
		INC ESI
		MOV DWORD PTR SS:[ESP+044h],ESI
		POP ESI
		FILD DWORD PTR SS:[ESP+040h]
		POP EBP
		POP EBX
		FADDP ST(1),ST
		ADD ESP,030h
		RETN
L00401403:
		MOV ECX,DWORD PTR SS:[ESP+048h]
		MOV EAX,0Bh
		LEA EDX,DWORD PTR DS:[ECX-01h]
		LEA ECX,DWORD PTR SS:[ESP+03Ch]
L00401413:
		CMP EAX,EDX
		JE SHORT L00401438
		MOV EBX,DWORD PTR DS:[ECX]
		SUB ECX,04h
		ADD ESI,EBX
		DEC EAX
		TEST EAX,EAX
		JGE SHORT L00401413
		DEC ESI
		POP EDI
		MOV DWORD PTR SS:[ESP+044h],ESI
		POP ESI
		FILD DWORD PTR SS:[ESP+040h]
		POP EBP
		POP EBX
		FADDP ST(1),ST
		FCHS
		ADD ESP,030h
		RETN
L00401438:
		MOV EDX,DWORD PTR SS:[ESP+EAX*04h+010h]
		POP EDI
		SUB EDX,EBP
		ADD ESI,EDX
		DEC ESI
		MOV DWORD PTR SS:[ESP+044h],ESI
		POP ESI
		FILD DWORD PTR SS:[ESP+040h]
		POP EBP
		POP EBX
		FADDP ST(1),ST
		FCHS
		ADD ESP,030h
		RETN
	}
}
double yf1 = 0.0;
double yf2 = 1.000000000000000;
double yf3 = 30.00000000000000;
__declspec(naked) INT WINAPIV GetYearMonthDay ()
{
	__asm
	{
		FLD QWORD PTR SS:[ESP+04h]
		FCOM yf1
		SUB ESP,030h
		PUSH EDI
		FSTSW AX
		TEST AH,040h
		JNZ L00401855
		FCOM yf2
		FSTSW AX
		TEST AH,040h
		JNZ L00401855
		FCOM yf1
		FSUB yf2
		PUSH EBX
		PUSH EBP
		PUSH ESI
		FSTSW AX
		TEST AH,041h
		JNZ SHORT L00401753
		CALL _ftol
		MOV EDI,EAX
		MOV ECX,076Ch
		MOV EBX,01h
		JMP SHORT L00401764
L00401753:
		FABS
		CALL _ftol
		MOV EDI,EAX
		MOV ECX,076Bh
		OR EBX,0FFFFFFFFh
L00401764:
		MOV EAX,ECX
		MOV EBP,0190h
		CDQ
		IDIV EBP
		MOV ESI,016Dh
		TEST EDX,EDX
		JE SHORT L00401795
		MOV EAX,ECX
		MOV EBP,064h
		CDQ
		IDIV EBP
		TEST EDX,EDX
		JE SHORT L0040179A
		MOV EAX,ECX
		AND EAX,080000003h
		JNS SHORT L00401793
		DEC EAX
		OR EAX,0FFFFFFFCh
		INC EAX
L00401793:
		JNZ SHORT L0040179A
L00401795:
		MOV ESI,016Eh
L0040179A:
		CMP EDI,ESI
		JLE SHORT L004017A4
		SUB EDI,ESI
		ADD ECX,EBX
		JMP SHORT L00401764
L004017A4:
		MOV EDX,DWORD PTR SS:[ESP+04Ch]
		CMP EBX,-01h
		MOV DWORD PTR DS:[EDX],ECX
		JNZ SHORT L004017B3
		SUB ESI,EDI
		MOV EDI,ESI
L004017B3:
		MOV EAX,01Fh
		MOV EDX,01Eh
		MOV DWORD PTR SS:[ESP+010h],EAX
		MOV DWORD PTR SS:[ESP+018h],EAX
		MOV DWORD PTR SS:[ESP+020h],EAX
		MOV DWORD PTR SS:[ESP+028h],EAX
		MOV DWORD PTR SS:[ESP+02Ch],EAX
		MOV DWORD PTR SS:[ESP+034h],EAX
		MOV DWORD PTR SS:[ESP+03Ch],EAX
		MOV EAX,ECX
		MOV DWORD PTR SS:[ESP+01Ch],EDX
		MOV DWORD PTR SS:[ESP+024h],EDX
		MOV DWORD PTR SS:[ESP+030h],EDX
		MOV DWORD PTR SS:[ESP+038h],EDX
		CDQ
		MOV ESI,0190h
		MOV DWORD PTR SS:[ESP+014h],01Ch
		IDIV ESI
		TEST EDX,EDX
		JE SHORT L0040181C
		MOV EAX,ECX
		MOV ESI,064h
		CDQ
		IDIV ESI
		TEST EDX,EDX
		JE SHORT L00401824
		AND ECX,080000003h
		JNS SHORT L0040181A
		DEC ECX
		OR ECX,0FFFFFFFCh
		INC ECX
L0040181A:
		JNZ SHORT L00401824
L0040181C:
		MOV DWORD PTR SS:[ESP+014h],01Dh
L00401824:
		POP ESI
		POP EBP
		XOR EAX,EAX
		LEA EDX,DWORD PTR SS:[ESP+08h]
		POP EBX
L0040182D:
		MOV ECX,DWORD PTR DS:[EDX]
		CMP EDI,ECX
		JBE SHORT L00401843
		SUB EDI,ECX
		INC EAX
		ADD EDX,04h
		CMP EAX,0Ch
		JL SHORT L0040182D
		POP EDI
		ADD ESP,030h
		RETN
L00401843:
		MOV ECX,DWORD PTR SS:[ESP+044h]
		MOV EDX,DWORD PTR SS:[ESP+048h]
		INC EAX
		MOV DWORD PTR DS:[ECX],EAX
		MOV DWORD PTR DS:[EDX],EDI
		POP EDI
		ADD ESP,030h
		RETN
L00401855:
		MOV EAX,DWORD PTR SS:[ESP+040h]
		MOV ECX,DWORD PTR SS:[ESP+044h]
		FADD yf3
		MOV DWORD PTR DS:[EAX],076Bh
		MOV DWORD PTR DS:[ECX],0Ch
		CALL _ftol
		MOV EDX,DWORD PTR SS:[ESP+048h]
		POP EDI
		MOV DWORD PTR DS:[EDX],EAX
		ADD ESP,030h
		RETN
	}
}

double hf1 = 86400.00000000000;
double hf2 = 0.5000000000000000;
__declspec(naked) INT WINAPIV GetHourMinuteSecond ()
{
	__asm
	{
			FLD QWORD PTR SS:[ESP+04h]
			FABS
			FMUL hf1
			FADD hf2
			CALL _ftol
			MOV ECX,EAX
			MOV EAX,091A2B3C5h
			IMUL ECX
			ADD EDX,ECX
			SAR EDX,0Bh
			MOV EAX,EDX
			SHR EAX,01Fh
			ADD EDX,EAX
			MOV EAX,DWORD PTR SS:[ESP+0Ch]
			MOV DWORD PTR DS:[EAX],EDX
			MOV EAX,ECX
			CDQ
			MOV ECX,0E10h
			IDIV ECX
			MOV EAX,088888889h
			MOV ECX,EDX
			IMUL ECX
			ADD EDX,ECX
			SAR EDX,05h
			MOV EAX,EDX
			SHR EAX,01Fh
			ADD EDX,EAX
			MOV EAX,DWORD PTR SS:[ESP+010h]
			MOV DWORD PTR DS:[EAX],EDX
			MOV EAX,ECX
			CDQ
			MOV ECX,03Ch
			IDIV ECX
			MOV EAX,DWORD PTR SS:[ESP+014h]
			MOV DWORD PTR DS:[EAX],EDX
			RETN
	}
}

__declspec(naked) __int64 WINAPI ToTime(char* Text)			//将文本时间转换为长整数时间
{
	__asm
	{
		PUSH EBP
		MOV EBP,ESP
		SUB ESP,024h
		MOV DWORD PTR SS:[EBP-04h],0h
		MOV DWORD PTR SS:[EBP-08h],0h
		MOV DWORD PTR SS:[EBP-0Ch],0h
		MOV DWORD PTR SS:[EBP-010h],0h
		MOV DWORD PTR SS:[EBP-014h],0h
		MOV DWORD PTR SS:[EBP-018h],0h
		MOV DWORD PTR SS:[EBP-01Ch],0h
		MOV DWORD PTR SS:[EBP-024h],0h
		MOV DWORD PTR SS:[EBP-020h],0h
		PUSH ESI
		PUSH EBX
		PUSH EDI
		MOV EAX,DWORD PTR SS:[EBP+08h]
		TEST EAX,EAX
		JE L0040133A
		CMP BYTE PTR DS:[EAX],0h
		JE L0040133A
		LEA EBX,DWORD PTR SS:[EBP-01Ch]
		XOR EDX,EDX
		XOR EDI,EDI
L00401104:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		MOV BYTE PTR SS:[EBP-04h],CL
		JE SHORT L00401130
		CMP CL,030h
		JB SHORT L00401130
		CMP CL,039h
		JA SHORT L00401130
		LEA ECX,DWORD PTR DS:[EDI+EDI*04h]
		MOV EDI,DWORD PTR SS:[EBP-04h]
		AND EDI,0FFh
		INC EAX
		INC EDX
		LEA EDI,DWORD PTR DS:[EDI+ECX*02h-030h]
		CMP EDX,03h
		MOV DWORD PTR DS:[EBX],EDI
		JLE SHORT L00401104
L00401130:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		JE SHORT L0040115E
		CMP CL,02Fh
		JE SHORT L0040115D
		CMP CL,02Dh
		JE SHORT L0040115D
		CMP CL,03Ah
		JE SHORT L0040115D
		CMP CL,02Eh
		JE SHORT L0040115D
		XOR EDX,EDX
		MOV DH,CL
		MOV DL,BYTE PTR DS:[EAX+01h]
		CMP DX,0C4EAh
		JNZ SHORT L0040115E
		ADD EAX,02h
		JMP SHORT L0040115E
L0040115D:
		INC EAX
L0040115E:
		XOR EDX,EDX
		XOR EDI,EDI
L00401162:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		MOV BYTE PTR SS:[EBP-04h],CL
		JE SHORT L0040118F
		CMP CL,030h
		JB SHORT L0040118F
		CMP CL,039h
		JA SHORT L0040118F
		LEA ECX,DWORD PTR DS:[EDI+EDI*04h]
		MOV EDI,DWORD PTR SS:[EBP-04h]
		AND EDI,0FFh
		INC EAX
		INC EDX
		LEA EDI,DWORD PTR DS:[EDI+ECX*02h-030h]
		CMP EDX,01h
		MOV DWORD PTR DS:[EBX+04h],EDI
		JLE SHORT L00401162
L0040118F:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		JE SHORT L004011BF
		CMP CL,02Fh
		JE SHORT L004011BE
		CMP CL,02Dh
		JE SHORT L004011BE
		CMP CL,03Ah
		JE SHORT L004011BE
		CMP CL,02Eh
		JE SHORT L004011BE
		XOR EDX,EDX
		MOV DH,CL
		MOV DL,BYTE PTR DS:[EAX+01h]
		CMP DX,0D4C2h
		JNZ SHORT L004011BF
		ADD EAX,02h
		XOR EDX,EDX
		JMP SHORT L004011BF
L004011BE:
		INC EAX
L004011BF:
		XOR EDX,EDX
		XOR EDI,EDI
L004011C3:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		MOV BYTE PTR SS:[EBP-04h],CL
		JE SHORT L004011F0
		CMP CL,030h
		JB SHORT L004011F0
		CMP CL,039h
		JA SHORT L004011F0
		LEA ECX,DWORD PTR DS:[EDI+EDI*04h]
		MOV EDI,DWORD PTR SS:[EBP-04h]
		AND EDI,0FFh
		INC EAX
		INC EDX
		LEA EDI,DWORD PTR DS:[EDI+ECX*02h-030h]
		CMP EDX,01h
		MOV DWORD PTR DS:[EBX+08h],EDI
		JLE SHORT L004011C3
L004011F0:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		JE SHORT L00401225
		CMP CL,02Fh
		JE SHORT L00401224
		CMP CL,02Dh
		JE SHORT L00401224
		CMP CL,03Ah
		JE SHORT L00401224
		CMP CL,020h
		JE SHORT L00401224
		CMP CL,02Eh
		JE SHORT L00401224
		XOR EDX,EDX
		MOV DH,CL
		MOV DL,BYTE PTR DS:[EAX+01h]
		CMP DX,0C8D5h
		JNZ SHORT L00401225
		ADD EAX,02h
		XOR EDX,EDX
		JMP SHORT L00401225
L00401224:
		INC EAX
L00401225:
		XOR EDX,EDX
		XOR EDI,EDI
L00401229:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		MOV BYTE PTR SS:[EBP-04h],CL
		JE SHORT L00401256
		CMP CL,030h
		JB SHORT L00401256
		CMP CL,039h
		JA SHORT L00401256
		LEA ECX,DWORD PTR DS:[EDI+EDI*04h]
		MOV EDI,DWORD PTR SS:[EBP-04h]
		AND EDI,0FFh
		INC EAX
		INC EDX
		LEA EDI,DWORD PTR DS:[EDI+ECX*02h-030h]
		CMP EDX,01h
		MOV DWORD PTR DS:[EBX+0Ch],EDI
		JLE SHORT L00401229
L00401256:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		JE SHORT L00401286
		CMP CL,02Fh
		JE SHORT L00401285
		CMP CL,02Dh
		JE SHORT L00401285
		CMP CL,03Ah
		JE SHORT L00401285
		CMP CL,02Eh
		JE SHORT L00401285
		XOR EDX,EDX
		MOV DH,CL
		MOV DL,BYTE PTR DS:[EAX+01h]
		CMP DX,0CAB1h
		JNZ SHORT L00401286
		ADD EAX,02h
		XOR EDX,EDX
		JMP SHORT L00401286
L00401285:
		INC EAX
L00401286:
		XOR EDX,EDX
		XOR EDI,EDI
L0040128A:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		MOV BYTE PTR SS:[EBP-04h],CL
		JE SHORT L004012B7
		CMP CL,030h
		JB SHORT L004012B7
		CMP CL,039h
		JA SHORT L004012B7
		LEA ECX,DWORD PTR DS:[EDI+EDI*04h]
		MOV EDI,DWORD PTR SS:[EBP-04h]
		AND EDI,0FFh
		INC EAX
		INC EDX
		LEA EDI,DWORD PTR DS:[EDI+ECX*02h-030h]
		CMP EDX,01h
		MOV DWORD PTR DS:[EBX+010h],EDI
		JLE SHORT L0040128A
L004012B7:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		JE SHORT L004012E7
		CMP CL,02Fh
		JE SHORT L004012E6
		CMP CL,02Dh
		JE SHORT L004012E6
		CMP CL,03Ah
		JE SHORT L004012E6
		CMP CL,02Eh
		JE SHORT L004012E6
		XOR EDX,EDX
		MOV DH,CL
		MOV DL,BYTE PTR DS:[EAX+01h]
		CMP DX,0B7D6h
		JNZ SHORT L004012E7
		ADD EAX,02h
		XOR EDX,EDX
		JMP SHORT L004012E7
L004012E6:
		INC EAX
L004012E7:
		XOR EDX,EDX
		XOR EDI,EDI
L004012EB:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		MOV BYTE PTR SS:[EBP-04h],CL
		JE SHORT L00401318
		CMP CL,030h
		JB SHORT L00401318
		CMP CL,039h
		JA SHORT L00401318
		LEA ECX,DWORD PTR DS:[EDI+EDI*04h]
		MOV EDI,DWORD PTR SS:[EBP-04h]
		AND EDI,0FFh
		INC EAX
		INC EDX
		LEA EDI,DWORD PTR DS:[EDI+ECX*02h-030h]
		CMP EDX,01h
		MOV DWORD PTR DS:[EBX+014h],EDI
		JLE SHORT L004012EB
L00401318:
		MOV CL,BYTE PTR DS:[EAX]
		TEST CL,CL
		JE SHORT L0040132C
		XOR EDX,EDX
		MOV DH,CL
		MOV DL,BYTE PTR DS:[EAX+01h]
		CMP DX,0C3EBh
		JNZ SHORT L0040133A
L0040132C:
		PUSH DWORD PTR SS:[EBP-08h]
		PUSH DWORD PTR SS:[EBP-0Ch]
		PUSH DWORD PTR SS:[EBP-010h]
		PUSH DWORD PTR SS:[EBP-014h]
		PUSH DWORD PTR SS:[EBP-018h]
		PUSH DWORD PTR SS:[EBP-01Ch]
		CALL TimeToInt64
		ADD ESP,018h
		FSTP QWORD PTR [EBP-024h]
		JMP SHORT L0040134A
L0040133A:
		MOV EDX,0C1241034h
		MOV EAX,0h
		JMP SHORT L00401353
L0040134A:

		MOV EDX,DWORD PTR SS:[EBP-020h]
		MOV EAX,DWORD PTR SS:[EBP-024h]
L00401353:
		POP EBX
		POP EDI
		POP ESI
		MOV ESP,EBP
		POP EBP
		RETN 04h
	}
}

VOID WINAPIV TimeInt64ToTimeFields(IN __int64 Time , OUT PTIME_FIELDS TimeFields)
{
		LARGE_INTEGER LargeInteger;
		LargeInteger.QuadPart = Time;
		RtlTimeToTimeFields(&LargeInteger , TimeFields);	
}
__int64 WINAPIV TimeFieldsToTimeInt64(IN PTIME_FIELDS TimeFields)
{
	MARG_INF MargInf;
	MargInf.m_double = TimeToInt64(
		TimeFields->Year ,
		TimeFields->Month ,
		TimeFields->Day ,
		TimeFields->Hour ,
		TimeFields->Minute,
		TimeFields->Second);
	return MargInf.m_int64;
}
