#include <iostream>
#include <stdio.h>
#include "demo.h"
////////////////////////////////

//int g_iSystemNo = -1;
//bool g_bLogin = false;
//char g_cUserToken[USERTOKENLEN] = {0};
//��ӡӦ���
void ShowPacket(int iIssueType, IF2UnPacker *pUnPacker)
{
    int i = 0, t = 0, j = 0, k = 0;

    for (i = 0; i < pUnPacker->GetDatasetCount(); ++i)
    {
        // ���õ�ǰ�����
        pUnPacker->SetCurrentDatasetByIndex(i);

        // ��ӡ�ֶ�
        for (t = 0; t < pUnPacker->GetColCount(); ++t)
        {
            printf("%20s", pUnPacker->GetColName(t));
        }

        putchar('\n');

        // ��ӡ���м�¼
        for (j = 0; j < (int)pUnPacker->GetRowCount(); ++j)
        {
            // ��ӡÿ����¼
            for (k = 0; k < pUnPacker->GetColCount(); ++k)
            {
                switch (pUnPacker->GetColType(k))
                {
                    case 'I':
                        printf("%20d", pUnPacker->GetIntByIndex(k));
                        break;

                    case 'C':
                        printf("%20c", pUnPacker->GetCharByIndex(k));
                        break;

                    case 'S':
                        printf("%20s", pUnPacker->GetStrByIndex(k));
                        break;

                    case 'F':
                        printf("%20f", pUnPacker->GetDoubleByIndex(k));
                        break;

                    case 'R':
                        {
                            break;
                        }				
                    default:
                        // δ֪��������
                        printf("δ֪�������͡�\n");
                        break;
                }
            }		
            putchar('\n');		
            pUnPacker->Next();
        }
        putchar('\n');
    }
}

/////////////////////////////////////////////////////////////
//CBusiness ʵ��
CBusiness::CBusiness()
{
    memset(m_cUserToken, 0, sizeof(m_cUserToken));
}

CBusiness::~CBusiness()
{

}

