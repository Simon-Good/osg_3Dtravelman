
#include "dbHandler.h"
#include<iostream>
#include<iomanip>
#include<fstream>
using namespace std;

dbHandler * dbHandler::dbH_Instance =NULL;

dbHandler::dbHandler(){
	//初始化Com对象
		CoInitialize(NULL);
		try
		{
			std::ifstream infile("./config.ini",ios::in|ios::_Nocreate);//从配置文件中读入数据库配置信息
			if (!infile)
			{
				cerr<<"open error";//提示打开错误，退出
				exit(1);
			}
			char str[100];
			infile.getline(str,100);//读入数据
			string s(&str[0],&str[strlen(str)]);//类型转换
			infile.close();//关闭文件
			_bstr_t cstr=s.c_str();//类型转换

			//初始化数据库连接对象
			 pConn=_ConnectionPtr("ADODB.Connection");
			//打开数据库连接
			HRESULT rt = pConn->Open(cstr,"","",adConnectUnspecified);
			//初始化记录集对象
			pRs=_RecordsetPtr("ADODB.Recordset");
			
		}
		catch(_com_error &e)
		{		
			QMessageBox::information(0,"警告","数据库连接失败");
			//pConn->Close();/*this line is not for release*/
			pConn = NULL;/*this line is not for release*/
			//exit(0);/*decomment when release*/
		}
}

string dbHandler::get_dbMessage(string message,string table,string restrict)
{
	//string cmd="Select "+message+" from "+table+" where ID ="+restrict;
	if(pConn != NULL){
		string cmd="Select * from V_T_RT_UNT_R";
		_bstr_t bstring=cmd.c_str();//类型转换
	
		pRs=pConn->Execute(bstring,0,adCmdText);
		string a;

		a=_bstr_t(pRs->GetCollect("Name"));
		return a;
	}else{
		return "";
	}
}


string dbHandler::get_dbMessage(string messgage){
	pRs=pConn->Execute("Select * From Staff_Infor",0,adCmdText);
	string a=_bstr_t(pRs->GetCollect("Name"));

	return a;
}
dbHandler * dbHandler::Get_dbH_Instance()
{
	if (dbH_Instance==NULL)
	{
		dbH_Instance=new dbHandler();
	}
	return dbH_Instance;
}

bool dbHandler::get_dbMessage(int index,vector<map<string,string>*>* v_map)
{
	if(pConn != NULL){
		try
		{
			string cmd="select * from V_T_RT_UNT_R_";
			_bstr_t bstring=cmd.c_str();//类型转换

			string cmd2="select * from V_T_RT_YS_R";
			_bstr_t bstring2=cmd2.c_str();
			//map<string,string> *m;
			pRs=pConn->Execute(bstring,0,adCmdText);
			pRs2=pConn->Execute(bstring2,0,adCmdText);
			int i=0;
			while(!pRs->EndOfFile)
			{
				if (index==2)
				{
					v_map->at(i)->at("叶片角度")=_bstr_t(pRs->GetCollect("YPJD"))+"°";
					v_map->at(i)->at("电流")=_bstr_t(pRs->GetCollect("DZDLAB"))+"A";
					v_map->at(i)->at("励磁电流")=_bstr_t(pRs->GetCollect("LCDL"))+"A";
					v_map->at(i)->at("有功功率")=_bstr_t(pRs->GetCollect("YGGL"))+"KW";
					v_map->at(i)->at("无功功率")=_bstr_t(pRs->GetCollect("WGGL"))+"KW";
					v_map->at(i)->at("主机转速")=_bstr_t(pRs->GetCollect("ZS"))+"rpm";
					v_map->at(i)->at("闸下水位")=_bstr_t(pRs->GetCollect("CJSW"))+"m";
					v_map->at(i)->at("闸上水位")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";
					v_map->at(i)->at("定子温度")=_bstr_t(pRs->GetCollect("DZWD1"))+"℃";
					v_map->at(i)->at("上导温度")=_bstr_t(pRs->GetCollect("SDWD1"))+"℃";
					v_map->at(i)->at("下导温度")=_bstr_t(pRs->GetCollect("SDWD3"))+"℃";
					v_map->at(i)->at("上油缸温度")=_bstr_t(pRs->GetCollect("SYGWD"))+"℃";
					v_map->at(i)->at("下油缸温度")=_bstr_t(pRs->GetCollect("XYGWD"))+"℃";
					v_map->at(i)->at("推力瓦温度")=_bstr_t(pRs->GetCollect("TLWWD1"))+"℃";
					string KG="KG"+to_string((long long)(i+1));
					//cout<<KG;

					_bstr_t kg=KG.c_str();
					v_map->at(i+9)->at(("闸门高度"))=_bstr_t(pRs2->GetCollect(kg))+"m";
					//cout<<v_map->at(i+9)->at("闸门高度")<<endl;
				}
				i++;
				pRs->MoveNext();
			}
			return true;
		}
		catch(_com_error &e)
		{	
			return false;
		}
	}else{
		return false;
	}
}