#include "struct.h"
#include "lib2.h"
#include "fnshare.h"
#include "kernel.h"
#include "dkrnln.h"

//#pragma comment(lib,"libcntpr.lib")
//#pragma comment(lib,"bufferoverflowK.lib")
//#pragma comment(lib,"ntoskrnl.lib")

//////////////////////////////////////////////////////////////////////////
// 位取反
__declspec(naked) KRNLN_FUNC(fnBNot)
{
	__asm
	{
		MOV EAX,DWORD PTR SS:[ESP+8]
		NOT EAX
		RETN
	}
}
// 位或
__declspec(naked) KRNLN_FUNC(fnBOr)
{
	__asm
	{
		MOV EDX,DWORD PTR SS:[ESP+0x4]
		MOV EAX,DWORD PTR SS:[ESP+0x8]
		CMP EDX,1
		JLE L013
		PUSH ESI
		LEA ECX,DWORD PTR SS:[ESP+0x18]
		DEC EDX
L007:
		MOV ESI,DWORD PTR DS:[ECX]
		ADD ECX,0xC
		OR EAX,ESI
		DEC EDX
		JNZ L007
		POP ESI
L013:
		RETN
	}
}

// 位与
__declspec(naked) KRNLN_FUNC(fnBAnd)
{
	__asm
	{
		MOV EDX,DWORD PTR SS:[ESP+0x4]
		MOV EAX,DWORD PTR SS:[ESP+0x8]
		CMP EDX,1
		JLE L013
		PUSH ESI
		LEA ECX,DWORD PTR SS:[ESP+0x18]
		DEC EDX
L007:
		MOV ESI,DWORD PTR DS:[ECX]
		ADD ECX,0xC
		AND EAX,ESI
		DEC EDX
		JNZ L007
		POP ESI
L013:
		RETN
	}
	
}

__declspec(naked) KRNLN_FUNC(fnBXor)  //位异或
{
	__asm
	{
		MOV EDX,DWORD PTR SS:[ESP+04h]
		MOV EAX,DWORD PTR SS:[ESP+08h]
		CMP EDX,01h
		JLE SHORT L004011FE
		PUSH ESI
		LEA ECX,DWORD PTR SS:[ESP+018h]
		DEC EDX
L004011F3:
		MOV ESI,DWORD PTR DS:[ECX]
		ADD ECX,0Ch
		XOR EAX,ESI
		DEC EDX
		JNZ SHORT L004011F3
		POP ESI
L004011FE:
		RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnSHL)   //左移
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+08h]
			MOV ECX,DWORD PTR SS:[ESP+014h]
			SHL EAX,CL
			RETN
	}
}
__declspec(naked) KRNLN_FUNC(fnSHR)   //右移
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+08h]
			MOV ECX,DWORD PTR SS:[ESP+014h]
			SAR EAX,CL
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnMAKELONG)  //合并整数
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+014h]
			MOV ECX,DWORD PTR SS:[ESP+08h]
			AND EAX,0FFFFh
			AND ECX,0FFFFh
			SHL EAX,010h
			OR EAX,ECX
			RETN
	}
}
__declspec(naked) KRNLN_FUNC(fnMAKEWORD)  //合并短整数
{
	__asm
	{
			XOR EAX,EAX
			MOV AH,BYTE PTR SS:[ESP+014h]
			MOV AL,BYTE PTR SS:[ESP+08h]
			RETN
	}
}

//	〈双精度小数型〉 取绝对值 （双精度小数型 欲取其绝对值的数值）
__declspec(naked) KRNLN_FUNC(fnAbs)
{
	__asm
	{
			PUSH EBP
			MOV EBP,ESP
			FLD QWORD PTR SS:[EBP+0Ch]
			FABS
			FSTP QWORD PTR SS:[EBP+0Ch]
			MOV EAX,DWORD PTR SS:[EBP+0Ch]
			MOV EDX,DWORD PTR SS:[EBP+010h]
			POP EBP
			RETN
	}
}

//	〈整数型〉 取整 （双精度小数型 欲取整的小数）
EXTERN_C INT WINAPIV krnln_fnInt(INT nArgCount, MDATA_INF ArgInf,...)
{
	return (INT)ArgInf.m_double;
}

//	〈整数型〉 绝对取整 （双精度小数型 欲取整的小数）
EXTERN_C INT WINAPIV krnln_fnFix(INT nArgCount, MDATA_INF ArgInf,...)
{
	if (ArgInf.m_double <= 0.0)
	{
		return (INT)ceil(ArgInf.m_double);
	}
		return (INT)floor(ArgInf.m_double);
}

//	〈双精度小数型〉 求次方 （双精度小数型 欲求次方数值，双精度小数型 次方数）
EXTERN_C __int64 WINAPIV krnln_fnPow(INT nArgCount, MDATA_INF ArgInf,...)
{
	PMDATA_INF pArgInf = &ArgInf;
	MARG_INF  mArgInf  = {0}; 
	mArgInf.m_double = pow(pArgInf[0].m_double, pArgInf[1].m_double);
	return mArgInf.m_int64;
}
//	〈双精度小数型〉 求平方根 （双精度小数型 欲求其平方根的数值）
EXTERN_C __int64 WINAPIV krnln_fnSqr(INT nArgCount,MDATA_INF ArgInf,...)
{
	MARG_INF  mArgInf  = {0}; 
	mArgInf.m_double = sqrt(ArgInf.m_double);
	return mArgInf.m_int64;
}
//	〈双精度小数型〉 求正弦 （双精度小数型 欲进行计算的角）
EXTERN_C __int64 WINAPIV  krnln_fnSin(INT nArgCount, MDATA_INF ArgInf,...)
{
	MARG_INF  mArgInf  = {0}; 
	mArgInf.m_double = sin(ArgInf.m_double);
	return mArgInf.m_int64;
}
//	〈双精度小数型〉 求余弦 （双精度小数型 欲进行计算的角）
EXTERN_C __int64 WINAPIV krnln_fnCos(INT nArgCount, MDATA_INF ArgInf,...)
{
	MARG_INF  mArgInf  = {0};
	mArgInf.m_double = cos(ArgInf.m_double);
	return mArgInf.m_int64;
}
//	〈双精度小数型〉 求正切 （双精度小数型 欲进行计算的角
EXTERN_C __int64 WINAPIV krnln_fnTan(INT nArgCount, MDATA_INF ArgInf,...)
{
	MARG_INF  mArgInf  = {0};
	mArgInf.m_double = tan(ArgInf.m_double);
	return mArgInf.m_int64;
}

//	〈双精度小数型〉 求反正切 （双精度小数型 欲求其反正切值的数值）
EXTERN_C __int64 WINAPIV krnln_fnAtn(INT nArgCount, MDATA_INF ArgInf,...)
{
	MARG_INF  mArgInf  = {0};
	mArgInf.m_double = atan(ArgInf.m_double);
	return mArgInf.m_int64;
}
//	〈双精度小数型〉 求自然对数 （双精度小数型 欲求其自然对数的数值）
EXTERN_C __int64 WINAPIV krnln_fnLog(INT nArgCount, MDATA_INF ArgInf,...)
{
	MARG_INF  mArgInf  = {0};
	mArgInf.m_double = log(ArgInf.m_double);
	return mArgInf.m_int64;
}
//	〈双精度小数型〉 求反对数 （双精度小数型 欲求其反对数的数值）
EXTERN_C __int64 WINAPIV krnln_fnExp(INT nArgCount, MDATA_INF ArgInf,...)
{
	MARG_INF  mArgInf  = {0};
	mArgInf.m_double = exp(ArgInf.m_double);
	return mArgInf.m_int64;
}
//	〈逻辑型〉 是否运算正确 （双精度小数型 欲校验的计算结果
EXTERN_C INT WINAPIV krnln_IsCalcOK(INT nArgCount, MDATA_INF ArgInf,...)
{
	return BL_TRUE;
}
//	〈双精度小数型〉 四舍五入 （双精度小数型 欲被四舍五入的数值，［整数型 被舍入的位置］）
EXTERN_C __int64 WINAPIV krnln_fnRound(INT nArgCount, MDATA_INF ArgInf,...)
{
		PMDATA_INF	pArgInf = &ArgInf;
		MARG_INF	mArgInf  = {0};
		double		Number = pArgInf[0].m_double;
		INT			Pos = pArgInf[1].m_int;
		if (Number < 0)
		{
			mArgInf.m_double = -(INT)(-Number * pow(10, Pos) + 0.5 ) / pow(10 , Pos);
		} 
		else
		{
			mArgInf.m_double = (INT)(Number * pow(10 , Pos) + 0.5) / pow(10,Pos);
		}
		return mArgInf.m_int64;
}

//	〈双精度小数型〉 求余数 （双精度小数型 被除数，双精度小数型 除数，... ）
EXTERN_C __int64 WINAPIV krnln_fnMod (INT nArgCount, MDATA_INF ArgInf,...)
{
	PMDATA_INF  pArgInf = &ArgInf;
	MARG_INF	mArgInf  = {0};
	INT i = 1;
	DOUBLE db = pArgInf [0].m_double;
	for (; i < nArgCount; i++)
	{
		pArgInf++;
		if (pArgInf->m_double == 0)
		{
			return 0;				//不能求余数于零
		}
		db = fmod (db, pArgInf->m_double);
	}
	 mArgInf.m_double = db;
	 return mArgInf.m_int64;
}

//	〈双精度小数型〉 整除 （双精度小数型 被除数，双精度小数型 除数，... ）
EXTERN_C __int64 WINAPIV krnln_fnIDiv(INT nArgCount, MDATA_INF ArgInf,...)
{
	PMDATA_INF pArgInf = &ArgInf;
	MARG_INF	mArgInf  = {0};
	DOUBLE db = pArgInf[0].m_double;
	INT i = 1;
	for (; i < nArgCount; i++)
	{
		pArgInf++;
		if (pArgInf->m_double == 0)
		{
		return 0;			//不能除数为零
		}
		db =  db / pArgInf->m_double;
	}
	// 设置返回的双精度小数数据。
	if ( db > 0 )
	{
		mArgInf.m_double = floor(db);
	}
	else
	{
		mArgInf.m_double =  ceil(db);
	}
	return mArgInf.m_int64;
}
//〈整数型〉 取符号 （双精度小数型 欲取其符号的数值）
EXTERN_C INT WINAPIV krnln_fnSgn(INT nArgCount, MDATA_INF ArgInf,...)
{
	int result; 
	if ( ArgInf.m_double <= 0.0 )
	{
		if ( ArgInf.m_double >= 0.0 )
			result = 0;
		else
			result = -1;
	}
	else
	{
		result = 1;
	}
	return result;
}

//	〈整数型〉 取随机数 （［整数型 欲取随机数的最小值］，［整数型 欲取随机数的最大值］）
EXTERN_C INT WINAPIV krnln_fnRnd(INT nArgCount, MDATA_INF ArgInf,...)
{
	PMDATA_INF pArgInf = &ArgInf;
	if ( pArgInf[1].m_int )	//	有最大值限制时
	{
		return rand() % (pArgInf[1].m_int - pArgInf[0].m_int) + pArgInf[0].m_int;
	}
	else
	{
		return rand() + pArgInf[0].m_int;
	}
}
//〈整数型〉 取数据类型尺寸 （整数型 欲取其尺寸的数据类型）
EXTERN_C int WINAPIV krnln_fnGetDataTypeSize(INT nArgCount, MDATA_INF ArgInf,...)
{
	int result;
	switch ( ArgInf.m_int )
	{
    case 1:
		result = 1;
		break;
    case 2:
		result = 2;
		break;
    case 3:
		result = 4;
		break;
    case 5:
		result = 4;
		break;
    case 7:
		result = 4;
		break;
    case 9:
		result = 4;
		break;
    case 4:
		result = 8;
		break;
    case 6:
		result = 8;
		break;
    case 8:
		result = 8;
		break;
    default:
		result = 0;
		break;
	}
	return result;
}
// 〈文本型〉 到全角 （文本型 欲变换的文本）
EXTERN_C char* WINAPIV krnln_fnQJCase(INT nArgCount, MDATA_INF ArgInf,...)
{
	INT  InLen = 0 , OutLen = 0;
	char *ptext = {0}, *pRetText = {0};

	InLen = strlen(ArgInf.m_pText);
	OutLen = InLen * 2 + 1;
	if ( OutLen )
	{
		ptext = krnl_MMalloc(OutLen);
		if ( ptext )
		{
			if (Half2All(ArgInf.m_pText , InLen , ptext ,&OutLen))
			{
				pRetText = CloneTextData( ptext );
			}
		krnl_MFree(ptext);
		}
	}
	return pRetText;
}
//〈文本型〉 到半角 （文本型 欲变换的文本）
EXTERN_C char* WINAPIV krnln_fnBJCase(INT nArgCount, MDATA_INF ArgInf,...)
{
	INT  InLen = 0 , OutLen = 0;
	char *ptext = {0}, *pRetText = {0};
	
	InLen = strlen(ArgInf.m_pText);
	OutLen = InLen + 1;
	if ( OutLen )
	{
		ptext = krnl_MMalloc(OutLen);
		if ( ptext )
		{
			if (All2Half(ArgInf.m_pText , InLen , ptext ,&OutLen))
			{
				pRetText = CloneTextData( ptext );
			}
			krnl_MFree(ptext);
		}
	}
	return pRetText;
}
//<字节集数组〉 分割字节集 （字节集 待分割字节集，［字节集 用作分割的字节集］，［整数型 要返回的子字节集数目］）
EXTERN_C void* WINAPIV krnln_fnSplitBin (INT nArgCount,MDATA_INF ArgInf,...)
{
	PMDATA_INF pArgInf = &ArgInf;
	LPBYTE szBeSplited = NULL , szMark = NULL , pData = NULL ,p = NULL;
	INT nLen2 = 0, nCount = 0 ,nLen1 = 0 ,nLen = 0 , nSize = 0 ,mSize = 0, i = 0;
	BYTE bMark []= {0,0};
	DWORD *lpMem = 0 , *pMem = 0 ;

	if(pArgInf [0].m_pBin)
	{
		szBeSplited = pArgInf [0].m_pBin+2*sizeof(INT);
	}
	// 如果某个具有 AS_DEFAULT_VALUE_IS_EMPTY 标志的参数用户程序中没有为其提供参数值，
	// 则其数据类型为 _SDT_NULL 。
	if(pArgInf [1].m_dtDataType == _SDT_NULL)
	{
		szMark = bMark;
		nLen2 = 1;
	}
	else
	{
		if(pArgInf [1].m_pBin)
		{
			szMark = pArgInf [1].m_pBin +2 * sizeof(INT);
			nLen2 =  pArgInf [1].m_pInt[1];
		}
	}
	nCount = pArgInf [2].m_dtDataType == _SDT_NULL ? -1 : max (0, pArgInf [2].m_int);
	nLen1 = szBeSplited ? pArgInf [0].m_pInt[1]:0;
	
	if (nCount > 0)				//判断返回数量,如果确定就申请指定大小的内存
	{
		mSize = nCount * sizeof(DWORD);
	} 
	else						//不确定就申请字节集长度的内存
	{
		mSize = pArgInf [0].m_pInt[1] * sizeof(DWORD);
	}
	lpMem =  krnl_MMalloc(mSize);
	pMem =lpMem;					//备份内存地址
	if (nLen1 > 0 && nCount != 0  && lpMem)
	{
		memset(lpMem , 0, mSize); 	
		if (nLen2 == 0)
		{
			pData = CloneBinData(szBeSplited,nLen1);
			*lpMem = (DWORD)pData;					//设置数据
			*lpMem++;
			i++;
		}
		else
		{
			LPBYTE pBegin = szBeSplited;
			LPBYTE ps = pBegin;
			LPBYTE pEnd = pBegin +  nLen1;
			
			while (nLen1 >= nLen2)
			{
				if (!memcmp (ps, szMark, nLen2))
				{
					nLen = ps - pBegin;
					pData = CloneBinData(pBegin,nLen);
					*lpMem = (DWORD)pData;					//设置数据
					*lpMem++;
					i++;
					ps += nLen2;
					nLen1 -= nLen2;
					pBegin = ps;
					if (nCount != -1)
					{
						nCount--;
						if (nCount == 0)  break;
					}
				}
				else
				{
					ps++;
					nLen1--;
				}
			}
			
			if (pBegin < pEnd && nCount != 0)
			{
				nLen = pEnd - pBegin;
				pData = CloneBinData(pBegin,nLen);
				*lpMem = (DWORD)pData;				//设置数据
				*lpMem++;
				i++;
			}
		}
	}
	
	// 建立数组数据。
	nSize = i * sizeof (DWORD);
	p = (LPBYTE)krnl_MMalloc (sizeof (INT) * 2 + nSize);
	*(LPINT)p = 1;  // 数组维数。
	*(LPINT)(p + sizeof (INT)) = i;
	memcpy (p + sizeof (INT) * 2, pMem, nSize);				//复制数组数据表
	krnl_MFree(pMem);
	return p;  // 返回内容数组。
}

