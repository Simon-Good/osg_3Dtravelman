//#include "stdafx.h"
#include <icrsint.h>
#include<iostream>
#include<iomanip>
#include <string>
#include <QMessageBox>
#include <vector>
#include <map>
using namespace std;
#import "c:\program files\common files\system\ado\msado15.dll"  no_namespace rename("EOF","EndOfFile")//#import "c:\Program Files\Common Files\System\ADO\msado15.dll"  no_namespace rename("EOF", "EndOfFile")
class dbHandler{
private:
	dbHandler();
	~dbHandler();
	

public:
	string  get_dbMessage(string message,string table,string restrict);
	static dbHandler *Get_dbH_Instance();
	string get_dbMessage(string);
	void get_dbMessage(int index,vector<map<string,string>*>*);
public:
	_RecordsetPtr pRs;
	_ConnectionPtr pConn;
private:
	static dbHandler *dbH_Instance;
};