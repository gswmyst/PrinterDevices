#include "pch.h"
#include "UtilCommonFunction.h"
#include <winspool.h>

#define REG_PRINTER_PATH_STR   _T("SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\")

DWORD UtilRunCmd(CString strCmd)
{
#ifdef DEBUG_RUN
	return 0;
#endif
	CString str = _T("cmd.exe /c ");
	str += strCmd;
	TCHAR tchExePath[_MAX_PATH];
	wcscpy_s(tchExePath, str);		// unicodeÓÃ

	PROCESS_INFORMATION  pro_id;
	::ZeroMemory(&pro_id, sizeof(PROCESS_INFORMATION));

	STARTUPINFO  str_inf;
	::ZeroMemory(&str_inf, sizeof(STARTUPINFO));
	str_inf.cb = sizeof(STARTUPINFO);
	str_inf.dwFlags = STARTF_USESHOWWINDOW;
	str_inf.wShowWindow = SW_HIDE;

	int ret_code = ::CreateProcess(
		NULL,
		tchExePath,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&str_inf,
		&pro_id
	);

	::CloseHandle(pro_id.hThread);
	WaitForSingleObject(pro_id.hProcess, INFINITE);

	DWORD ret = 0;
	//GetExitCodeProcess(pro_id.hProcess, &ret);

	::CloseHandle(pro_id.hProcess);

	return ret;
}

CString UtilStrAddQuote(CString str)
{
	return (_T("\"") + str + _T("\""));
}

CString UtilGetErrorMsg(DWORD id)
{
	WCHAR lpMsgBuf[1024] = { 0 };
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		id,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		lpMsgBuf,
		1024, NULL);
	return CString(lpMsgBuf);
}

