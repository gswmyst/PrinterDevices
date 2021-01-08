#include "pch.h"
#include "UtilPrinterStatus.h"

#define DEBUG_FW_VERSION

#define CHECK_DATA_LENGTH(index, length) \
		if(index > length) { return false; }

CUtilPrinterStatus::CUtilPrinterStatus()
{
	memset(m_pContainerID, 0, CONTAINER_ID_LENGTH + 1);
	memset(m_pContainerVersion, 0, CONTAINER_VERSION_LENGTH + 1);
	memset(m_pContainerLength, 0, CONTAINER_SIZE_LENGTH + 1);
	memset(m_pVenderNameFieldLen, 0, VENDER_NAME_FIELD_SIZE_LENGTH + 1);
	memset(m_pVenderNameValue, 0, 255 + 1);
	memset(m_pProductNameFieldLen, 0, PRODUCT_NAME_FIELD_SIZE_LENGTH + 1);
	memset(m_pProductNameValue, 0, 255 + 1);
	memset(m_pSerialNumberFieldLen, 0, SERIAL_NUMBER_FIALD_SIZE_LENGTH + 1);
	memset(m_pSerialNumberValue, 0, 255 + 1);
	memset(m_pErrorSummaryField, 0, ERROR_SUMMARY_FIELD_SIZE_LENGTH + 1);
	memset(m_pPrinterStatus, 0, PRINTER_STATUS_LENGTH + 1);
	memset(m_pJobNameFieldLen, 0, JOB_NAME_FIELD_SIZE_LENGTH + 1);
	memset(m_pJobNameValue, 0, 255 + 1);
	memset(m_pNumberOfErr, 0, NUMBER_OF_ERROR_LENGTH + 1);
	memset(m_pErrorCode, 0, ERROR_CODE_LENGTH + 1);
	memset(m_pNumberOfWarning, 0, NUMBER_OF_WARNING_LENGTH + 1);
	memset(m_pWarningErrorCode, 0, WARNING_ERROR_CODE_LENGTH + 1);
	memset(m_pFWVersionFieldLen, 0, PRINTER_FW_SIZE_LENGTH + 1);
	memset(m_pFWVersion, 0, 255 + 1);
	m_dwVenderNameFieldLen = 0;
	m_dwProductNameFieldLen = 0;
	m_dwSerialNumberFieldLen = 0;
	m_dwJobNameFieldLen = 0;
	m_dwPrinterStatusCode = 0;
	m_dwNumberOfErr = 0;
	m_dwNumberOfWarning = 0;
	m_vecError.clear();
	m_vecWarning.clear();

	memset(m_pSuppliesContainerID, 0, CONTAINER_ID_LENGTH + 1);
	memset(m_pSuppliesContainerVersion, 0, CONTAINER_VERSION_LENGTH + 1);
	memset(m_pSuppliesContainerLength, 0, CONTAINER_SIZE_LENGTH + 1);
	memset(m_pNumberOfSuppliesInfo, 0, NUMBER_OF_SUPPLIES_INFO_LENGTH + 1);
	memset(m_pCartridgeSerialNumber, 0, CARTRIDGE_SERIAL_NUMBER_LENGTH + 1);
	memset(m_pTonerType, 0, TONER_TYPE_LENGTH + 1);
	memset(m_pRemainingToner, 0, REMAINING_TONER_LENGTH + 1);
	memset(m_pWasteToner, 0, WASTE_TONER_LENGTH + 1);
	memset(m_pOPCRotation, 0, OPC_ROTATION_LENGTH + 1);
	memset(m_pNumberOfPrint, 0, NUMBER_OF_PRINT_LENGTH + 1);
	memset(m_pPrintDotsCount, 0, PRINT_DOTS_COUNT_LENGTH + 1);
	memset(m_pPrinterNumOfPages, 0, PRINTER_NUM_OF_PAGES_LENGTH + 1);
	memset(m_pPrinterNumOfPrint, 0, PRINTER_NUM_OF_PRINT_LENGTH + 1);
	m_dwTonerTypeCode = 0;
	m_dwNumOfSuplliesInfo = 0;
	m_dwRemainingToner = 0;
	m_dwWasteToner = 0;
	m_dwOPCRotation = 0;
	m_dwNumberOfPrint = 0;
	m_dwPrintDotsCount = 0;
	m_dwPrinterNumOfPages = 0;
	m_dwPrinterNumOfPrint = 0;
}

