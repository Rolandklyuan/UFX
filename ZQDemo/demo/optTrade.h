#include "tool.h"

class COptionRequestMode;
// �Զ�����CCallback��ͨ���̳У�ʵ�֣�CCallbackInterface�����Զ�������¼����������ӳɹ���
// ���ӶϿ���������ɡ��յ����ݵȣ�����ʱ�Ļص�����
class CTradeCallback : public CCallbackInterface
{
public:
    // ��ΪCCallbackInterface�����մ��������IKnown��������Ҫʵ��һ����3������
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();

    // �����¼�����ʱ�Ļص�������ʵ��ʹ��ʱ���Ը�����Ҫ��ѡ��ʵ�֣����ڲ���Ҫ���¼��ص���������ֱ��return
    // Reserved?Ϊ����������Ϊ�Ժ���չ��׼����ʵ��ʱ��ֱ��return��return 0��
    void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData);
    void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d);
    void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d);
    int  FUNCTION_CALL_MODE Reserved3();
    void FUNCTION_CALL_MODE Reserved4();
    void FUNCTION_CALL_MODE Reserved5();
    void FUNCTION_CALL_MODE Reserved6();
    void FUNCTION_CALL_MODE Reserved7();
    void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult);
	void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);
	void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg);
public:
	void SetRequestMode(COptionRequestMode* lpMode);
	//331100 ����
	void OnResponse_331100(IF2UnPacker *lpUnPacker);
	
private:
	COptionRequestMode* lpReqMode;
};

class COptionRequestMode
{
public:
	COptionRequestMode()
	{
		lpConfig = NULL;
		lpConnection = NULL;
		callback.SetRequestMode(this);

		lpConfig = NewConfig();
		lpConfig->AddRef();
	};

	~COptionRequestMode()
	{
		lpConnection->Release();
		lpConfig->Release();
	};

	int InitConn(const char* clientName, const char* licFile, const char* serverAddr);
	unsigned long Release();
public:
	string m_opUserToken;
	int m_BranchNo;
	char m_client_id[18];
	int m_op_branch_no;
	int l_entrust_reference;

	//331100 ����
	int ReqFunction331100(const char* AccountName, const char* Password, const char EntrustWay);
	//338000��Ȩ������Ϣ��ѯ 
	int ReqFunction338000();
	//338010��Ȩ�ɽ���������ȡ
	int ReqFunction338010(char* exchange_type,char* option_code, double opt_entrust_price,char entrust_bs,char entrust_oc);
	//��Ȩ��������ȷ�� 
	int ReqFunction338002(char* exchange_type, char* option_code);
	//338020��Ȩί�в�ѯ
	int ReqFunction338020();
	//338021��Ȩ�ɽ���ѯ
	int ReqFunction338021();
	//338022��Ȩ�ʲ���ѯ
	int ReqFunction338022();
	//338023��Ȩ�ֲֲ�ѯ
	int ReqFunction338023();
	//338011��Ȩί�� 
	int ReqFunciton338011( char* exchange_type,char* option_code,int entrust_amount,double entrust_price,char entrust_bs,char entrust_oc);
	//338012��Ȩ����
	int ReqFunction338012(char* exchange_type, int entrust_no);
	//339800��ʷ��Ȩί�в�ѯ
	int ReqFunction339800(int startDate, int endDate);
	//339801��ʷ��Ȩ�ɽ���ѯ
	int ReqFunction339801(int startDate, int endDate);
	//338001��Ȩ�����Ϣ��ѯ
	int ReqFunction338001();
	//338024��Ȩ��Ȩָ����Ϣ��ѯ
	int ReqFunction338024();
	//338025��Ȩ��Ȩ������Ϣ��ѯ
	int ReqFunction338025();
	//338026��Ȩ��Ȩָ��Ƿ��Ƿȯ��ѯ
	int ReqFunction338026();
	//338027��Ȩ���֤ȯ������ʾ
	int ReqFunction338027();
	//338028�ͻ���Ȩ�ֲֺ�Լ��Ϣ��ʾ
	int ReqFunction338028();
	//338029�ͻ�����֤ȯ�����ѯ
	int ReqFunction338029();
	//338030����֤ȯ�ɻ�ת������ȡ
	int ReqFunction338030();
	//338031����֤ȯ��ת
	int ReqFunction338031(char* exchange_type, char* stock_code, int entrust_amount);
	//
private:
	CConfigInterface* lpConfig;
	CConnectionInterface *lpConnection;
	CTradeCallback callback;

	string m_opAccountName;
	string m_opPassword;
	char m_opEntrustWay;
	string m_opFuturesAccount;
	string m_opStation;
	
	int m_SubSystemNo;
};