//〈文本型数组〉 分割文本 （文本型 待分割文本，［文本型 用作分割的文本］，［整数型 要返回的子文本数目］
EXTERN_C void* WINAPIV krnln_fnSplit (INT nArgCount,MDATA_INF ArgInf,...)
{
	PMDATA_INF pArgInf = &ArgInf;
	LPTSTR szBeSplited = pArgInf [0].m_pText;
	LPBYTE p = NULL;
	int nSize = 0,mSize = 0 , i = 0;
	DWORD *lpMem = 0 , *pMem = 0 ;

	// 如果某个具有 AS_DEFAULT_VALUE_IS_EMPTY 标志的参数用户程序中没有为其提供参数值，
	// 则其数据类型为 _SDT_NULL 。
	LPTSTR szMark = pArgInf [1].m_dtDataType == _SDT_NULL ? "," : pArgInf [1].m_pText;
	INT nCount = pArgInf [2].m_dtDataType == _SDT_NULL ? -1 : max (0, pArgInf [2].m_int);
	
	INT nLen1 = strlen (szBeSplited);
	INT nLen2 = strlen (szMark);	
	if (nCount > 0)				//判断返回数量,如果确定就申请指定大小的内存
	{
		mSize = nCount * sizeof(DWORD);
	} 
	else						//不确定就申请文本长度的内存
	{
		mSize = nLen1 * sizeof(DWORD);
	}
	lpMem =  krnl_MMalloc(mSize);
	pMem =lpMem;					//备份内存地址	
	if (nLen1 > 0 && nCount != 0 && lpMem)
	{
		memset(lpMem , 0, mSize); 	
		if (nLen2 == 0)
		{
			INT nLen = strlen(szBeSplited);
			char *pText = (char*)krnl_MMalloc(nLen+1);
			strcpy(pText,szBeSplited);
			*lpMem = (DWORD)pText;			//设置数据
			*lpMem++;
			i++;
		}
		else
		{
			LPTSTR pBegin = szBeSplited;
			LPTSTR ps = pBegin;
			
			while (nLen1 >= nLen2)
			{
				if (!memcmp (ps, szMark, nLen2))
				{
					INT nLen = ps - pBegin;
					char *pText = (char*)krnl_MMalloc(nLen+1);
					strncpy(pText,pBegin,nLen);
					pText[nLen]=0;
					*lpMem = (DWORD)pText;		//设置数据
					*lpMem++;
					i++;
					ps += nLen2;
					nLen1 -= nLen2;
					pBegin = ps;
					if (nCount != -1)
					{
						nCount--;
						if (nCount == 0)  break;
					}
				}
				else
				{
					if (IS_CC (*ps))
					{
						if (ps [1] == 0)  break;
						ps++;
						nLen1--;
					}
					ps++;
					nLen1--;
				}
			}
			
			if (*pBegin != '\0' && nCount != 0)
			{
				INT nLen = strlen(pBegin);
				char *pText = (char*)krnl_MMalloc(nLen+1);
				strcpy(pText,pBegin);
				*lpMem = (DWORD)pText;			//设置数据
				*lpMem++;
				i++;
			}
		}
	}
	
	// 建立数组数据。
	nSize = i * sizeof (DWORD);
	p = (LPBYTE)krnl_MMalloc (sizeof (INT) * 2 + nSize);
	*(LPINT)p = 1;  // 数组维数。
	*(LPINT)(p + sizeof (INT)) = i;
	memcpy (p + sizeof (INT) * 2, pMem, nSize);
	krnl_MFree(pMem);
	return p;  // 返回内容数组。
}
//	〈逻辑型〉 近似等于 （文本型 被比较文本，文本型 比较文本）
__declspec(naked) KRNLN_FUNC(fnTextLikeEqu)	
{
	__asm
	{
			PUSH EBX
			MOV EBX,DWORD PTR SS:[ESP+018h]
			PUSH ESI
			PUSH EDI
			MOV EDI,EBX
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			MOV ESI,DWORD PTR SS:[ESP+014h]
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			MOV EDI,ESI
			MOV EDX,ECX
			OR ECX,0FFFFFFFFh
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			CMP ECX,EDX
			JNB SHORT L0040120B
			POP EDI
			POP ESI
			POP EBX
			RETN
L0040120B:
			MOV ECX,EDX
			MOV EDI,EBX
			XOR EAX,EAX
			REPE CMPSB
			POP EDI
			POP ESI
			SETE AL
			POP EBX
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnLen)			//取文本长度
{
	__asm
	{
		PUSH EDI
		MOV EDI,DWORD PTR SS:[ESP+0xC]
		OR ECX,0xFFFFFFFF
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		POP EDI
		MOV EAX,ECX
		RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnLeft)	//取文本左边
{
	__asm
	{
		PUSH EBX
		PUSH ESI
		MOV ESI,DWORD PTR SS:[ESP+0x10]
		PUSH EDI
		MOV EDI,ESI
		OR ECX,0xFFFFFFFF
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		MOV EBX,ECX
		JE L035
		MOV EAX,DWORD PTR SS:[ESP+0x20]
		TEST EAX,EAX
		JLE L035
		CMP EAX,EBX
		JGE L018
		MOV EBX,EAX
L018:
		LEA EAX,DWORD PTR DS:[EBX+0x1]
		PUSH EAX
		CALL krnl_MMalloc
		MOV ECX,EBX
		MOV EDI,EAX
		MOV EDX,ECX
		ADD ESP,4
		SHR ECX,2
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EDX
		AND ECX,3
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
		POP EDI
		MOV BYTE PTR DS:[EAX+EBX],0
		POP ESI
		POP EBX
		RETN
L035:
		POP EDI
		POP ESI
		XOR EAX,EAX
		POP EBX
		RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnRight)	//取文本右边
{
	__asm
	{
		MOV EDX,DWORD PTR SS:[ESP+0x8]
		PUSH EDI
		MOV EDI,EDX
		OR ECX,0xFFFFFFFF
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		MOV EAX,ECX
		JE L045
		MOV ECX,DWORD PTR SS:[ESP+0x18]
		TEST ECX,ECX
		JLE L045
		SUB EAX,ECX
		JNS L016
		XOR EAX,EAX
L016:
		PUSH ESI
		LEA ESI,DWORD PTR DS:[EAX+EDX]
		MOV EDI,ESI
		OR ECX,0xFFFFFFFF
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		PUSH ECX
		CALL krnl_MMalloc
		MOV EDX,EAX
		MOV EDI,ESI
		OR ECX,0xFFFFFFFF
		XOR EAX,EAX
		ADD ESP,4
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		SUB EDI,ECX
		MOV EAX,ECX
		MOV ESI,EDI
		MOV EDI,EDX
		SHR ECX,2
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EAX
		MOV EAX,EDX
		AND ECX,3
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
		POP ESI
		POP EDI
		RETN
L045:
		XOR EAX,EAX
		POP EDI
		RETN
	}

}

__declspec(naked) KRNLN_FUNC(fnMid)	//取文本中间
{
	__asm
	{
		PUSH EBX
		PUSH EBP
		MOV EBP,DWORD PTR SS:[ESP+0x10]
		PUSH ESI
		PUSH EDI
		MOV EDI,EBP
		OR ECX,0xFFFFFFFF
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		JE L045
		MOV EBX,DWORD PTR SS:[ESP+0x30]
		TEST EBX,EBX
		JLE L045
		MOV ESI,DWORD PTR SS:[ESP+0x24]
		TEST ESI,ESI
		JLE L045
		CMP ESI,ECX
		JG L045
		LEA EAX,DWORD PTR DS:[ESI+EBX]
		CMP EAX,ECX
		JLE L026
		SUB ECX,ESI
		INC ECX
		MOV EBX,ECX
L026:
		LEA ECX,DWORD PTR DS:[EBX+0x01]
		PUSH ECX
		CALL krnl_MMalloc
		MOV ECX,EBX
		LEA ESI,DWORD PTR DS:[ESI+EBP-1]
		MOV EDX,ECX
		MOV EDI,EAX
		SHR ECX,2
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EDX
		ADD ESP,4
		AND ECX,3
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
		POP EDI
		POP ESI
		MOV BYTE PTR DS:[EAX+EBX],0
		POP EBP
		POP EBX
		RETN
L045:
		POP EDI
		POP ESI
		POP EBP
		XOR EAX,EAX
		POP EBX
		RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnChr)    //字符
{
	__asm
	{
		PUSH EBX
		MOV BL,BYTE PTR SS:[ESP+0xC]
		TEST BL,BL
		JNZ L007
		XOR EAX,EAX
		POP EBX
		RETN
L007:
		PUSH 2
		CALL krnl_MMalloc
		ADD ESP,4
		MOV BYTE PTR DS:[EAX],BL
		MOV BYTE PTR DS:[EAX+1],0
		POP EBX
		RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnAsc)  //取代码
{
	__asm
	{
		MOV EDX,DWORD PTR SS:[ESP+0x8]
		PUSH EDI
		MOV EDI,EDX
		OR ECX,0xFFFFFFFF
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		POP EDI
		JE L022
		MOV EAX,DWORD PTR SS:[ESP+0x1C]
		TEST EAX,EAX
		JNZ L015
		MOV EAX,1
		JMP L018
L015:
		MOV EAX,DWORD PTR SS:[ESP+0x14]
		TEST EAX,EAX
		JLE L022
L018:
		CMP EAX,ECX
		JG L022
		MOVSX EAX,BYTE PTR DS:[EAX+EDX-1]
		RETN
L022:
		XOR EAX,EAX
		RETN
	}
}

//	〈文本型〉 到小写 （文本型 欲变换的文本）
__declspec(naked) KRNLN_FUNC(fnLCase)
{
	__asm
	{
			PUSH EBP
			PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+010h]
			PUSH EDI
			MOV EDI,ESI
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			MOV EBP,ECX
			JNZ SHORT L004012CB
			POP EDI
			POP ESI
			POP EBP
			RETN
L004012CB:
			LEA EAX,DWORD PTR SS:[EBP+01h]
			PUSH EBX
			PUSH EAX
			CALL krnl_MMalloc
			MOV EBX,EAX
			MOV EDI,ESI
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			ADD ESP,04h
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			SUB EDI,ECX
			MOV EDX,ECX
			MOV ESI,EDI
			MOV EDI,EBX
			SHR ECX,02h
			REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
			MOV ECX,EDX
			AND ECX,03h
			REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
			XOR ESI,ESI
			TEST EBP,EBP
			JBE SHORT L0040131F
L004012FF:
			MOV AL,BYTE PTR DS:[ESI+EBX]
			TEST AL,AL
			JGE SHORT L0040130B
			ADD ESI,02h
			JMP SHORT L0040131B
L0040130B:
			MOVSX EAX,AL
			PUSH EAX
			CALL tolower
			ADD ESP,04h
			MOV BYTE PTR DS:[ESI+EBX],AL
			INC ESI
L0040131B:
			CMP ESI,EBP
			JB SHORT L004012FF
L0040131F:
			MOV EAX,EBX
			POP EBX
			POP EDI
			POP ESI
			POP EBP
			RETN
	}
}

//	〈文本型〉 到大写 （文本型 欲变换的文本）
__declspec(naked) KRNLN_FUNC(fnUCase)
{
	__asm
	{
			PUSH EBP
			PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+010h]
			PUSH EDI
			MOV EDI,ESI
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			MOV EBP,ECX
			JNZ SHORT L004012CB
			POP EDI
			POP ESI
			POP EBP
			RETN
L004012CB:
			LEA EAX,DWORD PTR SS:[EBP+01h]
			PUSH EBX
			PUSH EAX
			CALL krnl_MMalloc
			MOV EBX,EAX
			MOV EDI,ESI
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			ADD ESP,04h
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			SUB EDI,ECX
			MOV EDX,ECX
			MOV ESI,EDI
			MOV EDI,EBX
			SHR ECX,02h
			REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
			MOV ECX,EDX
			AND ECX,03h
			REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
			XOR ESI,ESI
			TEST EBP,EBP
			JBE SHORT L0040131F
L004012FF:
			MOV AL,BYTE PTR DS:[ESI+EBX]
			TEST AL,AL
			JGE SHORT L0040130B
			ADD ESI,02h
			JMP SHORT L0040131B
L0040130B:
			MOVSX EAX,AL
			PUSH EAX
			CALL toupper
			ADD ESP,04h
			MOV BYTE PTR DS:[ESI+EBX],AL
			INC ESI
L0040131B:
			CMP ESI,EBP
			JB SHORT L004012FF
L0040131F:
			MOV EAX,EBX
			POP EBX
			POP EDI
			POP ESI
			POP EBP
			RETN
	}
}

//	〈文本型〉 删首空 （文本型 欲删除空格的文本）
__declspec(naked) KRNLN_FUNC(fnLTrim)
{
		__asm
		{
				PUSH EBX
				PUSH ESI
				MOV ESI,DWORD PTR SS:[ESP+010h]
				PUSH EDI
				MOV EDI,ESI
				OR ECX,0FFFFFFFFh
				XOR EAX,EAX
				REPNE SCAS BYTE PTR ES:[EDI]
				NOT ECX
				DEC ECX
				MOV EBX,ECX
				JNZ SHORT L004012CB
				POP EDI
				POP ESI
				POP EBX
				RETN
L004012CB:
				XOR EDI,EDI
				TEST EBX,EBX
				JLE SHORT L004012FD
L004012D1:
				CMP BYTE PTR DS:[ESI],020h
				JE SHORT L004012EA
				PUSH 2h
				PUSH offset USpace
				PUSH ESI
				CALL strncmp
				ADD ESP,0Ch
				TEST EAX,EAX
				JNZ SHORT L004012FD
L004012EA:
				CMP BYTE PTR DS:[ESI],0h
				JGE SHORT L004012F7
				ADD EDI,02h
				ADD ESI,02h
				JMP SHORT L004012F9
L004012F7:
				INC ESI
				INC EDI
L004012F9:
				CMP EDI,EBX
				JL SHORT L004012D1
L004012FD:
				MOV EDI,ESI
				OR ECX,0FFFFFFFFh
				XOR EAX,EAX
				REPNE SCAS BYTE PTR ES:[EDI]
				NOT ECX
				DEC ECX
				JNZ SHORT L0040130F
				POP EDI
				POP ESI
				POP EBX
				RETN
L0040130F:
				INC ECX
				PUSH ECX
				CALL krnl_MMalloc
				MOV EDX,EAX
				MOV EDI,ESI
				OR ECX,0FFFFFFFFh
				XOR EAX,EAX
				ADD ESP,04h
				REPNE SCAS BYTE PTR ES:[EDI]
				NOT ECX
				SUB EDI,ECX
				MOV EAX,ECX
				MOV ESI,EDI
				MOV EDI,EDX
				SHR ECX,02h
				REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
				MOV ECX,EAX
				MOV EAX,EDX
				AND ECX,03h
				REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
				POP EDI
				POP ESI
				POP EBX
				RETN
		}
}

//	〈文本型〉 删尾空 （文本型 欲删除空格的文本）
__declspec(naked) KRNLN_FUNC(fnRTrim)
{
	__asm
	{
			PUSH EBX
			PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+010h]
			PUSH EDI
			MOV EDI,ESI
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			MOV EBX,ECX
			JNZ SHORT L004012CB
			POP EDI
			POP ESI
			POP EBX
			RETN
L004012CB:
			LEA EAX,DWORD PTR DS:[EBX+01h]
			PUSH EAX
			CALL krnl_MMalloc
			MOV EDX,EAX
			MOV EDI,ESI
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			ADD ESP,04h
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			SUB EDI,ECX
			MOV EAX,ECX
			MOV ESI,EDI
			MOV EDI,EDX
			SHR ECX,02h
			REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
			MOV ECX,EAX
			LEA EAX,DWORD PTR DS:[EDX+EBX-01h]
			AND ECX,03h
			CMP EAX,EDX
			REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
			JB SHORT L00401326
			MOV BL,0A1h
L00401302:
			MOV CL,BYTE PTR DS:[EAX]
			CMP CL,020h
			JE SHORT L00401312
			CMP CL,BL
			JNZ SHORT L00401322
			CMP BYTE PTR DS:[EAX-01h],BL
			JNZ SHORT L00401322
L00401312:
			TEST CL,CL
			JGE SHORT L0040131B
			SUB EAX,02h
			JMP SHORT L0040131C
L0040131B:
			DEC EAX
L0040131C:
			CMP EAX,EDX
			JNB SHORT L00401302
			JMP SHORT L00401326
L00401322:
			MOV BYTE PTR DS:[EAX+01h],0h
L00401326:
			MOV EDI,EDX
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			JNZ SHORT L0040133F
			PUSH EDX
			CALL krnl_MFree
			ADD ESP,04h
			XOR EDX,EDX
L0040133F:
			POP EDI
			POP ESI
			MOV EAX,EDX
			POP EBX
			RETN
	}
}

//	〈文本型〉 删首尾空 （文本型 欲删除空格的文本）
__declspec(naked) KRNLN_FUNC(fnTrim)
{
		__asm
		{
				MOV ECX,DWORD PTR SS:[ESP+08h]
				PUSH EDI
				MOV EDX,DWORD PTR SS:[ESP+010h]
				SUB ESP,0Ch
				MOV EAX,ESP
				PUSH 1h
				MOV DWORD PTR DS:[EAX],ECX
				MOV ECX,DWORD PTR SS:[ESP+024h]
				MOV DWORD PTR DS:[EAX+04h],EDX
				MOV DWORD PTR DS:[EAX+08h],ECX
				CALL krnln_fnLTrim
				MOV EDX,EAX
				ADD ESP,010h
				TEST EDX,EDX
				JNZ SHORT L004012DC
				POP EDI
				RETN
L004012DC:
				MOV EDI,EDX
				OR ECX,0FFFFFFFFh
				XOR EAX,EAX
				REPNE SCAS BYTE PTR ES:[EDI]
				NOT ECX
				DEC ECX
				LEA EAX,DWORD PTR DS:[ECX+EDX-01h]
				CMP EAX,EDX
				JB SHORT L00401315
L004012F0:
				MOV CL,BYTE PTR DS:[EAX]
				CMP CL,020h
				JE SHORT L00401301
				CMP CL,0A1h
				JNZ SHORT L00401311
				CMP BYTE PTR DS:[EAX-01h],CL
				JNZ SHORT L00401311
L00401301:
				TEST CL,CL
				JGE SHORT L0040130A
				SUB EAX,02h
				JMP SHORT L0040130B
L0040130A:
				DEC EAX
L0040130B:
				CMP EAX,EDX
				JNB SHORT L004012F0
				JMP SHORT L00401315
L00401311:
				MOV BYTE PTR DS:[EAX+01h],0h
L00401315:
				MOV EDI,EDX
				OR ECX,0FFFFFFFFh
				XOR EAX,EAX
				REPNE SCAS BYTE PTR ES:[EDI]
				NOT ECX
				DEC ECX
				JNZ SHORT L0040132E
				PUSH EDX
				CALL krnl_MFree
				ADD ESP,04h
				XOR EDX,EDX
L0040132E:
				MOV EAX,EDX
				POP EDI
				RETN
		}
}

//	〈文本型〉 删全部空 （文本型 欲删除空格的文本）
__declspec(naked) KRNLN_FUNC(fnTrimAll)
{
	__asm
	{
		SUB ESP,08h
		PUSH ESI
		MOV ESI,DWORD PTR SS:[ESP+014h]
		PUSH EDI
		MOV EDI,ESI
		OR ECX,0FFFFFFFFh
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		MOV EDI,ECX
		MOV DWORD PTR SS:[ESP+0Ch],EDI
		JNZ SHORT L004012D3
		POP EDI
		POP ESI
		ADD ESP,08h
		RETN
L004012D3:
		LEA EAX,DWORD PTR DS:[EDI+01h]
		PUSH EBX
		PUSH EAX
		CALL krnl_MMalloc
		ADD ESP,04h
		MOV EDX,EAX
		XOR EBX,EBX
		MOV DWORD PTR SS:[ESP+0Ch],EDX
		TEST EDI,EDI
		MOV DWORD PTR SS:[ESP+01Ch],EDX
		JLE L00401383
		PUSH EBP
L004012F5:
		CMP BYTE PTR DS:[ESI],020h
		JE SHORT L0040130E
		PUSH 2h
		PUSH offset USpace
		PUSH ESI
		CALL strncmp
		ADD ESP,0Ch
		TEST EAX,EAX
		JNZ SHORT L0040131F
L0040130E:
		CMP BYTE PTR DS:[ESI],0h
		JGE SHORT L0040131B
		ADD EBX,02h
		ADD ESI,02h
		JMP SHORT L004012F5
L0040131B:
		INC ESI
		INC EBX
		JMP SHORT L004012F5
L0040131F:
		MOV AL,BYTE PTR DS:[ESI]
		MOV EBP,ESI
		TEST AL,AL
		JE SHORT L00401354
L00401327:
		CMP AL,020h
		JE SHORT L00401354
		PUSH 2h
		PUSH offset USpace
		PUSH ESI
		CALL strncmp
		ADD ESP,0Ch
		TEST EAX,EAX
		JE SHORT L00401354
		CMP BYTE PTR DS:[ESI],0h
		JGE SHORT L0040134C
		ADD EBX,02h
		ADD ESI,02h
		JMP SHORT L0040134E
L0040134C:
		INC ESI
		INC EBX
L0040134E:
		MOV AL,BYTE PTR DS:[ESI]
		TEST AL,AL
		JNZ SHORT L00401327
L00401354:
		MOV EDI,ESI
		SUB EDI,EBP
		JE SHORT L0040136F
		PUSH EDI
		PUSH EBP
		MOV EBP,DWORD PTR SS:[ESP+028h]
		PUSH EBP
		CALL strncpy
		ADD ESP,0Ch
		ADD EBP,EDI
		MOV DWORD PTR SS:[ESP+020h],EBP
L0040136F:
		CMP BYTE PTR DS:[ESI],0h
		JE SHORT L0040137E
		CMP EBX,DWORD PTR SS:[ESP+014h]
		JL L004012F5
L0040137E:
		MOV EDX,DWORD PTR SS:[ESP+010h]
		POP EBP
L00401383:
		MOV ECX,DWORD PTR SS:[ESP+01Ch]
		MOV EDI,EDX
		XOR EAX,EAX
		POP EBX
		MOV BYTE PTR DS:[ECX],0h
		OR ECX,0FFFFFFFFh
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		JNZ SHORT L004013AE
		PUSH EDX
		CALL krnl_MFree
		MOV DWORD PTR SS:[ESP+0Ch],0h
		ADD ESP,04h
		MOV EDX,DWORD PTR SS:[ESP+08h]
L004013AE:
		POP EDI
		MOV EAX,EDX
		POP ESI
		ADD ESP,08h
		RETN
	}
}
//<小数型〉 到小数 （通用型 待转换的文本或数值）
__declspec(naked) KRNLN_FUNC(fnToFloat)
{
	__asm
	{
			PUSH EBP
			MOV EBP,ESP
			PUSH ECX
			MOV EAX,DWORD PTR SS:[EBP+014h]
			PUSH ESI
			CMP EAX,080000201h
			MOV DWORD PTR SS:[EBP-04h],0h
			JA SHORT L00401365
			JE SHORT L00401359
			CMP EAX,080000006h
			JA SHORT L00401342
			JE L00401381
			CMP EAX,080000002h
			JE L00401381
			CMP EAX,080000004h
			JNZ L0040139B
			MOV EDX,DWORD PTR SS:[EBP+0Ch]
			SUB ESP,0Ch
			MOV ECX,ESP
			PUSH 1h
			MOV DWORD PTR DS:[ECX],EDX
			MOV EDX,DWORD PTR SS:[EBP+010h]
			MOV DWORD PTR DS:[ECX+04h],EDX
			MOV DWORD PTR DS:[ECX+08h],EAX
			CALL krnln_fnBJCase
			MOV ESI,EAX
			ADD ESP,010h
			TEST ESI,ESI
			JE SHORT L0040139B
			PUSH ESI
			CALL _atof
			FSTP DWORD PTR SS:[EBP-04h]
			PUSH ESI
			CALL krnl_MFree
			ADD ESP,08h
			JMP SHORT L0040139B
L00401342:
			CMP EAX,080000101h
			JNZ SHORT L0040139B
			MOV EAX,DWORD PTR SS:[EBP+0Ch]
			AND EAX,0FFh
			MOV DWORD PTR SS:[EBP+014h],EAX
			FILD DWORD PTR SS:[EBP+014h]
			JMP SHORT L00401398
L00401359:
			MOVSX ECX,WORD PTR SS:[EBP+0Ch]
			MOV DWORD PTR SS:[EBP+014h],ECX
			FILD DWORD PTR SS:[EBP+014h]
			JMP SHORT L00401398
L00401365:
			CMP EAX,080000501h
			JA SHORT L0040138E
			JE SHORT L00401386
			CMP EAX,080000301h
			JE SHORT L00401381
			CMP EAX,080000401h
			JNZ SHORT L0040139B
			FILD QWORD PTR SS:[EBP+0Ch]
			JMP SHORT L00401398
L00401381:
			FILD DWORD PTR SS:[EBP+0Ch]
			JMP SHORT L00401398
L00401386:
			MOV EDX,DWORD PTR SS:[EBP+0Ch]
			MOV DWORD PTR SS:[EBP-04h],EDX
			JMP SHORT L0040139B
L0040138E:
			CMP EAX,080000601h
			JNZ SHORT L0040139B
			FLD QWORD PTR SS:[EBP+0Ch]
L00401398:
			FSTP DWORD PTR SS:[EBP-04h]
L0040139B:
			MOV EAX,DWORD PTR SS:[EBP-04h]
			POP ESI
			MOV ESP,EBP
			POP EBP
			RETN
	}
}
//<双精度小数型〉 到数值 （通用型 待转换的文本或数值） 
__declspec(naked) KRNLN_FUNC(fnVal)
{
	__asm
	{
			PUSH EBP
			MOV EBP,ESP
			PUSH ECX
			MOV EAX,DWORD PTR SS:[EBP+014h]
			PUSH ESI
			CMP EAX,080000201h
			JA L00401333
			JE SHORT L00401327
			CMP EAX,080000003h
			JE L00401358
			CMP EAX,080000004h
			JE SHORT L004012DE
			CMP EAX,080000101h
			JNZ SHORT L00401348
			MOV EAX,DWORD PTR SS:[EBP+0Ch]
			AND EAX,0FFh
			MOV DWORD PTR SS:[EBP-04h],EAX
			FILD DWORD PTR SS:[EBP-04h]
			JMP SHORT L00401355
L004012DE:
			MOV EDX,DWORD PTR SS:[EBP+0Ch]
			SUB ESP,0Ch
			MOV ECX,ESP
			PUSH 1h
			MOV DWORD PTR DS:[ECX],EDX
			MOV EDX,DWORD PTR SS:[EBP+010h]
			MOV DWORD PTR DS:[ECX+04h],EDX
			MOV DWORD PTR DS:[ECX+08h],EAX
			CALL krnln_fnBJCase
			MOV ESI,EAX
			ADD ESP,010h
			TEST ESI,ESI
			JE SHORT L00401317
			PUSH 00h
			PUSH ESI
			CALL _strtod
			FSTP QWORD PTR SS:[EBP+0Ch]
			PUSH ESI
			CALL krnl_MFree
			ADD ESP,0Ch
			JMP SHORT L00401358
L00401317:
			MOV DWORD PTR SS:[EBP+0Ch],0h
			MOV DWORD PTR SS:[EBP+010h],0h
			JMP SHORT L00401358
L00401327:
			MOVSX EAX,WORD PTR SS:[EBP+0Ch]
			MOV DWORD PTR SS:[EBP-04h],EAX
			FILD DWORD PTR SS:[EBP-04h]
			JMP SHORT L00401355
L00401333:
			CMP EAX,080000401h
			JE SHORT L00401352
			CMP EAX,080000501h
			JE SHORT L0040134D
			CMP EAX,080000601h
			JE SHORT L00401358
L00401348:
			FILD DWORD PTR SS:[EBP+0Ch]
			JMP SHORT L00401355
L0040134D:
			FLD DWORD PTR SS:[EBP+0Ch]
			JMP SHORT L00401355
L00401352:
			FILD QWORD PTR SS:[EBP+0Ch]
L00401355:
			FSTP QWORD PTR SS:[EBP+0Ch]
L00401358:
			MOV EAX,DWORD PTR SS:[EBP+0Ch]
			MOV EDX,DWORD PTR SS:[EBP+010h]
			POP ESI
			MOV ESP,EBP
			POP EBP
			RETN
	}
}

//	到整数
EXTERN_C INT WINAPIV krnln_fnToInt(INT nArgCount, MDATA_INF ArgInf)
{
	switch ( ArgInf.m_dtDataType )
	{
	case SDT_BYTE:
		return ArgInf.m_byte;
		break;
	case SDT_SHORT:
		return ArgInf.m_short;
		break;
	case SDT_INT:
		return ArgInf.m_int;
		break;
	case SDT_INT64:
		return (INT)ArgInf.m_int64;
		break;
	case SDT_FLOAT:
		return (INT)ArgInf.m_float;
		break;
	case SDT_DOUBLE:
		return (INT)ArgInf.m_double;
		break;
	case SDT_BOOL:
		return ArgInf.m_bool;
		break;
	case SDT_DATE_TIME:
		break;
	case SDT_TEXT:
		if ( ArgInf.m_pText )
		{
			return atoi(ArgInf.m_pText);
		}
		break;
	case SDT_BIN:
		break;
	case SDT_SUB_PTR:
		return (INT)ArgInf.m_pdwSubCodeAdr;
		break;
	}
	return 0;
}

//取十六进制文本
EXTERN_C CHAR* WINAPIV krnln_fnHex(INT nArgCount, MDATA_INF ArgInf)
{
	CHAR lsHex[10]={0};
	if (ArgInf.m_int)
	{
		sprintf(lsHex, "%X", ArgInf.m_int);
		return CloneTextData(lsHex);
	}
	return NULL;
}
//〈文本型〉 取八进制文本 （整数型 欲取进制文本的数值）
EXTERN_C CHAR* WINAPIV krnln_fnOct(INT nArgCount, MDATA_INF ArgInf)
{
	CHAR lsOct[10]={0};
	if (ArgInf.m_int)
	{
		sprintf(lsOct, "%o", ArgInf.m_int);
		return CloneTextData(lsOct);
	}
	return NULL;
}
//〈整数型〉 取启动时间 （）
EXTERN_C INT WINAPIV krnln_fnGetTickCount(INT nArgCount, MDATA_INF ArgInf)
{
	LARGE_INTEGER Tickcount={0};
	ULONG	TimeInc = KeQueryTimeIncrement();
	KeQueryTickCount(&Tickcount);
	Tickcount.QuadPart *=TimeInc;
	Tickcount.QuadPart /=10000;
	return Tickcount.LowPart;
}
//	〈无返回值〉 置随机数种子 （［整数型 欲置入的种子数值］）
EXTERN_C void WINAPIV krnln_fnRandomize(INT nArgCount, MDATA_INF ArgInf,...)
{
	MDATA_INF MArgInf = {0};
	if( ArgInf.m_int )		 //		种子非0时
	{
		srand(ArgInf.m_int);
	}
	else
	{
		srand(krnln_fnGetTickCount(0 , MArgInf));
	}
}
//〈无返回值〉 延时 （整数型 欲等待的时间）
EXTERN_C VOID WINAPIV krnln_fnSleep(INT nArgCount, MDATA_INF ArgInf)
{
	LARGE_INTEGER lDelay;
	lDelay = RtlConvertLongToLargeInteger(-10000 * ArgInf.m_int);
	KeDelayExecutionThread(KernelMode , FALSE ,&lDelay);
}
//〈日期时间型〉 取现行时间 （）
EXTERN_C __int64 WINAPIV krnln_fnNow(INT nArgCount, MDATA_INF ArgInf)
{
	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER LocalTime;
	TIME_FIELDS   TimeFields;
	KeQuerySystemTime(&CurrentTime);
	ExSystemTimeToLocalTime(&CurrentTime, &LocalTime);
	RtlTimeToTimeFields(&LocalTime , &TimeFields);
	return TimeFieldsToTimeInt64(&TimeFields);
}
//〈整数型〉 取年份 （日期时间型 时间）
__declspec(naked) INT WINAPIV krnln_fnYear(INT nArgCount, MDATA_INF ArgInf)
{	
	__asm
	{
			MOV ECX,DWORD PTR SS:[ESP+0Ch]
			MOV EDX,DWORD PTR SS:[ESP+08h]
			SUB ESP,010h
			LEA EAX,DWORD PTR SS:[ESP+08h]
			PUSH EAX
			PUSH ECX
			PUSH EDX
			CALL _modf
			LEA EAX,DWORD PTR SS:[ESP+0Ch]
			LEA ECX,DWORD PTR SS:[ESP+010h]
			PUSH EAX
			MOV EAX,DWORD PTR SS:[ESP+01Ch]
			LEA EDX,DWORD PTR SS:[ESP+028h]
			PUSH ECX
			MOV ECX,DWORD PTR SS:[ESP+01Ch]
			PUSH EDX
			PUSH EAX
			PUSH ECX
			FSTP ST
			CALL GetYearMonthDay
			MOV EAX,DWORD PTR SS:[ESP+038h]
			ADD ESP,030h
			RETN
	}
}
//〈整数型〉 取月份 （日期时间型 时间）
__declspec(naked) INT WINAPIV krnln_fnMonth(INT nArgCount, MDATA_INF ArgInf)
{	
	__asm
	{
			MOV ECX,DWORD PTR SS:[ESP+0Ch]
			MOV EDX,DWORD PTR SS:[ESP+08h]
			SUB ESP,010h
			LEA EAX,DWORD PTR SS:[ESP+08h]
			PUSH EAX
			PUSH ECX
			PUSH EDX
			CALL _modf
			LEA EAX,DWORD PTR SS:[ESP+0Ch]
			LEA ECX,DWORD PTR SS:[ESP+024h]
			PUSH EAX
			MOV EAX,DWORD PTR SS:[ESP+01Ch]
			LEA EDX,DWORD PTR SS:[ESP+014h]
			PUSH ECX
			MOV ECX,DWORD PTR SS:[ESP+01Ch]
			PUSH EDX
			PUSH EAX
			PUSH ECX
			FSTP ST
			CALL GetYearMonthDay
			MOV EAX,DWORD PTR SS:[ESP+038h]
			ADD ESP,030h
			RETN
	}
}
//〈整数型〉 取日 （日期时间型 时间）
__declspec(naked) INT WINAPIV krnln_fnDay(INT nArgCount, MDATA_INF ArgInf)
{	
	__asm
	{
			MOV ECX,DWORD PTR SS:[ESP+0Ch]
			MOV EDX,DWORD PTR SS:[ESP+08h]
			SUB ESP,010h
			LEA EAX,DWORD PTR SS:[ESP+08h]
			PUSH EAX
			PUSH ECX
			PUSH EDX
			CALL _modf
			LEA EAX,DWORD PTR SS:[ESP+024h]
			LEA ECX,DWORD PTR SS:[ESP+0Ch]
			PUSH EAX
			MOV EAX,DWORD PTR SS:[ESP+01Ch]
			LEA EDX,DWORD PTR SS:[ESP+014h]
			PUSH ECX
			MOV ECX,DWORD PTR SS:[ESP+01Ch]
			PUSH EDX
			PUSH EAX
			PUSH ECX
			FSTP ST
			CALL GetYearMonthDay
			MOV EAX,DWORD PTR SS:[ESP+038h]
			ADD ESP,030h
			RETN
	}
}
//<整数型〉 取小时 （日期时间型 时间）
__declspec(naked) INT WINAPIV krnln_fnHour(INT nArgCount, MDATA_INF ArgInf)
{	
	__asm
	{
			SUB ESP,018h
			MOV ECX,DWORD PTR SS:[ESP+024h]
			MOV EDX,DWORD PTR SS:[ESP+020h]
			LEA EAX,DWORD PTR SS:[ESP+010h]
			PUSH EAX
			PUSH ECX
			PUSH EDX
			CALL _modf
			FSTP QWORD PTR SS:[ESP+014h]
			LEA EAX,DWORD PTR SS:[ESP+0Ch]
			LEA ECX,DWORD PTR SS:[ESP+010h]
			PUSH EAX
			MOV EAX,DWORD PTR SS:[ESP+01Ch]
			LEA EDX,DWORD PTR SS:[ESP+030h]
			PUSH ECX
			MOV ECX,DWORD PTR SS:[ESP+01Ch]
			PUSH EDX
			PUSH EAX
			PUSH ECX
			CALL GetHourMinuteSecond
			MOV EAX,DWORD PTR SS:[ESP+040h]
			ADD ESP,038h
			RETN
	}
}
//〈整数型〉 取分钟 （日期时间型 时间）
__declspec(naked) INT WINAPIV krnln_fnMinute(INT nArgCount, MDATA_INF ArgInf)
{	
	__asm
	{
			SUB ESP,018h
			MOV ECX,DWORD PTR SS:[ESP+024h]
			MOV EDX,DWORD PTR SS:[ESP+020h]
			LEA EAX,DWORD PTR SS:[ESP+010h]
			PUSH EAX
			PUSH ECX
			PUSH EDX
			CALL _modf
			FSTP QWORD PTR SS:[ESP+014h]
			LEA EAX,DWORD PTR SS:[ESP+0Ch]
			LEA ECX,DWORD PTR SS:[ESP+02Ch]
			PUSH EAX
			MOV EAX,DWORD PTR SS:[ESP+01Ch]
			LEA EDX,DWORD PTR SS:[ESP+014h]
			PUSH ECX
			MOV ECX,DWORD PTR SS:[ESP+01Ch]
			PUSH EDX
			PUSH EAX
			PUSH ECX
			CALL GetHourMinuteSecond
			MOV EAX,DWORD PTR SS:[ESP+040h]
			ADD ESP,038h
			RETN
	}
}
//〈整数型〉 取秒 （日期时间型 时间）
__declspec(naked) INT WINAPIV krnln_fnSecond(INT nArgCount, MDATA_INF ArgInf)
{	
	__asm
	{
			SUB ESP,018h
			MOV ECX,DWORD PTR SS:[ESP+024h]
			MOV EDX,DWORD PTR SS:[ESP+020h]
			LEA EAX,DWORD PTR SS:[ESP+010h]
			PUSH EAX
			PUSH ECX
			PUSH EDX
			CALL _modf
			FSTP QWORD PTR SS:[ESP+014h]
			LEA EAX,DWORD PTR SS:[ESP+02Ch]
			LEA ECX,DWORD PTR SS:[ESP+0Ch]
			PUSH EAX
			MOV EAX,DWORD PTR SS:[ESP+01Ch]
			LEA EDX,DWORD PTR SS:[ESP+014h]
			PUSH ECX
			MOV ECX,DWORD PTR SS:[ESP+01Ch]
			PUSH EDX
			PUSH EAX
			PUSH ECX
			CALL GetHourMinuteSecond
			MOV EAX,DWORD PTR SS:[ESP+040h]
			ADD ESP,038h
			RETN
	}
}
///////////////////////////////////////////////
__declspec(naked) WINAPIV GetMonth()
{
	__asm
	{
			SUB ESP,030h
			MOV ECX,01Eh
			MOV EAX,01Fh
			MOV DWORD PTR SS:[ESP+0Ch],ECX
			MOV DWORD PTR SS:[ESP+014h],ECX
			MOV DWORD PTR SS:[ESP+020h],ECX
			MOV DWORD PTR SS:[ESP+028h],ECX
			MOV ECX,DWORD PTR SS:[ESP+034h]
			MOV DWORD PTR SS:[ESP],EAX
			MOV DWORD PTR SS:[ESP+08h],EAX
			MOV DWORD PTR SS:[ESP+010h],EAX
			MOV DWORD PTR SS:[ESP+018h],EAX
			MOV DWORD PTR SS:[ESP+01Ch],EAX
			MOV DWORD PTR SS:[ESP+024h],EAX
			MOV DWORD PTR SS:[ESP+02Ch],EAX
			MOV EAX,ECX
			PUSH ESI
			CDQ
			MOV ESI,0190h
			MOV DWORD PTR SS:[ESP+08h],01Ch
			IDIV ESI
			TEST EDX,EDX
			JE SHORT L004014E1
			MOV EAX,ECX
			MOV ESI,064h
			CDQ
			IDIV ESI
			TEST EDX,EDX
			JE SHORT L004014F6
			AND ECX,080000003h
			JNS SHORT L004014DF
			DEC ECX
			OR ECX,0FFFFFFFCh
			INC ECX
L004014DF:
			JNZ SHORT L00401503
L004014E1:
			MOV EAX,DWORD PTR SS:[ESP+03Ch]
			MOV DWORD PTR SS:[ESP+08h],01Dh
			POP ESI
			MOV EAX,DWORD PTR SS:[ESP+EAX*04h-04h]
			ADD ESP,030h
			RETN
L004014F6:
			MOV ECX,DWORD PTR SS:[ESP+03Ch]
			POP ESI
			MOV EAX,DWORD PTR SS:[ESP+ECX*04h-04h]
			ADD ESP,030h
			RETN
L00401503:
			MOV EDX,DWORD PTR SS:[ESP+03Ch]
			POP ESI
			MOV EAX,DWORD PTR SS:[ESP+EDX*04h-04h]
			ADD ESP,030h
			RETN
	}
}
//〈整数型〉 取某月天数 （整数型 年份，整数型 月份）
__declspec(naked) KRNLN_FUNC(fnGetDaysOfSpecMonth)
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+014h]
			TEST EAX,EAX
			JLE SHORT L00401238
			CMP EAX,0Ch
			JG SHORT L00401238
			MOV ECX,DWORD PTR SS:[ESP+08h]
			CMP ECX,064h
			JL SHORT L00401238
			CMP EAX,0270Fh
			JG SHORT L00401238
			PUSH EAX
			PUSH ECX
			CALL GetMonth
			ADD ESP,08h
			RETN
L00401238:
			XOR EAX,EAX
			RETN
	}
}