bool CUtilPrinterStatus::parseRecvBuf(LPBYTE pSendData, DWORD dwTotalLength)
{
	//data length
	//DWORD dwTotalLength = byteToDWORD(&pSendData[1], 2) + 2;

	DWORD dwTopIndex = 7;
	//Printer status
    /* Container ID */
	CHECK_DATA_LENGTH(dwTopIndex + CONTAINER_ID_LENGTH, dwTotalLength);
    memcpy(m_pContainerID, &pSendData[dwTopIndex], CONTAINER_ID_LENGTH);
    dwTopIndex += CONTAINER_ID_LENGTH;

    /* Container Version */
	CHECK_DATA_LENGTH(dwTopIndex + CONTAINER_VERSION_LENGTH, dwTotalLength);
    memcpy(m_pContainerVersion, &pSendData[dwTopIndex], CONTAINER_VERSION_LENGTH);
    dwTopIndex += CONTAINER_VERSION_LENGTH;

    /* ‚±‚ÌƒRƒ“ƒeƒi‚Ì’·‚³ */
	CHECK_DATA_LENGTH(dwTopIndex + CONTAINER_SIZE_LENGTH, dwTotalLength);
    memcpy(m_pContainerLength, &pSendData[dwTopIndex], CONTAINER_SIZE_LENGTH);
    dwTopIndex += CONTAINER_SIZE_LENGTH;

    /* Vendor Name Field ‚Ì’·‚³ */
	CHECK_DATA_LENGTH(dwTopIndex + VENDER_NAME_FIELD_SIZE_LENGTH, dwTotalLength);
    memcpy(m_pVenderNameFieldLen, &pSendData[dwTopIndex], VENDER_NAME_FIELD_SIZE_LENGTH);
    dwTopIndex += VENDER_NAME_FIELD_SIZE_LENGTH;
	m_dwVenderNameFieldLen = byteToDWORD(m_pVenderNameFieldLen, VENDER_NAME_FIELD_SIZE_LENGTH);
 
    /* Vendor Name */
	CHECK_DATA_LENGTH(dwTopIndex + m_dwVenderNameFieldLen, dwTotalLength);
    memcpy(m_pVenderNameValue, &pSendData[dwTopIndex], m_dwVenderNameFieldLen);
    dwTopIndex += m_dwVenderNameFieldLen;

    /* Product Name Field ‚Ì’·‚³ */
	CHECK_DATA_LENGTH(dwTopIndex + PRODUCT_NAME_FIELD_SIZE_LENGTH, dwTotalLength);
    memcpy(m_pProductNameFieldLen, &pSendData[dwTopIndex], PRODUCT_NAME_FIELD_SIZE_LENGTH);
    dwTopIndex += PRODUCT_NAME_FIELD_SIZE_LENGTH;
	m_dwProductNameFieldLen = byteToDWORD(m_pProductNameFieldLen, PRODUCT_NAME_FIELD_SIZE_LENGTH);

    /* Product Name */
	CHECK_DATA_LENGTH(dwTopIndex + m_dwProductNameFieldLen, dwTotalLength);
    memcpy(m_pProductNameValue, &pSendData[dwTopIndex], m_dwProductNameFieldLen);
    dwTopIndex += m_dwProductNameFieldLen;

    /* Serial Number Field ‚Ì’·‚³ */
	CHECK_DATA_LENGTH(dwTopIndex + SERIAL_NUMBER_FIALD_SIZE_LENGTH, dwTotalLength);
    memcpy(m_pSerialNumberFieldLen, &pSendData[dwTopIndex], SERIAL_NUMBER_FIALD_SIZE_LENGTH);
    dwTopIndex += SERIAL_NUMBER_FIALD_SIZE_LENGTH;
    m_dwSerialNumberFieldLen = byteToDWORD(m_pSerialNumberFieldLen, SERIAL_NUMBER_FIALD_SIZE_LENGTH);

    /* Serial Number */
	CHECK_DATA_LENGTH(dwTopIndex + m_dwSerialNumberFieldLen, dwTotalLength);
    memcpy(m_pSerialNumberValue, &pSendData[dwTopIndex], m_dwSerialNumberFieldLen);
    dwTopIndex += m_dwSerialNumberFieldLen;

    /* Error Summary Field */
	CHECK_DATA_LENGTH(dwTopIndex + ERROR_SUMMARY_FIELD_SIZE_LENGTH, dwTotalLength);
    memcpy(m_pErrorSummaryField, &pSendData[dwTopIndex], ERROR_SUMMARY_FIELD_SIZE_LENGTH);
    dwTopIndex += ERROR_SUMMARY_FIELD_SIZE_LENGTH;

    /* Printer Status */
	CHECK_DATA_LENGTH(dwTopIndex + PRINTER_STATUS_LENGTH, dwTotalLength);
    memcpy(m_pPrinterStatus, &pSendData[dwTopIndex], PRINTER_STATUS_LENGTH);
    dwTopIndex += PRINTER_STATUS_LENGTH;
	m_dwPrinterStatusCode = byteToDWORD(m_pPrinterStatus, PRINTER_STATUS_LENGTH);

#ifdef DEBUG_FW_VERSION
	/* FW Version */
	CHECK_DATA_LENGTH(dwTopIndex + PRINTER_FW_SIZE_LENGTH, dwTotalLength);
	memcpy(m_pFWVersionFieldLen, &pSendData[dwTopIndex], PRINTER_FW_SIZE_LENGTH);
	dwTopIndex += PRINTER_FW_SIZE_LENGTH;
	DWORD dwLength = byteToDWORD(m_pFWVersionFieldLen, PRINTER_FW_SIZE_LENGTH);

	CHECK_DATA_LENGTH(dwTopIndex + dwLength, dwTotalLength);
	memcpy(m_pFWVersion, &pSendData[dwTopIndex], dwLength);
	dwTopIndex += dwLength;
#endif // DEBUG_FW_VERSION

    /* Job Name Field ‚Ì’·‚³ */
	CHECK_DATA_LENGTH(dwTopIndex + JOB_NAME_FIELD_SIZE_LENGTH, dwTotalLength);
    memcpy(m_pJobNameFieldLen, &pSendData[dwTopIndex], JOB_NAME_FIELD_SIZE_LENGTH);
    dwTopIndex += JOB_NAME_FIELD_SIZE_LENGTH;

    /* Job Name Field */
	m_dwJobNameFieldLen = byteToDWORD(m_pJobNameFieldLen, JOB_NAME_FIELD_SIZE_LENGTH);
	CHECK_DATA_LENGTH(dwTopIndex + m_dwJobNameFieldLen, dwTotalLength);
    memcpy(m_pJobNameValue, &pSendData[dwTopIndex], m_dwJobNameFieldLen);
    dwTopIndex += m_dwJobNameFieldLen;

    /* Number Of Error */
	CHECK_DATA_LENGTH(dwTopIndex + NUMBER_OF_ERROR_LENGTH, dwTotalLength);
    memcpy(m_pNumberOfErr, &pSendData[dwTopIndex], NUMBER_OF_ERROR_LENGTH);
    dwTopIndex += NUMBER_OF_ERROR_LENGTH;
	m_dwNumberOfErr = byteToDWORD(m_pNumberOfErr, NUMBER_OF_ERROR_LENGTH);

    /* Error Code */
    for (DWORD i = 0; i < m_dwNumberOfErr; i++)
    {
		CHECK_DATA_LENGTH(dwTopIndex + ERROR_CODE_LENGTH, dwTotalLength);
        memcpy(m_pErrorCode, &pSendData[dwTopIndex], ERROR_CODE_LENGTH);
        m_vecError.push_back(byteToDWORD(m_pErrorCode, ERROR_CODE_LENGTH));
        dwTopIndex += ERROR_CODE_LENGTH;
    }

    /* Number Of WARNING */
	CHECK_DATA_LENGTH(dwTopIndex + NUMBER_OF_WARNING_LENGTH, dwTotalLength);
    memcpy(m_pNumberOfWarning, &pSendData[dwTopIndex], NUMBER_OF_WARNING_LENGTH);
    dwTopIndex += NUMBER_OF_WARNING_LENGTH;
	m_dwNumberOfWarning = byteToDWORD(m_pNumberOfWarning, NUMBER_OF_WARNING_LENGTH);

    /* Error Code(Warning) */
    for (DWORD i = 0; i < m_dwNumberOfWarning; i++)
    {
		CHECK_DATA_LENGTH(dwTopIndex + WARNING_ERROR_CODE_LENGTH, dwTotalLength);
        memcpy(m_pWarningErrorCode, &pSendData[dwTopIndex], WARNING_ERROR_CODE_LENGTH); 
		m_vecWarning.push_back(byteToDWORD(m_pWarningErrorCode, WARNING_ERROR_CODE_LENGTH));
        dwTopIndex += WARNING_ERROR_CODE_LENGTH;
    }

	//Supplies info
	DWORD dwSuppliesID = 0;
	DWORD dwSuppliesDataLength = 0;
	BYTE pSuppliesID[SUPPLIES_ID_LENGTH + 1];
	BYTE pSuppliesDataLength[SUPPLIES_DATA_SIZE_LENGTH + 1];
	BYTE pSuppliesDataValue[SUPPLIES_DATA_LENGTH + 1];
	memset(pSuppliesID, 0, SUPPLIES_ID_LENGTH + 1);
	memset(pSuppliesDataLength, 0, SUPPLIES_DATA_SIZE_LENGTH + 1);
	memset(pSuppliesDataValue, 0, SUPPLIES_DATA_LENGTH + 1);

	/* Container ID */
	CHECK_DATA_LENGTH(dwTopIndex + CONTAINER_ID_LENGTH, dwTotalLength);
	memcpy(m_pSuppliesContainerID, &pSendData[dwTopIndex], CONTAINER_ID_LENGTH);
	dwTopIndex += CONTAINER_ID_LENGTH;

	/* Container Version */
	CHECK_DATA_LENGTH(dwTopIndex + CONTAINER_VERSION_LENGTH, dwTotalLength);
	memcpy(m_pSuppliesContainerVersion, &pSendData[dwTopIndex], CONTAINER_VERSION_LENGTH);
	dwTopIndex += CONTAINER_VERSION_LENGTH;

	/* ¤³¤Î¥³¥ó¥Æ¥Ê¤ÎéL¤µ */
	CHECK_DATA_LENGTH(dwTopIndex + CONTAINER_SIZE_LENGTH, dwTotalLength);
	memcpy(m_pSuppliesContainerLength, &pSendData[dwTopIndex], CONTAINER_SIZE_LENGTH);
	dwTopIndex += CONTAINER_SIZE_LENGTH;

	/* ¤³¤Î¥³¥ó¥Æ¥Ê¤Ëº¬¤Þ¤ì¤ë Supplies Çéˆó¤ÎÊý */
	CHECK_DATA_LENGTH(dwTopIndex + NUMBER_OF_SUPPLIES_INFO_LENGTH, dwTotalLength);
	memcpy(m_pNumberOfSuppliesInfo, &pSendData[dwTopIndex], NUMBER_OF_SUPPLIES_INFO_LENGTH);
	dwTopIndex += NUMBER_OF_SUPPLIES_INFO_LENGTH;
	m_dwNumOfSuplliesInfo = byteToDWORD(m_pNumberOfSuppliesInfo, NUMBER_OF_SUPPLIES_INFO_LENGTH);

	for (DWORD i = 0; i < m_dwNumOfSuplliesInfo; i++)
	{
		/* Supplies ID */
		CHECK_DATA_LENGTH(dwTopIndex + SUPPLIES_ID_LENGTH, dwTotalLength);
		memcpy(pSuppliesID, &pSendData[dwTopIndex], SUPPLIES_ID_LENGTH);
		dwTopIndex += SUPPLIES_ID_LENGTH;
		dwSuppliesID = byteToDWORD(pSuppliesID, SUPPLIES_ID_LENGTH);

		/* Supplies Data ¤ÎéL¤µ */
		CHECK_DATA_LENGTH(dwTopIndex + SUPPLIES_DATA_SIZE_LENGTH, dwTotalLength);
		memcpy(pSuppliesDataLength, &pSendData[dwTopIndex], SUPPLIES_DATA_SIZE_LENGTH);
		dwTopIndex += SUPPLIES_DATA_SIZE_LENGTH;
		dwSuppliesDataLength = byteToDWORD(pSuppliesDataLength, SUPPLIES_DATA_SIZE_LENGTH);

		/* Supplies Data */
		CHECK_DATA_LENGTH(dwTopIndex + dwSuppliesDataLength, dwTotalLength);
		memcpy(pSuppliesDataValue, &pSendData[dwTopIndex], dwSuppliesDataLength);
		dwTopIndex += dwSuppliesDataLength;

		setSuppliesData(dwSuppliesID, pSuppliesDataValue, dwSuppliesDataLength);
	}
	return true;
}

