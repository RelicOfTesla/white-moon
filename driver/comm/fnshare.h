#ifndef __FN_SHARE_H
#define __FN_SHARE_H
#include "lib2.h"
#include "lang.h"
#include <windef.h>

char* WINAPIV CloneTextData (char* ps);
char* WINAPIV _CloneTextData (char* ps, INT nTextLen);
LPBYTE WINAPIV CloneBinData (LPBYTE pData, INT nDataSize);
void GReportError (char* szErrText);
void* WINAPIV MMalloc (INT nSize);
void WINAPIV MFree (void* p);
LPBYTE GetAryElementInf (void* pAryData, LPINT pnElementCount);
INT GetDataTypeType (DATA_TYPE dtDataType);

#endif //__FN_SHARE_H
