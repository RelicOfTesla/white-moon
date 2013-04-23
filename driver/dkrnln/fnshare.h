/*
    版权声明：
    本文件版权为易语言作者吴涛所有，仅授权给第三方用作开发易语言支持库，禁止用于其他任何场合。
*/

//本单元不使用MFC类, 可供编写跨平台的支持库或非窗口组件支持库使用

#ifndef __FN_SHARE_H
#define __FN_SHARE_H
#include "lib2.h"
#include "lang.h"
#include <windef.h>

char* CloneTextData (char* ps);
char* _CloneTextData (char* ps, INT nTextLen);
LPBYTE CloneBinData (LPBYTE pData, INT nDataSize);
void GReportError (char* szErrText);
void* MMalloc (INT nSize);
void MFree (void* p);
LPBYTE GetAryElementInf (void* pAryData, LPINT pnElementCount);
INT GetDataTypeType (DATA_TYPE dtDataType);

#endif //__FN_SHARE_H