////////////////////////////////////////////////////////////////////////// 到文本 
__declspec(naked) WINAPIV format(char* szText, int Max)			//max 返回的最大长度,超过就截断
{
	__asm
	{
			MOV EDX,DWORD PTR SS:[ESP+04h]
			PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+0Ch]
			PUSH EDI
			MOV EDI,EDX
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			CMP ECX,ESI
			JLE SHORT L00401023
			MOV BYTE PTR DS:[EDX+ESI],AL
			MOV ECX,ESI
L00401023:
			POP ESI
			POP EDI
			LEA EAX,DWORD PTR DS:[ECX+EDX-01h]
			MOV CL,BYTE PTR DS:[ECX+EDX-01h]
			CMP CL,02Eh
			JE SHORT L00401046
L00401033:
			CMP CL,030h
			JNZ SHORT L0040104B
			CMP EAX,EDX
			JBE SHORT L00401050
			MOV CL,BYTE PTR DS:[EAX-01h]
			DEC EAX
			CMP CL,02Eh
			JNZ SHORT L00401033
L00401046:
			MOV BYTE PTR DS:[EAX],0h
			RETN
L0040104B:
			MOV BYTE PTR DS:[EAX+01h],0h
L00401050:
			RETN
	}
}
//	〈文本型〉 到文本 （通用型数组/非数组 待转换的数据）
EXTERN_C char* WINAPIV  krnln_fnStr(INT nArgCount, MDATA_INF ArgInf)
{
	CHAR strText[30] = {0};
	CHAR strHMS [20] = {0};
	TIME_FIELDS TimeFileds = {0};
	switch (ArgInf.m_dtDataType)
	{
	case SDT_BYTE:
		sprintf(strText, "%d", ArgInf.m_byte);
		break;
	case SDT_SHORT:
		sprintf(strText, "%d", ArgInf.m_short);
		break;
	case SDT_INT:
		sprintf(strText, "%d", ArgInf.m_int);
		break;
	case SDT_INT64:
		sprintf(strText, "%ld", ArgInf.m_int64);
		break;
	case SDT_FLOAT:
		_ftoa(strText, ArgInf.m_float , 7);
		format(strText , 7);
		break;
	case SDT_DOUBLE:
		_ftoa(strText, ArgInf.m_double, 14);
		format(strText , 14);
		break;
	case SDT_BOOL:
		ArgInf.m_bool ? strcpy(strText, "真") : strcpy(strText, "假");
		break;
	case SDT_DATE_TIME:
		TimeFileds.Year =	(CSHORT)krnln_fnYear(1 ,ArgInf);
		TimeFileds.Month=	(CSHORT)krnln_fnMonth(1,ArgInf);
		TimeFileds.Day  =	(CSHORT)krnln_fnDay(1,ArgInf);
		TimeFileds.Hour =	(CSHORT)krnln_fnHour(1,ArgInf);
		TimeFileds.Minute=	(CSHORT)krnln_fnMinute(1,ArgInf);
		TimeFileds.Second=  (CSHORT)krnln_fnSecond(1,ArgInf);
		sprintf(strText,"%d年%d月%d日",TimeFileds.Year , TimeFileds.Month ,TimeFileds.Day);
		if (TimeFileds.Second)
		{
			sprintf(strHMS,"%d时%d分%d秒",TimeFileds.Hour ,TimeFileds.Minute ,TimeFileds.Second);
			strcat(strText , strHMS);
			break;
		}
		if (TimeFileds.Minute)
		{
			sprintf(strHMS,"%d时%d分",TimeFileds.Hour ,TimeFileds.Minute);
			strcat(strText , strHMS);
			break;
		}
		if (TimeFileds.Hour)
		{
			sprintf(strHMS,"%d时",TimeFileds.Hour);
			strcat(strText , strHMS);
			break;
		}
		break;
	case SDT_TEXT:
		return CloneTextData(ArgInf.m_pText);
		break;
	case SDT_BIN:
        return _CloneTextData((CHAR*)ArgInf.m_pBin+ sizeof (INT)*2,  *(LPINT)(ArgInf.m_pBin + sizeof (INT)));
		break;
	case SDT_SUB_PTR:
		sprintf(strText, "%d", (INT)ArgInf.m_pdwSubCodeAdr);
		break;
	}
	return CloneTextData(strText);
}
//〈文本型〉 时间到文本 （日期时间型 欲转换到文本的时间，［整数型 转换部分］） 
EXTERN_C char* WINAPIV  krnln_fnTimeToText(INT nArgCount, MDATA_INF ArgInf)
{
	CHAR strText[30] = {0};
	TIME_FIELDS TimeFileds = {0};
	PMDATA_INF pArgInf = &ArgInf;

	TimeFileds.Year =	(CSHORT)krnln_fnYear(1 ,ArgInf);
	TimeFileds.Month=	(CSHORT)krnln_fnMonth(1,ArgInf);
	TimeFileds.Day  =	(CSHORT)krnln_fnDay(1,ArgInf);
	TimeFileds.Hour =	(CSHORT)krnln_fnHour(1,ArgInf);
	TimeFileds.Minute=	(CSHORT)krnln_fnMinute(1,ArgInf);
	TimeFileds.Second=  (CSHORT)krnln_fnSecond(1,ArgInf);
	switch(pArgInf[1].m_int)
	{
	case 2:
		sprintf(strText,"%d年%d月%d日",TimeFileds.Year , TimeFileds.Month ,TimeFileds.Day);
	break;
	case 3:
		sprintf(strText,"%d时%d分%d秒",TimeFileds.Hour ,TimeFileds.Minute ,TimeFileds.Second);
	break;
	default:
		sprintf(strText,"%d年%d月%d日%d时%d分%d秒",TimeFileds.Year , TimeFileds.Month ,TimeFileds.Day ,TimeFileds.Hour ,TimeFileds.Minute ,TimeFileds.Second);
	}
	return CloneTextData(strText);
}
//〈逻辑型〉 置现行时间 （日期时间型 欲设置的时间）
EXTERN_C BOOL WINAPIV krnln_fnSetSysTime(INT nArgCount, MDATA_INF ArgInf)
{
	LARGE_INTEGER SystemTime = {0};
	LARGE_INTEGER PreviousTime = {0};
	TIME_FIELDS TimeFileds = {0};
	TimeFileds.Year =	(CSHORT)krnln_fnYear(1 ,ArgInf);
	TimeFileds.Month=	(CSHORT)krnln_fnMonth(1,ArgInf);
	TimeFileds.Day  =	(CSHORT)krnln_fnDay(1,ArgInf);
	TimeFileds.Hour =	(CSHORT)krnln_fnHour(1,ArgInf);
	TimeFileds.Minute=	(CSHORT)krnln_fnMinute(1,ArgInf);
	TimeFileds.Second=  (CSHORT)krnln_fnSecond(1,ArgInf);
	RtlTimeFieldsToTime(&TimeFileds , &SystemTime);
	if (ZwSetSystemTime(&SystemTime , &PreviousTime))
	{
		return FALSE;
	}
		return TRUE;
}
//〈日期时间型〉 取日期 （日期时间型 时间）
EXTERN_C __int64 WINAPIV krnln_fnGetDatePart(INT nArgCount, MDATA_INF ArgInf)
{
	TIME_FIELDS TimeFileds = {0};
	TimeFileds.Year =	(CSHORT)krnln_fnYear(1 ,ArgInf);
	TimeFileds.Month=	(CSHORT)krnln_fnMonth(1,ArgInf);
	TimeFileds.Day  =	(CSHORT)krnln_fnDay(1,ArgInf);
	return TimeFieldsToTimeInt64(&TimeFileds);
}
//〈日期时间型〉 取时间 （日期时间型 时间）
EXTERN_C __int64 WINAPIV krnln_fnGetTimePart(INT nArgCount, MDATA_INF ArgInf)
{
	TIME_FIELDS TimeFileds = {0};
	TimeFileds.Year =	(CSHORT)2000;
	TimeFileds.Month=	(CSHORT)00001;
	TimeFileds.Day  =	(CSHORT)00001;
	TimeFileds.Hour =	(CSHORT)krnln_fnHour(1,ArgInf);
	TimeFileds.Minute=	(CSHORT)krnln_fnMinute(1,ArgInf);
	TimeFileds.Second=  (CSHORT)krnln_fnSecond(1,ArgInf);
	return TimeFieldsToTimeInt64(&TimeFileds);
}
//〈日期时间型〉 指定时间 （整数型 年，［整数型 月］，［整数型 日］，［整数型 小时］，［整数型 分钟］，［整数型 秒］） 
__declspec (naked) TimeSerial(INT Year, INT Month)		//取得某一年某一月的最大天数,返回该月的最大天数
{
	__asm
	{
			SUB ESP,030h
			MOV ECX,01Eh
			MOV EAX,01Fh
			MOV DWORD PTR SS:[ESP+0Ch],ECX
			MOV DWORD PTR SS:[ESP+014h],ECX
			MOV DWORD PTR SS:[ESP+020h],ECX
			MOV DWORD PTR SS:[ESP+028h],ECX
			MOV ECX,DWORD PTR SS:[ESP+034h]
			MOV DWORD PTR SS:[ESP],EAX
			MOV DWORD PTR SS:[ESP+08h],EAX
			MOV DWORD PTR SS:[ESP+010h],EAX
			MOV DWORD PTR SS:[ESP+018h],EAX
			MOV DWORD PTR SS:[ESP+01Ch],EAX
			MOV DWORD PTR SS:[ESP+024h],EAX
			MOV DWORD PTR SS:[ESP+02Ch],EAX
			MOV EAX,ECX
			PUSH ESI
			CDQ
			MOV ESI,0190h
			MOV DWORD PTR SS:[ESP+08h],01Ch
			IDIV ESI
			TEST EDX,EDX
			JE SHORT L00401571
			MOV EAX,ECX
			MOV ESI,064h
			CDQ
			IDIV ESI
			TEST EDX,EDX
			JE SHORT L00401586
			AND ECX,080000003h
			JNS SHORT L0040156F
			DEC ECX
			OR ECX,0FFFFFFFCh
			INC ECX
L0040156F:
			JNZ SHORT L00401593
L00401571:
			MOV EAX,DWORD PTR SS:[ESP+03Ch]
			MOV DWORD PTR SS:[ESP+08h],01Dh
			POP ESI
			MOV EAX,DWORD PTR SS:[ESP+EAX*04h-04h]
			ADD ESP,030h
			RETN
L00401586:
			MOV ECX,DWORD PTR SS:[ESP+03Ch]
			POP ESI
			MOV EAX,DWORD PTR SS:[ESP+ECX*04h-04h]
			ADD ESP,030h
			RETN
L00401593:
			MOV EDX,DWORD PTR SS:[ESP+03Ch]
			POP ESI
			MOV EAX,DWORD PTR SS:[ESP+EDX*04h-04h]
			ADD ESP,030h
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnTimeSerial)		//指定时间
{
	__asm
	{
			PUSH EBP
			MOV EBP,ESP
			MOV EAX,DWORD PTR SS:[EBP+0Ch]
			PUSH EBX
			PUSH ESI
			CMP EAX,064h
			PUSH EDI
			MOV EBX,EAX
			JGE SHORT L00401215
			MOV EBX,064h
L00401215:
			MOV EAX,DWORD PTR SS:[EBP+020h]
			TEST EAX,EAX
			JNZ SHORT L00401223
			MOV EDI,01h
			JMP SHORT L0040123E
L00401223:
			MOV EAX,DWORD PTR SS:[EBP+018h]
			CMP EAX,01h
			MOV EDI,EAX
			JGE SHORT L00401234
			MOV EDI,01h
			JMP SHORT L0040123E
L00401234:
			CMP EAX,0Ch
			JLE SHORT L0040123E
			MOV EDI,0Ch
L0040123E:
			MOV EAX,DWORD PTR SS:[EBP+02Ch]
			TEST EAX,EAX
			JNZ SHORT L0040124C
			MOV ESI,01h
			JMP SHORT L0040126B
L0040124C:
			MOV ESI,DWORD PTR SS:[EBP+024h]
			CMP ESI,01h
			JGE SHORT L0040125B
			MOV ESI,01h
			JMP SHORT L0040126B
L0040125B:
			PUSH EDI
			PUSH EBX
			CALL TimeSerial
			ADD ESP,08h
			CMP ESI,EAX
			JLE SHORT L0040126B
			MOV ESI,EAX
L0040126B:
			MOV EAX,DWORD PTR SS:[EBP+030h]
			TEST EAX,EAX
			MOV EDX,EAX
			JGE SHORT L00401278
			XOR EDX,EDX
			JMP SHORT L00401282
L00401278:
			CMP EAX,017h
			JLE SHORT L00401282
			MOV EDX,017h
L00401282:
			MOV EAX,DWORD PTR SS:[EBP+03Ch]
			TEST EAX,EAX
			MOV ECX,EAX
			JGE SHORT L0040128F
			XOR ECX,ECX
			JMP SHORT L00401299
L0040128F:
			CMP EAX,03Bh
			JLE SHORT L00401299
			MOV ECX,03Bh
L00401299:
			TEST EAX,EAX
			JGE SHORT L004012A1
			XOR EAX,EAX
			JMP SHORT L004012AB
L004012A1:
			CMP EAX,03Bh
			JLE SHORT L004012AB
			MOV EAX,03Bh
L004012AB:
			PUSH EAX
			PUSH ECX
			PUSH EDX
			PUSH ESI
			PUSH EDI
			PUSH EBX
			CALL TimeToInt64
			FSTP QWORD PTR SS:[EBP+0Ch]
			ADD ESP,018h
			MOV EAX,DWORD PTR SS:[EBP+0Ch]
			MOV EDX,DWORD PTR SS:[EBP+010h]
			POP EDI
			POP ESI
			POP EBX
			POP EBP
			RETN
	}
}
//〈日期时间型〉 到时间 （通用型 欲转换的文本）
EXTERN_C __int64 WINAPIV krnln_fnToTime(INT nArgCount, MDATA_INF ArgInf)
{
	switch(ArgInf.m_dtDataType)
	{
	case SDT_TEXT:
	   	return ToTime(ArgInf.m_pText);
	case SDT_DATE_TIME:
		return ArgInf.m_int64;
	default:
		return (__int64)0xC124103400000000;
	}
}
//〈整数型〉 文本比较 （文本型 待比较文本一，文本型 待比较文本二，逻辑型 是否区分大小写）
__declspec(naked) KRNLN_FUNC(fnStrComp)
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+020h]
			TEST EAX,EAX
			JE SHORT L00401223
			MOV EAX,DWORD PTR SS:[ESP+08h]
			PUSH EBX
			PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+01Ch]
