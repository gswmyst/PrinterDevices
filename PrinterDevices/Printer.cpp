#include "pch.h"
#include "Printer.h"
Printer::Printer(CString name, CString port, CString drivername, CString ip, int type)
{
	this->drivername = drivername;
	this->ip = ip;
	switch (type)
	{
	case 0:
		this->type = local;
		break;
	case 1:
		this->type = remote;
		break;
	}
	this->name = name;
	this->port = port;
}

Printer::Printer()
{
}

