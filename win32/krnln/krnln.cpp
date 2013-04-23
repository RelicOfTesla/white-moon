#include "stdafx.h"

EXTERN_C int EStartup();
EXTERN_C INT WINAPI krnl_ProcessNotifyLib (INT nMsg, DWORD dwParam1, DWORD dwParam2);
INT WINAPI krnl_NotifySys (INT nMsg, DWORD dwParam1, DWORD dwParam2);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	krnl_ProcessNotifyLib(NL_SYS_NOTIFY_FUNCTION, (DWORD)krnl_NotifySys, 0);
	return EStartup();
}

void ReportAllocMemError(SIZE_T dwBytes)
{

}


EXTERN_C INT krnl_MOtherHelp()
{
	return 0;
}

EXTERN_C INT krnl_MMessageLoop()
{
	return 0;
}

EXTERN_C INT WINAPI krnl_ProcessNotifyLib (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
	return ProcessNotifyLib(nMsg, dwParam1, dwParam2);
}

EXTERN_C void krnl_MReportError(UINT uErrCode)
{
};

EXTERN_C INT krnl_MCallDllCmd()
{
	return 0;
};

EXTERN_C __declspec(naked) void krnl_MCallLibCmd()
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

EXTERN_C __declspec(naked) void krnl_MCallKrnlLibCmd()
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
/*
EXTERN_C MDATA_INF krnl_MCallKrnlLibCmd(INT nArgCount, MDATA_INF ArgInf)
{
	MDATA_INF RetData = { 0 };
	__asm
	{
		lea		eax, ArgInf
		push		eax
		push		nArgCount
		lea		eax, RetData
		push		eax
		call		ebx
		add		esp, 0xC
	}
	return RetData;
}
*/

EXTERN_C INT krnl_MReadProperty()
{
	return 0;
};

EXTERN_C INT krnl_MWriteProperty()
{
	return 0;
};

EXTERN_C PVOID krnl_MMalloc(DWORD dwBytes)
{
	PVOID lpMem = malloc(dwBytes);
	if ( !lpMem )
	{
		ReportAllocMemError(dwBytes);
	}
	return lpMem;
};

EXTERN_C PVOID krnl_MRealloc(PVOID lpMem, DWORD dwBytes)
{
	lpMem = realloc(lpMem, dwBytes);
	if ( !lpMem )
	{
		ReportAllocMemError(dwBytes);
	}
	return lpMem;
};

EXTERN_C void krnl_MFree(PVOID lpMem)
{
	if ( lpMem )
	{
		free(lpMem);
	}
};

EXTERN_C void krnl_MExitProcess(UINT uExitCode)
{
	exit(uExitCode);
};

EXTERN_C void krnl_MLoadBeginWin()
{

};
/*
易语言函数传递的每1个参数为pArgInf [0] 第2个为pArgInf [1]...
返回值保存在pRetData中
krnl_MCallKrnlLibCmd 的作用是对参数进行整合 并将返回值保存到对应的寄存器
*/
//	〈双精度小数型〉 整除 （双精度小数型 被除数，双精度小数型 除数，... ）
EXTERN_C void krnln_fnIDiv(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	DOUBLE db = pArgInf [0].m_double;
	for (INT i = 1; i < nArgCount; i++)
	{
		pArgInf++;
		if (pArgInf->m_double == 0)
		{
			GReportError ("不能除数为零");
		}
		db =  db / pArgInf->m_double;
	}
	// 设置返回的双精度小数数据。
	if ( db > 0 )
	{
		pRetData->m_double = floor(db);
	}
	else
	{
		pRetData->m_double = ceil(db);
	}
}

//	〈双精度小数型〉 求余数 （双精度小数型 被除数，双精度小数型 除数，... ）
EXTERN_C void krnln_fnMod (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	DOUBLE db = pArgInf [0].m_double;
	for (INT i = 1; i < nArgCount; i++)
	{
		pArgInf++;
		if (pArgInf->m_double == 0)
		{
			GReportError ("不能求余数于零");
		}
		db = fmod (db, pArgInf->m_double);
	}

	// 设置返回的双精度小数数据。
	pRetData->m_double = db;
}

//	〈整数型〉 取符号 （双精度小数型 欲取其符号的数值）
EXTERN_C void krnln_fnSgn (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	INT nRet = 0;
	if( pArgInf [0].m_double > 0 )
	{
		nRet = 1;
	}
	else if ( pArgInf[0].m_double < 0 )
	{
		nRet = -1;
	}
	pRetData->m_int = nRet;
}

//	〈双精度小数型〉 取绝对值 （双精度小数型 欲取其绝对值的数值）
EXTERN_C void krnln_fnAbs(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_double = fabs(pArgInf [0].m_double);
}