L004011F2:
			MOV DL,BYTE PTR DS:[EAX]
			MOV BL,BYTE PTR DS:[ESI]
			MOV CL,DL
			CMP DL,BL
			JNZ SHORT L0040121B
			TEST CL,CL
			JE SHORT L00401216
			MOV DL,BYTE PTR DS:[EAX+01h]
			MOV BL,BYTE PTR DS:[ESI+01h]
			MOV CL,DL
			CMP DL,BL
			JNZ SHORT L0040121B
			ADD EAX,02h
			ADD ESI,02h
			TEST CL,CL
			JNZ SHORT L004011F2
L00401216:
			POP ESI
			XOR EAX,EAX
			POP EBX
			RETN
L0040121B:
			SBB EAX,EAX
			POP ESI
			SBB EAX,-01h
			POP EBX
			RETN
L00401223:
			MOV EAX,DWORD PTR SS:[ESP+014h]
			MOV ECX,DWORD PTR SS:[ESP+08h]
			PUSH EAX
			PUSH ECX
			CALL _stricmp
			ADD ESP,08h
			RETN
	}
}

__declspec(naked) WINAPIV Replace1()

{
	__asm
	{
			PUSH EBX
			MOV EBX,DWORD PTR SS:[ESP+0xC]
			TEST EBX,EBX
			JG L007
			XOR EAX,EAX
			POP EBX
			RETN
L007:
			PUSH ESI
			LEA EAX,DWORD PTR DS:[EBX+0x1]
			PUSH EDI
			PUSH EAX
			CALL krnl_MMalloc
			MOV ESI,DWORD PTR SS:[ESP+0x14]
			MOV ECX,EBX
			MOV EDX,ECX
			MOV EDI,EAX
			SHR ECX,2
			REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
			MOV ECX,EDX
			ADD ESP,4
			AND ECX,3
			REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
			POP EDI
			MOV BYTE PTR DS:[EAX+EBX],0
			POP ESI
			POP EBX
			RETN
	}
}

