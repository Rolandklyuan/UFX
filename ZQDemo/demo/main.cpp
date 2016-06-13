//��C++ Demoʹ���첽���ͽ���ģʽ
//Demo���ӵ��Ǻ���֤ȯ �ķ��滷�������Ҫ�Խ�ȯ�̻�������Ҫ�޸������ļ���ip�˿ڣ��Լ�license�ļ����û�������
//��ͬ��������֤��Ϣ���ܲ�һ�����������ӻ���ʱ����ͷ�ֶ�������Ҫȷ��
//�ӿ��ֶ�˵����ο��ӿ��ĵ�"����ͳһ����ƽ̨_�ܱ߽ӿڹ淶(��Ʊ��Ȩ).xls"
//T2��������˵���ο������ĵ���T2SDK �ⲿ�濪��ָ��.docx"
//����t2�������ʿ���ϵUFX֧�������ڱ�(�����Ⱥ ����-���ڱ� qq 740658023)
//optTrade.h&optTrade.cpp����������ͨ����
//optMD.h&optMD.cpp �������Զ��ĳɽ��ر�����ί������
//demo�����ο�
#include "SecuTrade.h"
#include "SecuMD.h"

int showmenu1()
{
    cerr<<"------------------------------------------------"<<endl;
    cerr<<" [1]  331100 �����¼                           "<<endl;
    cerr<<" [2]  400 ֤ȯ�����ѯ                          "<<endl;
    cerr<<" [3]  333002 ֤ȯ��ͨί��                       "<<endl;
    cerr<<" [4]  333103 ֤ȯ�ֲֲ�ѯ                       "<<endl;
    cerr<<" [5]  331300 ֤ȯ�ɶ���Ϣ��ѯ                   "<<endl;
    cerr<<" [6]  333101 ֤ȯί�в�ѯ                       "<<endl;
    cerr<<" [7]  333102 ֤ȯ�ɽ���ѯ                       "<<endl;
    cerr<<" [0]  EXIT                                      "<<endl;
    cerr<<"------------------------------------------------"<<endl;
    return 0;

}
int showmenu2()
{
    cerr<<"------------------------------------------------"<<endl;
    cerr<<" [1]  331100 �����¼                           "<<endl;
    cerr<<" [2]  62001_12����֤ȯ�ɽ��ر�                  "<<endl;
    cerr<<" [3] 62001_23����֤ȯί�лر�                   "<<endl;
    cerr<<" [4] 333002 ��ͨί��                            "<<endl;
    cerr<<" [0]  EXIT                                      "<<endl;
    cerr<<"------------------------------------------------"<<endl;
    return 0;
}

int TestSecuTrade()
{
    char exchange_type[4];
    char stock_code[16];
    double entrust_price=0.0;
    memset(exchange_type,0,sizeof(exchange_type));
    memset(stock_code,0,sizeof(stock_code));
    double entrust_amount=0.0;
    char entrust_bs='\0';
    int chose=0;
    SecuRequestMode* lp_SecuRequestMode = new SecuRequestMode();
    lp_SecuRequestMode->InitConn();
    showmenu1();
    while(1)
    {
        cin>>chose;
        switch(chose)
        {
            case 1:
                lp_SecuRequestMode->ReqFunction331100();
                break;
            case 2:
                cout<<"---------֤ȯ�����ѯ----------"<<endl;
                cout<<"Enter exchange_type:";cin>>exchange_type;
                cout<<"Enter stock_code:";cin>>stock_code;
                lp_SecuRequestMode->ReqFunction400(exchange_type,stock_code);
                break;
            case 3:
                cout<<"---------֤ȯ��ͨί��----------"<<endl;
                cout<<"exchange_type:"; cin>>exchange_type;
                cout<<"stock_code:"; cin>>stock_code;
                cout<<"entrust_amount:"; cin>>entrust_amount;
                cout<<"entrust_price:";  cin>>entrust_price;
                cout<<"entrust_bs:";cin>>entrust_bs;
                lp_SecuRequestMode->ReqFunction333002(exchange_type, stock_code, entrust_amount, entrust_price,entrust_bs);
                break;
            case 4:
                lp_SecuRequestMode->ReqFunction333104(" ");
                break;
            case 5:
                lp_SecuRequestMode->ReqFunction331300();
                break;
            case 6:
                lp_SecuRequestMode->ReqFunction333101();
                break;
            case 7:
                lp_SecuRequestMode->ReqFunction333102();
                break;
            case 0:
                exit(0);
        }
        Sleep(500);
#ifndef LINUX
        system("pause");
#endif
        showmenu1();
    }

    delete lp_SecuRequestMode;
    return 0;
}

int TestSecutMd()
{
    char exchange_type[4];
    char stock_code[16];
    // double entrust_price=0.0;
    memset(exchange_type,0,sizeof(exchange_type));
    memset(stock_code,0,sizeof(stock_code));
    // double entrust_amount=0.0;
    // char entrust_bs='\0';
    int chose=0;
    // int i=0;
    CMdRequestMode* lp_CMdRequestMode = new CMdRequestMode();
    lp_CMdRequestMode->InitConn();
    showmenu2();
    while(1)
    {
        cin>>chose;
        switch(chose)
        {
            case 1:
                lp_CMdRequestMode->ReqFunction331100();
                break;
            case 2: 
                lp_CMdRequestMode->SubFunction12(ISSUE_TYPE_REALTIME_SECU);
                break;
            case 3:
                lp_CMdRequestMode->SunFunction23(ISSUE_TYPE_ENTR_BACK);
                break;
            case 4:
                cout<<"---------֤ȯ��ͨί��----------"<<endl;
                lp_CMdRequestMode->ReqFunction333002();
                break;
            case 0:
                exit(0);
        }
        Sleep(500);
        system("pause");
        showmenu2();
    }
    delete lp_CMdRequestMode;
    return 0;
}
int main()
{

    cerr<<"-----------------------------------------------"<<endl;
    cerr<<" [1] TestSecuTrade                          "<<endl;
    cerr<<" [2] TestSecuMd                             "<<endl;
    cerr<<" [0] Exit                                  "<<endl;
    cerr<<"-----------------------------------------------"<<endl;

    while(1)
    {
        int cmd;  
        cin>>cmd;
        switch(cmd){
            case 1: 
                TestSecuTrade();
                break;
            case 2:
                TestSecutMd();
                break;
            case 0: exit(0);
        }
    }

    //ͨ��getchar�����̣߳��ȴ������Ӧ�������
    getchar();

    return 0;
}