//	〈整数型〉 取整 （双精度小数型 欲取整的小数）
EXTERN_C void krnln_fnInt(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = (INT)floor(pArgInf [0].m_double);
}

//	〈整数型〉 绝对取整 （双精度小数型 欲取整的小数）
EXTERN_C void krnln_fnFix(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = (INT)ceil(pArgInf [0].m_double);
}

//	〈双精度小数型〉 四舍五入 （双精度小数型 欲被四舍五入的数值，［整数型 被舍入的位置］）
EXTERN_C void krnln_fnRound(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
}

//	〈双精度小数型〉 求次方 （双精度小数型 欲求次方数值，双精度小数型 次方数）
EXTERN_C void krnln_fnPow(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_double = pow(pArgInf[0].m_double, pArgInf[1].m_double);
}

//	〈双精度小数型〉 求平方根 （双精度小数型 欲求其平方根的数值）
EXTERN_C void krnln_fnSqr(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_double = sqrt(pArgInf[0].m_double);
}

//	〈双精度小数型〉 求正弦 （双精度小数型 欲进行计算的角）
EXTERN_C void krnln_fnSin(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_double = sin(pArgInf[0].m_double);
}
//	〈双精度小数型〉 求余弦 （双精度小数型 欲进行计算的角）
EXTERN_C void krnln_fnCos(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_double = cos(pArgInf[0].m_double);
}
//	〈双精度小数型〉 求正切 （双精度小数型 欲进行计算的角
EXTERN_C void krnln_fnTan(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_double = tan(pArgInf[0].m_double);
}
//	〈双精度小数型〉 求反正切 （双精度小数型 欲求其反正切值的数值）
EXTERN_C void krnln_fnAtn(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_double = atan(pArgInf[0].m_double);
}
//	〈双精度小数型〉 求自然对数 （双精度小数型 欲求其自然对数的数值）
EXTERN_C void krnln_fnLog(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_double = log(pArgInf[0].m_double);
}
//	〈双精度小数型〉 求反对数 （双精度小数型 欲求其反对数的数值）
EXTERN_C void krnln_fnExp(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_double = exp(pArgInf[0].m_double);
}
//	〈逻辑型〉 是否运算正确 （双精度小数型 欲校验的计算结果
EXTERN_C void krnln_IsCalcOK(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = BL_TRUE;
}
//	〈无返回值〉 置随机数种子 （［整数型 欲置入的种子数值］）
EXTERN_C void krnln_fnRandomize(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if( pArgInf[0].m_int )		 //		种子非0时
	{
		srand(pArgInf[0].m_int);
	}
	else
	{
		srand(time(NULL));
	}
}
//	〈整数型〉 取随机数 （［整数型 欲取随机数的最小值］，［整数型 欲取随机数的最大值］）
EXTERN_C void krnln_fnRnd(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[1].m_int )	//	有最大值限制时
	{
		pRetData->m_int = rand() % (pArgInf[1].m_int - pArgInf[0].m_int) + pArgInf[0].m_int;
	}
	else
	{
		pRetData->m_int = rand() + pArgInf[0].m_int;
	}
}

//	〈逻辑型〉 近似等于 （文本型 被比较文本，文本型 比较文本）
EXTERN_C void krnln_fnTextLikeEqu(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	char *pText1 = pArgInf[0].m_pText;
	char *pText2 = pArgInf[1].m_pText;
	if ( pText1 == NULL )
	{
		if ( pText2 == NULL )
		{
			pRetData->m_bool = BL_TRUE;
		}
		{
			pRetData->m_bool = BL_FALSE;
		}
	}
	else
	{
		if ( pText2 == NULL )
		{
			pRetData->m_bool = BL_TRUE;
		}
		else
		{
			size_t nLen1 = strlen(pText1);
			size_t nLen2 = strlen(pText2);
			if ( nLen1 >= nLen2 )
			{
				if ( (nLen2 == 0) || (memcmp(pText1, pText2, nLen2) == 0) )
				{
					pRetData->m_bool = BL_TRUE;
				}
				else
				{
					pRetData->m_bool = BL_FALSE;
				}
			}
			else
			{
				pRetData->m_bool = BL_FALSE;
			}
		}
	}
}

