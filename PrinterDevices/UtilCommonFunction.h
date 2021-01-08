#pragma once

#include "afxwin.h"
#include "GlobalDef.h"

DWORD UtilRunCmd(CString strCmd);
CString UtilStrAddQuote(CString str);
CString UtilGetErrorMsg(DWORD id);
BOOL GetRegNumber(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD * pdwValue);
CString GetRegString(LPCTSTR lpSubKey, LPCTSTR lpValueName);
void UtilGetPrinterArray(CString strDriver, bool bIsUSB, CStringArray &strArray);
void UtilGetDriverArray(CStringArray &strArray);
int GetPrinterInfo(StPrinterInfo *stInfo);
BOOL GetPrinterType(CString strDriverName, EnPrinterType & type);
int GetScanEnum(CStringArray &arrayPAth);
bool CreateMultipleDirectory(const CString strPath);