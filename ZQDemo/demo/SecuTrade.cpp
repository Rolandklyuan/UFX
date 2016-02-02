#include "SecuTrade.h"
unsigned long CTradeCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    return 0;
}

unsigned long CTradeCallback::AddRef()
{
    return 0;
}

unsigned long CTradeCallback::Release()
{
    return 0;
}


void CTradeCallback::OnConnect(CConnectionInterface *lpConnection)
{
    puts("CTradeCallback::OnConnect");
}

void CTradeCallback::OnSafeConnect(CConnectionInterface *lpConnection)
{
    puts("CTradeCallback::OnSafeConnect");
}

void CTradeCallback::OnRegister(CConnectionInterface *lpConnection)
{
    puts("CTradeCallback::OnRegister");
}
void CTradeCallback::OnClose(CConnectionInterface *lpConnection)
{
    puts("CTradeCallback::OnClose");
}

void CTradeCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
	puts("CTradeCallback::Onsent");
}

void CTradeCallback::Reserved1(void *a, void *b, void *c, void *d)
{
	puts("CTradeCallback::Reserved1");
}


void CTradeCallback::Reserved2(void *a, void *b, void *c, void *d)
{
	puts("CTradeCallback::Reserved2");
}

void CTradeCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
	puts("CTradeCallback::OnReceivedBizEx");
}
void CTradeCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
	puts("CTradeCallback::OnReceivedBizMsg");
	
	if (lpMsg!=NULL)
	{
		//�ɹ�,Ӧ�ó������ͷ�lpBizMessageRecv��Ϣ
		if (lpMsg->GetErrorNo() ==0)
		{
			 
			//���Ҫ����Ϣ�ŵ������̴߳����������п������������£�
			//int iMsgLen = 0;
			//void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
			//��lpMsgBuffer�����ߣ�Ȼ���������߳��лָ�����Ϣ�ɽ������²�����
			//lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
			//û�д�����Ϣ
			puts("ҵ������ɹ�");
			int iLen = 0;
			const void * lpBuffer = lpMsg->GetContent(iLen);
			IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
			cout<<"Function ID:"<<lpMsg->GetFunction()<<endl;
			lpMsg->GetFunction();
			switch(lpMsg->GetFunction())
			{
			case 331100:
			    puts("show user meaasge");
				ShowPacket(lpUnPacker);
				//puts("Get user_token and brnch_no");
				OnResponse_331100(lpUnPacker);
				break;
			/*case 330851:
				OnResponse_330851(lpUnPacker);
				break;*/
			/*case 338302:
				OnResponse_338302(lpUnPacker);
				break;*/
			default:
				ShowPacket(lpUnPacker);
				break;
			}
		}
		else
		{
			//�д�����Ϣ
			puts(lpMsg->GetErrorInfo());
		}
	}
}
void CTradeCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{

}

int  CTradeCallback::Reserved3()
{
    return 0;
}

void CTradeCallback::Reserved4()
{
}

void CTradeCallback::Reserved5()
{
}

void CTradeCallback::Reserved6()
{
}

void CTradeCallback::Reserved7()
{
}

void CTradeCallback::SetRequestMode(SecuRequestMode* lpMode)
{
	lpReqMode = lpMode; 
	  puts("CTradeCallback::SetRequestMode");
}

void CTradeCallback::OnResponse_331100(IF2UnPacker *lpUnPacker)
{   
	int iSystemNo=-1;
	puts("CTradeCallBack::331100");
	/*lpReqMode->m_BranchNo = lpUnPacker->GetInt("branch_no");
	cout<<lpReqMode->m_BranchNo;*/
	const char *pClientId = lpUnPacker->GetStr("client_id");
	if(pClientId)
	strcpy(lpReqMode->m_client_id, pClientId);
	//cout<<"clientId:"<<lpReqMode->m_client_id;
	if(lpUnPacker->GetStr("user_token") != NULL)
		{
			lpReqMode->m_opUserToken = lpUnPacker->GetStr("user_token");
	    }
	cout<<"UserToken:"<<lpUnPacker->GetStr("user_token")<<endl;
	if(lpUnPacker->GetStr("branch_no") != NULL)
		lpReqMode->m_BranchNo = lpUnPacker->GetInt("branch_no");
	//cout<<"BranchNo:"<<lpUnPacker->GetStr("branch_no")<<endl;
	iSystemNo = lpUnPacker->GetInt("sysnode_id");
	//cout<<"iSystemNo:"<<lpUnPacker->GetInt("sysnode_id")<<endl;
	if (lpUnPacker->GetInt("op_branch_no")!=NULL)
		lpReqMode->m_op_branch_no=lpUnPacker->GetInt("op_branch_no");
	    cout<<"op_branch_no:"<<lpReqMode->m_op_branch_no<<endl;

	return;
}