__declspec(naked) WINAPIV Replace2()
{
	__asm
	{
		MOV ECX,DWORD PTR SS:[ESP+0Ch]
		PUSH EDI
		TEST ECX,ECX
		JE SHORT L00401533
		PUSH ESI
		PUSH EBX
		MOV EBX,ECX
		MOV ESI,DWORD PTR SS:[ESP+014h]
		TEST ESI,03h
		MOV EDI,DWORD PTR SS:[ESP+010h]
		JNZ SHORT L004014D4
		SHR ECX,02h
		JNZ SHORT L00401541
		JMP SHORT L004014F5
L004014D4:
		MOV AL,BYTE PTR DS:[ESI]
		INC ESI
		MOV BYTE PTR DS:[EDI],AL
		INC EDI
		DEC ECX
		JE SHORT L00401502
		TEST AL,AL
		JE SHORT L0040150A
		TEST ESI,03h
		JNZ SHORT L004014D4
		MOV EBX,ECX
		SHR ECX,02h
		JNZ SHORT L00401541
L004014F0:
		AND EBX,03h
		JE SHORT L00401502
L004014F5:
		MOV AL,BYTE PTR DS:[ESI]
		INC ESI
		MOV BYTE PTR DS:[EDI],AL
		INC EDI
		TEST AL,AL
		JE SHORT L0040152E
		DEC EBX
		JNZ SHORT L004014F5
L00401502:
		MOV EAX,DWORD PTR SS:[ESP+010h]
		POP EBX
		POP ESI
		POP EDI
		RETN
L0040150A:
		TEST EDI,03h
		JE SHORT L00401524
L00401512:
		MOV BYTE PTR DS:[EDI],AL
		INC EDI
		DEC ECX
		JE L004015A6
		TEST EDI,03h
		JNZ SHORT L00401512
L00401524:
		MOV EBX,ECX
		SHR ECX,02h
		JNZ SHORT L00401597
L0040152B:
		MOV BYTE PTR DS:[EDI],AL
		INC EDI
L0040152E:
		DEC EBX
		JNZ SHORT L0040152B
		POP EBX
		POP ESI
L00401533:
		MOV EAX,DWORD PTR SS:[ESP+08h]
		POP EDI
		RETN
L00401539:
		MOV DWORD PTR DS:[EDI],EDX
		ADD EDI,04h
		DEC ECX
		JE SHORT L004014F0
L00401541:
		MOV EDX,07EFEFEFFh
		MOV EAX,DWORD PTR DS:[ESI]
		ADD EDX,EAX
		XOR EAX,0FFFFFFFFh
		XOR EAX,EDX
		MOV EDX,DWORD PTR DS:[ESI]
		ADD ESI,04h
		TEST EAX,081010100h
		JE SHORT L00401539
		TEST DL,DL
		JE SHORT L0040158B
		TEST DH,DH
		JE SHORT L00401581
		TEST EDX,0FF0000h
		JE SHORT L00401577
		TEST EDX,0FF000000h
		JNZ SHORT L00401539
		MOV DWORD PTR DS:[EDI],EDX
		JMP SHORT L0040158F
L00401577:
		AND EDX,0FFFFh
		MOV DWORD PTR DS:[EDI],EDX
		JMP SHORT L0040158F
L00401581:
		AND EDX,0FFh
		MOV DWORD PTR DS:[EDI],EDX
		JMP SHORT L0040158F
L0040158B:
		XOR EDX,EDX
		MOV DWORD PTR DS:[EDI],EDX
L0040158F:
		ADD EDI,04h
		XOR EAX,EAX
		DEC ECX
		JE SHORT L004015A1
L00401597:
		XOR EAX,EAX
L00401599:
		MOV DWORD PTR DS:[EDI],EAX
		ADD EDI,04h
		DEC ECX
		JNZ SHORT L00401599
L004015A1:
		AND EBX,03h
		JNZ SHORT L0040152B
L004015A6:
		MOV EAX,DWORD PTR SS:[ESP+010h]
		POP EBX
		POP ESI
		POP EDI
		RETN
	}
}
__declspec(naked) KRNLN_FUNC(fnReplaceText) //文本替换
{
	__asm
	{
  PUSH EBX
  PUSH EBP
  PUSH ESI
  MOV ESI,DWORD PTR SS:[ESP+0x14]
  PUSH EDI
  MOV EDI,ESI
  OR ECX,0xFFFFFFFF
  XOR EAX,EAX
  REPNE SCAS BYTE PTR ES:[EDI]
  NOT ECX
  DEC ECX
  MOV EDX,ECX
  JNZ L044
  MOV EAX,DWORD PTR SS:[ESP+0x44]
  TEST EAX,EAX
  JNZ L022
  POP EDI
  POP ESI
  POP EBP
  XOR EAX,EAX
  POP EBX
  RETN
L022:
  MOV EDX,DWORD PTR SS:[ESP+0x3C]
  OR ECX,0xFFFFFFFF
  MOV EDI,EDX
  XOR EAX,EAX
  REPNE SCAS BYTE PTR ES:[EDI]
  NOT ECX
  DEC ECX
  JNZ L035
  POP EDI
  POP ESI
  POP EBP
  POP EBX
  RETN
L035:
  PUSH ECX
  PUSH EDX
  CALL Replace1
  ADD ESP,8
  POP EDI
  POP ESI
  POP EBP
  POP EBX
  RETN
L044:
  MOV EBP,DWORD PTR SS:[ESP+0x24]
  CMP EBP,EDX
  JA L147
  MOV EBX,DWORD PTR SS:[ESP+0x30]
  TEST EBX,EBX
  JLE L147
  MOV EAX,DWORD PTR SS:[ESP+0x44]
  XOR ECX,ECX
  XOR ESI,ESI
  TEST EAX,EAX
  JE L062
  MOV ESI,DWORD PTR SS:[ESP+0x3C]
  OR ECX,0xFFFFFFFF
  MOV EDI,ESI
  XOR EAX,EAX
  REPNE SCAS BYTE PTR ES:[EDI]
  NOT ECX
  DEC ECX
L062:
  TEST EBP,EBP
  JG L065
  MOV EBP,1
L065:
  DEC EBP
  MOV EAX,EDX
  SUB EAX,EBP
  CMP EAX,EBX
  JGE L073
  ADD ECX,EBP
  MOV EDI,1
  JMP L076
L073:
  SUB ECX,EBX
  ADD ECX,EDX
  XOR EDI,EDI
L076:
  INC ECX
  PUSH ECX
  CALL krnl_MMalloc
  ADD ESP,4
  MOV EBX,EAX
  TEST EDI,EDI
  PUSH EBP
  JE L094
  MOV ECX,DWORD PTR SS:[ESP+0x1C]
  PUSH ECX
  PUSH EBX
  CALL Replace2
  ADD ESP,0x0C
  MOV BYTE PTR DS:[EBX+EBP],0
  TEST ESI,ESI
  JE L141
  MOV EDI,ESI
  JMP L124
L094:
  MOV EAX,DWORD PTR SS:[ESP+0x1C]
  PUSH EAX
  PUSH EBX
  CALL Replace2
  ADD ESP,0x0C
  MOV BYTE PTR DS:[EBX+EBP],0
  TEST ESI,ESI
  JE L120
  MOV EDI,ESI
  OR ECX,0xFFFFFFFF
  XOR EAX,EAX
  REPNE SCAS BYTE PTR ES:[EDI]
  NOT ECX
  SUB EDI,ECX
  MOV ESI,EDI
  MOV EDX,ECX
  MOV EDI,EBX
  OR ECX,0xFFFFFFFF
  REPNE SCAS BYTE PTR ES:[EDI]
  MOV ECX,EDX
  DEC EDI
  SHR ECX,2
  REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
  MOV ECX,EDX
  AND ECX,3
  REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
L120:
  MOV EAX,DWORD PTR SS:[ESP+0x30]
  LEA EDI,DWORD PTR DS:[EAX+EBP]
  MOV EBP,DWORD PTR SS:[ESP+0x18]
  ADD EDI,EBP
L124:
  OR ECX,0xFFFFFFFF
  XOR EAX,EAX
  REPNE SCAS BYTE PTR ES:[EDI]
  NOT ECX
  SUB EDI,ECX
  MOV ESI,EDI
  MOV EDX,ECX
  MOV EDI,EBX
  OR ECX,0xFFFFFFFF
  REPNE SCAS BYTE PTR ES:[EDI]
  MOV ECX,EDX
  DEC EDI
  SHR ECX,2
  REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
  MOV ECX,EDX
  AND ECX,3
  REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
L141:
  POP EDI
  POP ESI
  MOV EAX,EBX
  POP EBP
  POP EBX
  RETN
L147:
  PUSH EDX
  PUSH ESI
  CALL Replace1
  ADD ESP,8
  POP EDI
  POP ESI
  POP EBP
  POP EBX
  RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnSpace)  //取空白文本
{
	__asm
	{
			PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+0Ch]
			TEST ESI,ESI
			JG SHORT L004012BD
			XOR EAX,EAX
			POP ESI
			RETN
L004012BD:
			PUSH EBX
			LEA EAX,DWORD PTR DS:[ESI+01h]
			PUSH EDI
			PUSH EAX
		    CALL krnl_MMalloc
			MOV ECX,ESI
			MOV EDX,EAX
			MOV EBX,ECX
			MOV EAX,020202020h
			MOV EDI,EDX
			ADD ESP,04h
			SHR ECX,02h
			REP STOS DWORD PTR ES:[EDI]
			MOV ECX,EBX
			AND ECX,03h
			REP STOS BYTE PTR ES:[EDI]
			POP EDI
			MOV BYTE PTR DS:[EDX+ESI],0h
			POP EBX
			MOV EAX,EDX
			POP ESI
			RETN
			

	}
}

__declspec(naked) KRNLN_FUNC(fnString)  //取重复文本
{
__asm
{
		PUSH EBX
		PUSH EDI
		MOV EDI,DWORD PTR SS:[ESP+01Ch]
		OR ECX,0FFFFFFFFh
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		MOV EDI,ECX
		JE SHORT L00401313
		MOV EBX,DWORD PTR SS:[ESP+010h]
		TEST EBX,EBX
		JLE SHORT L00401313
		MOV EAX,EBX
		PUSH EBP
		IMUL EAX,EDI
		INC EAX
		PUSH ESI
		PUSH EAX
		CALL krnl_MMalloc
		ADD ESP,04h
		MOV EBP,EAX
		TEST EBX,EBX
		MOV ESI,EBP
		JLE SHORT L00401309
L004012F5:
		MOV ECX,DWORD PTR SS:[ESP+024h]
		PUSH EDI
		PUSH ECX
		PUSH ESI
		CALL Replace2
		ADD ESP,0Ch
		ADD ESI,EDI
		DEC EBX
		JNZ SHORT L004012F5
L00401309:
		MOV BYTE PTR DS:[ESI],0h
		MOV EAX,EBP
		POP ESI
		POP EBP
		POP EDI
		POP EBX
		RETN
L00401313:
		POP EDI
		XOR EAX,EAX
		POP EBX
		RETN
}
}

__declspec(naked) KRNLN_FUNC(fnpstr) //指针到文本
{
	__asm
	{
			PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+0Ch]
			PUSH EDI
			MOV EDI,ESI
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			JNZ SHORT L004012C7
			POP EDI
			POP ESI
			RETN
L004012C7:
			INC ECX
			PUSH ECX
			CALL krnl_MMalloc
			MOV EDX,EAX
			MOV EDI,ESI
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			ADD ESP,04h
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			SUB EDI,ECX
			MOV EAX,ECX
			MOV ESI,EDI
			MOV EDI,EDX
			SHR ECX,02h
			REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
			MOV ECX,EAX
			MOV EAX,EDX
			AND ECX,03h
			REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
			POP EDI
			POP ESI
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnBinLen) //取字节集长度
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+08h]
			TEST EAX,EAX
			JNZ SHORT L004011D9
			RETN