//	〈整数型〉 位取反 （整数型 欲取反的数值）
EXTERN_C void krnln_fnBNot(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = !pArgInf[0].m_int;
}
//	〈整数型〉 位与 （整数型 位运算数值一，整数型 位运算数值二，... ）
EXTERN_C void krnln_fnBAnd(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	INT n = pArgInf [0].m_int;
	for (INT i = 1; i < nArgCount; i++)
	{
		pArgInf++;
		n &=pArgInf->m_int;
	}
	pRetData->m_int = n;
}
//	〈整数型〉 位或 （整数型 位运算数值一，整数型 位运算数值二，... ）
EXTERN_C void krnln_fnBOr(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	INT nRet = pArgInf [0].m_int;
	for (INT i = 1; i < nArgCount; i++)
	{
		pArgInf++;
		nRet |=pArgInf->m_int;
	}
	pRetData->m_int = nRet;
}
//	〈整数型〉 位异或 （整数型 位运算数值一，整数型 位运算数值二，... ）
EXTERN_C void krnln_fnBXor(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	INT nRet = pArgInf [0].m_int;
	for (INT i = 1; i < nArgCount; i++)
	{
		pArgInf++;
		nRet ^=pArgInf->m_int;
	}
	pRetData->m_int = nRet;
}
//	〈整数型〉 左移 （整数型 欲移动的整数，整数型 欲移动的位数）
EXTERN_C void krnln_fnSHL(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = pArgInf[0].m_int << pArgInf[1].m_int;
}
//	〈整数型〉 右移 （整数型 欲移动的整数，整数型 欲移动的位数）
EXTERN_C void krnln_fnSHR(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = pArgInf[0].m_int >> pArgInf[1].m_int;
}
//	 〈整数型〉 合并整数 （整数型 用作合并的整数1，整数型 用作合并的整数2）
EXTERN_C void krnln_fnMAKELONG(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = MAKELONG(pArgInf[0].m_int, pArgInf[1].m_int);
}
//	〈短整数型〉 合并短整数 （整数型 用作合并的整数1，整数型 用作合并的整数2）
EXTERN_C void krnln_fnMAKEWORD(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = MAKEWORD(pArgInf[0].m_int, pArgInf[1].m_int);
}

//	 〈无返回值〉 数组清零 （通用型变量数组 数值数组变量）
EXTERN_C void krnln_fnZeroAry(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	/*
	if ( pArgInf[0].m_ppAryData )
	{
		INT *pArg = (INT*)*pArgInf[0].m_ppAryData;
		if ( pArg )
		{
			INT nLen = 1;
			for (int i=1; i<pArg[0]+1; i++)
			{
				nLen *= pArg[i];
			}
			if ( (pArgInf[0].m_dtDataType == SDT_TEXT) || (pArgInf[0].m_dtDataType == SDT_BIN) )
			{
				for (int i=pArg[0]+1; i<pArg[0]+1+nLen; i++)
				{
					MFree((void*)pArg[i]);		//		文本或字节集则释放内存
				}
			}
			ZeroMemory(pArg + pArg[0] + 1, nLen * sizeof(INT));
		}
	}
	*/
}
//	〈无返回值〉 数组排序 （通用型变量数组 数值数组变量，［逻辑型 排序方向是否为从小到大］）
EXTERN_C void krnln_fnSortAry(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
}

#define IS_CC(x) ((x) == 0xCC)
//	〈无返回值〉 取命令行 （文本型变量数组 存放被取回命令行文本的数组变量）
EXTERN_C void krnln_fnGetCmdLine (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	/*
	LPBYTE p = (LPBYTE)GetCommandLine (), pb;
	// 跳过调用程序名。
	BYTE ch = ' ';
	if (*p++ == '\"')
		ch = '\"';
	while (*p++ != ch);
	if (ch != ' ' && *p == ' ')  p++;    // 跳过第一个空格。

	CMyDWordArray aryText;

	while (*p != '\0')
	{
		if (*p == '\"')
		{
			p++;
			pb = p;
			while (*pb != '\0' && *pb != '\"')
			{
				if (IS_CC (*pb) == TRUE)
				{
					if (pb [1] == 0)  break;
					pb++;
				}
				pb++;
			}

			aryText.Add ((DWORD)CloneTextData ((char*)p, pb - p));

			p = pb;
			if (*p != '\0')  p++;
		}
		else if (*p > ' ')
		{
			pb = p;
			while (*pb != '\0' && *pb != '\"' && *pb > ' ')
			{
				if (IS_CC (*pb) == TRUE)
				{
					if (pb [1] == 0)  break;
					pb++;
				}
				pb++;
			}

			aryText.Add ((DWORD)CloneTextData ((char*)p, pb - p));

			p = pb;
		}
		else
			p++;
	}

	//!!! 必须先释放原变量数组的数据内容。
	NotifySys (NRS_FREE_ARY, (DWORD)pArgInf->m_dtDataType, (DWORD)*pArgInf->m_ppAryData);

	// 建立新变量数组数据。
	INT nSize = aryText.GetDWordCount () * sizeof (DWORD);
	p = (LPBYTE)MMalloc (sizeof (INT) * 2 + nSize);
	*(LPINT)p = 1;  // 数组维数。
	*(LPINT)(p + sizeof (INT)) = aryText.GetDWordCount ();
	memcpy (p + sizeof (INT) * 2, aryText.GetPtr (), nSize);

	*pArgInf->m_ppAryData = p;  // 将新内容写回该数组变量。
	*/
}

