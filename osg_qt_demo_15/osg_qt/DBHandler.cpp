
#include "DBHandler.h"
#include<iostream>
#include<iomanip>
#include<fstream>
using namespace std;

DBHandler * DBHandler::dbH_Instance =NULL;

DBHandler::DBHandler(){
	//初始化Com对象
		CoInitialize(NULL);
		try
		{
			std::ifstream infile("../config.ini",ios::in|ios::_Nocreate);//从配置文件中读入数据库配置信息
			if (!infile)
			{
				cerr<<"no config file ";//提示打开错误，退出
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

string DBHandler::get_dbMessage(string message,string table,string restrict)
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


string DBHandler::get_dbMessage(string messgage){
	pRs=pConn->Execute("Select * From Staff_Infor",0,adCmdText);
	string a=_bstr_t(pRs->GetCollect("Name"));

	return a;
}
DBHandler * DBHandler::Get_dbH_Instance()
{
	if (dbH_Instance==NULL)
	{
		dbH_Instance=new DBHandler();
	}
	return dbH_Instance;
}

bool DBHandler::get_dbMessage(int index,vector<map<string,string, MyCompRule>*>* v_map)
{
	if(pConn != NULL){
		try
		{
			if(index==0)
			{
				
				string cmd0="select * from V_T_RT_ZB_R";
				pRs=pConn->Execute(cmd0.c_str(),0,adCmdText);
				v_map->at(0)->at("高压侧电压")=_bstr_t(pRs->GetCollect("GDYAB"))+"KV";
			    v_map->at(0)->at("高压侧电流")=_bstr_t(pRs->GetCollect("GDLA"))+"A";
				v_map->at(0)->at("高压侧有功功率")=_bstr_t(pRs->GetCollect("YGGL"))+"KW";
				v_map->at(0)->at("高压侧无功功率")=_bstr_t(pRs->GetCollect("WGGL"))+"KW";
				v_map->at(0)->at("主变频率")=_bstr_t(pRs->GetCollect("PL"))+"HZ";
				v_map->at(0)->at("主变油温")=_bstr_t(pRs->GetCollect("ZBWD"))+"℃";
				v_map->at(0)->at("低压侧电压")=_bstr_t(pRs->GetCollect("DDYAB"))+"KV";
				v_map->at(0)->at("低压侧电流")=_bstr_t(pRs->GetCollect("DDLA"))+"A";
				v_map->at(0)->at("低压侧有功功率")=_bstr_t(pRs->GetCollect("DYGGL"))+"KW";
				v_map->at(0)->at("低压侧无功功率")=_bstr_t(pRs->GetCollect("DWGGL"))+"KW";

			}
			else if (index==1)
			{
				
				string cmd1="select * from V_T_RT_SSZ_R";
				string cmd2="select DDQSW from V_T_RT_DDZ_R";
				_bstr_t bstring1=cmd1.c_str();
				_bstr_t bstring2=cmd2.c_str();
				pRs=pConn->Execute(bstring1,0,adCmdText);
				pRs2=pConn->Execute(bstring2,0,adCmdText);

				v_map->at(0)->at("1#闸门开高")=_bstr_t(pRs->GetCollect("KG1"))+"m";
				v_map->at(0)->at("2#闸门开高")=_bstr_t(pRs->GetCollect("KG2"))+"m";
				v_map->at(0)->at("送水河水位")=_bstr_t(pRs->GetCollect("SSHSW"))+"m";
				v_map->at(0)->at("调度区水位")=_bstr_t(pRs2->GetCollect("DDQSW"))+"m";

				v_map->at(1)->at("3#闸门开高")=_bstr_t(pRs->GetCollect("KG3"))+"m";
				v_map->at(1)->at("送水河水位")=_bstr_t(pRs->GetCollect("SSHSW"))+"m";
				v_map->at(1)->at("调度区水位")=_bstr_t(pRs2->GetCollect("DDQSW"))+"m";		

			}

			else if(index==2)
			{
				int i=0;
				string cmd="select * from V_T_RT_UNT_R";
				_bstr_t bstring=cmd.c_str();//类型转换
				pRs=pConn->Execute(bstring,0,adCmdText);

				while(!pRs->EndOfFile)
				{
					v_map->at(i)->at("叶片角度")=_bstr_t(pRs->GetCollect("YPJD"))+"°";
					v_map->at(i)->at("电流")=_bstr_t(pRs->GetCollect("DZDLAB"))+"A";
					v_map->at(i)->at("励磁电流")=_bstr_t(pRs->GetCollect("LCDL"))+"A";
					v_map->at(i)->at("励磁电压")=_bstr_t(pRs->GetCollect("LCDY"))+"kv";
					v_map->at(i)->at("有功功率")=_bstr_t(pRs->GetCollect("YGGL"))+"KW";
					v_map->at(i)->at("无功功率")=_bstr_t(pRs->GetCollect("WGGL"))+"KW";
					v_map->at(i)->at("主机转速")=_bstr_t(pRs->GetCollect("ZS"))+"rpm";
					v_map->at(i)->at("闸上水位")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";
					v_map->at(i)->at("定子温度")=_bstr_t(pRs->GetCollect("DZWD1"))+"℃";
					v_map->at(i)->at("上导温度")=_bstr_t(pRs->GetCollect("SDWD1"))+"℃";
					v_map->at(i)->at("下导温度")=_bstr_t(pRs->GetCollect("SDWD3"))+"℃";
					v_map->at(i)->at("上油缸温度")=_bstr_t(pRs->GetCollect("SYGWD"))+"℃";
					v_map->at(i)->at("下油缸温度")=_bstr_t(pRs->GetCollect("XYGWD"))+"℃";
					v_map->at(i)->at("推力瓦温度")=_bstr_t(pRs->GetCollect("TLWWD1"))+"℃";
					v_map->at(i)->at("瞬时流量类型")=_bstr_t(pRs->GetCollect("YXGK"));
					v_map->at(i)->at("瞬时流量")=_bstr_t(pRs->GetCollect("LL"))+"m3/s";
					if (i<3)
					{
						v_map->at(i)->at("调度区水位")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";
					} 
					else
					{
						v_map->at(i)->at("闸下水位")=_bstr_t(pRs->GetCollect("CJSW"))+"m";
					}
					v_map->at(i+9)->at(("A门闸门高度"))=_bstr_t(pRs->GetCollect("AZM"))+"m";
					v_map->at(i+9)->at(("B门闸门高度"))=_bstr_t(pRs->GetCollect("BZM"))+"m";

					i++;
					pRs->MoveNext();
				}

			}
			else if (index==3)
			{
				string cmd3="select * from V_T_RT_WS_R";
				pRs=pConn->Execute(cmd3.c_str(),0,adCmdText);
				v_map->at(0)->at("1#供水母管压力")=_bstr_t(pRs->GetCollect("GSMG1"))+"MPa";
				v_map->at(0)->at("2#供水母管压力")=_bstr_t(pRs->GetCollect("GSMG2"))+"MPa";
				v_map->at(0)->at("1#供泵状态")=type_Inference(pRs->GetCollect("GSB1"));
				v_map->at(0)->at("2#供泵状态")=type_Inference(pRs->GetCollect("GSB2"));
				v_map->at(0)->at("3#供泵状态")=type_Inference(pRs->GetCollect("GSB3"));
			}
			else if (index==4)//调度闸//retVec[map(kaigao1,kaigao2),map(kaigao3, kaigao4)]
			{
				string cmd4="select * from V_T_RT_DDZ_R";
				_bstr_t bstring4=cmd4.c_str();//类型转换
				pRs=pConn->Execute(bstring4,0,adCmdText);

				v_map->at(0)->at("1#闸门开高")=_bstr_t(pRs->GetCollect("KG1"))+"m";
				v_map->at(0)->at("2#闸门开高")=_bstr_t(pRs->GetCollect("KG2"))+"m";
				v_map->at(0)->at("闸下水位")=_bstr_t(pRs->GetCollect("YJHSW"))+"m";
				v_map->at(0)->at("调度区水位")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";

				v_map->at(1)->at("3#闸门开高")=_bstr_t(pRs->GetCollect("KG3"))+"m";
				v_map->at(1)->at("4#闸门开高")=_bstr_t(pRs->GetCollect("KG4"))+"m";
				v_map->at(1)->at("闸下水位")=_bstr_t(pRs->GetCollect("YJHSW"))+"m";
				v_map->at(1)->at("调度区水位")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";

			}
			else if (index==5)
			{
				string cmd5="select * from V_T_RT_WGT_R";
				_bstr_t bstring5=cmd5.c_str();
				pRs=pConn->Execute(bstring5,0,adCmdText);

				v_map->at(0)->at("1#闸门开高")=_bstr_t(pRs->GetCollect("KG1"))+"m";
				v_map->at(0)->at("2#闸门开高")=_bstr_t(pRs->GetCollect("KG2"))+"m";
				v_map->at(0)->at("内河侧水位")=_bstr_t(pRs->GetCollect("SYSW"))+"m";
				v_map->at(0)->at("长江侧水位")=_bstr_t(pRs->GetCollect("XYSW"))+"m";
				v_map->at(0)->at("实际流量")=_bstr_t(pRs->GetCollect("SJLL"))+"m";
					
				v_map->at(1)->at("3#闸门开高")=_bstr_t(pRs->GetCollect("KG3"))+"m";
				v_map->at(1)->at("4#闸门开高")=_bstr_t(pRs->GetCollect("KG4"))+"m";
				v_map->at(1)->at("内河侧水位")=_bstr_t(pRs->GetCollect("SYSW"))+"m";
				v_map->at(1)->at("长江侧水位")=_bstr_t(pRs->GetCollect("XYSW"))+"m";
				v_map->at(1)->at("实际流量")=_bstr_t(pRs->GetCollect("SJLL"))+"m";

				v_map->at(2)->at("5#闸门开高")=_bstr_t(pRs->GetCollect("KG5"))+"m";
				v_map->at(2)->at("内河侧水位")=_bstr_t(pRs->GetCollect("SYSW"))+"m";
				v_map->at(2)->at("长江侧水位")=_bstr_t(pRs->GetCollect("XYSW"))+"m";
				v_map->at(2)->at("实际流量")=_bstr_t(pRs->GetCollect("SJLL"))+"m";

			}
			else if(index==6)
			{
				string cmd6="select * from V_T_RT_ZB_R";
				_bstr_t bstring6=cmd6.c_str();
				pRs=pConn->Execute(bstring6,0,adCmdText);

				v_map->at(0)->at("所变高压侧电压")=_bstr_t(pRs->GetCollect("SBGDYAB"))+"KV";
				v_map->at(0)->at("所变高压侧电流")=_bstr_t(pRs->GetCollect("SBGDLA"))+"A";
				v_map->at(0)->at("所变低压侧电压")=_bstr_t(pRs->GetCollect("SBDDYBC"))+"KV";
				v_map->at(0)->at("所变低压侧电流")=_bstr_t(pRs->GetCollect("SBDDLA"))+"A";

				v_map->at(1)->at("站变高压侧电压")=_bstr_t(pRs->GetCollect("DDYAB"))+"KV";	
				v_map->at(1)->at("站变高压侧电流")=_bstr_t(pRs->GetCollect("DDLA"))+"A";
				v_map->at(1)->at("站变低压侧电压")=_bstr_t(pRs->GetCollect("ZNBDDYAB"))+"KV";				
				v_map->at(1)->at("站变低压侧电流")=_bstr_t(pRs->GetCollect("ZNBDDLB"))+"A";

				v_map->at(2)->at("站变铁芯温度")=_bstr_t(pRs->GetCollect("ZNBWD"))+"℃";
			
				v_map->at(3)->at("所变铁芯温度")=_bstr_t(pRs->GetCollect("SBWD"))+"℃";
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

string DBHandler::type_Inference( _variant_t infor){
	string backInfor="";
	if (infor.intVal==0)
		backInfor="关";
	else if (infor.intVal==1)
		backInfor="开";
	else
		backInfor="未知";
	return backInfor;
}