L004011D9:
			MOV EAX,DWORD PTR DS:[EAX+04h]
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnBinLeft) //取字节集左边
{
	__asm
	{
			MOV EDX,DWORD PTR SS:[ESP+08h]
			TEST EDX,EDX
			JE SHORT L004012EB
			MOV EAX,DWORD PTR DS:[EDX+04h]
			TEST EAX,EAX
			JE SHORT L004012EB
			MOV ECX,DWORD PTR SS:[ESP+014h]
			TEST ECX,ECX
			JLE SHORT L004012EB
			CMP EAX,ECX
			JLE SHORT L004012DD
			MOV EAX,ECX
L004012DD:
			ADD EDX,08h
			PUSH EAX
			PUSH EDX
			CALL CloneBinData
			ADD ESP,08h
			RETN
L004012EB:
			XOR EAX,EAX
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnBinRight)		//取字节集右边
{
	__asm
	{
			MOV EDX,DWORD PTR SS:[ESP+08h]
			TEST EDX,EDX
			JE SHORT L004012EC
			MOV EAX,DWORD PTR DS:[EDX+04h]
			TEST EAX,EAX
			JE SHORT L004012EC
			MOV ECX,DWORD PTR SS:[ESP+014h]
			TEST ECX,ECX
			JLE SHORT L004012EC
			SUB EAX,ECX
			JNS SHORT L004012DD
			XOR EAX,EAX
L004012DD:
			LEA EAX,DWORD PTR DS:[EDX+EAX+08h]
			PUSH ECX
			PUSH EAX
			CALL CloneBinData
			ADD ESP,08h
			RETN
L004012EC:
			XOR EAX,EAX
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnBinMid)		//取字节集中间
{
	__asm
	{
			PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+0Ch]
			TEST ESI,ESI
			PUSH EDI
			JE SHORT L0040131B
			MOV EAX,DWORD PTR DS:[ESI+04h]
			MOV EDX,DWORD PTR SS:[ESP+028h]
			TEST EAX,EAX
			JE SHORT L0040131B
			TEST EDX,EDX
			JLE SHORT L0040131B
			MOV ECX,DWORD PTR SS:[ESP+01Ch]
			LEA EDI,DWORD PTR DS:[ECX-01h]
			CMP EDI,EAX
			JG SHORT L0040131B
			TEST ECX,ECX
			JG SHORT L004012FD
			MOV ECX,01h
L004012FD:
			LEA EDI,DWORD PTR DS:[EDX+ECX-01h]
			CMP EDI,EAX
			JL SHORT L0040130A
			SUB EAX,ECX
			INC EAX
			MOV EDX,EAX
L0040130A:
			LEA EAX,DWORD PTR DS:[ESI+ECX+07h]
			PUSH EDX
			PUSH EAX
			CALL CloneBinData
			ADD ESP,08h
			POP EDI
			POP ESI
			RETN
L0040131B:
			POP EDI
			XOR EAX,EAX
			POP ESI
			RETN
	}
}


__declspec(naked) KRNLN_FUNC(fnInBin)		//寻找字节集
{
	__asm
	{
			PUSH ECX
			MOV EDX,DWORD PTR SS:[ESP+0Ch]
			PUSH EBX
			PUSH EBP
			PUSH ESI
			TEST EDX,EDX
			PUSH EDI
			JE SHORT L00401257
			MOV EBP,DWORD PTR SS:[ESP+028h]
			TEST EBP,EBP
			JE SHORT L00401257
			MOV EAX,DWORD PTR DS:[EDX+04h]
			MOV ECX,DWORD PTR SS:[EBP+04h]
			TEST EAX,EAX
			MOV DWORD PTR SS:[ESP+010h],ECX
			JE SHORT L00401257
			TEST ECX,ECX
			JE SHORT L00401257
			CMP ECX,EAX
			JG SHORT L00401257
			MOV ESI,DWORD PTR SS:[ESP+03Ch]
			TEST ESI,ESI
			JE SHORT L0040121B
			MOV EDI,DWORD PTR SS:[ESP+034h]
			TEST EDI,EDI
			JG SHORT L00401220
L0040121B:
			MOV EDI,01h
L00401220:
			SUB EAX,ECX
			MOV ESI,EAX
			LEA EAX,DWORD PTR DS:[ESI+01h]
			CMP EDI,EAX
			JG SHORT L00401257
			ADD EDX,08h
			ADD EBP,08h
			MOV DWORD PTR SS:[ESP+01Ch],EBP
			LEA EAX,DWORD PTR DS:[EDX+EDI-01h]
			LEA EBX,DWORD PTR DS:[EDX+ESI]
			CMP EAX,EBX
			JA SHORT L00401257
			JMP SHORT L00401246
L00401242:
			MOV ECX,DWORD PTR SS:[ESP+010h]
L00401246:
			MOV EDI,DWORD PTR SS:[ESP+01Ch]
			MOV ESI,EAX
			XOR EBP,EBP
			REPE CMPSB
			JE SHORT L00401260
			INC EAX
			CMP EAX,EBX
			JBE SHORT L00401242
L00401257:
			POP EDI
			POP ESI
			POP EBP
			OR EAX,0FFFFFFFFh
			POP EBX
			POP ECX
			RETN
L00401260:
			POP EDI
			SUB EAX,EDX
			POP ESI
			POP EBP
			INC EAX
			POP EBX
			POP ECX
			RETN
	}
}
__declspec(naked) KRNLN_FUNC(fnInBinRev)	//倒找字节集
{
	__asm
	{
			MOV EDX,DWORD PTR SS:[ESP+08h]
			PUSH EBX
			PUSH EBP
			PUSH ESI
			TEST EDX,EDX
			PUSH EDI
			JE SHORT L00401248
			MOV ESI,DWORD PTR SS:[ESP+024h]
			TEST ESI,ESI
			JE SHORT L00401248
			MOV EAX,DWORD PTR DS:[EDX+04h]
			MOV EBX,DWORD PTR DS:[ESI+04h]
			TEST EAX,EAX
			JE SHORT L00401248
			TEST EBX,EBX
			JE SHORT L00401248
			CMP EBX,EAX
			JG SHORT L00401248
			MOV ECX,DWORD PTR SS:[ESP+038h]
			SUB EAX,EBX
			TEST ECX,ECX
			JE SHORT L00401222
			MOV ECX,DWORD PTR SS:[ESP+030h]
			TEST ECX,ECX
			JLE SHORT L00401222
			SUB ECX,EBX
			JNS SHORT L0040121E
			XOR ECX,ECX
L0040121E:
			CMP ECX,EAX
			JLE SHORT L00401224
L00401222:
			MOV ECX,EAX
L00401224:
			ADD EDX,08h
			ADD ESI,08h
			MOV DWORD PTR SS:[ESP+018h],ESI
			LEA EAX,DWORD PTR DS:[EDX+ECX]
			CMP EAX,EDX
			JB SHORT L00401248
L00401235:
			MOV EDI,DWORD PTR SS:[ESP+018h]
			MOV ECX,EBX
			MOV ESI,EAX
			XOR EBP,EBP
			REPE CMPSB
			JE SHORT L00401250
			DEC EAX
			CMP EAX,EDX
			JNB SHORT L00401235
L00401248:
			POP EDI
			POP ESI
			POP EBP
			OR EAX,0FFFFFFFFh
			POP EBX
			RETN
L00401250:
			POP EDI
			SUB EAX,EDX
			POP ESI
			POP EBP
			INC EAX
			POP EBX
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnRpBin)	//字节集替换
{
	__asm
	{
		MOV ECX,DWORD PTR SS:[ESP+08h]
		SUB ESP,010h
		PUSH EBX
		PUSH EDI
		XOR EDI,EDI
		CMP ECX,EDI
		JNZ SHORT L004012E7
		POP EDI
		XOR EAX,EAX
		POP EBX
		ADD ESP,010h
		RETN
L004012E7:
		MOV EAX,DWORD PTR DS:[ECX+04h]
		CMP EAX,EDI
		JNZ SHORT L00401320
		MOV ECX,DWORD PTR SS:[ESP+044h]
		CMP ECX,EDI
		JNZ SHORT L004012FE
		POP EDI
		XOR EAX,EAX
		POP EBX
		ADD ESP,010h
		RETN
L004012FE:
		MOV EAX,DWORD PTR DS:[ECX+04h]
		CMP EAX,EDI
		JNZ SHORT L0040130D
		POP EDI
		XOR EAX,EAX
		POP EBX
		ADD ESP,010h
		RETN
L0040130D:
		ADD ECX,08h
		PUSH EAX
		PUSH ECX
		CALL CloneBinData
		ADD ESP,08h
		POP EDI
		POP EBX
		ADD ESP,010h
		RETN
L00401320:
		MOV EBX,DWORD PTR SS:[ESP+02Ch]
		ADD ECX,08h
		CMP EBX,EAX
		MOV DWORD PTR SS:[ESP+0Ch],ECX
		JA L00401427
		MOV EDX,DWORD PTR SS:[ESP+038h]
		CMP EDX,EDI
		JLE L00401427
		MOV ECX,DWORD PTR SS:[ESP+04Ch]
		PUSH EBP
		XOR EBP,EBP
		CMP ECX,EDI
		PUSH ESI
		MOV DWORD PTR SS:[ESP+010h],EDI
		JE SHORT L00401361
		MOV ECX,DWORD PTR SS:[ESP+04Ch]
		CMP ECX,EDI
		JE SHORT L00401361
		MOV EBP,DWORD PTR DS:[ECX+04h]
		LEA ESI,DWORD PTR DS:[ECX+08h]
		MOV DWORD PTR SS:[ESP+010h],ESI
L00401361:
		CMP EBX,EDI
		JG SHORT L0040136A
		MOV EBX,01h
L0040136A:
		DEC EBX
		MOV ECX,EAX
		SUB ECX,EBX
		CMP ECX,EDX
		MOV DWORD PTR SS:[ESP+018h],ECX
		JGE SHORT L00401381
		LEA ESI,DWORD PTR DS:[EBX+EBP]
		MOV EDI,01h
		JMP SHORT L00401387
L00401381:
		MOV ESI,EBP
		SUB ESI,EDX
		ADD ESI,EAX
L00401387:
		LEA EAX,DWORD PTR DS:[ESI+08h]
		PUSH EAX
		CALL krnl_MMalloc
		LEA EDX,DWORD PTR DS:[EAX+04h]
		MOV DWORD PTR DS:[EAX],01h
		ADD ESP,04h
		MOV ECX,EBX
		MOV DWORD PTR DS:[EDX],ESI
		MOV ESI,DWORD PTR SS:[ESP+014h]
		ADD EDX,04h
		MOV DWORD PTR SS:[ESP+01Ch],EAX
		TEST EDI,EDI
		MOV EDI,EDX
		MOV EAX,ECX
		JE SHORT L004013CE
		SHR ECX,02h
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EAX
		AND ECX,03h
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
		MOV ESI,DWORD PTR SS:[ESP+010h]
		TEST ESI,ESI
		JE SHORT L0040141B
		MOV ECX,EBP
		LEA EDI,DWORD PTR DS:[EDX+EBX]
		JMP SHORT L0040140D
L004013CE:
		SHR ECX,02h
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EAX
		ADD EDX,EBX
		AND ECX,03h
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
		MOV ESI,DWORD PTR SS:[ESP+010h]
		TEST ESI,ESI
		JE SHORT L004013F8
		MOV ECX,EBP
		MOV EDI,EDX
		MOV EAX,ECX
		SHR ECX,02h
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EAX
		AND ECX,03h
		ADD EDX,EBP
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
L004013F8:
		MOV EAX,DWORD PTR SS:[ESP+040h]
		MOV ECX,DWORD PTR SS:[ESP+018h]
		MOV EBP,DWORD PTR SS:[ESP+014h]
		SUB ECX,EAX
		LEA ESI,DWORD PTR DS:[EAX+EBX]
		MOV EDI,EDX
		ADD ESI,EBP
L0040140D:
		MOV EDX,ECX
		SHR ECX,02h
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EDX
		AND ECX,03h
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
L0040141B:
		MOV EAX,DWORD PTR SS:[ESP+01Ch]
		POP ESI
		POP EBP
		POP EDI
		POP EBX
		ADD ESP,010h
		RETN
L00401427:
		PUSH EAX
		PUSH ECX
		CALL CloneBinData
		ADD ESP,08h
		POP EDI
		POP EBX
		ADD ESP,010h
		RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnRpSubBin)	//子字节集替换
{
	__asm
	{
		MOV EAX,DWORD PTR SS:[ESP+08h]
		SUB ESP,028h
		XOR EDX,EDX
		PUSH EBX
		PUSH EBP
		PUSH ESI
		CMP EAX,EDX
		PUSH EDI
		JNZ SHORT L004012EB
		POP EDI
		POP ESI
		POP EBP
		XOR EAX,EAX
		POP EBX
		ADD ESP,028h
		RETN
L004012EB:
		MOV EDI,DWORD PTR DS:[EAX+04h]
		LEA ESI,DWORD PTR DS:[EAX+08h]
		CMP EDI,EDX
		MOV DWORD PTR SS:[ESP+018h],EDI
		MOV DWORD PTR SS:[ESP+02Ch],ESI
		JNZ SHORT L00401307
		POP EDI
		POP ESI
		POP EBP
		XOR EAX,EAX
		POP EBX
		ADD ESP,028h
		RETN
L00401307:
		MOV EAX,DWORD PTR SS:[ESP+04Ch]
		CMP EAX,EDX
		JE L004013E1
		MOV EBP,DWORD PTR DS:[EAX+04h]
		CMP EBP,EDI
		MOV DWORD PTR SS:[ESP+034h],EBP
		JLE SHORT L00401328
		POP EDI
		POP ESI
		POP EBP
		XOR EAX,EAX
		POP EBX
		ADD ESP,028h
		RETN
L00401328:
		CMP EBP,EDX
		JE L004013E1
		ADD EAX,08h
		MOV DWORD PTR SS:[ESP+024h],EDX
		MOV DWORD PTR SS:[ESP+020h],EAX
		MOV EAX,DWORD PTR SS:[ESP+060h]
		CMP EAX,EDX
		MOV DWORD PTR SS:[ESP+014h],EDX
		JE SHORT L0040135D
		MOV EAX,DWORD PTR SS:[ESP+058h]
		CMP EAX,EDX
		JE SHORT L0040135D
		LEA ECX,DWORD PTR DS:[EAX+08h]
		MOV EAX,DWORD PTR DS:[EAX+04h]
		MOV DWORD PTR SS:[ESP+024h],ECX
		MOV DWORD PTR SS:[ESP+014h],EAX
L0040135D:
		MOV EAX,DWORD PTR SS:[ESP+06Ch]
		CMP EAX,EDX
		MOV EAX,01h
		JE SHORT L0040136E
		MOV EAX,DWORD PTR SS:[ESP+064h]
L0040136E:
		MOV ECX,DWORD PTR SS:[ESP+078h]
		MOV EBX,07FFFFFFFh
		CMP ECX,EDX
		JE SHORT L0040137F
		MOV EBX,DWORD PTR SS:[ESP+070h]
L0040137F:
		MOV ECX,EDI
		MOV DWORD PTR SS:[ESP+01Ch],EBX
		SUB ECX,EBP
		CMP EAX,ECX
		JG SHORT L004013E1
		CMP EBX,EDX
		JLE SHORT L004013E1
		CMP EAX,EDX
		JLE SHORT L004013E1
		LEA EAX,DWORD PTR DS:[EAX+ESI-01h]
		LEA ECX,DWORD PTR DS:[ECX+ESI+01h]
		CMP EAX,ECX
		MOV DWORD PTR SS:[ESP+028h],ESI
		MOV DWORD PTR SS:[ESP+030h],EAX
		MOV DWORD PTR SS:[ESP+010h],ECX
		MOV DWORD PTR SS:[ESP+040h],EDX
		JNB SHORT L004013E1
L004013AF:
		MOV EDI,DWORD PTR SS:[ESP+020h]
		MOV ECX,EBP
		MOV ESI,EAX
		XOR EBX,EBX
		REPE CMPSB
		JNZ SHORT L004013CA
		MOV ECX,DWORD PTR SS:[ESP+01Ch]
		INC EDX
		CMP EDX,ECX
		JE SHORT L004013D1
		ADD EAX,EBP
		JMP SHORT L004013CB
L004013CA:
		INC EAX
L004013CB:
		CMP EAX,DWORD PTR SS:[ESP+010h]
		JB SHORT L004013AF
L004013D1:
		TEST EDX,EDX
		MOV DWORD PTR SS:[ESP+040h],EDX
		JNZ SHORT L004013F3
		MOV ESI,DWORD PTR SS:[ESP+02Ch]
		MOV EDI,DWORD PTR SS:[ESP+018h]
L004013E1:
		PUSH EDI
		PUSH ESI
		CALL CloneBinData
		ADD ESP,08h
		POP EDI
		POP ESI
		POP EBP
		POP EBX
		ADD ESP,028h
		RETN
L004013F3:
		MOV ESI,DWORD PTR SS:[ESP+014h]
		MOV EBX,DWORD PTR SS:[ESP+018h]
		SUB ESI,EBP
		IMUL ESI,EDX
		ADD ESI,EBX
		LEA ECX,DWORD PTR DS:[ESI+08h]
		PUSH ECX
		CALL krnl_MMalloc
		MOV EDX,DWORD PTR SS:[ESP+034h]
		LEA EBX,DWORD PTR DS:[EAX+04h]
		MOV DWORD PTR SS:[ESP+020h],EAX
		MOV DWORD PTR DS:[EAX],01h
		MOV EAX,DWORD PTR SS:[ESP+014h]
		MOV DWORD PTR DS:[EBX],ESI
		ADD ESP,04h
		ADD EBX,04h
		CMP EDX,EAX
		JNB L004014D0
L00401430:
		MOV EDI,DWORD PTR SS:[ESP+020h]
		MOV ECX,EBP
		MOV ESI,EDX
		XOR EAX,EAX
		REPE CMPSB
		JNZ SHORT L00401491
		MOV ESI,DWORD PTR SS:[ESP+028h]
		MOV EAX,EDX
		SUB EAX,ESI
		MOV EDI,EBX
		MOV ECX,EAX
		ADD EBX,EAX
		MOV EBP,ECX
		SHR ECX,02h
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EBP
		AND ECX,03h
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
		MOV ESI,DWORD PTR SS:[ESP+024h]
		TEST ESI,ESI
		JE SHORT L0040147A
		MOV EAX,DWORD PTR SS:[ESP+014h]
		MOV EDI,EBX
		MOV ECX,EAX
		MOV EBP,ECX
		SHR ECX,02h
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EBP
		AND ECX,03h
		ADD EBX,EAX
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
L0040147A:
		MOV EBP,DWORD PTR SS:[ESP+034h]
		MOV EAX,DWORD PTR SS:[ESP+040h]
		ADD EDX,EBP
		DEC EAX
		MOV DWORD PTR SS:[ESP+028h],EDX
		MOV DWORD PTR SS:[ESP+040h],EAX
		JE SHORT L004014A4
		JMP SHORT L00401492
L00401491:
		INC EDX
L00401492:
		CMP EDX,DWORD PTR SS:[ESP+010h]
		JB SHORT L00401430
		MOV EAX,DWORD PTR SS:[ESP+01Ch]
		POP EDI
		POP ESI
		POP EBP
		POP EBX
		ADD ESP,028h
		RETN
L004014A4:
		MOV ESI,DWORD PTR SS:[ESP+02Ch]
		MOV EAX,DWORD PTR SS:[ESP+018h]
		MOV ECX,EDX
		SUB ECX,ESI
		CMP ECX,EAX
		JG SHORT L004014D0
		MOV EAX,DWORD PTR SS:[ESP+010h]
		MOV ESI,EDX
		SUB EAX,EDX
		MOV EDI,EBX
		LEA ECX,DWORD PTR DS:[EAX+EBP-01h]
		MOV EDX,ECX
		SHR ECX,02h
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EDX
		AND ECX,03h
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
L004014D0:
		MOV EAX,DWORD PTR SS:[ESP+01Ch]
		POP EDI
		POP ESI
		POP EBP
		POP EBX
		ADD ESP,028h
		RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnSpaceBin)	//取空白字节集
{
	__asm
	{
			PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+0Ch]
			TEST ESI,ESI
			JG SHORT L004012BD
			XOR EAX,EAX
			POP ESI
			RETN
L004012BD:
			LEA EAX,DWORD PTR DS:[ESI+08h]
			PUSH EAX
			CALL krnl_MMalloc
			ADD ESP,04h
			MOV DWORD PTR DS:[EAX+04h],ESI
			MOV DWORD PTR DS:[EAX],01h
			POP ESI
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnBin)	//取重复字节集
{
	__asm
	{
			PUSH ECX
			PUSH EBX
			PUSH EDI
			MOV EDI,DWORD PTR SS:[ESP+014h]
			TEST EDI,EDI
			JLE SHORT L00401333
			MOV EAX,DWORD PTR SS:[ESP+020h]
			TEST EAX,EAX
			JE SHORT L00401333
			MOV EBX,DWORD PTR DS:[EAX+04h]
			TEST EBX,EBX
			JE SHORT L00401333
			PUSH ESI
			MOV ESI,EDI
			IMUL ESI,EBX
			ADD EAX,08h
			MOV DWORD PTR SS:[ESP+0Ch],EAX
			LEA EAX,DWORD PTR DS:[ESI+08h]
			PUSH EAX
			CALL krnl_MMalloc
			LEA EDX,DWORD PTR DS:[EAX+04h]
			MOV DWORD PTR DS:[EAX],01h
			ADD ESP,04h
			MOV DWORD PTR DS:[EDX],ESI
			ADD EDX,04h
			TEST EDI,EDI
			JLE SHORT L0040132E
			PUSH EBP
			MOV DWORD PTR SS:[ESP+01Ch],EDI
L0040130A:
			MOV ESI,DWORD PTR SS:[ESP+010h]
			MOV ECX,EBX
			MOV EBP,ECX
			MOV EDI,EDX
			SHR ECX,02h
			REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
			MOV ECX,EBP
			ADD EDX,EBX
			AND ECX,03h
			REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
			MOV ECX,DWORD PTR SS:[ESP+01Ch]
			DEC ECX
			MOV DWORD PTR SS:[ESP+01Ch],ECX
			JNZ SHORT L0040130A
			POP EBP
L0040132E:
			POP ESI
			POP EDI
			POP EBX
			POP ECX
			RETN
L00401333:
			POP EDI
			XOR EAX,EAX
			POP EBX
			POP ECX
			RETN
	}
}
__declspec(naked) KRNLN_FUNC(fnpbin)	//指针到字节集
{
	__asm
	{
			PUSH ESI
			PUSH EDI
			MOV EDI,DWORD PTR SS:[ESP+01Ch]
			TEST EDI,EDI
			JLE SHORT L004012F9
			MOV ESI,DWORD PTR SS:[ESP+010h]
			LEA EAX,DWORD PTR DS:[EDI+08h]
			PUSH EAX
			CALL krnl_MMalloc
			MOV ECX,EDI
			MOV DWORD PTR DS:[EAX+04h],EDI
			MOV EDX,ECX
			LEA EDI,DWORD PTR DS:[EAX+08h]
			SHR ECX,02h
			MOV DWORD PTR DS:[EAX],01h
			ADD ESP,04h
			REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
			MOV ECX,EDX
			AND ECX,03h
			REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
			POP EDI
			POP ESI
			RETN
L004012F9:
			POP EDI
			XOR EAX,EAX
			POP ESI
			RETN
	}
}
__declspec(naked) WINAPIV write1()
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+04h]
			MOV ECX,01h
			MOV EDX,DWORD PTR DS:[EAX]
			ADD EAX,04h
			TEST EDX,EDX
			JLE SHORT L00401386
			PUSH ESI
L00401373:
			MOV ESI,DWORD PTR DS:[EAX]
			ADD EAX,04h
			IMUL ECX,ESI
			DEC EDX
			JNZ SHORT L00401373
			MOV EDX,DWORD PTR SS:[ESP+0Ch]
			POP ESI
			MOV DWORD PTR DS:[EDX],ECX
			RETN
L00401386:
			MOV EDX,DWORD PTR SS:[ESP+08h]
			MOV DWORD PTR DS:[EDX],ECX
			RETN
	}
}

__declspec(naked) WINAPIV write2()
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+04h]
			CMP EAX,080000201h
			JA SHORT L0040133F
			JE SHORT L00401339
			CMP EAX,080000005h
			JA SHORT L00401325
			CMP EAX,080000004h
			JNB SHORT L0040131F
			CMP EAX,080000002h
			JE SHORT L0040131F
			CMP EAX,080000003h
L00401317:
			JNZ SHORT L0040135D
L00401319:
			MOV EAX,08h
			RETN
L0040131F:
			MOV EAX,04h
			RETN
L00401325:
			CMP EAX,080000006h
			JE SHORT L0040131F
			CMP EAX,080000101h
			JNZ SHORT L0040135D
			MOV EAX,01h
			RETN
L00401339:
			MOV EAX,02h
			RETN
L0040133F:
			CMP EAX,080000501h
			JA SHORT L00401356
			JE SHORT L0040131F
			CMP EAX,080000301h
			JE SHORT L0040131F
			CMP EAX,080000401h
			JMP SHORT L00401317
L00401356:
			CMP EAX,080000601h
			JE SHORT L00401319
