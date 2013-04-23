/*
    版权声明：
    本文件版权为易语言作者吴涛所有，仅授权给第三方用作开发易语言支持库，禁止用于其他任何场合。
*/
#include "fnshare.h"
EXTERN_C PVOID WINAPIV krnl_MMalloc(DWORD dwBytes);
EXTERN_C void WINAPIV krnl_MFree(PVOID lpMem);
EXTERN_C void WINAPIV krnl_MReportError(UINT uErrCode);

// 使用指定文本数据建立易程序中使用的文本数据。
char* WINAPIV CloneTextData (char* ps)
{
	INT nTextLen;
	char* pd;
    if (ps == NULL || *ps == '\0')
        return NULL;
	
	nTextLen = strlen (ps);

	pd = (char*)krnl_MMalloc (nTextLen + 1);
    memcpy (pd, ps, nTextLen);
    pd [nTextLen] = '\0';
    return pd;
}

// 使用指定文本数据建立易程序中使用的文本数据。
//   nTextLen用作指定文本部分的长度（不包含结束零），
// 如果为-1，则取ps的全部长度。
char* WINAPIV _CloneTextData (char* ps, INT nTextLen)
{
	char* pd;
    if (nTextLen <= 0)
        return NULL;

    pd = (char*)krnl_MMalloc (nTextLen + 1);
    memcpy (pd, ps, nTextLen);
    pd [nTextLen] = '\0';
    return pd;
}

// 使用指定数据建立易程序中使用的字节集数据。
LPBYTE WINAPIV CloneBinData (LPBYTE pData, INT nDataSize)
{
	LPBYTE pd;
    if (nDataSize == 0)
        return NULL;

    pd = (LPBYTE)krnl_MMalloc (sizeof (INT) * 2 + nDataSize);
    *(LPINT)pd = 1;
    *(LPINT)(pd + sizeof (INT)) = nDataSize;
    memcpy (pd + sizeof (INT) * 2, pData, nDataSize);
    return pd;
}

// 报告运行时错误。
void GReportError (char* szErrText)
{
	//krnl_MReportError(szErrText);
}

void* WINAPIV MMalloc (INT nSize)
{
	return (void*)krnl_MMalloc (nSize);
}

void WINAPIV MFree (void* p)
{
	krnl_MFree( p );
}

// 返回数组的数据部分首地址及成员数目。
LPBYTE GetAryElementInf (void* pAryData, LPINT pnElementCount)
{
	INT nElementCount;
	LPINT pnData = (LPINT)pAryData;
	INT nArys = *pnData++;  // 取得维数。
	// 计算成员数目。
	nElementCount = 1;
	while (nArys > 0)
	{
		nElementCount *= *pnData++;
		nArys--;
	}

	if (pnElementCount != NULL)
		*pnElementCount = nElementCount;
	return (LPBYTE)pnData;
}

#define DTT_IS_NULL_DATA_TYPE   0
#define DTT_IS_SYS_DATA_TYPE    1
#define DTT_IS_USER_DATA_TYPE   2
#define DTT_IS_LIB_DATA_TYPE    3
// 取回数据类型的类别。
INT GetDataTypeType (DATA_TYPE dtDataType)
{
	DWORD dw;
	if (dtDataType == _SDT_NULL)
		return DTT_IS_NULL_DATA_TYPE;

	dw = dtDataType & 0xC0000000;
	return dw == DTM_SYS_DATA_TYPE_MASK ? DTT_IS_SYS_DATA_TYPE :
			dw == DTM_USER_DATA_TYPE_MASK ? DTT_IS_USER_DATA_TYPE :
			DTT_IS_LIB_DATA_TYPE;
}