//	〈文本型〉 取运行目录 （）
EXTERN_C void krnln_fnGetRunPath(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CHAR strPath[MAX_PATH];
	DWORD dwLen = GetModuleFileName(GetModuleHandle(NULL), strPath, MAX_PATH);
	while( dwLen )
	{
		if ( strPath[dwLen - 1] == '\\' )
		{
			pRetData->m_pText = CloneTextData(strPath);
			break;
		}
		strPath[dwLen - 1] = '\0';
		dwLen--;
	}
}

//	〈文本型〉 取执行文件名 （）
EXTERN_C void krnln_fnGetRunFileName(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CHAR strPath[MAX_PATH];
	DWORD dwLen = GetModuleFileName(GetModuleHandle(NULL), strPath, MAX_PATH);
	while( dwLen )
	{
		if (strPath[dwLen - 1] == '\\' )
		{
			pRetData->m_pText = CloneTextData(&strPath[dwLen]);
			break;
		}
		dwLen--;
	}
}

//	〈文本型〉 读环境变量 （文本型 环境变量名称）
EXTERN_C void krnln_fnGetEnv(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	DWORD dwLen = MAX_PATH;
	CHAR* pstrVar = (CHAR*)MMalloc(dwLen);
	while ( GetEnvironmentVariable(pArgInf[0].m_pText, pstrVar, dwLen) == dwLen )	//	内存不足 则重新分配内存
	{
		MFree(pstrVar);
		dwLen *= 2;
		pstrVar = (CHAR*)MMalloc(dwLen);
	}
	pRetData->m_pText = CloneTextData(pstrVar);
	MFree(pstrVar);
}

//	〈逻辑型〉 写环境变量 （文本型 环境变量名称，文本型 欲写入内容）
EXTERN_C void krnln_fnPutEnv(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = SetEnvironmentVariable(pArgInf[0].m_pText, pArgInf[1].m_pText) ? BL_TRUE : BL_FALSE;
}

//	〈整数型〉 取文本长度 （文本型 文本数据）
EXTERN_C void krnln_fnLen(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_pText )
	{
		pRetData->m_int = strlen(pArgInf[0].m_pText );
	}
}

//	〈文本型〉 取文本左边 （文本型 欲取其部分的文本，整数型 欲取出字符的数目
EXTERN_C void krnln_fnLeft(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_pText )
	{
		INT nLen = pArgInf[1].m_int;
		INT nStrLen = strlen(pArgInf[0].m_pText);
		if ( nLen > nStrLen )	//	长度大于字符串长度
		{
			nLen = nStrLen;
		}
		pRetData->m_pText = CloneTextData(pArgInf[0].m_pText, nLen);
	}
}

//	〈文本型〉 取文本右边 （文本型 欲取其部分的文本，整数型 欲取出字符的数目）
EXTERN_C void krnln_fnRight(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_pText )
	{
		INT nLen = pArgInf[1].m_int;
		INT nStrLen = strlen(pArgInf[0].m_pText);
		if ( nLen > nStrLen )	//	长度大于字符串长度
		{
			nLen = nStrLen;
		}
		pRetData->m_pText = CloneTextData(pArgInf[0].m_pText + nStrLen - nLen, nLen);
	}
}

//	〈文本型〉 取文本中间 （文本型 欲取其部分的文本，整数型 起始取出位置，整数型 欲取出字符的数目）
EXTERN_C void krnln_fnMid(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_pText )
	{
		INT nLen = pArgInf[2].m_int;
		INT nStrLen = strlen(pArgInf[0].m_pText);
		if ( nLen + (pArgInf[1].m_int - 1) > nStrLen )	//	长度大于字符串长度
		{
			nLen = nStrLen - (pArgInf[1].m_int - 1);
		}
		pRetData->m_pText = CloneTextData(pArgInf[0].m_pText + (pArgInf[1].m_int - 1), nLen);
	}
}

//	〈文本型〉 字符 （字节型 欲取其字符的字符代码）
EXTERN_C void krnln_fnChr(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_byte )	//	未测试 不知道字符0是不是返回空文本串
	{
		pRetData->m_pText = CloneTextData((char*)&pArgInf[0].m_byte, 1);
	}
}

//	〈整数型〉 取代码 （文本型 欲取字符代码的文本，［整数型 欲取其代码的字符位置］）
EXTERN_C void krnln_fnAsc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_pText )
	{
		size_t nIdx = 1;
		if ( pArgInf[1].m_dtDataType != _SDT_NULL )
		{
			nIdx = pArgInf[1].m_int;
		}
		if ( strlen(pArgInf[0].m_pText) >= nIdx )	//	位置超出字符串长度
		{
			pRetData->m_int = pArgInf[0].m_pText[nIdx - 1];
		}
	}
}

