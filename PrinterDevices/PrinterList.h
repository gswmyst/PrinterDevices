#pragma once
#include <list>
#include "Printer.h"
class PrinterList
{
private:
	PrinterList();
public:
	std::list<Printer> printerList;
	PrinterList(const PrinterList&) = delete;
	PrinterList& operator=(const PrinterList&) = delete;
	static PrinterList& getInstance();
	void push_back(Printer printer);
	void deletePrinter(CString name);
	int printerCounts;
	Printer findPrinterByName(CString name);
	bool checkIfRepeat(Printer printer);
};

