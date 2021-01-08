#pragma once
class Printer
{
public:
	CString name;
	CString port;
	CString drivername;
	CString ip;
	enum TYPE
	{
		local,
		remote
	}type;
	Printer(CString name, CString port, CString drivername, CString ip, int type);
	Printer();
};