//	〈整数型〉 寻找文本 （文本型 被搜寻的文本，文本型 欲寻找的文本，［整数型 起始搜寻位置］，逻辑型 是否不区分大小写）
EXTERN_C void krnln_fnInStr(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = -1;
	if ( pArgInf[0].m_pText )
	{
		if ( pArgInf[1].m_pText == NULL )
		{
			pRetData->m_int = 1;
		}
		else
		{
			size_t nIdx = 1;
			if ( pArgInf[2].m_dtDataType != _SDT_NULL )
			{
				nIdx = pArgInf[2].m_int;
			}
			if ( strlen(pArgInf[0].m_pText) >= nIdx )
			{
				CHAR *pStr = pArgInf[0].m_pText + (nIdx - 1);
				if ( pArgInf[3].m_bool )
				{
					pStr = StrStrI(pStr, pArgInf[1].m_pText);
				}
				else
				{
					pStr = StrStr(pStr, pArgInf[1].m_pText);
				}
				if ( pStr )
				{
					pRetData->m_int = pStr - pArgInf[0].m_pText + 1;
				}
			}
		}
	}
}

//	〈整数型〉 倒找文本 （文本型 被搜寻的文本，文本型 欲寻找的文本，［整数型 起始搜寻位置］，逻辑型 是否不区分大小写）
//	存在效率问题
EXTERN_C void krnln_fnInStrRev(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_int = -1;
	if ( pArgInf[0].m_pText )
	{
		if ( pArgInf[1].m_pText == NULL )
		{
			pRetData->m_int = 1;
		}
		else
		{
			size_t nIdx = 1;
			if ( pArgInf[2].m_dtDataType != _SDT_NULL )
			{
				nIdx = pArgInf[2].m_int;
			}
			if ( strlen(pArgInf[0].m_pText) >= nIdx )
			{
				CHAR *pStr = pArgInf[0].m_pText + (nIdx - 1);
				CHAR *pLastStr = NULL;
				do 
				{
					if ( pArgInf[3].m_bool )
					{
						pStr = StrStrI(pStr, pArgInf[1].m_pText);
					}
					else
					{
						pStr = StrStr(pStr, pArgInf[1].m_pText);
					}
					if ( pStr )
					{
						pLastStr = pStr;
					}
				} while ( pStr++ );

				if ( pLastStr )
				{
					pRetData->m_int = pLastStr - pArgInf[0].m_pText + 1;
				}
			}
		}
	}
}

//	〈文本型〉 到大写 （文本型 欲变换的文本）
EXTERN_C void krnln_fnUCase(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_pText = CloneTextData(pArgInf[0].m_pText);
	if ( pRetData->m_pText )
	{
		INT nIdx = 0;
		do
		{
			if ( pRetData->m_pText[nIdx] == '\0' )
			{
				break;
			}
			pRetData->m_pText[nIdx] = toupper(pRetData->m_pText[nIdx]);
			nIdx++;
		} while ( TRUE );
	}
}

//	〈文本型〉 到小写 （文本型 欲变换的文本）
EXTERN_C void krnln_fnLCase(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_pText = CloneTextData(pArgInf[0].m_pText);
	if ( pRetData->m_pText )
	{
		INT nIdx = 0;
		do
		{
			if ( pRetData->m_pText[nIdx] == '\0' )
			{
				break;
			}
			pRetData->m_pText[nIdx] = tolower(pRetData->m_pText[nIdx]);
			nIdx++;
		} while ( TRUE );
	}
}

//	〈文本型〉 到文本 （通用型数组/非数组 待转换的数据）
EXTERN_C void krnln_fnStr(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CHAR strText[30] = {0};
	switch ( pArgInf[0].m_dtDataType )
	{
	case SDT_BYTE:
		sprintf(strText, "%d", pArgInf[0].m_byte);
		break;
	case SDT_SHORT:
		sprintf(strText, "%d", pArgInf[0].m_short);
		break;
	case SDT_INT:
		sprintf(strText, "%d", pArgInf[0].m_int);
		break;
	case SDT_INT64:
		sprintf(strText, "%ld", pArgInf[0].m_int64);
		break;
	case SDT_FLOAT:
		sprintf(strText, "%.12f", pArgInf[0].m_float);
		break;
	case SDT_DOUBLE:
		sprintf(strText, "%.12lf", pArgInf[0].m_double);
		break;
	case SDT_BOOL:
		pArgInf[0].m_bool ? strcpy(strText, " 真") : strcpy(strText, "假");
		break;
	case SDT_DATE_TIME:
		break;
	case SDT_TEXT:
		pRetData->m_pText = CloneTextData(pArgInf[0].m_pText);
		return;
		break;
	case SDT_BIN:
		pRetData->m_pText = CloneTextData((CHAR*)pArgInf[0].m_pBin+ sizeof (INT)*2,  *(LPINT)(pArgInf[0].m_pBin + sizeof (INT)));
		return;
		break;
	case SDT_SUB_PTR:
		sprintf(strText, "%d", (INT)pArgInf[0].m_pdwSubCodeAdr);
		break;
	}
	pRetData->m_pText = CloneTextData(strText);
}