LPBYTE CUtilPrinterStatus::getPrinterStatusValue()
{
    return m_pPrinterStatus;
}

void CUtilPrinterStatus::setSuppliesData(DWORD suppliesID, LPBYTE suppliesData, DWORD suppliesDataLen)
{
	switch (suppliesID)
	{
		/* Œ»ÝŽæ‚è•t‚¯‚ç‚ê‚Ä‚¢‚éƒgƒi[ƒJ[ƒgƒŠƒbƒW‚ÌƒVƒŠƒAƒ‹”Ô† */
	case 0x0000: memcpy(m_pCartridgeSerialNumber, suppliesData, suppliesDataLen);
		break;

		/* Œ»ÝŽæ‚è•t‚¯‚ç‚ê‚Ä‚¢‚éƒgƒi[ƒJ[ƒgƒŠƒbƒW‚Ìƒgƒi[ƒ^ƒCƒv */
		case 0x0001: memcpy(m_pTonerType, suppliesData, suppliesDataLen);
			m_dwTonerTypeCode = byteToDWORD(suppliesData, suppliesDataLen);
		break;

		/* Œ»ÝŽæ‚è•t‚¯‚ç‚ê‚Ä‚¢‚éƒgƒi[ƒJ[ƒgƒŠƒbƒW‚Ìƒgƒi[Žc—Ê */
	case 0x1000: memcpy(m_pRemainingToner, suppliesData, suppliesDataLen);
			m_dwRemainingToner = byteToDWORD(suppliesData, suppliesDataLen);
		break;

		/* Œ»ÝŽæ‚è•t‚¯‚ç‚ê‚Ä‚¢‚éƒgƒi[ƒJ[ƒgƒŠƒbƒW‚Ì”pƒgƒi[—Ê */
	case 0x1001: memcpy(m_pWasteToner, suppliesData, suppliesDataLen);
			m_dwWasteToner = byteToDWORD(suppliesData, suppliesDataLen);
		break;

		/* Œ»ÝŽæ‚è•t‚¯‚ç‚ê‚Ä‚¢‚éƒgƒi[ƒJ[ƒgƒŠƒbƒW‚ÌOPC‰ñ“]” */
	case 0x1002: memcpy(m_pOPCRotation, suppliesData, suppliesDataLen);
		m_dwOPCRotation = byteToDWORD(suppliesData, suppliesDataLen);
		break;

		/* Œ»ÝŽæ‚è•t‚¯‚ç‚ê‚Ä‚¢‚éƒgƒi[ƒJ[ƒgƒŠƒbƒW‚ÌŽcˆóü‹–—e–Ê” */
		case 0x1003: memcpy(m_pNumberOfPages, suppliesData, suppliesDataLen);
		m_dwNumberOfPages = byteToDWORD(suppliesData, suppliesDataLen);
		/* break; */

		/* Œ»ÝŽæ‚è•t‚¯‚ç‚ê‚Ä‚¢‚éƒgƒi[ƒJ[ƒgƒŠƒbƒW‚Ìˆóü–‡” */
	case 0x1004: memcpy(m_pNumberOfPrint, suppliesData, suppliesDataLen);
		m_dwNumberOfPrint = byteToDWORD(suppliesData, suppliesDataLen);
		break;

		/* ƒvƒŠƒ“ƒ^–{‘Ì‚ÌƒhƒbƒgƒJƒEƒ“ƒg */
	case 0x1100: memcpy(m_pPrintDotsCount, suppliesData, suppliesDataLen);
		m_dwPrintDotsCount = byteToDWORD(suppliesData, suppliesDataLen);
		break;

		/* ƒvƒŠƒ“ƒ^–{‘Ì‚ÌŽcˆóü‹–—e–Ê” */
	case 0x1101: memcpy(m_pPrinterNumOfPages, suppliesData, suppliesDataLen);
		m_dwPrinterNumOfPages = byteToDWORD(suppliesData, suppliesDataLen);
		break;

		/* ƒvƒŠƒ“ƒ^–{‘Ì‚Ì‹–—e–‡”*/
	case 0x1102: memcpy(m_pPrinterNumOfPrint, suppliesData, suppliesDataLen);
		m_dwPrinterNumOfPrint = byteToDWORD(suppliesData, suppliesDataLen);
		break;

	default:
		break;
	}
}

