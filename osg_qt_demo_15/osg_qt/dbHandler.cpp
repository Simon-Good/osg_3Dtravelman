
#include "dbHandler.h"

dbHandler * dbHandler::dbH_Instance =NULL;

dbHandler::dbHandler(){
	//初始化Com对象
		CoInitialize(NULL);
		try
		{
			//初始化数据库连接对象
			 pConn=_ConnectionPtr("ADODB.Connection");
			//打开数据库连接
			HRESULT rt = pConn->Open("Provider=SQLOLEDB;Server=PGOS;Database=YJH;uid=sa;pwd=123456","","",adConnectUnspecified);
			//初始化记录集对象
			pRs=_RecordsetPtr("ADODB.Recordset");
			//cout<<"db return:"<<rt<<endl;
			////打开指定记录集中数据
			//pRs->Open("select * from Staff_infor",_variant_t(pConn,true),adOpenStatic,adLockOptimistic,adCmdText);

			//	//访问记录集中数据
			//	while (!pRs->EndOfFile)
			//	{
			//		cout<<"ID"<<"  "<<_bstr_t(pRs->GetCollect("ID"))<<"\t";
			//		cout<<"Name"<<"  "<<_bstr_t(pRs->GetCollect("Name"))<<"\t";
			//		pRs->MoveNext();
			//	}
			/*}*/

			//关闭记录集
			//pRs->Close();
			//关闭数据库连接
			//pConn->Close();
			//alm=true;
		}
		catch(_com_error &e)
		{		
			QMessageBox::information(0,"警告","数据库连接失败");
			exit(0);
		}
}

string dbHandler::get_dbMessage(string message,string table,string restrict)
{
	//string cmd="Select "+message+" from "+table+" where ID ="+restrict;
	string cmd="Select * from v_T_RT_UNT_R";
	_bstr_t bstring=cmd.c_str();//类型转换
	
	pRs=pConn->Execute(bstring,0,adCmdText);
	string a;

	a=_bstr_t(pRs->GetCollect("Name"));
	return a;
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

void dbHandler::get_dbMessage(int index,vector<map<string,string>*>* v_map)
{
	string cmd="select * from v_T_RT_UNT_R_1";
	_bstr_t bstring=cmd.c_str();//类型转换
	//map<string,string> *m;
	pRs=pConn->Execute(bstring,0,adCmdText);
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
			v_map->at(i)->at("下油缸温度")=_bstr_t(pRs->GetCollect("XYGWD")+"℃";
			v_map->at(i)->at("推力瓦温度")=_bstr_t(pRs->GetCollect("TLWWD1"))+"℃";
		}
		i++;
		pRs->MoveNext();
	}
}