//��¼
int CBusiness::Login()
{
    int hSend = 0, iSystemNo = -1;


    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

    //���ܺ�
    lpBizMessage->SetFunction(331100);
    //��������
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    //����Ӫҵ����
    lpBizMessage->SetBranchNo(1);
    //����company_id
    lpBizMessage->SetCompanyID(91000);
    //����SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);

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

    ///������Ӧ����Ϣ
    //LPRET_DATA pRetData = NULL;
    ///��ʼ���
    pPacker->BeginPack();

    ///�����ֶ���
    pPacker->AddField("op_branch_no", 'I', 5);//������֧����
    pPacker->AddField("op_entrust_way", 'C', 1);//ί�з�ʽ 
    pPacker->AddField("op_station", 'S', 255);//վ���ַ
    pPacker->AddField("branch_no", 'I', 5);     
    pPacker->AddField("input_content", 'C'); 
    pPacker->AddField("account_content", 'S', 30);
    pPacker->AddField("content_type", 'S', 6);  
    pPacker->AddField("password", 'S', 10);      
    pPacker->AddField("password_type", 'C');   

    ///�����Ӧ���ֶ�ֵ
    pPacker->AddInt(0);			//op_branch_no			
    pPacker->AddChar('7');		//	op_entrust_way	
    pPacker->AddStr("hs");	//		op_station	
    pPacker->AddInt(1);		//	branch_no
    pPacker->AddChar('1');		//input_content			
    pPacker->AddStr("70001172");	//account_content		
    pPacker->AddStr("0");	//content_type
    pPacker->AddStr("111111");		//password	
    pPacker->AddChar('1');	//password_type

    ///�������
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    ///ͬ�����͵�¼����Ӧ����RecvBizEx�д���
    /*(hSend = g_pConnection->SendBizEx(331100,pPacker,NULL,SYNCSEND,0,0,1,NULL)*/
    if((hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
    {
        printf("���͹���331100ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        goto EXIT;
    }

    printf("���͹���331100�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);



    //iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
    //int a;  int &ra=a;  //��������ra,���Ǳ���a�����ã�������

    hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
    if(hSend != 0)
    {
        printf("���չ���331100ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        goto EXIT;
    }else{

        int iReturnCode = lpBizMessageRecv->GetReturnCode();
        if(iReturnCode!=0) //����
        {
            printf("���չ���331100ʧ��,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo());

        }
        else if(iReturnCode==0) // ��ȷ
        {
            //���Ҫ����Ϣ�ŵ������̴߳����������п������������£�
            //int iMsgLen = 0;
            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
            //��lpMsgBuffer�����ߣ�Ȼ���������߳��лָ�����Ϣ�ɽ������²�����
            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
            //û�д�����Ϣ
            puts("ҵ������ɹ�");
            int iLen = 0;
            //���հ���       "��ͷ|������|ǩ��|����";   ���硰12|331100|XyIxtt..|username=sundsun,password=123456,��;
            const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
            //
            IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            //����ϵͳ��
            iSystemNo = lpUnPacker->GetInt("sysnode_id");
            printf("��֧�����ţ�"+iSystemNo);
            //��֧����
            m_branch_no = lpUnPacker->GetInt("branch_no");
            const char *pUserToken = lpUnPacker->GetStr("user_token");
            if(pUserToken){
                strcpy(m_cUserToken, pUserToken);
                //printf("\r\nuser_token[%s]\r\n",m_cUserToken);
            }
            //�ͻ����
            pUserToken = lpUnPacker->GetStr("client_id");
            if(pUserToken){
                strcpy(m_client_id, pUserToken);
                //printf("\r\nclient_id[%s]\r\n",m_client_id);
            }
            //�ʲ��˻�
            pUserToken = lpUnPacker->GetStr("fund_account");
            if(pUserToken){
                strcpy(m_fund_account, pUserToken);
                //printf("\r\nfund_account[%s]\r\n",m_fund_account);
            }
            ShowPacket(0,lpUnPacker);
        }
    }

EXIT:
    ///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
    if(pPacker)
    {
        pPacker->FreeMem(pPacker->GetPackBuf());
        ///�ͷ������pack
        pPacker->Release();
    }
    if(lpBizMessage){
        lpBizMessage->Release();
    }
    return iSystemNo;
}

//֤ȯί�� 333002
int CBusiness::SecuEntrust(int iSystemNo)
{

    int hSend = 0;

    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

    //���ܺ�
    lpBizMessage->SetFunction(333002);
    //��������
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    //����Ӫҵ����
    lpBizMessage->SetBranchNo(1);
    //����company_id
    lpBizMessage->SetCompanyID(91000);
    //����SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
    //����ϵͳ��
    lpBizMessage->SetSystemNo(iSystemNo);



    ///��ȡ�汾Ϊ2���͵�pack�����
    IF2Packer *pPacker = NewPacker(2);
    if(!pPacker)
    {
        printf("ȡ�����ʧ��!\r\n");
        return -1;
    }
    pPacker->AddRef();

    ///��ʼ���
    pPacker->BeginPack();

    ///�����ֶ���
    pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 
    pPacker->AddField("client_id", 'S', 18);
    pPacker->AddField("fund_account", 'S', 18);
    pPacker->AddField("password", 'S', 50);
    pPacker->AddField("password_type", 'C', 1); 
    pPacker->AddField("user_token", 'S', 512);
    pPacker->AddField("exchange_type", 'S', 4);
    pPacker->AddField("stock_account", 'S', 11);
    pPacker->AddField("stock_code", 'S', 16);
    pPacker->AddField("entrust_amount", 'F', 19, 2);
    pPacker->AddField("entrust_price", 'F', 18, 3);
    pPacker->AddField("entrust_bs", 'C', 1);
    pPacker->AddField("entrust_prop", 'S', 3);	
    pPacker->AddField("batch_no", 'I', 8);


    ///�����Ӧ���ֶ�ֵ
    pPacker->AddInt(0);		//op_branch_no				
    pPacker->AddChar('7');		//op_entrust_way			
    pPacker->AddStr("1");	//	op_station			
    pPacker->AddInt(m_branch_no);//branch_no
    pPacker->AddStr(m_client_id);	//client_id		
    pPacker->AddStr(m_fund_account);	//fund_account		
    pPacker->AddStr("111111");		//password		
    pPacker->AddChar('2');			//password_type		
    pPacker->AddStr(m_cUserToken); //user_token
    pPacker->AddStr("1");		//exchange_type			
    pPacker->AddStr("A070000172");	//stock_account		
    pPacker->AddStr("600570");		//stock_code		
    pPacker->AddDouble(100);		//entrust_amount		
    pPacker->AddDouble(39.07); 	//entrust_price		
    pPacker->AddChar('1');		//	entrust_bs		
    pPacker->AddStr("0");		//	entrust_prop		
    pPacker->AddInt(13);//batch_no



    ///�������
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());



    ///ͬ�������ʽ��ѯ����Ӧ����RecvBizEx�д���
    if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
    {
        printf("���͹���333002ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        hSend=-2;
        goto EXIT;
    }

    printf("���͹���333002�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);

    //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
    hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
    if(hSend != 0)
    {
        printf("���չ���333002ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        goto EXIT;
    }else{
        int iReturnCode = lpBizMessageRecv->GetReturnCode();
        if(iReturnCode!= 0) //����
        {
            printf("���չ���333002ʧ��,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetReturnCode(),lpBizMessageRecv->GetErrorInfo()); 			
        }
        else if(iReturnCode==0) // ��ȷ
        {
            //���Ҫ����Ϣ�ŵ������̴߳����������п������������£�
            //int iMsgLen = 0;
            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
            //��lpMsgBuffer�����ߣ�Ȼ���������߳��лָ�����Ϣ�ɽ������²�����
            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
            //û�д�����Ϣ
            puts("ҵ������ɹ�");
            int iLen = 0;
            const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
            IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            ShowPacket(0,lpUnPacker);
        }

    }

EXIT:
    ///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
    if(pPacker)
    {
        pPacker->FreeMem(pPacker->GetPackBuf());
        ///�ͷ������pack
        pPacker->Release();
    }
    if(lpBizMessage){
        lpBizMessage->Release();
    }

    return hSend;
}









//֤ȯ�ɶ���Ϣ��ѯ 331300
int CBusiness::ClientStkacctQry(int iSystemNo)
{

    int hSend = 0;

    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

    //���ܺ�
    lpBizMessage->SetFunction(331300);
    //��������
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    //����Ӫҵ����
    lpBizMessage->SetBranchNo(1);
    lpBizMessage->SetCompanyID(91000);
    //����SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
    //����ϵͳ��
    lpBizMessage->SetSystemNo(iSystemNo);



    ///��ȡ�汾Ϊ2���͵�pack�����
    IF2Packer *pPacker = NewPacker(2);
    if(!pPacker)
    {
        printf("ȡ�����ʧ��!\r\n");
        return -1;
    }
    pPacker->AddRef();

    ///��ʼ���
    pPacker->BeginPack();

    ///�����ֶ���
    pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 
    pPacker->AddField("client_id", 'S', 18);
    pPacker->AddField("fund_account", 'S', 18);
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("password_type", 'C', 1); 
    pPacker->AddField("user_token", 'S', 512);
    pPacker->AddField("exchange_type", 'S', 4);



    ///�����Ӧ���ֶ�ֵ
    pPacker->AddInt(0);			//op_branch_no			
    pPacker->AddChar('7');			//	op_entrust_way	
    pPacker->AddStr("hs");	//op_station				
    pPacker->AddInt(m_branch_no);		//	branch_no	
    pPacker->AddStr(m_client_id);	//		client_id
    pPacker->AddStr(m_fund_account);	//	fund_account	
    pPacker->AddStr("111111");		//password		
    pPacker->AddChar('2');			//	password_type	
    pPacker->AddStr(m_cUserToken);	//	user_token	
    pPacker->AddStr("1");			//exchange_type		



    ///�������
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());



    ///ͬ�������ʽ��ѯ����Ӧ����RecvBizEx�д���
    if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
    {
        printf("���͹���331300ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        hSend=-2;
        goto EXIT;
    }

    printf("���͹���333002�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);

    //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
    hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
    if(hSend != 0)
    {
        printf("���չ���331300ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        goto EXIT;
    }else{
        int iReturnCode = lpBizMessageRecv->GetReturnCode();
        if(iReturnCode!= 0) //����
        {
            printf("���չ���331300ʧ��,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo()); 			
        }
        else if(iReturnCode==0) // ��ȷ
        {
            //���Ҫ����Ϣ�ŵ������̴߳����������п������������£�
            //int iMsgLen = 0;
            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
            //��lpMsgBuffer�����ߣ�Ȼ���������߳��лָ�����Ϣ�ɽ������²�����
            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
            //û�д�����Ϣ
            puts("ҵ������ɹ�");
            int iLen = 0;
            const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
            IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            ShowPacket(0,lpUnPacker);
        }

    }

EXIT:
    ///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
    if(pPacker)
    {
        pPacker->FreeMem(pPacker->GetPackBuf());
        ///�ͷ������pack
        pPacker->Release();
    }
    if(lpBizMessage){
        lpBizMessage->Release();
    }

    return hSend;


}


//�ֲֲ�ѯ 333104
int CBusiness::SecuStockQry(int iSystemNo /* = 2 */)
{

    printf("--------------------------->m_cUserToken[%s]\n",m_cUserToken);
    int hSend = 0;

    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

    //���ܺ�
    lpBizMessage->SetFunction(333104);
    //��������
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    //����Ӫҵ����
    lpBizMessage->SetBranchNo(1);
    lpBizMessage->SetCompanyID(91000);
    //����SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
    //����ϵͳ��
    lpBizMessage->SetSystemNo(iSystemNo);



    ///��ȡ�汾Ϊ2���͵�pack�����
    IF2Packer *pPacker = NewPacker(2);
    if(!pPacker)
    {
        printf("ȡ�����ʧ��!\r\n");
        return -1;
    }
    pPacker->AddRef();

    ///��ʼ���
    pPacker->BeginPack();

    ///�����ֶ���
    pPacker->AddField("op_branch_no", 'I', 5);
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 		
    pPacker->AddField("client_id", 'S', 18);			
    pPacker->AddField("fund_account", 'S', 18); 	
    pPacker->AddField("password", 'S', 10); 					
    pPacker->AddField("password_type", 'C', 1); 	
    pPacker->AddField("user_token", 'S', 512);		

    ///�����Ӧ���ֶ�ֵ
    pPacker->AddInt(0); 			//	op_branch_no	
    pPacker->AddChar('7');			//		op_entrust_way
    pPacker->AddStr("hs");		//op_station
    pPacker->AddInt(m_branch_no);	//	branch_no		
    pPacker->AddStr(m_client_id);	//		client_id
    pPacker->AddStr(m_fund_account);	//	fund_account	
    pPacker->AddStr("111111");		//	password	
    pPacker->AddChar('1');				//	password_type
    pPacker->AddStr(m_cUserToken);	//user_token


    ///�������
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());



    ///ͬ�������ʽ��ѯ����Ӧ����RecvBizEx�д���
    if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
    {
        printf("���͹���333104ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        hSend=-2;
        goto EXIT;
    }

    printf("���͹���333104�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);

    //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
    hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
    if(hSend != 0)
    {
        printf("���չ���333104ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        goto EXIT;
    }else{
        int iReturnCode = lpBizMessageRecv->GetReturnCode();
        if(iReturnCode!= 0) //����
        {
            printf("���չ���333104ʧ��,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo()); 			
        }
        else if(iReturnCode==0) // ��ȷ
        {
            //���Ҫ����Ϣ�ŵ������̴߳����������п������������£�
            //int iMsgLen = 0;
            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
            //��lpMsgBuffer�����ߣ�Ȼ���������߳��лָ�����Ϣ�ɽ������²�����
            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
            //û�д�����Ϣ
            puts("ҵ������ɹ�");
            int iLen = 0;
            const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
            IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            ShowPacket(0,lpUnPacker);
        }

    }

EXIT:
    ///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
    if(pPacker)
    {
        pPacker->FreeMem(pPacker->GetPackBuf());
        ///�ͷ������pack
        pPacker->Release();
    }
    if(lpBizMessage){
        lpBizMessage->Release();
    }

    return hSend;

}

//����
int CBusiness::funcSubscribePush(int nFUnctionNo, int nIssueType){
    printf("--------------------------->m_cUserToken[%s]\n",m_cUserToken);
    int hSend = 0;

    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;


    //���ܺ�
    lpBizMessage->SetFunction(nFUnctionNo);
    //��������
    lpBizMessage->SetPacketType(REQUEST_PACKET);

    lpBizMessage->SetCompanyID(91000);
    //����SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
    //������Ϣ����
    lpBizMessage->SetIssueType(nIssueType);

    ///��ȡ�汾Ϊ2���͵�pack�����
    IF2Packer *pPacker = NewPacker(2);
    if(!pPacker)
    {
        printf("ȡ�����ʧ��!\r\n");
        return -1;
    }
    pPacker->AddRef();

    ///��ʼ���
    pPacker->BeginPack();

    ///�����ֶ���
    /*		pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
            pPacker->AddField("op_entrust_way", 'C', 1);
            pPacker->AddField("op_station", 'S', 255);
            pPacker->AddField("password", 'S', 50);
            pPacker->AddField("client_id", 'S', 18);
            pPacker->AddField("user_token", 'S', 512);


            pPacker->AddField("fund_account", 'S', 255);
            pPacker->AddField("branch_no", 'I', 5);
            pPacker->AddField("issue_type", 'I', 5);
            */
    /*	pPacker->AddField("op_branch_no");//���� ���� ����
        pPacker->AddField("op_entrust_way");
        pPacker->AddField("op_station");
        pPacker->AddField("password");
        pPacker->AddField("client_id");
        pPacker->AddField("user_token");*/


    pPacker->AddField("fund_account", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5);
    //	pPacker->AddField("issue_type", 'I', 5);


    ///�����Ӧ���ֶ�ֵ
    /*	pPacker->AddStr("");		//op_branch_no				
        pPacker->AddChar('7');		//op_entrust_way			
        pPacker->AddStr("station1");	//	op_station	
        pPacker->AddStr("111111");		//password
        pPacker->AddStr("");
        pPacker->AddStr(""); //user_token
        */


    pPacker->AddStr("70000172");			//	fund_account	
    pPacker->AddInt(m_branch_no);				//	branch_no	
    //pPacker->AddInt(nIssueType); //issue_type

    ///�������
    pPacker->EndPack();


    //lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpBizMessage->SetKeyInfo(pPacker->GetPackBuf(),pPacker->GetPackLen());


    ///ͬ�������ʽ��ѯ����Ӧ����RecvBizEx�д���
    if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,1)) < 0)
    {
        printf("���͹���%d ��Ϣ����%d ʧ��, �����: %d, ԭ��: %s!\r\n", nFUnctionNo, nIssueType, hSend, g_pConnection->GetErrorMsg(hSend));
        hSend=-2;
        goto EXIT;

    }

    printf("���͹���%d ��Ϣ����%d �ɹ�, ���ؽ��վ��: %d!\r\n", nFUnctionNo, nIssueType, hSend);

    //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
    /*		hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,5000);
            if(hSend != 0)
            {
            printf("���չ���%d ��Ϣ����%d ʧ��, �����: %d, ԭ��: %s!\r\n", nFUnctionNo, nIssueType,hSend, g_pConnection->GetErrorMsg(hSend));
            goto EXIT;
            }else{
            int iReturnCode = lpBizMessageRecv->GetReturnCode();
            if(iReturnCode!= 0) //����
            {
            printf("���չ���%d ��Ϣ����%d ʧ��,errorNo:%d,errorInfo:%s\n",nFUnctionNo, nIssueType,lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo()); 			
            }
            else if(iReturnCode==0) // ��ȷ
            {
    //���Ҫ����Ϣ�ŵ������̴߳����������п������������£�
    //int iMsgLen = 0;
    //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
    //��lpMsgBuffer�����ߣ�Ȼ���������߳��лָ�����Ϣ�ɽ������²�����
    //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
    //û�д�����Ϣ
    puts("ҵ������ɹ�");
    int iLen = 0;
    const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
    IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
    ShowPacket(0,lpUnPacker);
    }

    }
    */	
EXIT:
    ///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
    if(pPacker)
    {
        pPacker->FreeMem(pPacker->GetPackBuf());
        ///�ͷ������pack
        pPacker->Release();
    }
    if(lpBizMessage){
        lpBizMessage->Release();
    }

    return hSend;


}

// �ʽ��ѯ 332255 //�첽����ʽ 
int CBusiness::ClientFundAllQry(int iSystemNo /* = 2 */)
{
    printf("--------------------------->m_cUserToken[%s]\n",m_cUserToken);
    int hSend = 0;

    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

    //���ܺ�
    lpBizMessage->SetFunction(332255);
    //��������
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    //����Ӫҵ����
    lpBizMessage->SetBranchNo(1);
    //lpBizMessage->SetBranchNo(m_branch_no);
    lpBizMessage->SetCompanyID(91000);
    //����SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
    //����ϵͳ��
    lpBizMessage->SetSystemNo(iSystemNo);



    ///��ȡ�汾Ϊ2���͵�pack�����
    IF2Packer *pPacker = NewPacker(2);
    if(!pPacker)
    {
        printf("ȡ�����ʧ��!\r\n");
        return -1;
    }
    pPacker->AddRef();

    ///��ʼ���
    pPacker->BeginPack();

    ///�����ֶ���
    pPacker->AddField("op_branch_no", 'I', 5);
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5);			
    pPacker->AddField("client_id", 'S', 18);			
    pPacker->AddField("fund_account", 'S', 18);		
    pPacker->AddField("password", 'S', 10);						
    pPacker->AddField("password_type", 'C', 1);		
    pPacker->AddField("user_token", 'S', 512);

    ///�����Ӧ���ֶ�ֵ
    pPacker->AddInt(0);				//	op_branch_no	
    pPacker->AddChar('7');			//	op_entrust_way	
    pPacker->AddStr("hs");	//op_station
    pPacker->AddInt(m_branch_no);	//branch_no
    pPacker->AddStr(m_client_id);	//		client_id
    pPacker->AddStr(m_fund_account);	//	fund_account
    pPacker->AddStr("111111");		//	password	
    pPacker->AddChar('1');			//	password_type	
    pPacker->AddStr(m_cUserToken);//user_token

    ///�������
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());








    ///ͬ�������ʽ��ѯ����Ӧ����RecvBizEx�д���
    if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
    {
        printf("���͹���332255ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        hSend=-2;
        goto EXIT;
    }

    printf("���͹���332255�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);

    //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
    hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
    if(hSend != 0)
    {
        printf("���չ���332255ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        goto EXIT;
    }else{
        int iReturnCode = lpBizMessageRecv->GetReturnCode();
        if(iReturnCode!= 0) //����
        {
            printf("���չ���332255ʧ��,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo()); 			
        }
        else if(iReturnCode==0) // ��ȷ
        {
            //���Ҫ����Ϣ�ŵ������̴߳����������п������������£�
            //int iMsgLen = 0;
            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
            //��lpMsgBuffer�����ߣ�Ȼ���������߳��лָ�����Ϣ�ɽ������²�����
            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
            //û�д�����Ϣ
            puts("ҵ������ɹ�");
            int iLen = 0;
            const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
            IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            ShowPacket(0,lpUnPacker);
        }

    }

EXIT:
    ///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
    if(pPacker)
    {
        pPacker->FreeMem(pPacker->GetPackBuf());
        ///�ͷ������pack
        pPacker->Release();
    }
    if(lpBizMessage){
        lpBizMessage->Release();
    }

    return hSend;

}






/*	ԭʼ����
///ͬ�������ʽ��ѯ����Ӧ����RecvBizEx�д���
if((hSend = g_pConnection->SendBizMsg(lpBizMessage,1)) < 0)
{
printf("���͹���332255ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
hSend=-2;
goto EXIT;
}

printf("���͹���332255�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);

EXIT:
///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
if(pPacker)
{
pPacker->FreeMem(pPacker->GetPackBuf());
///�ͷ������pack
pPacker->Release();
}
if(lpBizMessage){
lpBizMessage->Release();
}

return hSend;
}*/



////////////////////////////////////////////////////////////
//Callback ʵ��
unsigned long CCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    return 0;
}

unsigned long CCallback::AddRef()
{
    return 0;
}

unsigned long CCallback::Release()
{
    return 0;
}

// ���¸��ص�������ʵ�ֽ���Ϊ��ʾʹ��
void CCallback::OnConnect(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnConnect");
}

void CCallback::OnSafeConnect(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnSafeConnect");
}

void CCallback::OnRegister(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnRegister");
}

void CCallback::OnClose(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnClose");;
}

void CCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
    puts("CCallback::OnSent");
}

void CCallback::Reserved1(void *a, void *b, void *c, void *d)
{
    puts("CCallback::Reserved1");
}

void CCallback::Reserved2(void *a, void *b, void *c, void *d)
{
    puts("CCallback::Reserved2");
}

void CCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
    puts("CCallback::OnReceivedBizEx");
}

void CCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    puts("CCallback::OnReceivedBiz");
}

int  CCallback::Reserved3()
{
    puts("CCallback::Reserved3");
    return 0;
}

void CCallback::Reserved4()
{
    puts("CCallback::Reserved4");
}

void CCallback::Reserved5()
{
    puts("CCallback::Reserved5");
}

void CCallback::Reserved6()
{
    puts("CCallback::Reserved6");
}

void CCallback::Reserved7()
{
    puts("CCallback::Reserved7");
}

void CCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
    puts("CCallback::OnReceivedBizMsg");
    if (lpMsg!=NULL)
    {
        //�ɹ�,Ӧ�ó������ͷ�lpBizMessageRecv��Ϣ
        int iReturnCode = lpMsg->GetReturnCode();
        if(iReturnCode!= 0) //����
        {
            printf("���չ���332255ʧ��,errorNo:%d,errorInfo:%s\n",lpMsg->GetErrorNo(),lpMsg->GetErrorInfo());			 
        }
        else if(iReturnCode==0) // ��ȷ


        {

            //���Ҫ����Ϣ�ŵ������̴߳����������п������������£�
            //int iMsgLen = 0;
            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
            //��lpMsgBuffer�����ߣ�Ȼ���������߳��лָ�����Ϣ�ɽ������²�����
            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
            //û�д�����Ϣ
            int Func = lpMsg->GetFunction();
            printf("-----func----[%d]\n",Func);
            if(620000 == Func){
                printf("------------������----------\n");
                lpMsg->ChangeReq2AnsMessage();
                lpConnection->SendBizMsg(lpMsg,1);
                return ;
            }
            //puts("ҵ������ɹ�");
            int iLen = 0;
            const void * lpBuffer = lpMsg->GetKeyInfo(iLen);
            IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            int iIssueType =lpMsg->GetIssueType();
            if(23 == iIssueType && 620003 == Func){
                printf("------------ί�лر�----------\n");
            }else if(12 == iIssueType && 620003 == Func){
                printf("------------�ɽ��ر�----------\n");
            }else if(23 == iIssueType && 620001 == Func){
                printf("------------����ί�лر�����----------\n");
            }else if(12 == iIssueType && 620001 == Func){
                printf("------------���ĳɽ��ر�����----------\n");
            }else{
                printf("------------δ֪----------func [%d]iIssueType[%d]\n",Func,iIssueType);
            }
            ShowPacket(0,lpUnPacker);

            /*ԭʼ
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
            ShowPacket(0,lpUnPacker);

            if (null != lpUnPacker)
            ShowPacket(0,lpUnPacker);
            else {
            //error happen within NewUnPacker;
            printf("error happen within NewUnpacker\n");
            }
            */

        }

    }
}

////////////////////////////////////////////////////////////////
//main
// ȫ�����Ӷ���
CConnectionInterface *g_pConnection = NULL;
CConnectionInterface *g_pConnectionHq = NULL;

CBusiness g_szBusiness;
CBusiness g_szBusinessHq;

// 120.55.176.113:9369

int main()
{

    //ͨ��T2SDK����������������ȡһ���µ�CConfig����
    //�˶����ڴ������Ӷ���ʱ�����ݣ��������������������Ӷ���ĸ������ԣ����������IP��ַ����ȫģʽ�ȣ�
    //ֵ��ע����ǣ��������ö�������������Ϣʱ��������Ϣ�ȿ��Դ�ini�ļ������룬
    //Ҳ�����ڳ���������趨��������2�ߵĻ�ϣ������ͬһ���������費ͬ��ֵ���������һ������Ϊ׼
    CConfigInterface * lpConfig = NewConfig();

    //ͨ��T2SDK����������NewXXXX���صĶ�����Ҫ���ö����Release�����ͷţ�������ֱ����delete
    //��Ϊt2sdk.dll�͵��ó���������ɲ�ͬ�ı�����������ģʽ���ɣ�delete���ܻᵼ���쳣
    //Ϊ���ʺ�Delphi��ʹ�ã�Delphi�Խӿ��Զ�����AddRef����������C/C++�����Ĵ��룬��Ҫ��NewXXXX֮�����һ��AddRef
    //�Ա�֤���ü�����ȷ
    lpConfig->AddRef();

    ///�������ļ��м�������
    if (0 == lpConfig->Load("t2sdk.ini"))
    {
        std::cout << "load t2sdk.ini successfully" << std::endl;
    }
    else
    {
        std::cout << "load t2sdk.ini failed" << std::endl;
    }



    ///�����ڴ����ж�lpConfig�����еľ���ֵ��������

    //[t2sdk] serversָ����Ҫ���ӵ�IP��ַ���˿ڣ������ö�����м��ԡ�;�����//111.207.179.6:8066  18.8.1.171:8066
    /*
       lpConfig->SetString("t2sdk", "servers", "192.168.51.162:3335");

    //[t2sdk] license_fileָ�����֤�ļ�·��
    lpConfig->SetString("t2sdk", "license_file", "(20130306)tyjrnb-TYJRTYJR-ALL-0001_3rd.dat");

    //[t2sdk] langָ��������Ϣ�����Ժţ�ȱʡΪ��������2052����1033ΪӢ��
    lpConfig->SetString("t2sdk", "lang", "1033");

    //[t2sdk] send_queue_sizeָ��T2_SDK�ķ��Ͷ��д�С
    lpConfig->SetString("t2sdk", "send_queue_size", "100");

    //[t2sdk] send_queue_sizeָ��T2_SDK�ķ��Ͷ��д�С
    lpConfig->SetString("t2sdk", "license_pwd", "888888");

    //[safe] safe_levelָ�����ӵİ�ȫģʽ����Ҫ��T2ͨ���İ�ȫģʽһ�£���������ʧ��
    lpConfig->SetString("safe", "safe_level", "ssl");

    //[safe] 
    lpConfig->SetString("safe", "cert_file", "c20121011.pfx");

    //[safe] 
    lpConfig->SetString("safe", "cert_pwd", "111111");
    */

    g_szBusiness.SetConfig(lpConfig);
    //ͨ��T2SDK����������������ȡһ���µ�CConnection����ָ��
    g_pConnection = NewConnection(lpConfig);

    g_pConnection->AddRef();

    // �����Զ�����CCallback�Ķ����ڳ�ʼ�����Ӷ���ʱ�贫�ݴ˶����뿴������룩
    CCallback szCallback;

    int iRet = 0, iSystemNo = 0;

    //��ʼ�����Ӷ��󣬷���0��ʾ��ʼ���ɹ���ע���ʱ��û��ʼ���ӷ�����
    if (/*(0 == (iRet = g_pConnection->CreateEx(&szCallback)))*/(0 == (iRet = g_pConnection->Create2BizMsg(&szCallback))))
    {
        // �����Ͽ������̣߳������ڷǶϿ�����£����̴߳���Wait״̬���ʶ�Ӧ������Ӱ����΢
        //��ʽ��ʼ���ӣ�����5000Ϊ��ʱ��������λ����
        if (iRet = g_pConnection->Connect(5000))
        {
            printf("���ӷ�����ʧ��, �����: %d, ԭ��: %s!\r\n", iRet, g_pConnection->GetErrorMsg(iRet));
        }
        else
        {
            printf("���ӷ������ɹ�!\r\n");
            ////////////////////////////֤ȯ����//////////////////////////////////////
            //Subscribe(12, 8113, "811300000008");		//���Ͷ�������:�ɽ����� 12
            iSystemNo = g_szBusiness.Login();
            if(iSystemNo >= 0)
            {
                int chose = 0;
                printf("------------------------------��ӭʹ��demo------------------------------\n");
                //printf("1.����-֤ȯ�ɽ��ر�. 2.֤ȯί�� 3.֤ȯί�в�ѯ 4.ί�г��� 5.�ɽ���ѯ 6.�ֲֲ�ѯ 7.�ʽ��ѯ 0.�˳���ϵͳ!\n��������Ĳ���:");
                printf("1.֤ȯί�� 2.�ֲֲ�ѯ 3.�ʽ��ѯ 4:֤ȯ�ɶ���Ϣ��ѯ 5.ί�ж��� 0.�˳���ϵͳ!\n��������Ĳ���:");
                while(scanf("%d",&chose),0 != chose)
                {
                    getchar();
                    switch (chose)
                    {

                        case 1: g_szBusiness.SecuEntrust(iSystemNo);
                                printf("�밴�������ʾ���˵�!:\n");
                                break;
                        case 2: g_szBusiness.SecuStockQry(iSystemNo);
                                printf("�밴�������ʾ���˵�!:\n");
                                break;
                        case 3: g_szBusiness.ClientFundAllQry(iSystemNo);
                                printf("�밴�������ʾ���˵�!:\n");
                                break;
                        case 4: g_szBusiness.ClientStkacctQry(iSystemNo);
                                printf("�밴�������ʾ���˵�!:\n");
                                break;
                        case 5:g_szBusiness.funcSubscribePush(620001, 23);
                               printf("�밴�������ʾ���˵�!:\n");
                               break;
                        default:printf("������Ĳ�������ȷ,��������ȷ�Ĳ���!\n");
                                printf("�밴�������ʾ���˵�!:\n");
                                break;
                    }
                    getchar();
                    //printf("1.����-֤ȯ�ɽ��ر�. 2.֤ȯί�� 3.֤ȯί�в�ѯ 4.ί�г��� 5.�ɽ���ѯ 6.�ֲֲ�ѯ 7.�ʽ��ѯ  8:֤ȯ�ɶ���Ϣ��ѯ 0.�˳���ϵͳ!\n��������Ĳ���(Ŀǰʵ��2��6��7��8):");
                    printf("1.֤ȯί�� 2.�ֲֲ�ѯ 3.�ʽ��ѯ 4:֤ȯ�ɶ���Ϣ��ѯ 5.ί�ж��� 0.�˳���ϵͳ!\n��������Ĳ���:");

                }
            } 
            else
                printf("ȡ��ϵͳ�ڵ��ʧ��, ���ؽ��: %d!\r\n", iSystemNo);
        }
    }
    else
    {
        if(0 != iRet)
            puts(g_pConnection->GetErrorMsg(iRet));
    }

    //ͨ��getchar�����̣߳��ȴ������Ӧ�������
    printf("�����������!\r\n");
    getchar();

    g_pConnection->Close();
    g_pConnection->Release();
    lpConfig->Release();

    return 0;
}