//	〈文本型〉 删首空 （文本型 欲删除空格的文本）
EXTERN_C void krnln_fnLTrim(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CHAR *pstrText = pArgInf[0].m_pText ;
	if ( pstrText )
	{
		while ( *pstrText )
		{
			if ( *pstrText == ' '  )
			{
				pstrText++;
			}
			 else if ( *pstrText == -95  && *(pstrText + 1) == -95 )	//	全角
			{
				pstrText += 2;
			}
			else
			{
				break;
			}
		};
		pRetData->m_pText = CloneTextData(pstrText);
	}
}

//	〈文本型〉 删尾空 （文本型 欲删除空格的文本）
EXTERN_C void krnln_fnRTrim(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CHAR *pstrText = pArgInf[0].m_pText ;
	if ( pstrText )
	{
		size_t nLen = strlen(pstrText);
		if ( nLen )
		{
			pstrText += nLen;
			pstrText--;
			INT nLen1 = 0;
			while ( pstrText >= pArgInf[0].m_pText )
			{
				if ( *pstrText == ' '  )
				{
					nLen1++;
					pstrText--;
				}
				else if ( *pstrText == -95  && *(pstrText - 1) == -95 )	//	全角
				{
					nLen1 += 2;
					pstrText -= 2;
				}
				else
				{
					break;
				}
			};
			pRetData->m_pText = CloneTextData(pArgInf[0].m_pText, nLen - nLen1);
		}
	}
}

//	〈文本型〉 删首尾空 （文本型 欲删除空格的文本）
EXTERN_C void krnln_fnTrim(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CHAR *pstrText1, *pstrText = pArgInf[0].m_pText ;
	if ( pstrText )
	{
		while ( *pstrText )
		{
			if ( *pstrText == ' '  )
			{
				pstrText++;
			}
			else if ( *pstrText == -95  && *(pstrText + 1) == -95 )	//	全角
			{
				pstrText += 2;
			}
			else
			{
				break;
			}
		};

		pstrText1 = pstrText;
		size_t nLen = strlen(pstrText);
		if ( nLen )
		{
			pstrText += nLen;
			pstrText--;
			INT nLen1 = 0;
			while ( pstrText >= pstrText1 )
			{
				if ( *pstrText == ' '  )
				{
					nLen1++;
					pstrText--;
				}
				else if ( *pstrText == -95  && *(pstrText - 1) == -95 )	//	全角
				{
					nLen1 += 2;
					pstrText -= 2;
				}
				else
				{
					break;
				}
			};
			pRetData->m_pText = CloneTextData(pstrText1, nLen - nLen1);
		}
	}
}

//	〈文本型〉 删全部空 （文本型 欲删除空格的文本）
EXTERN_C void krnln_fnTrimAll(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CHAR *pstrText = pArgInf[0].m_pText ;
	if ( pstrText )
	{
		CHAR *pstrText1 = (CHAR *)MMalloc(strlen(pstrText)+ 1);
		INT nLen = 0;
		while ( *pstrText )
		{
			if ( *pstrText == ' '  )
			{
				pstrText++;
			}
			else if ( *pstrText == -95  && *(pstrText + 1) == -95 )	//	全角
			{
				pstrText += 2;
			}
			else if ( *pstrText < 0 && *(pstrText + 1) < 0 )
			{
				pstrText1[nLen++] = *pstrText++;
				pstrText1[nLen++] = *pstrText++;
			}
			else
			{
				pstrText1[nLen++] = *pstrText++;
			}
		};
		pRetData->m_pText = CloneTextData(pstrText1, nLen);
		MFree(pstrText1);
	}
}

