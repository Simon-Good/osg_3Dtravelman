
#include "dbHandler.h"

dbHandler * dbHandler::dbH_Instance =NULL;

dbHandler::dbHandler(){
	//��ʼ��Com����
		CoInitialize(NULL);
		try
		{
			//��ʼ�����ݿ����Ӷ���
			 pConn=_ConnectionPtr("ADODB.Connection");
			//�����ݿ�����
			HRESULT rt = pConn->Open("Provider=SQLOLEDB;Server=PGOS;Database=YJH;uid=sa;pwd=123456","","",adConnectUnspecified);
			//��ʼ����¼������
			pRs=_RecordsetPtr("ADODB.Recordset");
			//cout<<"db return:"<<rt<<endl;
			////��ָ����¼��������
			//pRs->Open("select * from Staff_infor",_variant_t(pConn,true),adOpenStatic,adLockOptimistic,adCmdText);

			//	//���ʼ�¼��������
			//	while (!pRs->EndOfFile)
			//	{
			//		cout<<"ID"<<"  "<<_bstr_t(pRs->GetCollect("ID"))<<"\t";
			//		cout<<"Name"<<"  "<<_bstr_t(pRs->GetCollect("Name"))<<"\t";
			//		pRs->MoveNext();
			//	}
			/*}*/

			//�رռ�¼��
			//pRs->Close();
			//�ر����ݿ�����
			//pConn->Close();
			//alm=true;
		}
		catch(_com_error &e)
		{		
			QMessageBox::information(0,"����","���ݿ�����ʧ��");
			//pConn->Close();/*this line is not for release*/
			pConn = NULL;/*this line is not for release*/
			//exit(0);/*decomment when release*/
		}
}

string dbHandler::get_dbMessage(string message,string table,string restrict)
{
	//string cmd="Select "+message+" from "+table+" where ID ="+restrict;
	if(pConn != NULL){
		string cmd="Select * from v_T_RT_UNT_R";
		_bstr_t bstring=cmd.c_str();//����ת��
	
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
		string cmd="select * from v_T_RT_UNT_R_1";
		_bstr_t bstring=cmd.c_str();//����ת��

		string cmd2="select * from V_T_RT_YS_R1";
		_bstr_t bstring2=cmd2.c_str();
		//map<string,string> *m;
		pRs=pConn->Execute(bstring,0,adCmdText);
		pRs2=pConn->Execute(bstring2,0,adCmdText);
		int i=0;
		while(!pRs->EndOfFile)
		{
			if (index==2)
			{
				v_map->at(i)->at("ҶƬ�Ƕ�")=_bstr_t(pRs->GetCollect("YPJD"))+"��";
				v_map->at(i)->at("����")=_bstr_t(pRs->GetCollect("DZDLAB"))+"A";
				v_map->at(i)->at("���ŵ���")=_bstr_t(pRs->GetCollect("LCDL"))+"A";
				v_map->at(i)->at("�й�����")=_bstr_t(pRs->GetCollect("YGGL"))+"KW";
				v_map->at(i)->at("�޹�����")=_bstr_t(pRs->GetCollect("WGGL"))+"KW";
				v_map->at(i)->at("����ת��")=_bstr_t(pRs->GetCollect("ZS"))+"rpm";
				v_map->at(i)->at("բ��ˮλ")=_bstr_t(pRs->GetCollect("CJSW"))+"m";
				v_map->at(i)->at("բ��ˮλ")=_bstr_t(pRs->GetCollect("DDQSW"))+"m";
				v_map->at(i)->at("�����¶�")=_bstr_t(pRs->GetCollect("DZWD1"))+"��";
				v_map->at(i)->at("�ϵ��¶�")=_bstr_t(pRs->GetCollect("SDWD1"))+"��";
				v_map->at(i)->at("�µ��¶�")=_bstr_t(pRs->GetCollect("SDWD3"))+"��";
				v_map->at(i)->at("���͸��¶�")=_bstr_t(pRs->GetCollect("SYGWD"))+"��";
				v_map->at(i)->at("���͸��¶�")=_bstr_t(pRs->GetCollect("XYGWD"))+"��";
				v_map->at(i)->at("�������¶�")=_bstr_t(pRs->GetCollect("TLWWD1"))+"��";
				string KG="KG"+to_string((long long)(i+1));
				cout<<KG;

				_bstr_t kg=KG.c_str();
				v_map->at(i)->at(("բ�Ÿ߶�"))=_bstr_t(pRs2->GetCollect(kg))+"m";
			}
			i++;
			pRs->MoveNext();
		}





		return true;
	}else{
		return false;
	}
}