DWORD CUtilPrinterStatus::byteToDWORD(LPBYTE pBuf, DWORD bufLength)
{
	DWORD dwRet = 0;
	DWORD dwFirstByte = 0;
	DWORD dwSecondByte = 0;
	DWORD dwThirdByte = 0;
	DWORD dwFourthByte = 0;

	memcpy(&dwFirstByte, &pBuf[0], 1);


	if (bufLength == 4)
	{
		memcpy(&dwSecondByte, &pBuf[1], 1);
		memcpy(&dwThirdByte, &pBuf[2], 1);
		memcpy(&dwFourthByte, &pBuf[3], 1);

		dwRet = (dwFirstByte * 0x1000000) + (dwSecondByte * 0x10000) + (dwThirdByte * 0x100) + dwFourthByte;
	}
	else if (bufLength == 3)
	{
		memcpy(&dwSecondByte, &pBuf[1], 1);
		memcpy(&dwThirdByte, &pBuf[2], 1);
		dwRet = (dwFirstByte * 0x10000) + (dwSecondByte * 0x100) + dwThirdByte;
	}
	else if (bufLength == 2)
	{
		memcpy(&dwSecondByte, &pBuf[1], 1);
		dwRet = (dwFirstByte * 0x100) + dwSecondByte;
	}
	else
	{
		dwRet = dwFirstByte;
	}

	return dwRet;
}