//	〈文本型〉 文本替换 （文本型 欲被替换的文本，整数型 起始替换位置，整数型 替换长度，［文本型 用作替换的文本］）
EXTERN_C void krnln_fnReplaceText(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_pText )
	{
		int nLen1, nLen = (int)strlen(pArgInf[0].m_pText);
		if ( pArgInf[1].m_int > nLen)
		{
			pArgInf[1].m_int = nLen + 1;
			pArgInf[2].m_int = 0;
		}
		else if ( pArgInf[1].m_int + pArgInf[2].m_int - 1 >= nLen )
		{
			pArgInf[2].m_int = nLen - (pArgInf[1].m_int - 1);
		}
		if ( (pArgInf[3].m_dtDataType == _SDT_NULL) || pArgInf[3].m_pText )
		{
			nLen1 = nLen - pArgInf[2].m_int;
		}
		else
		{
			nLen1 = nLen + strlen(pArgInf[3].m_pText) - pArgInf[2].m_int;
		}
		pRetData->m_pText = (CHAR *)MMalloc(nLen1 + 1);
		ZeroMemory(pRetData->m_pText, nLen1 + 1);
		memcpy(pRetData->m_pText, pArgInf[0].m_pText,  pArgInf[1].m_int - 1);

		if ( pArgInf[1].m_int + pArgInf[2].m_int - 1 >= nLen )
		{
			strcat(pRetData->m_pText, pArgInf[0].m_pText + pArgInf[1].m_int + pArgInf[2].m_int - 1);

			if ( (pArgInf[3].m_dtDataType != _SDT_NULL) && pArgInf[3].m_pText )
			{
				strcat(pRetData->m_pText, pArgInf[3].m_pText);
			}
		}
		else
		{
			if ( (pArgInf[3].m_dtDataType != _SDT_NULL) && pArgInf[3].m_pText )
			{
				strcat(pRetData->m_pText, pArgInf[3].m_pText);
			}
			strcat(pRetData->m_pText, pArgInf[0].m_pText + pArgInf[1].m_int + pArgInf[2].m_int - 1);
		}
	}
}

//	〈文本型〉 子文本替换 （文本型 欲被替换的文本，文本型 欲被替换的子文本，［文本型 用作替换的子文本］，［整数型 进行替换的起始位置］，［整数型 替换进行的次数］，逻辑型 是否区分大小写）
EXTERN_C void krnln_fnRpSubText(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CHAR *pstrText = pArgInf[0].m_pText;
	CHAR *pstrText1 = pArgInf[1].m_pText;
	if ( pstrText && pstrText1 )
	{
		int nLen = (int) strlen(pstrText);
		int nLen1 = (int) strlen(pstrText1);
		if ( nLen1 && nLen >= nLen1 )
		{
			CHAR *pstrText2 = (CHAR *)MMalloc(0);
			while( *pstrText )
			{
				if ( memcmp(pstrText, pstrText1, nLen) == 0 )
				{

				}
			}
		}
	}
}

//	〈文本型〉 取空白文本 （整数型 重复次数）
EXTERN_C void krnln_fnSpace(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_int )	//	pArgInf[0].m_int 是重复次数
	{
		pRetData->m_pText = (CHAR *)MMalloc(pArgInf[0].m_int + 1);	//	分配文本的内存
		memset(pRetData->m_pText, ' ', pArgInf[0].m_int);	//	设置空白文本
		pRetData->m_pText[pArgInf[0].m_int] = '\0';		//	设置文本结束符
	}
}

//	〈文本型〉 取重复文本 （整数型 重复次数，文本型 待重复文本）
EXTERN_C void krnln_fnString(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_int )	//	pArgInf[0].m_int 是重复次数
	{
		if ( pArgInf[1].m_pText )
		{
			int nLen = (int)strlen(pArgInf[1].m_pText);
			if ( nLen )
			{
				pRetData->m_pText = (CHAR *)MMalloc(pArgInf[0].m_int * nLen + 1);	//	分配文本的内存
				pRetData->m_pText[pArgInf[0].m_int * nLen] = '\0';		//	设置文本结束符
				do 
				{
					memcpy(pRetData->m_pText + nLen * (pArgInf[0].m_int - 1), pArgInf[1].m_pText, nLen);
				} while ( --pArgInf[0].m_int );
			}
		}
	}
}

//	〈整数型〉 文本比较 （文本型 待比较文本一，文本型 待比较文本二，逻辑型 是否区分大小写）
EXTERN_C void krnln_fnStrComp(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[2].m_bool )
	{
		pRetData->m_int = strcmp(pArgInf[0].m_pText, pArgInf[1].m_pText);
	}
	else
	{
		pRetData->m_int = strcmpi(pArgInf[0].m_pText, pArgInf[1].m_pText);
	}
}

//	〈文本型数组〉 分割文本 （文本型 待分割文本，［文本型 用作分割的文本］，［整数型 要返回的子文本数目］）
EXTERN_C void krnln_fnSplit(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
}

//	〈文本型〉 指针到文本 （整数型 内存文本指针）
EXTERN_C void krnln_fnpstr(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if( !IsBadReadPtr((PVOID)pArgInf[0].m_int, 1) )
	{
		pRetData->m_pText = CloneTextData((CHAR*) pArgInf[0].m_int);
	}
}

