#include "pch.h"
#include "PrinterList.h"

PrinterList::PrinterList()
{
	printerCounts = 0;
}
PrinterList & PrinterList::getInstance()
{
	// TODO: 在此处插入 return 语句
	static PrinterList instance;
	return instance;
}

void PrinterList::push_back(Printer printer)
{
	printerList.push_back(printer);
	printerCounts++;
}

void PrinterList::deletePrinter(CString name)
{
	for (std::list<Printer>::const_iterator iter = printerList.begin(); iter != printerList.end(); iter++)
	{
		if (iter->name.Compare(name) == 0)
		{
			printerList.erase(iter);
			printerCounts--;
			break;
		}
	}
}

Printer PrinterList::findPrinterByName(CString name)
{
	Printer printer;
	for (std::list<Printer>::const_iterator iter = printerList.begin(); iter != printerList.end(); iter++)
	{
		if (iter->name.Compare(name) == 0)
		{
			printer = *iter;
			return printer;
		}
	}
	return printer;
}

bool PrinterList::checkIfRepeat(Printer printer)
{
	for (std::list<Printer>::const_iterator iter = printerList.begin(); iter != printerList.end(); iter++)
	{
		if (iter->name.Compare(printer.name) == 0)
			return true;
	}
	return false;
}