int SecuRequestMode::InitConn()
{   
	lpConfig->Load("t2sdk.ini");
	const char *p_fund_account = lpConfig->GetString("ufx", "fund_account", "");
	const char *p_password = lpConfig->GetString("ufx", "password", "");
	strcpy(m_AccountName,p_fund_account);
	strcpy(m_Password,p_password);
	m_EntrustWay='7';
	
	//�������Ӷ���
	//lpConfig->SetString("t2sdk", "servers", serverAddr);
	////cout<<"start checking license"<<endl;
	//lpConfig->SetString("t2sdk", "license_file", licFile);
	////cout<<"start loading clientname"<<endl;
	//lpConfig->SetString("t2sdk", "login_name", clientName);
	//cout<<"Connect Successful"<<endl;

	//�������ar������safe_level������Ҫ�����´���
	//begin
	//lpConfig->SetString("safe", "safe_level", "ssl");
	//lpConfig->SetString("safe", "cert_file", "c20121011.pfx");
	//lpConfig->SetString("safe", "cert_pwd", "111111");
	//end

	int iRet = 0;
		

	if(lpConnection != NULL)
	{
		lpConnection->Release();
		lpConnection = NULL;
	}
	
	lpConnection = NewConnection(lpConfig);
	//cout<<"Connected successfully"<<endl;
	lpConnection->AddRef();
	if (0 != (iRet = lpConnection->Create2BizMsg(&callback)))
    {
		cerr<<"��ʼ��ʧ��.iRet="<<iRet<<" msg:"<<lpConnection->GetErrorMsg(iRet)<<endl; 
		return -1;
	}
	if (0 != (iRet = lpConnection->Connect(0)))
	{
		cerr<<"����.iRet="<<iRet<<" msg:"<<lpConnection->GetErrorMsg(iRet)<<endl; 
		return -1;
	}

	


	return 0;
}

unsigned long SecuRequestMode::Release()
{
	delete this;
	return 0;
};