L0040135D:
			XOR EAX,EAX
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnWriteMem)  //写到内存
{
	__asm
	{
			PUSH ECX
			MOV EAX,DWORD PTR SS:[ESP+014h]
			PUSH EDI
			MOV ECX,EAX
			AND ECX,020000000h
			CMP ECX,020000000h
			JNZ SHORT L0040122A
			AND EAX,0DFFFFFFFh
			CMP EAX,080000101h
			MOV DWORD PTR SS:[ESP+018h],EAX
			JNZ L004012B2
			MOV EAX,DWORD PTR SS:[ESP+010h]
			LEA EDX,DWORD PTR SS:[ESP+04h]
			PUSH EDX
			PUSH EAX
			CALL write1
			MOV ECX,DWORD PTR SS:[ESP+0Ch]
			ADD ESP,08h
			TEST ECX,ECX
			JE L004012B2
			JMP SHORT L00401282
L0040122A:
			CMP EAX,080000004h
			JNZ SHORT L00401250
			MOV EDX,DWORD PTR SS:[ESP+010h]
			TEST EDX,EDX
			JE SHORT L004012B2
			MOV EDI,EDX
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			JE SHORT L004012B2
			INC ECX
			MOV EAX,EDX
			MOV DWORD PTR SS:[ESP+04h],ECX
			JMP SHORT L00401282
L00401250:
			CMP EAX,080000005h
			JNZ SHORT L0040126B
			MOV EAX,DWORD PTR SS:[ESP+010h]
			TEST EAX,EAX
			JE SHORT L004012B2
			MOV ECX,DWORD PTR DS:[EAX+04h]
			ADD EAX,08h
			MOV DWORD PTR SS:[ESP+04h],ECX
			JMP SHORT L00401282
L0040126B:
			PUSH EAX
			CALL write2
			MOV ECX,EAX
			ADD ESP,04h
			TEST ECX,ECX
			MOV DWORD PTR SS:[ESP+04h],ECX
			JE SHORT L004012B2
			LEA EAX,DWORD PTR SS:[ESP+010h]
L00401282:
			MOV EDX,DWORD PTR SS:[ESP+030h]
			TEST EDX,EDX
			JE SHORT L0040129C
			MOV EDX,DWORD PTR SS:[ESP+028h]
			TEST EDX,EDX
			JLE SHORT L0040129C
			CMP EDX,ECX
			JNB SHORT L0040129C
			MOV ECX,EDX
			MOV DWORD PTR SS:[ESP+04h],ECX
L0040129C:
			MOV EDI,DWORD PTR SS:[ESP+01Ch]
			MOV EDX,ECX
			PUSH ESI
			MOV ESI,EAX
			SHR ECX,02h
			REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
			MOV ECX,EDX
			AND ECX,03h
			REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
			POP ESI
L004012B2:
			POP EDI
			POP ECX
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnZeroAry)   //数组清零
{
	__asm
	{
			PUSH ESI
			PUSH EDI
			MOV EDI,DWORD PTR SS:[ESP+010h]
			TEST EDI,EDI
			JE L004013B8
			MOV EAX,DWORD PTR SS:[ESP+018h]
			CMP EAX,080000004h
			JE SHORT L0040137B
			CMP EAX,080000005h
			JE SHORT L0040137B
			PUSH EAX
			CALL write2
			MOV ESI,EAX
			ADD ESP,04h
			TEST ESI,ESI
			JE SHORT L004013B8
			MOV ECX,DWORD PTR DS:[EDI]
			LEA EAX,DWORD PTR SS:[ESP+010h]
			PUSH EAX
			PUSH ECX
			MOV DWORD PTR SS:[ESP+018h],0h
			CALL write1
			MOV EDI,EAX
			ADD ESP,08h
			TEST EDI,EDI
			JE SHORT L004013B8
			MOV ECX,DWORD PTR SS:[ESP+010h]
			TEST ECX,ECX
			JE SHORT L004013B8
			IMUL ECX,ESI
			MOV EDX,ECX
			XOR EAX,EAX
			SHR ECX,02h
			REP STOS DWORD PTR ES:[EDI]
			MOV ECX,EDX
			AND ECX,03h
			REP STOS BYTE PTR ES:[EDI]
			POP EDI
			POP ESI
			RETN
L0040137B:
			MOV ECX,DWORD PTR DS:[EDI]
			LEA EAX,DWORD PTR SS:[ESP+010h]
			PUSH EAX
			PUSH ECX
			CALL write1
			MOV ECX,DWORD PTR SS:[ESP+018h]
			ADD ESP,08h
			XOR EDI,EDI
			TEST ECX,ECX
			JBE SHORT L004013B8
			MOV ESI,EAX
L00401397:
			MOV EAX,DWORD PTR DS:[ESI]
			TEST EAX,EAX
			JE SHORT L004013A6
			PUSH EAX
			CALL krnl_MFree
			ADD ESP,04h
L004013A6:
			MOV DWORD PTR DS:[ESI],0h
			MOV EAX,DWORD PTR SS:[ESP+010h]
			INC EDI
			ADD ESI,04h
			CMP EDI,EAX
			JB SHORT L00401397
L004013B8:
			POP EDI
			POP ESI
			RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnInStrRev)		//倒找文本
{
	__asm
	{
			PUSH EBX
			MOV EBX,DWORD PTR SS:[ESP+0Ch]
			PUSH EBP
			PUSH ESI
			PUSH EDI
			MOV EDI,EBX
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			REPNE SCAS BYTE PTR ES:[EDI]
			MOV EBP,DWORD PTR SS:[ESP+024h]
			NOT ECX
			DEC ECX
			MOV EDI,EBP
			MOV EDX,ECX
			OR ECX,0FFFFFFFFh
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			TEST EDX,EDX
			MOV EDI,ECX
			JE SHORT L00401263
			TEST EDI,EDI
			JE SHORT L00401263
			CMP EDI,EDX
			JG SHORT L00401263
			MOV EAX,DWORD PTR SS:[ESP+038h]
			SUB EDX,EDI
			TEST EAX,EAX
			JE SHORT L0040123E
			MOV EAX,DWORD PTR SS:[ESP+030h]
			TEST EAX,EAX
			JLE SHORT L0040123E
			SUB EAX,EDI
			JNS SHORT L0040123A
			XOR EAX,EAX
L0040123A:
			CMP EAX,EDX
			JLE SHORT L00401240
L0040123E:
			MOV EAX,EDX
L00401240:
			LEA ESI,DWORD PTR DS:[EAX+EBX]
			MOV EAX,DWORD PTR SS:[ESP+03Ch]
			TEST EAX,EAX
			JE SHORT L0040126B
			CMP ESI,EBX
			JB SHORT L00401263
L0040124F:
			PUSH EDI
			PUSH EBP
			PUSH ESI
			CALL _strnicmp
			ADD ESP,0Ch
			TEST EAX,EAX
			JE SHORT L0040128B
			DEC ESI
			CMP ESI,EBX
			JNB SHORT L0040124F
L00401263:
			POP EDI
			POP ESI
			POP EBP
			OR EAX,0FFFFFFFFh
			POP EBX
			RETN
L0040126B:
			CMP ESI,EBX
			JB SHORT L00401263
L0040126F:
			PUSH EDI
			PUSH EBP
			PUSH ESI
			CALL strncmp
			ADD ESP,0Ch
			TEST EAX,EAX
			JE SHORT L0040128B
			DEC ESI
			CMP ESI,EBX
			JNB SHORT L0040126F
			POP EDI
			POP ESI
			POP EBP
			OR EAX,0FFFFFFFFh
			POP EBX
			RETN
L0040128B:
			MOV EAX,ESI
			POP EDI
			SUB EAX,EBX
			POP ESI
			POP EBP
			INC EAX
			POP EBX
			RETN
	}
} 
__declspec(naked) KRNLN_FUNC(fnInStr)	//寻找文本
{
	__asm
	{
		PUSH EBX
		PUSH EBP
		MOV EBP,DWORD PTR SS:[ESP+010h]
		PUSH ESI
		PUSH EDI
		MOV EDI,EBP
		OR ECX,0FFFFFFFFh
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		MOV EDI,DWORD PTR SS:[ESP+024h]
		NOT ECX
		DEC ECX
		MOV EDX,ECX
		OR ECX,0FFFFFFFFh
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		TEST EDX,EDX
		MOV EDI,ECX
		JE SHORT L00401273
		TEST EDI,EDI
		JE SHORT L00401273
		CMP EDI,EDX
		JG SHORT L00401273
		MOV EAX,DWORD PTR SS:[ESP+038h]
		TEST EAX,EAX
		JE SHORT L00401230
		MOV EAX,DWORD PTR SS:[ESP+030h]
		TEST EAX,EAX
		JG SHORT L00401235
L00401230:
		MOV EAX,01h
L00401235:
		SUB EDX,EDI
		LEA ECX,DWORD PTR DS:[EDX+01h]
		CMP EAX,ECX
		JG SHORT L00401273
		LEA ESI,DWORD PTR DS:[EAX+EBP-01h]
		MOV EAX,DWORD PTR SS:[ESP+03Ch]
		TEST EAX,EAX
		LEA EBX,DWORD PTR DS:[EDX+EBP]
		JE SHORT L0040127B
		CMP ESI,EBX
		JA SHORT L00401273
L00401251:
		MOV EDX,DWORD PTR SS:[ESP+024h]
		PUSH EDI
		PUSH EDX
		PUSH ESI
		CALL _strnicmp
		ADD ESP,0Ch
		TEST EAX,EAX
		JE SHORT L004012A9
		CMP BYTE PTR DS:[ESI],0h
		JGE SHORT L0040126E
		ADD ESI,02h
		JMP SHORT L0040126F
L0040126E:
		INC ESI
L0040126F:
		CMP ESI,EBX
		JBE SHORT L00401251
L00401273:
		POP EDI
		POP ESI
		POP EBP
		OR EAX,0FFFFFFFFh
		POP EBX
		RETN
L0040127B:
		CMP ESI,EBX
		JA SHORT L00401273
L0040127F:
		MOV EAX,DWORD PTR SS:[ESP+024h]
		PUSH EDI
		PUSH EAX
		PUSH ESI
		CALL strncmp
		ADD ESP,0Ch
		TEST EAX,EAX
		JE SHORT L004012A9
		CMP BYTE PTR DS:[ESI],0h
		JGE SHORT L0040129C
		ADD ESI,02h
		JMP SHORT L0040129D
L0040129C:
		INC ESI
L0040129D:
		CMP ESI,EBX
		JBE SHORT L0040127F
		POP EDI
		POP ESI
		POP EBP
		OR EAX,0FFFFFFFFh
		POP EBX
		RETN
L004012A9:
		MOV EAX,ESI
		POP EDI
		SUB EAX,EBP
		POP ESI
		POP EBP
		INC EAX
		POP EBX
		RETN
	}
}

__declspec(naked) KRNLN_FUNC(fnRpSubText)				//子文本替换
{
	__asm
	{
		SUB ESP,024h
		PUSH EBX
		PUSH EBP
		MOV EBP,DWORD PTR SS:[ESP+034h]
		PUSH ESI
		PUSH EDI
		MOV EDI,EBP
		OR ECX,0FFFFFFFFh
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		MOV EDI,DWORD PTR SS:[ESP+048h]
		XOR EDX,EDX
		NOT ECX
		DEC ECX
		MOV ESI,ECX
		OR ECX,0FFFFFFFFh
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		CMP ESI,EDX
		MOV EBX,ECX
		MOV DWORD PTR SS:[ESP+028h],ESI
		MOV DWORD PTR SS:[ESP+020h],EBX
		JNZ SHORT L0040135D
		POP EDI
		POP ESI
		POP EBP
		POP EBX
		ADD ESP,024h
		RETN
L0040135D:
		CMP EBX,EDX
		JE L00401626
		CMP EBX,ESI
		JG L00401626
		MOV EAX,DWORD PTR SS:[ESP+05Ch]
		MOV DWORD PTR SS:[ESP+024h],EDX
		CMP EAX,EDX
		MOV DWORD PTR SS:[ESP+01Ch],EDX
		JE SHORT L00401393
		MOV EDI,DWORD PTR SS:[ESP+054h]
		OR ECX,0FFFFFFFFh
		XOR EAX,EAX
		MOV DWORD PTR SS:[ESP+024h],EDI
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		DEC ECX
		MOV DWORD PTR SS:[ESP+01Ch],ECX
L00401393:
		MOV EAX,DWORD PTR SS:[ESP+068h]
		MOV ECX,01h
		CMP EAX,EDX
		JE SHORT L004013A4
		MOV ECX,DWORD PTR SS:[ESP+060h]
L004013A4:
		CMP DWORD PTR SS:[ESP+074h],EDX
		JNZ SHORT L004013B4
		MOV DWORD PTR SS:[ESP+018h],07FFFFFFFh
		JMP SHORT L004013BC
L004013B4:
		MOV EAX,DWORD PTR SS:[ESP+06Ch]
		MOV DWORD PTR SS:[ESP+018h],EAX
L004013BC:
		MOV EAX,ESI
		SUB EAX,EBX
		LEA ESI,DWORD PTR DS:[EAX+01h]
		CMP ECX,ESI
		JG L00401626
		CMP DWORD PTR SS:[ESP+018h],EDX
		JLE L00401626
		CMP ECX,EDX
		JLE L00401626
		LEA ECX,DWORD PTR DS:[ECX+EBP-01h]
		LEA EAX,DWORD PTR DS:[EAX+EBP+01h]
		MOV DWORD PTR SS:[ESP+02Ch],ECX
		MOV ESI,ECX
		MOV ECX,DWORD PTR SS:[ESP+078h]
		XOR EDI,EDI
		CMP ECX,EDX
		MOV DWORD PTR SS:[ESP+014h],EAX
		MOV DWORD PTR SS:[ESP+010h],EDI
		JE SHORT L00401438
		CMP ESI,EAX
		JNB L00401626
L00401405:
		MOV ECX,DWORD PTR SS:[ESP+048h]
		PUSH EBX
		PUSH ECX
		PUSH ESI
		CALL strncmp
		ADD ESP,0Ch
		TEST EAX,EAX
		JNZ SHORT L00401425
		MOV EAX,DWORD PTR SS:[ESP+018h]
		INC EDI
		CMP EDI,EAX
		JE SHORT L00401471
		ADD ESI,EBX
		JMP SHORT L00401430
L00401425:
		CMP BYTE PTR DS:[ESI],0h
		JGE SHORT L0040142F
		ADD ESI,02h
		JMP SHORT L00401430
L0040142F:
		INC ESI
L00401430:
		CMP ESI,DWORD PTR SS:[ESP+014h]
		JB SHORT L00401405
		JMP SHORT L00401471
L00401438:
		CMP ESI,EAX
		JNB L00401626
L00401440:
		MOV EDX,DWORD PTR SS:[ESP+048h]
		PUSH EBX
		PUSH EDX
		PUSH ESI
		CALL _strnicmp
		ADD ESP,0Ch
		TEST EAX,EAX
		JNZ SHORT L00401460
		MOV EAX,DWORD PTR SS:[ESP+018h]
		INC EDI
		CMP EDI,EAX
		JE SHORT L00401471
		ADD ESI,EBX
		JMP SHORT L0040146B
L00401460:
		CMP BYTE PTR DS:[ESI],0h
		JGE SHORT L0040146A
		ADD ESI,02h
		JMP SHORT L0040146B
L0040146A:
		INC ESI
L0040146B:
		CMP ESI,DWORD PTR SS:[ESP+014h]
		JB SHORT L00401440
L00401471:
		TEST EDI,EDI
		MOV DWORD PTR SS:[ESP+010h],EDI
		JE L00401626
		MOV EAX,DWORD PTR SS:[ESP+01Ch]
		MOV ESI,DWORD PTR SS:[ESP+028h]
		SUB EAX,EBX
		IMUL EAX,EDI
		ADD EAX,ESI
		MOV DWORD PTR SS:[ESP+030h],EAX
		INC EAX
		PUSH EAX
		CALL krnl_MMalloc
		MOV EDI,DWORD PTR SS:[ESP+030h]
		MOV DWORD PTR SS:[ESP+01Ch],EAX
		MOV ESI,EAX
		MOV EAX,DWORD PTR SS:[ESP+07Ch]
		ADD ESP,04h
		TEST EAX,EAX
		MOV EAX,DWORD PTR SS:[ESP+014h]
		JE L00401554
		CMP EDI,EAX
		JNB L00401612
L004014BC:
		MOV EAX,DWORD PTR SS:[ESP+048h]
		PUSH EBX
		PUSH EAX
		PUSH EDI
		CALL strncmp
		ADD ESP,0Ch
		TEST EAX,EAX
		JNZ SHORT L00401516
		MOV EBX,EDI
		SUB EBX,EBP
		PUSH EBX
		PUSH EBP
		PUSH ESI
		CALL strncpy
		MOV EAX,DWORD PTR SS:[ESP+030h]
		ADD ESI,EBX
		ADD ESP,0Ch
		TEST EAX,EAX
		MOV BYTE PTR DS:[ESI],0h
		JE SHORT L004014FF
		MOV EBX,DWORD PTR SS:[ESP+01Ch]
		PUSH EBX
		PUSH EAX
		PUSH ESI
		CALL strncpy
		ADD ESP,0Ch
		ADD ESI,EBX
		MOV BYTE PTR DS:[ESI],0h
L004014FF:
		MOV ECX,DWORD PTR SS:[ESP+020h]
		MOV EAX,DWORD PTR SS:[ESP+010h]
		ADD EDI,ECX
		DEC EAX
		MOV EBP,EDI
		MOV DWORD PTR SS:[ESP+010h],EAX
		JE SHORT L0040153B
		MOV EBX,ECX
		JMP SHORT L00401521
L00401516:
		CMP BYTE PTR DS:[EDI],0h
		JGE SHORT L00401520
		ADD EDI,02h
		JMP SHORT L00401521
L00401520:
		INC EDI
L00401521:
		CMP EDI,DWORD PTR SS:[ESP+014h]
		JB SHORT L004014BC
		MOV ECX,DWORD PTR SS:[ESP+030h]
		MOV EAX,DWORD PTR SS:[ESP+018h]
		POP EDI
		POP ESI
		POP EBP
		MOV BYTE PTR DS:[EAX+ECX],0h
		POP EBX
		ADD ESP,024h
		RETN
L0040153B:
		MOV EDX,DWORD PTR SS:[ESP+03Ch]
		MOV EAX,DWORD PTR SS:[ESP+028h]
		MOV ECX,EDI
		SUB ECX,EDX
		CMP ECX,EAX
		JG L00401612
		JMP L004015EB
L00401554:
		CMP EDI,EAX
		JNB L00401612
L0040155C:
		MOV EAX,DWORD PTR SS:[ESP+048h]
		PUSH EBX
		PUSH EAX
		PUSH EDI
		CALL _strnicmp
		ADD ESP,0Ch
		TEST EAX,EAX
		JNZ SHORT L004015B6
		MOV EBX,EDI
		SUB EBX,EBP
		PUSH EBX
		PUSH EBP
		PUSH ESI
		CALL strncpy
		MOV EAX,DWORD PTR SS:[ESP+030h]
		ADD ESI,EBX
		ADD ESP,0Ch
		TEST EAX,EAX
		MOV BYTE PTR DS:[ESI],0h
		JE SHORT L0040159F
		MOV EBX,DWORD PTR SS:[ESP+01Ch]
		PUSH EBX
		PUSH EAX
		PUSH ESI
		CALL strncpy
		ADD ESP,0Ch
		ADD ESI,EBX
		MOV BYTE PTR DS:[ESI],0h
L0040159F:
		MOV ECX,DWORD PTR SS:[ESP+020h]
		MOV EAX,DWORD PTR SS:[ESP+010h]
		ADD EDI,ECX
		DEC EAX
		MOV EBP,EDI
		MOV DWORD PTR SS:[ESP+010h],EAX
		JE SHORT L004015DB
		MOV EBX,ECX
		JMP SHORT L004015C1
L004015B6:
		CMP BYTE PTR DS:[EDI],0h
		JGE SHORT L004015C0
		ADD EDI,02h
		JMP SHORT L004015C1
L004015C0:
		INC EDI
L004015C1:
		CMP EDI,DWORD PTR SS:[ESP+014h]
		JB SHORT L0040155C
		MOV ECX,DWORD PTR SS:[ESP+030h]
		MOV EAX,DWORD PTR SS:[ESP+018h]
		POP EDI
		POP ESI
		POP EBP
		MOV BYTE PTR DS:[EAX+ECX],0h
		POP EBX
		ADD ESP,024h
		RETN
L004015DB:
		MOV EDX,DWORD PTR SS:[ESP+03Ch]
		MOV EAX,DWORD PTR SS:[ESP+028h]
		MOV ECX,EDI
		SUB ECX,EDX
		CMP ECX,EAX
		JG SHORT L00401612
L004015EB:
		OR ECX,0FFFFFFFFh
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		SUB EDI,ECX
		MOV ESI,EDI
		MOV EDI,DWORD PTR SS:[ESP+018h]
		MOV EDX,ECX
		OR ECX,0FFFFFFFFh
		REPNE SCAS BYTE PTR ES:[EDI]
		MOV ECX,EDX
		DEC EDI
		SHR ECX,02h
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EDX
		AND ECX,03h
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
L00401612:
		MOV ECX,DWORD PTR SS:[ESP+030h]
		MOV EAX,DWORD PTR SS:[ESP+018h]
		POP EDI
		POP ESI
		POP EBP
		MOV BYTE PTR DS:[EAX+ECX],0h
		POP EBX
		ADD ESP,024h
		RETN
L00401626:
		PUSH EBP
		CALL CloneTextData
		ADD ESP,04h
		POP EDI
		POP ESI
		POP EBP
		POP EBX
		ADD ESP,024h
		RETN
	}
}

//////////////////////////////////////////////////////// 到字节集
__declspec(naked) WINAPIV ToBin1()
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+04h]
			TEST EAX,EAX
			JNZ SHORT L00401489
			RETN
L00401489:
			AND EAX,0C0000000h
			CMP EAX,080000000h
			JNZ SHORT L0040149B
			MOV EAX,01h
			RETN
L0040149B:
			XOR ECX,ECX
			CMP EAX,040000000h
			SETNE CL
			ADD ECX,02h
			MOV EAX,ECX
			RETN
	}
}

int __cdecl ToBin2(int pArg, int pRet)
{
	unsigned int dwType; 
	int dwRet; 
	
	dwType = *(DWORD *)(pArg + 8);
	if ( dwType <= 0x80000101 )
	{
		if ( dwType == -2147483391 )
		{
			*(DWORD *)pRet = 1;
			return pArg;
		}
		switch ( dwType )
		{
		case 0x80000002u:
		case 0x80000006u:
			*(DWORD *)pRet = 4;
			return pArg;
		case 0x80000004u:
			*(DWORD *)pRet = strlen(*(const char **)pArg);
			return *(DWORD *)pArg;
		case 0x80000005u:
			dwRet = *(DWORD *)pArg + 8;
			*(DWORD *)pRet = *(DWORD *)(*(DWORD *)pArg + 4);
			return dwRet;
		case 0x80000003u:
			goto LABEL_14;
		default:
			return 0;
		}
		return 0;
	}
	if ( dwType <= 0x80000401 )
	{
		if ( dwType == -2147482623 )
		{
LABEL_14:
		dwRet = pArg;
		*(DWORD *)pRet = 8;
		return dwRet;
		}
		if ( dwType == -2147483135 )
		{
			*(DWORD *)pRet = 2;
			return pArg;
		}
		if ( dwType != -2147482879 )
			return 0;
LABEL_12:
		dwRet = pArg;
		*(DWORD *)pRet = 4;
		return dwRet;
	}
	if ( dwType == -2147482367 )
		goto LABEL_12;
	if ( dwType != -2147482111 )
		return 0;
	*(DWORD *)pRet = 8;
	return pArg;
}
//到字节集
__declspec(naked) KRNLN_FUNC(fnCnvToBin) 
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+010h]
			SUB ESP,08h
			PUSH ESI
			MOV ESI,EAX
			SHR EAX,01Dh
			AND ESI,0DFFFFFFFh
			AND EAX,01h
			CMP ESI,080000101h
			JNZ SHORT L004013DE
			CMP EAX,01h
			JNZ SHORT L004013E3
			MOV ECX,DWORD PTR SS:[ESP+014h]
			LEA EAX,DWORD PTR SS:[ESP+08h]
			PUSH EAX
			PUSH ECX
			CALL write1
			ADD ESP,08h
			MOV EDX,EAX
			MOV EAX,DWORD PTR SS:[ESP+08h]
			PUSH EAX
			PUSH EDX
			MOV DWORD PTR SS:[ESP+0Ch],EAX
			CALL CloneBinData
			ADD ESP,08h
			POP ESI
			ADD ESP,08h
			RETN
