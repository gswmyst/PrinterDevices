#pragma once

#include "afxwin.h"
#include <vector>

//printer info
#define CONTAINER_ID_LENGTH				1
#define CONTAINER_VERSION_LENGTH		2
#define CONTAINER_SIZE_LENGTH			2
#define VENDER_NAME_FIELD_SIZE_LENGTH	2
#define PRODUCT_NAME_FIELD_SIZE_LENGTH	2
#define SERIAL_NUMBER_FIALD_SIZE_LENGTH	2
#define ERROR_SUMMARY_FIELD_SIZE_LENGTH	2
#define PRINTER_STATUS_LENGTH			1
#define PRINTER_FW_SIZE_LENGTH			1
#define JOB_NAME_FIELD_SIZE_LENGTH		1
#define NUMBER_OF_ERROR_LENGTH			1
#define NUMBER_OF_WARNING_LENGTH		1
#define ERROR_CODE_LENGTH				2
#define WARNING_ERROR_CODE_LENGTH		2

//supply info
#define NUMBER_OF_SUPPLIES_INFO_LENGTH	1
#define SUPPLIES_ID_LENGTH				2
#define SUPPLIES_DATA_SIZE_LENGTH		1
#define SUPPLIES_DATA_LENGTH			64
#define CARTRIDGE_SERIAL_NUMBER_LENGTH	64
#define TONER_TYPE_LENGTH				4
#define REMAINING_TONER_LENGTH			2
#define WASTE_TONER_LENGTH				2
#define OPC_ROTATION_LENGTH				2
#define NUMBER_OF_PAGES_LENGTH          4
#define NUMBER_OF_PRINT_LENGTH			4
#define PRINT_DOTS_COUNT_LENGTH			2
#define PRINTER_NUM_OF_PAGES_LENGTH	    4
#define PRINTER_NUM_OF_PRINT_LENGTH	    4

using namespace std;

class CUtilPrinterStatus
{
public:
	CUtilPrinterStatus();

	//Printer status
    bool parseRecvBuf(LPBYTE pSendData, DWORD dwTotalLength);
    LPBYTE getPrinterStatusValue();
    std::vector<DWORD> m_vecError;
    std::vector<DWORD> m_vecWarning;

    BYTE m_pContainerID[CONTAINER_ID_LENGTH + 1];
    BYTE m_pContainerVersion[CONTAINER_VERSION_LENGTH + 1];
    BYTE m_pContainerLength[CONTAINER_SIZE_LENGTH + 1];
    BYTE m_pVenderNameFieldLen[VENDER_NAME_FIELD_SIZE_LENGTH + 1];
    BYTE m_pVenderNameValue[255 + 1];
	DWORD m_dwVenderNameFieldLen;
    BYTE m_pProductNameFieldLen[PRODUCT_NAME_FIELD_SIZE_LENGTH + 1];
    BYTE m_pProductNameValue[255 + 1];
	DWORD m_dwProductNameFieldLen;
    BYTE m_pSerialNumberFieldLen[SERIAL_NUMBER_FIALD_SIZE_LENGTH + 1];
    BYTE m_pSerialNumberValue[255 + 1];
	DWORD m_dwSerialNumberFieldLen;
    BYTE m_pErrorSummaryField[ERROR_SUMMARY_FIELD_SIZE_LENGTH + 1];
	BYTE m_pPrinterStatus[PRINTER_STATUS_LENGTH + 1];
    BYTE m_pJobNameFieldLen[JOB_NAME_FIELD_SIZE_LENGTH + 1];
    BYTE m_pJobNameValue[255 + 1];
    BYTE m_pNumberOfErr[NUMBER_OF_ERROR_LENGTH + 1];
    BYTE m_pErrorCode[ERROR_CODE_LENGTH + 1];
    BYTE m_pNumberOfWarning[NUMBER_OF_WARNING_LENGTH + 1];
    BYTE m_pWarningErrorCode[WARNING_ERROR_CODE_LENGTH + 1];
    DWORD m_dwJobNameFieldLen;
	DWORD m_dwNumberOfErr;
	DWORD m_dwNumberOfWarning;
	DWORD m_dwPrinterStatusCode;
	BYTE m_pFWVersion[255 + 1];
	BYTE m_pFWVersionFieldLen[PRINTER_FW_SIZE_LENGTH + 1];

	//Supplies info
	BYTE m_pSuppliesContainerID[CONTAINER_ID_LENGTH + 1];
	BYTE m_pSuppliesContainerVersion[CONTAINER_VERSION_LENGTH + 1];
	BYTE m_pSuppliesContainerLength[CONTAINER_SIZE_LENGTH + 1];
	BYTE m_pNumberOfSuppliesInfo[NUMBER_OF_SUPPLIES_INFO_LENGTH + 1];
	DWORD m_dwNumOfSuplliesInfo;
	BYTE m_pCartridgeSerialNumber[CARTRIDGE_SERIAL_NUMBER_LENGTH + 1];
	BYTE m_pTonerType[TONER_TYPE_LENGTH + 1];
	BYTE m_pRemainingToner[REMAINING_TONER_LENGTH + 1];	
	BYTE m_pWasteToner[WASTE_TONER_LENGTH + 1];	
	BYTE m_pOPCRotation[OPC_ROTATION_LENGTH + 1];
	BYTE m_pNumberOfPages[NUMBER_OF_PAGES_LENGTH + 1];
	BYTE m_pNumberOfPrint[NUMBER_OF_PRINT_LENGTH + 1];	
	BYTE m_pPrintDotsCount[PRINT_DOTS_COUNT_LENGTH + 1];	
	BYTE m_pPrinterNumOfPages[PRINTER_NUM_OF_PAGES_LENGTH + 1];	
	BYTE m_pPrinterNumOfPrint[PRINTER_NUM_OF_PRINT_LENGTH + 1];
	
	DWORD m_dwTonerTypeCode;
	DWORD m_dwRemainingToner;
	DWORD m_dwWasteToner;
	DWORD m_dwOPCRotation;
	DWORD m_dwNumberOfPages;
	DWORD m_dwNumberOfPrint;
	DWORD m_dwPrintDotsCount;
	DWORD m_dwPrinterNumOfPages;
	DWORD m_dwPrinterNumOfPrint;

private:
	void setSuppliesData(DWORD suppliesID, LPBYTE suppliesData, DWORD suppliesDataLen);
	DWORD byteToDWORD(LPBYTE pBuf, DWORD bufLength); 
	
};