BOOL GetRegNumber(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD * pdwValue)
{
	*pdwValue = 0;
	DWORD   dwType;
	HKEY    hKey;
	DWORD   dwDataSize = sizeof(DWORD);
	LONG    lRet;

	lRet = RegCreateKeyEx
	(
		HKEY_LOCAL_MACHINE,
		lpSubKey,
		NULL,
		NULL,
		NULL,
		KEY_READ,
		NULL,
		&hKey,
		NULL
	);

	if (lRet != ERROR_SUCCESS)
	{
		return FALSE;
	}

	lRet = RegQueryValueEx
	(
		hKey,
		lpValueName,
		0,
		&dwType,
		(LPBYTE)pdwValue,
		&dwDataSize
	);
	RegCloseKey(hKey);

	if (lRet != ERROR_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}

CString GetRegString(LPCTSTR lpSubKey, LPCTSTR lpValueName)
{
	DWORD   dwType;
	HKEY    hKey;
	DWORD   dwDataSize = 0;
	LONG    lRet;
	CString strValue = _T("");

	lRet = RegCreateKeyEx
	(
		HKEY_LOCAL_MACHINE,
		lpSubKey,
		NULL,
		NULL,
		NULL,
		KEY_READ,
		NULL,
		&hKey,
		NULL
	);

	if (lRet != ERROR_SUCCESS)
	{
		return strValue;
	}

	lRet = RegQueryValueEx
	(
		hKey,
		lpValueName,
		0,
		&dwType,
		0,
		&dwDataSize
	);
	
	if (lRet != ERROR_SUCCESS && dwDataSize > 0)
	{
		RegCloseKey(hKey);
		return strValue;
	}

	TCHAR *pExDataPath = (TCHAR*)malloc(dwDataSize);
	lRet = RegQueryValueEx
	(
		hKey,
		lpValueName,
		0,
		&dwType,
		(LPBYTE)pExDataPath,
		&dwDataSize
	);
	RegCloseKey(hKey);
	if (lRet != ERROR_SUCCESS)
	{
		free(pExDataPath);
		return strValue;
	}

	DWORD dwLength = dwDataSize / sizeof(TCHAR);
	for (int i = 0; i < dwLength; i++)
	{
		if (pExDataPath[i] == 0)
		{
			pExDataPath[i] = '\n';
		}
	}
	pExDataPath[dwLength - 1] = 0;

	strValue = pExDataPath;
	free(pExDataPath);
	return strValue;
}

void UtilGetPrinterArray(CString strDriver, bool bIsUSB, CStringArray & strArray)
{
	LPBYTE pPrinterEnum;
	DWORD dwNeeded;
	DWORD dwReturned;

	BOOL ret = EnumPrinters(PRINTER_ENUM_LOCAL,
		NULL,
		2,
		NULL,
		0,
		&dwNeeded,
		&dwReturned);

	pPrinterEnum = (LPBYTE)LocalAlloc(LPTR, dwNeeded);
	if (pPrinterEnum == NULL) {
		return;
	}

	ret = EnumPrinters(PRINTER_ENUM_LOCAL,
		NULL,
		2,
		pPrinterEnum,
		dwNeeded,
		&dwNeeded,
		&dwReturned);
	if (ret == FALSE || pPrinterEnum == NULL) {
		LocalFree(pPrinterEnum);
		return;
	}
	PRINTER_INFO_2 *pInfo;
	pInfo = (PRINTER_INFO_2 *)pPrinterEnum;

	//CString strDriverName = _T("DELI PRINTER");
	CString strPrintersPath = REG_PRINTER_PATH_STR;
	CString strPrinterName;
	CString strTargetKey;
	CString strSubKeyPath;
	CString strPort;

	for (int i = 0; i < dwReturned && i < 128; i++)
	{
		if (strDriver == pInfo->pDriverName)
		{
			strPrinterName = (pInfo->pPrinterName);
			strTargetKey = strPrintersPath + strPrinterName;
			strPort = GetRegString(strTargetKey, _T("Port"));
			if (bIsUSB)
			{
				if (strPort.Left(3) == _T("USB"))
				{
					strArray.Add(strPrinterName);
				}
			}
			else
			{
				if (strPort.Left(3) != _T("USB"))
				{
					strArray.Add(strPrinterName);
				}
			}
		}
		pInfo++;
	}
	LocalFree(pPrinterEnum);
}

void UtilGetDriverArray(CStringArray & strArray)
{
	LPBYTE pDriverEnum;
	DWORD dwNeeded;
	DWORD dwReturned;

	BOOL ret = EnumPrinterDrivers(NULL,
		NULL,
		1,
		NULL,
		0,
		&dwNeeded,
		&dwReturned);

	pDriverEnum = (LPBYTE)LocalAlloc(LPTR, dwNeeded);
	if (pDriverEnum == NULL) {
		return;
	}

	ret = EnumPrinterDrivers(NULL,
		NULL,
		1,
		pDriverEnum,
		dwNeeded,
		&dwNeeded,
		&dwReturned);
	if (ret == FALSE || pDriverEnum == NULL) {
		LocalFree(pDriverEnum);
		return;
	}

	DRIVER_INFO_1 *pInfo;
	pInfo = (DRIVER_INFO_1 *)pDriverEnum;

	for (int i = 0; i < dwReturned; i++)
	{
		EnPrinterType enType;
		if (GetPrinterType(pInfo->pName, enType))
		{
			strArray.Add(pInfo->pName);
		}
		pInfo++;
	}
	LocalFree(pDriverEnum);
	return;
}

int GetPrinterInfo(StPrinterInfo *stInfo)
{
	int iNum = 0;
	LPBYTE pPrinterEnum;
	DWORD dwNeeded;
	DWORD dwReturned;

	BOOL ret = EnumPrinters(PRINTER_ENUM_LOCAL,
		NULL,
		2,
		NULL,
		0,
		&dwNeeded,
		&dwReturned);

	pPrinterEnum = (LPBYTE)LocalAlloc(LPTR, dwNeeded);
	if (pPrinterEnum == NULL) {
		return iNum;
	}

	ret = EnumPrinters(PRINTER_ENUM_LOCAL,
		NULL,
		2,
		pPrinterEnum,
		dwNeeded,
		&dwNeeded,
		&dwReturned);
	if (ret == FALSE || pPrinterEnum == NULL) {
		LocalFree(pPrinterEnum);
		return iNum;
	}
	PRINTER_INFO_2 *pInfo;
	pInfo = (PRINTER_INFO_2 *)pPrinterEnum;

	//CString strDriverName = _T("DELI PRINTER");
	CString strPrintersPath = _T("SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\");
	CString strPrinterName;
	CString strTargetKey;
	CString strSubKeyPath;
	CString strPort;
	DWORD dwPrinterNo;
	DWORD dwConType;
	EnPrinterType enType;

	for (int i = 0; i < dwReturned && i < MAX_PRINTER_NUM; i++)
	{
		if (GetPrinterType(pInfo->pDriverName, enType))
		{
			strPrinterName = (pInfo->pPrinterName);
			strTargetKey = strPrintersPath + strPrinterName;
			strPort = GetRegString(strTargetKey, _T("Port"));
			if (strPort == _T(""))
			{
				continue;
			}
			if (strPort.Left(3) == _T("USB"))
			{
				dwConType = CONNECT_TYPE_USB;
			}
			else
			{
				dwConType = CONNECT_TYPE_NW;
			}

			strTargetKey += (_T("\\PrinterDriverData"));

			if (!GetRegNumber(strTargetKey, _T("PrinterNo"), &dwPrinterNo))
			{
				//continue;
			}

			strSubKeyPath = GetRegString(strTargetKey, _T("SubKeyPath"));
			//if (strSubKeyPath != _T(""))
			//GetRegNumber(strSubKeyPath, _T("ConnectType"), &dwConType);

			{
				stInfo[iNum].iNo = dwPrinterNo;
				stInfo[iNum].enType = enType;
				stInfo[iNum].iConnectType = dwConType;
				stInfo[iNum].strName = strPrinterName;
				stInfo[iNum].strPort = strPort;
				stInfo[iNum].strSubKeyPath = strSubKeyPath;
				iNum++;
			}
		}
		pInfo++;
	}
	LocalFree(pPrinterEnum);
	return iNum;
}

BOOL GetPrinterType(CString strDriverName, EnPrinterType & type)
{
	for (int i = 0; i < EN_PRINTER_TYPE_MAX; i++)
	{
		if (strDriverName == c_stModelTypeTag[i].strModel)
		{
			type = c_stModelTypeTag[i].enType;
			return TRUE;
		}
	}
	return FALSE;
}

int GetScanEnum(CStringArray &arrayPAth)
{
	int iNum = 0;
	LPCTSTR lpSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\Root\\Image\\");
	HKEY hKey;
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		lpSubKey,
		0,
		KEY_READ,
		&hKey);

	if (lRet != ERROR_SUCCESS)
	{
		return 0;
	}

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys = 0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.
	if (cSubKeys)
	{
		for (i = 0; i<cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				WCHAR tmpSubKey[MAX_PATH] = {0};
				wcscpy_s(tmpSubKey, lpSubKey);
				wcscat_s(tmpSubKey, achKey);
				CString strID = GetRegString(tmpSubKey, _T("HardwareID"));
				if (strID == _T("DeliScnDrv_ID"))
				{
					arrayPAth.Add(achKey);
					iNum++;
				}
			}
		}
	}

	// Enumerate the key values. 
	//if (cValues)
	//{
	//	UTIL_DEBUG_LOG(_T("Number of values: %d"), cValues);

	//	for (i = 0, retCode = ERROR_SUCCESS; i<cValues; i++)
	//	{
	//		cchValue = MAX_VALUE_NAME;
	//		achValue[0] = '\0';
	//		retCode = RegEnumValue(hKey, i,
	//			achValue,
	//			&cchValue,
	//			NULL,
	//			NULL,
	//			NULL,
	//			NULL);

	//		if (retCode == ERROR_SUCCESS)
	//		{
	//			UTIL_DEBUG_LOG(_T("(%d) %s"), i + 1, achValue);
	//		}
	//	}
	//}
	RegCloseKey(hKey);

	return iNum;
}

bool CreateMultipleDirectory(const CString strPath)
{
	if (strPath.IsEmpty())
	{
		return false;
	}

	CString strDir = strPath;
	if (strDir.GetAt(strDir.GetLength() - 1) != _T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}

	CStringArray arrayPath;
	bool bSuccess = false;

	int iFind = strDir.Find(_T("\\"));
	while (iFind > -1)
	{
		arrayPath.Add(strDir.Mid(0, iFind));
		iFind = strDir.Find(_T("\\"), iFind + 1);
	}

	for (int i = 0; i < arrayPath.GetCount(); i++)
	{
		if (!PathIsDirectory(arrayPath[i]))
		{
			bSuccess = CreateDirectory(arrayPath[i], NULL) ? true : false;
		}
	}
	return bSuccess;
}