L004013DE:
			CMP EAX,01h
			JE SHORT L00401447
L004013E3:
			PUSH ESI
			CALL ToBin1
			ADD ESP,04h
			CMP EAX,01h
			JNZ SHORT L00401447
			CMP ESI,080000004h
			JNZ SHORT L00401420
			MOV EDX,DWORD PTR SS:[ESP+014h]
			PUSH EDI
			MOV EDI,EDX
			OR ECX,0FFFFFFFFh
			XOR EAX,EAX
			REPNE SCAS BYTE PTR ES:[EDI]
			NOT ECX
			DEC ECX
			POP EDI
			MOV EAX,ECX
			MOV DWORD PTR SS:[ESP+04h],ECX
			PUSH EAX
			PUSH EDX
			CALL CloneBinData
			ADD ESP,08h
			POP ESI
			ADD ESP,08h
			RETN
L00401420:
			LEA ECX,DWORD PTR SS:[ESP+04h]
			LEA EDX,DWORD PTR SS:[ESP+014h]
			PUSH ECX
			PUSH EDX
			CALL ToBin2
			ADD ESP,08h
			MOV EDX,EAX
			MOV EAX,DWORD PTR SS:[ESP+04h]
			PUSH EAX
			PUSH EDX
			CALL CloneBinData
			ADD ESP,08h
			POP ESI
			ADD ESP,08h
			RETN
L00401447:
			XOR EAX,EAX
			POP ESI
			ADD ESP,08h
			RETN
	}
}
//取字节集数据
//////////////////////////////////////////////////////////////////////////
__declspec(naked) WINAPIV GetBin1()
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+04h]
			CMP EAX,080000201h
			JA SHORT L004016AF
			JE SHORT L004016A9
			CMP EAX,080000005h
			JA SHORT L00401695
			CMP EAX,080000004h
			JNB SHORT L0040168F
			CMP EAX,080000002h
			JE SHORT L0040168F
			CMP EAX,080000003h
L00401687:
			JNZ SHORT L004016CD
L00401689:
			MOV EAX,08h
			RETN
L0040168F:
			MOV EAX,04h
			RETN
L00401695:
			CMP EAX,080000006h
			JE SHORT L0040168F
			CMP EAX,080000101h
			JNZ SHORT L004016CD
			MOV EAX,01h
			RETN
L004016A9:
			MOV EAX,02h
			RETN
L004016AF:
			CMP EAX,080000501h
			JA SHORT L004016C6
			JE SHORT L0040168F
			CMP EAX,080000301h
			JE SHORT L0040168F
			CMP EAX,080000401h
			JMP SHORT L00401687
L004016C6:
			CMP EAX,080000601h
			JE SHORT L00401689
L004016CD:
			XOR EAX,EAX
			RETN
	}
}
__declspec(naked) WINAPIV GetBin3(int a1, int a2,int a3)
{
	__asm
	{
			PUSH EBP
			MOV EBP,ESP
			PUSH ECX
			MOV EAX,DWORD PTR SS:[EBP+0Ch]
			PUSH EDI
			TEST EAX,EAX
			MOV DWORD PTR SS:[EBP-04h],-01h
			JLE SHORT L00401652
			MOV EAX,DWORD PTR SS:[EBP+08h]
			MOV CL,BYTE PTR DS:[EAX]
			MOV AL,BYTE PTR SS:[EBP+010h]
			CMP CL,AL
			JNZ SHORT L00401636
			XOR EAX,EAX
			POP EDI
			MOV ESP,EBP
			POP EBP
			RETN
L00401636:
			PUSH EDI
			PUSH EAX
			PUSH ECX
			MOV AL,BYTE PTR SS:[EBP+010h]
			MOV ECX,DWORD PTR SS:[EBP+0Ch]
			PUSH ECX
			MOV EDI,DWORD PTR SS:[EBP+08h]
			CLD
			REPNE SCAS BYTE PTR ES:[EDI]
			POP EAX
			JNZ SHORT L0040164F
			INC ECX
			SUB EAX,ECX
			MOV DWORD PTR SS:[EBP-04h],EAX
L0040164F:
			POP ECX
			POP EAX
			POP EDI
L00401652:
			MOV EAX,DWORD PTR SS:[EBP-04h]
			POP EDI
			MOV ESP,EBP
			POP EBP
			RETN
	}
}
DWORD __cdecl GetBin2(int a1, int a2, int a3)
{
	DWORD pRet;
	int Len;
	
	pRet = *(DWORD *)(a1 + 8);
	if ( (unsigned int)pRet > 0x80000101 )
	{
		if ( (unsigned int)pRet > 0x80000401 )
		{
			if ( pRet == -2147482367 )
			{
				 pRet = a2;
				*(DWORD *)a1 = *(DWORD *)a2;
			}
			else
			{
				if ( pRet == -2147482111 )
				{
					pRet = a2;
					*(DWORD *)a1 = *(DWORD *)a2;
					*(DWORD *)(a1 + 4) = *(DWORD *)(a2 + 4);
				}
			}
		}
		else
		{
			switch ( pRet )
			{
			case -2147482623:
LABEL_5:
				*(DWORD *)a1 = *(DWORD *)a2;
				pRet = *(DWORD *)(a2 + 4);
				*(DWORD *)(a1 + 4) = pRet;
				break;
			case -2147483135:
				pRet = a2;
				*(WORD *)a1 = *(WORD *)a2;
				break;
			case -2147482879:
LABEL_15:
				*(DWORD *)a1 = *(DWORD *)a2;
				break;
			}
		}
	}
	else
	{
		if ( pRet == -2147483391 )
		{
			pRet = a2;
			*(BYTE *)a1 = *(BYTE *)a2;
		}
		else
		{
			switch ( pRet )
			{
			case -2147483646:
				pRet = *(DWORD *)a2 != 0;
				*(DWORD *)a1 = pRet;
				return pRet;
			case -2147483645:
				goto LABEL_5;
			case -2147483644:
				Len = GetBin3(a2, a3, 0);
				if ( Len == -1 )
					Len = a3;
				pRet =(DWORD) _CloneTextData((char*)a2, Len);
				*(DWORD *)a1 = pRet;
				return pRet;
			case -2147483643:
				pRet =(DWORD) CloneBinData((LPBYTE)a2, a3);
				*(DWORD *)a1 = pRet;
				return pRet;
			case -2147483642:
				goto LABEL_15;
			default:
				return pRet;
			}
		}
	}
	return pRet;
}

//〈通用型〉 取字节集数据 （字节集 欲取出其中数据的字节集，整数型 欲取出数据的类型，［整数型 起始索引位置］）
__declspec(naked) KRNLN_FUNC(fnGetBinData)
{
	__asm
	{
		PUSH EBP
		MOV EBP,ESP
		SUB ESP,01Ch
		PUSH ESI
		MOV ESI,DWORD PTR SS:[EBP+018h]
		CMP ESI,01h
		PUSH EDI
		JL SHORT L004013F5
		CMP ESI,0Ah
		JLE SHORT L004013FF
L004013F5:
		XOR EAX,EAX
		POP EDI
		POP ESI
		ADD ESP,01Ch
		MOV ESP,EBP
		POP EBP
		RETN
L004013FF:
		XOR EAX,EAX
		CMP ESI,01h
		MOV DWORD PTR SS:[EBP-01Ch],EAX
		MOV DWORD PTR SS:[EBP-018h],EAX
		MOV DWORD PTR SS:[EBP-014h],EAX
		JNZ SHORT L00401416
		MOV EAX,080000101h
		JMP SHORT L0040147A
L00401416:
		CMP ESI,02h
		JNZ SHORT L00401422
		MOV EAX,080000201h
		JMP SHORT L0040147A
L00401422:
		CMP ESI,03h
		JNZ SHORT L0040142E
		MOV EAX,080000301h
		JMP SHORT L0040147A
L0040142E:
		CMP ESI,04h
		JNZ SHORT L0040143A
		MOV EAX,080000401h
		JMP SHORT L0040147A
L0040143A:
		CMP ESI,05h
		JNZ SHORT L00401446
		MOV EAX,080000501h
		JMP SHORT L0040147A
L00401446:
		CMP ESI,06h
		JNZ SHORT L00401452
		MOV EAX,080000601h
		JMP SHORT L0040147A
L00401452:
		CMP ESI,07h
		JNZ SHORT L0040145E
		MOV EAX,080000002h
		JMP SHORT L0040147A
L0040145E:
		CMP ESI,08h
		JNZ SHORT L0040146A
		MOV EAX,080000003h
		JMP SHORT L0040147A
L0040146A:
		MOV EAX,ESI
		SUB EAX,09h
		NEG EAX
		SBB EAX,EAX
		AND AL,0FEh
		ADD EAX,080000006h
L0040147A:
		MOV ESI,DWORD PTR SS:[EBP+0Ch]
		XOR EDI,EDI
		CMP ESI,EDI
		MOV DWORD PTR SS:[EBP-014h],EAX
		JNZ SHORT L00401495
		MOV EAX,DWORD PTR SS:[EBP-01Ch]
		MOV EDX,DWORD PTR SS:[EBP-018h]
		MOV ECX,DWORD PTR SS:[EBP-014h]
		POP EDI
		POP ESI
		MOV ESP,EBP
		POP EBP
		RETN
L00401495:
		MOV ECX,DWORD PTR SS:[EBP+02Ch]
		ADD ESI,08h
		PUSH EBX
		CMP ECX,EDI
		MOV EBX,DWORD PTR DS:[ESI-04h]
		MOV DWORD PTR SS:[EBP-04h],EDI
		JE SHORT L004014BC
		CMP ECX,0A0000301h
		MOV ECX,DWORD PTR SS:[EBP+024h]
		JNZ SHORT L004014B9
		MOV EDI,DWORD PTR DS:[ECX]
		MOV DWORD PTR SS:[EBP-04h],ECX
		DEC EDI
		JMP SHORT L004014BC
L004014B9:
		LEA EDI,DWORD PTR DS:[ECX-01h]
L004014BC:
		PUSH EAX
		CALL GetBin1
		XOR ECX,ECX
		ADD ESP,04h
		ADD ESI,EDI
		SUB EBX,EDI
		CMP EDI,ECX
		MOV DWORD PTR SS:[EBP-08h],EAX
		JL SHORT L00401540
		CMP EBX,ECX
		JLE SHORT L00401540
		CMP DWORD PTR SS:[EBP+018h],0Ah
		JE SHORT L004014FE
		CMP EBX,EAX
		JGE SHORT L004014FE
		MOV DWORD PTR SS:[EBP-0Ch],ECX
		MOV DWORD PTR SS:[EBP-010h],ECX
		MOV ECX,EBX
		LEA EDI,DWORD PTR SS:[EBP-010h]
		MOV EDX,ECX
		MOV EBX,EAX
		SHR ECX,02h
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		MOV ECX,EDX
		AND ECX,03h
		REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
		LEA ESI,DWORD PTR SS:[EBP-010h]
L004014FE:
		PUSH EBX
		LEA EAX,DWORD PTR SS:[EBP-01Ch]
		PUSH ESI
		PUSH EAX
		CALL GetBin2
		MOV EDX,DWORD PTR SS:[EBP-04h]
		ADD ESP,0Ch
		TEST EDX,EDX
		JE SHORT L0040154D
		CMP DWORD PTR SS:[EBP+018h],0Ah
		JNZ SHORT L00401527
		MOV EDI,DWORD PTR SS:[EBP-01Ch]
		OR ECX,0FFFFFFFFh
		XOR EAX,EAX
		REPNE SCAS BYTE PTR ES:[EDI]
		NOT ECX
		JMP SHORT L0040152A
L00401527:
		MOV ECX,DWORD PTR SS:[EBP-08h]
L0040152A:
		MOV EAX,DWORD PTR DS:[EDX]
		ADD EAX,ECX
		MOV ECX,DWORD PTR SS:[EBP+0Ch]
		MOV DWORD PTR DS:[EDX],EAX
		CMP EAX,DWORD PTR DS:[ECX+04h]
		JLE SHORT L0040154D
		MOV DWORD PTR DS:[EDX],-01h
		JMP SHORT L0040154D
L00401540:
		MOV EAX,DWORD PTR SS:[EBP-04h]
		CMP EAX,ECX
		JE SHORT L0040154D
		MOV DWORD PTR DS:[EAX],-01h
L0040154D:
		MOV EAX,DWORD PTR SS:[EBP-01Ch]
		MOV EDX,DWORD PTR SS:[EBP-018h]
		MOV ECX,DWORD PTR SS:[EBP-014h]
		POP EBX
		POP EDI
		POP ESI
		MOV ESP,EBP
		POP EBP
		RETN
	}
}

//〈字节型〉 到字节 （通用型 待转换的文本或数值）
__declspec(naked) KRNLN_FUNC(fnToByte)
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+010h]
			PUSH EBX
			XOR BL,BL
			CMP EAX,080000301h
			PUSH ESI
			JA SHORT L00401311
			JE SHORT L004012C6
			CMP EAX,080000004h
			JE SHORT L004012CF
			CMP EAX,080000101h
			JE SHORT L004012C6
			CMP EAX,080000201h
			JNZ SHORT L004012CA
L004012C6:
			MOV BL,BYTE PTR SS:[ESP+010h]
L004012CA:
			MOV AL,BL
			POP ESI
			POP EBX
			RETN
L004012CF:
			MOV EDX,DWORD PTR SS:[ESP+010h]
			SUB ESP,0Ch
			MOV ECX,ESP
			PUSH 1h
			MOV DWORD PTR DS:[ECX],EDX
			MOV EDX,DWORD PTR SS:[ESP+024h]
			MOV DWORD PTR DS:[ECX+04h],EDX
			MOV DWORD PTR DS:[ECX+08h],EAX
			CALL krnln_fnBJCase
			MOV ESI,EAX
			ADD ESP,010h
			TEST ESI,ESI
			JE SHORT L0040130A
			PUSH ESI
			CALL atoi
			PUSH ESI
			MOV BL,AL
			CALL krnl_MFree
			ADD ESP,08h
			MOV AL,BL
			POP ESI
			POP EBX
			RETN
L0040130A:
			XOR BL,BL
			MOV AL,BL
			POP ESI
			POP EBX
			RETN
L00401311:
			CMP EAX,080000401h
			JE SHORT L004012C6
			CMP EAX,080000501h
			JE SHORT L00401334
			CMP EAX,080000601h
			JNZ SHORT L004012CA
			FLD QWORD PTR SS:[ESP+010h]
			CALL _ftol
			MOV BL,AL
			POP ESI
			POP EBX
			RETN
L00401334:
			FLD DWORD PTR SS:[ESP+010h]
			CALL _ftol
			MOV BL,AL
			POP ESI
			POP EBX
			RETN
	}
}
//〈短整数型〉 到短整数 （通用型 待转换的文本或数值） 
__declspec(naked) KRNLN_FUNC(fnToShort)
{
	__asm
	{
			MOV EAX,DWORD PTR SS:[ESP+010h]
			PUSH ESI
			XOR ESI,ESI
			CMP EAX,080000301h
			PUSH EDI
			JA SHORT L00401320
			JE SHORT L004012C6
			CMP EAX,080000004h
			JE SHORT L004012DC
			CMP EAX,080000101h
			JE SHORT L004012D0
			CMP EAX,080000201h
			JNZ SHORT L004012CA
L004012C6:
			MOV ESI,DWORD PTR SS:[ESP+010h]
L004012CA:
			MOV AX,SI
			POP EDI
			POP ESI
			RETN
L004012D0:
			MOVZX SI,BYTE PTR SS:[ESP+010h]
			MOV AX,SI
			POP EDI
			POP ESI
			RETN
L004012DC:
			MOV EDX,DWORD PTR SS:[ESP+010h]
			SUB ESP,0Ch
			MOV ECX,ESP
			PUSH 1h
			MOV DWORD PTR DS:[ECX],EDX
			MOV EDX,DWORD PTR SS:[ESP+024h]
			MOV DWORD PTR DS:[ECX+04h],EDX
			MOV DWORD PTR DS:[ECX+08h],EAX
			CALL krnln_fnBJCase
			MOV EDI,EAX
			ADD ESP,010h
			TEST EDI,EDI
			JE SHORT L00401318
			PUSH EDI
			CALL atoi
			PUSH EDI
			MOV ESI,EAX
			CALL krnl_MFree
			ADD ESP,08h
			MOV AX,SI
			POP EDI
			POP ESI
			RETN
L00401318:
			XOR ESI,ESI
			MOV AX,SI
			POP EDI
			POP ESI
			RETN
L00401320:
			CMP EAX,080000401h
			JE SHORT L004012C6
			CMP EAX,080000501h
			JE SHORT L00401346
			CMP EAX,080000601h
			JNZ SHORT L004012CA
			FLD QWORD PTR SS:[ESP+010h]
			CALL _ftol
			MOV ESI,EAX
			MOV AX,SI
			POP EDI
			POP ESI
			RETN
L00401346:
			FLD DWORD PTR SS:[ESP+010h]
			CALL _ftol
			MOV ESI,EAX
			POP EDI
			MOV AX,SI
			POP ESI
			RETN
	}
}
//〈长整数型〉 到长整数 （通用型 待转换的文本或数值）
__declspec(naked) KRNLN_FUNC(fnToLong)
{
	__asm
	{
			PUSH EBP
			MOV EBP,ESP
			MOV EAX,DWORD PTR SS:[EBP+014h]
			PUSH ESI
			CMP EAX,080000201h
			JA SHORT L0040130A
			JE SHORT L00401303
			CMP EAX,080000004h
			JE SHORT L004012C9
			CMP EAX,080000101h
			JNZ SHORT L00401330
			MOV EAX,DWORD PTR SS:[EBP+0Ch]
			AND EAX,0FFh
			CDQ
			JMP SHORT L0040132A
L004012C9:
			MOV EDX,DWORD PTR SS:[EBP+0Ch]
			SUB ESP,0Ch
			MOV ECX,ESP
			PUSH 1h
			MOV DWORD PTR DS:[ECX],EDX
			MOV EDX,DWORD PTR SS:[EBP+010h]
			MOV DWORD PTR DS:[ECX+04h],EDX
			MOV DWORD PTR DS:[ECX+08h],EAX
			CALL krnln_fnBJCase
			MOV ESI,EAX
			ADD ESP,010h
			TEST ESI,ESI
			JE SHORT L00401330
			PUSH 00Ah
			PUSH 00h
			PUSH ESI
			CALL _strtoi64
			PUSH ESI
			MOV DWORD PTR SS:[EBP+0Ch],EAX
			MOV DWORD PTR SS:[EBP+010h],EDX
			CALL krnl_MFree
			ADD ESP,10h
			JMP SHORT L00401330
L00401303:
			MOVSX EAX,WORD PTR SS:[EBP+0Ch]
			CDQ
			JMP SHORT L0040132A
L0040130A:
			CMP EAX,080000501h
			JE SHORT L00401322
			CMP EAX,080000601h
			JNZ SHORT L00401330
			FLD QWORD PTR SS:[EBP+0Ch]
			CALL _ftol
			JMP SHORT L0040132A
L00401322:
			FLD DWORD PTR SS:[EBP+0Ch]
			CALL _ftol
L0040132A:
			MOV DWORD PTR SS:[EBP+0Ch],EAX
			MOV DWORD PTR SS:[EBP+010h],EDX
L00401330:
			MOV EAX,DWORD PTR SS:[EBP+0Ch]
			MOV EDX,DWORD PTR SS:[EBP+010h]
			POP ESI
			POP EBP
			RETN
	}
}