//	信息框
EXTERN_C void krnln_fnMsgBox(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CHAR *lpText = pArgInf[2].m_pText;
	CHAR strText[30] = {0};
	if ( pArgInf[2].m_dtDataType == _SDT_NULL )
	{
		lpText = "信息：";
	}
	switch ( pArgInf[0].m_dtDataType )
	{
	case SDT_BYTE:
		sprintf(strText, "%d", pArgInf[0].m_byte);
		break;
	case SDT_SHORT:
		sprintf(strText, "%d", pArgInf[0].m_short);
		break;
	case SDT_INT:
		sprintf(strText, "%d", pArgInf[0].m_int);
		break;
	case SDT_INT64:
		sprintf(strText, "%ld", pArgInf[0].m_int64);
		break;
	case SDT_FLOAT:
		sprintf(strText, "%.12f", pArgInf[0].m_float);
		break;
	case SDT_DOUBLE:
		sprintf(strText, "%.12lf", pArgInf[0].m_double);
		break;
	case SDT_BOOL:
		pArgInf[0].m_bool ? strcpy(strText, " 真") : strcpy(strText, "假");
		break;
	case SDT_DATE_TIME:
		break;
	case SDT_TEXT:
		pRetData->m_int = MessageBox(NULL, pArgInf[0].m_pText, lpText, pArgInf[1].m_int);
		return;
		break;
	case SDT_BIN:
		break;
	case SDT_SUB_PTR:
		sprintf(strText, "%d", (INT)pArgInf[0].m_pdwSubCodeAdr);
		break;
	}
	pRetData->m_int = MessageBox(NULL, strText,  lpText, pArgInf[1].m_int);
}


//	到字节集
EXTERN_C void krnln_fnBinLen(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_pBin )
	{
		pRetData->m_int = *(LPINT)(pArgInf[0].m_pBin + sizeof (INT));
	}
}


//	到小数
EXTERN_C void krnln_fnToFloat(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if ( pArgInf[0].m_pText )
	{
		pRetData->m_float = (float)strtod(pArgInf[0].m_pText, NULL);
	}
}

//	到整数
EXTERN_C void krnln_fnToInt(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
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
		pRetData->m_int = (INT)pArgInf[0].m_float;
		break;
	case SDT_DOUBLE:
		pRetData->m_int = (INT)pArgInf[0].m_double;
		break;
	case SDT_BOOL:
		pRetData->m_int = pArgInf[0].m_bool;
		break;
	case SDT_DATE_TIME:
		break;
	case SDT_TEXT:
		if ( pArgInf[0].m_pText )
		{
			pRetData->m_int = strtol(pArgInf[0].m_pText, NULL, 10);
		}
		break;
	case SDT_BIN:
		break;
	case SDT_SUB_PTR:
		pRetData->m_int = (INT)pArgInf[0].m_pdwSubCodeAdr;
		break;
	}
}

//	到数值
EXTERN_C void krnln_fnVal(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	switch ( pArgInf[0].m_dtDataType )
	{
	case SDT_BYTE:
		pRetData->m_double = pArgInf[0].m_byte;
		break;
	case SDT_SHORT:
		pRetData->m_double = pArgInf[0].m_short;
		break;
	case SDT_INT:
		pRetData->m_double = pArgInf[0].m_int;
		break;
	case SDT_INT64:
		pRetData->m_double = (DOUBLE)pArgInf[0].m_int64;
		break;
	case SDT_FLOAT:
		pRetData->m_double = (DOUBLE)pArgInf[0].m_float;
		break;
	case SDT_DOUBLE:
		pRetData->m_double = (DOUBLE)pArgInf[0].m_double;
		break;
	case SDT_BOOL:
		pRetData->m_double = pArgInf[0].m_bool;
		break;
	case SDT_DATE_TIME:
		break;
	case SDT_TEXT:
		if ( pArgInf[0].m_pText )
		{
			pRetData->m_double = strtod(pArgInf[0].m_pText, NULL);
		}
		break;
	case SDT_BIN:
		break;
	case SDT_SUB_PTR:
		pRetData->m_double = (DOUBLE)(INT)pArgInf[0].m_pdwSubCodeAdr;
		break;
	}
}



INT WINAPI krnl_NotifySys (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
	INT nRet = NR_ERR;
	switch (nMsg)
	{
	case NRS_FREE_ARY:
		break;
	case NRS_MALLOC:
		nRet = (INT)krnl_MMalloc(dwParam1);
		break;
	case NRS_MFREE:
		krnl_MFree((PVOID)dwParam1);
		nRet = NR_OK;
		break;
	case NRS_MREALLOC:
		nRet = (INT)krnl_MRealloc((PVOID)dwParam1, (DWORD)dwParam2);
		break;
	}
	return nRet;
}