//331100֤ȯ�ͻ���½
int SecuRequestMode::ReqFunction331100()
{
	IBizMessage* lpBizMessage=NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetFunction(331100);
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	IF2Packer *pPacker=NewPacker(2);
	if (!pPacker)
	{
	  printf("ȡ�����ʧ�ܣ�\r\n");
	  return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///�����ֶ���
	//pPacker->AddField("op_branch_no", 'I', 5);//������֧����
	pPacker->AddField("op_entrust_way", 'C', 1);//ί�з�ʽ 
	pPacker->AddField("op_station", 'S', 255);//վ���ַ
	pPacker->AddField("branch_no", 'I', 5);     
	pPacker->AddField("input_content", 'C'); 
	pPacker->AddField("account_content", 'S', 30);
	pPacker->AddField("content_type", 'S', 6);  
	pPacker->AddField("password", 'S', 10);      
	pPacker->AddField("password_type", 'C');   
	///�����Ӧ���ֶ�ֵ
	//pPacker->AddInt( );						
	pPacker->AddChar(m_EntrustWay);				
	pPacker->AddStr(m_opStation.c_str());				
	pPacker->AddInt(1);			
	pPacker->AddChar('1');					
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr("0");	
	pPacker->AddStr(m_Password);			
	pPacker->AddChar(/*'2'*/'2');	
	
	///�������
	pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();

}

//400֤ȯ�����ѯ 
int SecuRequestMode::ReqFunction400(char* exchange_type, char* stock_code)
{   
	IF2Packer *pPacker=NewPacker(2);
	IBizMessage* lpBizMessage=NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetFunction(400);
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	
	if (!pPacker)
	{
	  printf("ȡ�����ʧ�ܣ�\r\n");
	  return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	//�����ֶ���
	pPacker->AddField("exchange_type",'S');
	pPacker->AddField("stock_code",'S');

    //�����ֶ�ֵ
	pPacker->AddStr(exchange_type);
	pPacker->AddStr(stock_code);

	pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}

//330300֤ȯ������Ϣ��ѯ
int SecuRequestMode::ReqFunction330300()
{  
	IF2Packer *pPacker=NewPacker(2);
	IBizMessage* lpBizMessage=NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetFunction(330300);
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	
	if (!pPacker)
	{
	  printf("ȡ�����ʧ�ܣ�\r\n");
	  return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);//������֧����
	pPacker->AddField("op_entrust_way", 'C', 1);//ί�з�ʽ 
	pPacker->AddField("op_station", 'S', 255);//վ���ַ
	pPacker->AddField("query_type",'C');
	pPacker->AddField("exchange_type",'S');
	pPacker->AddField("stock_type",'S');
	pPacker->AddField("stcok_code",'S');
	pPacker->AddField("position_str",'S');
	
	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_op_branch_no);						
	pPacker->AddChar(m_EntrustWay);				
	pPacker->AddStr(m_opStation.c_str());
	pPacker->AddChar('0');
	pPacker->AddStr("1");
	pPacker->AddStr("");
	pPacker->AddStr("600570");
	pPacker->AddStr(" ");
	
	
	///�������
	pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}

//333000֤ȯ��������ȷ��
int SecuRequestMode::ReqFunction333000(char* stock_code)
{
	
	IF2Packer *pPacker=NewPacker(2);
	IBizMessage* lpBizMessage=NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetFunction(333000);
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	
	if (!pPacker)
	{
	  printf("ȡ�����ʧ�ܣ�\r\n");
	  return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///�����ֶ���
    pPacker->AddField("op_branch_no", 'I', 5);//������֧����
	pPacker->AddField("op_entrust_way", 'C', 1);//ί�з�ʽ 
	pPacker->AddField("op_station", 'S');//վ���ַ
	pPacker->AddField("branch_no", 'I', 5);     
	//pPacker->AddField("client_id", 'C'); 
	pPacker->AddField("fund_account", 'S', 30);
	pPacker->AddField("password", 'S', 10);   
	//pPacker->AddField("user_token",'S',255);
	pPacker->AddField("entrust_prop",'S',3);
	pPacker->AddField("exchange_type",'S',4);
	pPacker->AddField("stock_account",'S',11);
	pPacker->AddField("stock_code",'S',16);

	//cout<<m_client_id<<endl;
	//�����ֶ�ֵ
	pPacker->AddInt(m_op_branch_no);
	pPacker->AddChar(m_EntrustWay);				
	pPacker->AddStr(m_opStation.c_str());				
    pPacker->AddInt(m_BranchNo);			
	//pPacker->AddStr(m_client_id);					
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr(m_Password);	
	
	//pPacker->AddStr(m_opUserToken.c_str());
	pPacker->AddStr("0");
	pPacker->AddStr(" ");
	pPacker->AddStr(" ");
	pPacker->AddStr(stock_code);

	pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
	
}
//333001 ��Լ�����ȡ
int SecuRequestMode::ReqFunction333001(char* exchange_type, char* stock_code,double entrust_price)
{
	int iRet = 0, hSend = 0;	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	//IF2UnPacker *pUnPacker = NULL;
	
	IBizMessage* lpBizMessage = NewBizMessage();

	lpBizMessage->AddRef();
	
	///Ӧ��ҵ����Ϣ
	IBizMessage* lpBizMessageRecv = NULL;
	//���ܺ�
	lpBizMessage->SetFunction(333001);
	//��������
	lpBizMessage->SetPacketType(REQUEST_PACKET);

	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("client_id", 'S', 18);//�ͻ�ID
	pPacker->AddField("fund_account", 'S', 18);//�ʽ��˺�
	pPacker->AddField("password", 'S', 10);
	pPacker->AddField("password_type", 'C', 1);	
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("stock_account", 'S', 15);
	pPacker->AddField("stock_code", 'S', 6);
	pPacker->AddField("entrust_price", 'F', 18, 3);

	//�����ֶ�ֵ
	pPacker->AddInt(m_op_branch_no);						
	pPacker->AddChar(m_EntrustWay);					
	pPacker->AddStr(m_opStation.c_str());					
	pPacker->AddInt(m_BranchNo);				
	pPacker->AddStr(m_client_id);			 
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr(m_Password);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_opUserToken.c_str());			
	pPacker->AddStr(exchange_type);					
	pPacker->AddStr("");
	pPacker->AddStr(stock_code);				
	pPacker->AddDouble(entrust_price);	

	pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();

}


//֤ȯ��ͨί��
int SecuRequestMode::ReqFunction333002(char* exchange_type,char* stock_code, double entrust_amount,double entrust_price,char entrust_bs)
{   
	int iRet = 0, hSend = 0;	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	//IF2UnPacker *pUnPacker = NULL;
	
	IBizMessage* lpBizMessage = NewBizMessage();

	lpBizMessage->AddRef();
	
	///Ӧ��ҵ����Ϣ
	IBizMessage* lpBizMessageRecv = NULL;
	//���ܺ�
	lpBizMessage->SetFunction(333002);
	//��������
	lpBizMessage->SetPacketType(REQUEST_PACKET);

	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("client_id", 'S', 18);//�ͻ�ID
	pPacker->AddField("fund_account", 'S', 18);//�ʽ��˺�
	pPacker->AddField("password", 'S', 10);
	pPacker->AddField("password_type", 'C', 1);	
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("stock_account", 'S', 15);
	pPacker->AddField("stock_code", 'S', 6);
	pPacker->AddField("entrust_amount", 'F', 19, 2);
	pPacker->AddField("entrust_price", 'F', 18, 3);
	pPacker->AddField("entrust_bs", 'C', 1);
	pPacker->AddField("entrust_prop", 'S', 3);	
	pPacker->AddField("batch_no", 'I', 8);
	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_op_branch_no);						
	pPacker->AddChar(m_EntrustWay);					
	pPacker->AddStr(m_opStation.c_str());					
	pPacker->AddInt(m_BranchNo);				
	pPacker->AddStr(m_client_id);			 
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr(m_Password);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_opUserToken.c_str());			
	pPacker->AddStr(exchange_type);					
	pPacker->AddStr("");
	pPacker->AddStr(stock_code);				
	pPacker->AddDouble(entrust_amount);
	pPacker->AddDouble(entrust_price);		
	pPacker->AddChar(entrust_bs);					
	pPacker->AddStr("0");					
	pPacker->AddInt(13);						
	
	pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}