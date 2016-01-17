
#include "DBHandler.h"
#include<iostream>
#include<iomanip>
#include<fstream>
using namespace std;

DBHandler * DBHandler::dbH_Instance =NULL;

DBHandler::DBHandler(){
	//��ʼ��Com����
		CoInitialize(NULL);
		try
		{
			std::ifstream infile("../config.ini",ios::in|ios::_Nocreate);//�������ļ��ж������ݿ�������Ϣ
			if (!infile)
			{
				cerr<<"no config file ";//��ʾ�򿪴����˳�
				exit(1);
			}
			char str[100];
			infile.getline(str,100);//��������
			string s(&str[0],&str[strlen(str)]);//����ת��
			infile.close();//�ر��ļ�
			_bstr_t cstr=s.c_str();//����ת��

			//��ʼ�����ݿ����Ӷ���
			 pConn=_ConnectionPtr("ADODB.Connection");
			//�����ݿ�����
			HRESULT rt = pConn->Open(cstr,"","",adConnectUnspecified);
			//��ʼ����¼������
			pRs=_RecordsetPtr("ADODB.Recordset");
			
		}
		catch(_com_error &e)
		{		
			QMessageBox::information(0,"����","���ݿ�����ʧ��");
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
		_bstr_t bstring=cmd.c_str();//����ת��
	
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
				v_map->at(0)->at("��ѹ���ѹ")=_bstr_t(pRs->GetCollect("GDYAB"))+"KV";
			    v_map->at(0)->at("��ѹ�����")=_bstr_t(pRs->GetCollect("GDLA"))+"A";
				v_map->at(0)->at("��ѹ���й�����")=_bstr_t(pRs->GetCollect("YGGL"))+"KW";
				v_map->at(0)->at("��ѹ���޹�����")=_bstr_t(pRs->GetCollect("WGGL"))+"KW";
				v_map->at(0)->at("����Ƶ��")=_bstr_t(pRs->GetCollect("PL"))+"HZ";
				v_map->at(0)->at("��������")=_bstr_t(pRs->GetCollect("ZBWD"))+"��";
				v_map->at(0)->at("��ѹ���ѹ")=_bstr_t(pRs->GetCollect("DDYAB"))+"KV";
				v_map->at(0)->at("��ѹ�����")=_bstr_t(pRs->GetCollect("DDLA"))+"A";
				v_map->at(0)->at("��ѹ���й�����")=_bstr_t(pRs->GetCollect("DYGGL"))+"KW";
				v_map->at(0)->at("��ѹ���޹�����")=_bstr_t(pRs->GetCollect("DWGGL"))+"KW";

			}
			else if (index==1)
			{
				
				string cmd1="select * from V_T_RT_SSZ_R";
				string cmd2="select DDQSW from V_T_RT_DDZ_R";
				_bstr_t bstring1=cmd1.c_str();
				_bstr_t bstring2=cmd2.c_str();
				pRs=pConn->Execute(bstring1,0,adCmdText);
				pRs2=pConn->Execute(bstring2,0,adCmdText);

				v_map->at(0)->at("1#բ�ſ���")=_bstr_t(pRs->GetCollect("KG1"))+"m";
				v_map->at(0)->at("2#բ�ſ���")=_bstr_t(pRs->GetCollect("KG2"))+"m";
				v_map->at(0)->at("��ˮ��ˮλ")=_bstr_t(pRs->GetCollect("SSHSW"))+"m";
				v_map->at(0)->at("������ˮλ")=_bstr_t(pRs2->GetCollect("DDQSW"))+"m";

				v_map->at(1)->at("3#բ�ſ���")=_bstr_t(pRs->GetCollect("KG3"))+"m";
				v_map->at(1)->at("��ˮ��ˮλ")=_bstr_t(pRs->GetCollect("SSHSW"))+"m";
				v_map->at(1)->at("������ˮλ")=_bstr_t(pRs2->GetCollect("DDQSW"))+"m";		

			}

			else if(index==2)
			{
				int i=0;
				string cmd="select * from V_T_RT_UNT_R";
				_bstr_t bstring=cmd.c_str();//����ת��
				pRs=pConn->Execute(bstring,0,adCmdText);

				while(!pRs->EndOfFile)
				{
					v_map->at(i)->at("ҶƬ�Ƕ�")=_bstr_t(pRs->GetCollect("YPJD"))+"��";
					v_map->at(i)->at("����")=_bstr_t(pRs->GetCollect("DZDLAB"))+"A";
					v_map->at(i)->at("���ŵ���")=_bstr_t(pRs->GetCollect("LCDL"))+"A";
					v_map->at(i)->at("���ŵ�ѹ")=_bstr_t(pRs->GetCollect("LCDY"))+"kv";
					v_map->at(i)->at("�й�����")=_bstr_t(pRs->GetCollect("YGGL"))+"KW";
					v_map->at(i)->at("�޹�����")=_bstr_t(pRs->GetCollect("WGGL"))+"KW";
					v_map->at(i)->at("����ת��")=_bstr_t(pRs->GetCollect("ZS"))+"rpm";
					v_map->at(i)->at("բ��ˮλ")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";
					v_map->at(i)->at("�����¶�")=_bstr_t(pRs->GetCollect("DZWD1"))+"��";
					v_map->at(i)->at("�ϵ��¶�")=_bstr_t(pRs->GetCollect("SDWD1"))+"��";
					v_map->at(i)->at("�µ��¶�")=_bstr_t(pRs->GetCollect("SDWD3"))+"��";
					v_map->at(i)->at("���͸��¶�")=_bstr_t(pRs->GetCollect("SYGWD"))+"��";
					v_map->at(i)->at("���͸��¶�")=_bstr_t(pRs->GetCollect("XYGWD"))+"��";
					v_map->at(i)->at("�������¶�")=_bstr_t(pRs->GetCollect("TLWWD1"))+"��";
					v_map->at(i)->at("˲ʱ��������")=_bstr_t(pRs->GetCollect("YXGK"));
					v_map->at(i)->at("˲ʱ����")=_bstr_t(pRs->GetCollect("LL"))+"m3/s";
					if (i<3)
					{
						v_map->at(i)->at("������ˮλ")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";
					} 
					else
					{
						v_map->at(i)->at("բ��ˮλ")=_bstr_t(pRs->GetCollect("CJSW"))+"m";
					}
					v_map->at(i+9)->at(("A��բ�Ÿ߶�"))=_bstr_t(pRs->GetCollect("AZM"))+"m";
					v_map->at(i+9)->at(("B��բ�Ÿ߶�"))=_bstr_t(pRs->GetCollect("BZM"))+"m";

					i++;
					pRs->MoveNext();
				}

			}
			else if (index==3)
			{
				string cmd3="select * from V_T_RT_WS_R";
				pRs=pConn->Execute(cmd3.c_str(),0,adCmdText);
				v_map->at(0)->at("1#��ˮĸ��ѹ��")=_bstr_t(pRs->GetCollect("GSMG1"))+"MPa";
				v_map->at(0)->at("2#��ˮĸ��ѹ��")=_bstr_t(pRs->GetCollect("GSMG2"))+"MPa";
				v_map->at(0)->at("1#����״̬")=type_Inference(pRs->GetCollect("GSB1"));
				v_map->at(0)->at("2#����״̬")=type_Inference(pRs->GetCollect("GSB2"));
				v_map->at(0)->at("3#����״̬")=type_Inference(pRs->GetCollect("GSB3"));
			}
			else if (index==4)//����բ//retVec[map(kaigao1,kaigao2),map(kaigao3, kaigao4)]
			{
				string cmd4="select * from V_T_RT_DDZ_R";
				_bstr_t bstring4=cmd4.c_str();//����ת��
				pRs=pConn->Execute(bstring4,0,adCmdText);

				v_map->at(0)->at("1#բ�ſ���")=_bstr_t(pRs->GetCollect("KG1"))+"m";
				v_map->at(0)->at("2#բ�ſ���")=_bstr_t(pRs->GetCollect("KG2"))+"m";
				v_map->at(0)->at("բ��ˮλ")=_bstr_t(pRs->GetCollect("YJHSW"))+"m";
				v_map->at(0)->at("������ˮλ")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";

				v_map->at(1)->at("3#բ�ſ���")=_bstr_t(pRs->GetCollect("KG3"))+"m";
				v_map->at(1)->at("4#բ�ſ���")=_bstr_t(pRs->GetCollect("KG4"))+"m";
				v_map->at(1)->at("բ��ˮλ")=_bstr_t(pRs->GetCollect("YJHSW"))+"m";
				v_map->at(1)->at("������ˮλ")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";

			}
			else if (index==5)
			{
				string cmd5="select * from V_T_RT_WGT_R";
				_bstr_t bstring5=cmd5.c_str();
				pRs=pConn->Execute(bstring5,0,adCmdText);

				v_map->at(0)->at("1#բ�ſ���")=_bstr_t(pRs->GetCollect("KG1"))+"m";
				v_map->at(0)->at("2#բ�ſ���")=_bstr_t(pRs->GetCollect("KG2"))+"m";
				v_map->at(0)->at("�ںӲ�ˮλ")=_bstr_t(pRs->GetCollect("SYSW"))+"m";
				v_map->at(0)->at("������ˮλ")=_bstr_t(pRs->GetCollect("XYSW"))+"m";
				v_map->at(0)->at("ʵ������")=_bstr_t(pRs->GetCollect("SJLL"))+"m";
					
				v_map->at(1)->at("3#բ�ſ���")=_bstr_t(pRs->GetCollect("KG3"))+"m";
				v_map->at(1)->at("4#բ�ſ���")=_bstr_t(pRs->GetCollect("KG4"))+"m";
				v_map->at(1)->at("�ںӲ�ˮλ")=_bstr_t(pRs->GetCollect("SYSW"))+"m";
				v_map->at(1)->at("������ˮλ")=_bstr_t(pRs->GetCollect("XYSW"))+"m";
				v_map->at(1)->at("ʵ������")=_bstr_t(pRs->GetCollect("SJLL"))+"m";

				v_map->at(2)->at("5#բ�ſ���")=_bstr_t(pRs->GetCollect("KG5"))+"m";
				v_map->at(2)->at("�ںӲ�ˮλ")=_bstr_t(pRs->GetCollect("SYSW"))+"m";
				v_map->at(2)->at("������ˮλ")=_bstr_t(pRs->GetCollect("XYSW"))+"m";
				v_map->at(2)->at("ʵ������")=_bstr_t(pRs->GetCollect("SJLL"))+"m";

			}
			else if(index==6)
			{
				string cmd6="select * from V_T_RT_ZB_R";
				_bstr_t bstring6=cmd6.c_str();
				pRs=pConn->Execute(bstring6,0,adCmdText);

				v_map->at(0)->at("�����ѹ���ѹ")=_bstr_t(pRs->GetCollect("SBGDYAB"))+"KV";
				v_map->at(0)->at("�����ѹ�����")=_bstr_t(pRs->GetCollect("SBGDLA"))+"A";
				v_map->at(0)->at("�����ѹ���ѹ")=_bstr_t(pRs->GetCollect("SBDDYBC"))+"KV";
				v_map->at(0)->at("�����ѹ�����")=_bstr_t(pRs->GetCollect("SBDDLA"))+"A";

				v_map->at(1)->at("վ���ѹ���ѹ")=_bstr_t(pRs->GetCollect("DDYAB"))+"KV";	
				v_map->at(1)->at("վ���ѹ�����")=_bstr_t(pRs->GetCollect("DDLA"))+"A";
				v_map->at(1)->at("վ���ѹ���ѹ")=_bstr_t(pRs->GetCollect("ZNBDDYAB"))+"KV";				
				v_map->at(1)->at("վ���ѹ�����")=_bstr_t(pRs->GetCollect("ZNBDDLB"))+"A";

				v_map->at(2)->at("վ����о�¶�")=_bstr_t(pRs->GetCollect("ZNBWD"))+"��";
			
				v_map->at(3)->at("������о�¶�")=_bstr_t(pRs->GetCollect("SBWD"))+"��";
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
		backInfor="��";
	else if (infor.intVal==1)
		backInfor="��";
	else
		backInfor="δ֪";
	return backInfor;
}