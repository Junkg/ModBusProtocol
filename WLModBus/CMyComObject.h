#ifndef __CMYCOMOBJECT_H__
#define __CMYCOMOBJECT_H__
#pragma once

#include "MBInterface.h"

extern "C" ULONG    g_ObjNumber;

// {33C2DAE3-6F89-4318-9F06-9820C87DE202}
extern "C" CLSID  CLSID_MODBUS;



class CModBus
{

public:
	CModBus();
	virtual  ~CModBus();

	//PDU	
	/*��鹦�����Ƿ���ȷ*/
	ModbusStatus CheckFunctionCode(FunctionCode fc);

	/*��ΪRTU��վ��TCP�ͻ��ˣ�ʱ�����ɶ�дRTU��վ��TCP�����������������*/
	uint16_t GenerateReadWriteCommand(ObjAccessInfo objInfo, bool* statusList, uint16_t* registerList, uint8_t* commandBytes);

	/*����RTU��վ�� TCP�ͻ��ˣ��ӷ�������ȡ������ */
	void TransformClientReceivedData(uint8_t* receivedMessage, uint16_t quantity, bool* statusList, uint16_t* registerLister);

	//RTU 
	/*ͨ��CRCУ��У����յ���Ϣ�Ƿ���ȷ*/
	bool CheckRTUMessageIntegrity(uint8_t* message, uint16_t length);

	/*����CRC-16/MODBUS У����  ����ʽ�� x16 + x15 + x2 + 1  */
	uint16_t  GenerateCRC16CheckCode(uint8_t* puckData, uint16_t usDataLen);

	//COMMON 

	/*�����յ���д����Coilֵת��Ϊ����������Ӧ0x05������*/
	bool CovertSingleCommandCoilToBoolStatus(uint16_t coilValue, bool value);

	/*������������Ȧ������״̬������ת��ΪMB�ֽ�����,������������ĳ���  */
	static uint8_t ConvertBoolArrayToMBByteArray(bool* sData, uint16_t length, uint8_t* oData);
	/*���Ĵ���������Ĵ����ͱ��ּĴ���������ת��ΪMB�ֽ�����,������������ĳ���*/
	static uint8_t ConvertRegisterArrayToMBByteArray(uint16_t* sData, uint16_t length, uint8_t* oData);

	/*�����յ���дCoil�ֽ�����ת��Ϊ��������*/
	static void ConvertMBByteArrayTotBoolArray(uint8_t* sData, bool* oData);
	/*�����յ���д���ּĴ������ֽ�����רΪ�Ĵ�������*/
	static void ConvertMBByteArrayToRegisterArray(uint8_t* sData, uint16_t* oData);

	/*��ȡ�ֽ���ĳһ��λ��ֵ ��0��ʼ*/
	bool  GetBitValue(uint8_t nData, char nChar);

	/*����ĳһλ�ϵ�ֵ������ԭ����ֵ*/
	void SetBitValue(uint8_t& nData, char nChar, bool value);
protected:

public:
	ModbusStatus  m_status;

};

class CModBusStorage
{
public:

	/*��ʼ�����ݴ洢���򣨴�����Ȧ��������״̬�������ּĴ���������Ĵ����Ĵ洢���� ����Ϊ�ṹ�����飬��Ӧ�ó����ж���*/
	void InitializeDataStorageStructure(DataObject dataObject[], int length);

protected:
	/*��ȡ��Ҫ��ȡ��Coil����ֵ*/
	void GetCoilStatus(uint16_t startAddress, uint16_t quantity, bool* statusList);

	/*��ȡ��Ҫ��ȡ��InputStatus����ֵ*/
	void GetInputStatus(uint16_t startAddress, uint16_t quantity, bool* statusValue);

	/*��ȡ��Ҫ��ȡ�ı��ּĴ�����ֵ*/
	void GetHoldingRegister(uint16_t startAddress, uint16_t quantity, uint16_t* registerValue);

	/*��ȡ��Ҫ��ȡ������Ĵ�����ֵ*/
	void GetInputRegister(uint16_t startAddress, uint16_t quantity, uint16_t* registerValue);

	/*���õ�����Ȧ��ֵ*/
	bool SetSingleCoil(uint16_t coilAddress, bool coilValue);

	/*���õ����Ĵ�����ֵ*/
	void SetSingleRegister(uint16_t registerAddress, uint16_t registerValue);

	/*���ö����Ȧ��ֵ*/
	void SetMultipleCoil(uint16_t startAddress, uint16_t quantity, bool* statusValue);

	/*���ö���Ĵ�����ֵ*/
	void SetMultipleRegister(uint16_t startAddress, uint16_t quantity, uint16_t* registerValue);
private:

	/*������Ȧ�����ֵ*/
	void ResetCoilStructure();
	/*��������״̬�����ֵ*/
	void ResetInputStatusStructure();
	/*��������Ĵ��������ֵ*/
	void ResetInputRegisterStructure();
	/*���ñ��ֶ����ֵ*/
	void ResetHoldingRegisterStructure();


private:
	StatusObject   m_coilObject;
	StatusObject   m_inputStatusObject;
	RegisterObject m_inputRegisterObject;
	RegisterObject m_holdingRegisterObject;
private:
	CRITICAL_SECTION  m_csCoil;
	CRITICAL_SECTION  m_csInputStatus;
	CRITICAL_SECTION  m_csInputRegister;
	CRITICAL_SECTION  m_csHodingRegister;
};



class INodelegationUnknown
{
public:
	virtual HRESULT __stdcall NodelegationQueryInyterface(const IID& iid, void** ppv) = 0;
	virtual ULONG   __stdcall NodelegationAddRef() = 0;
	virtual ULONG   __stdcall NodelegationRelease() = 0;
};

class CMyComObject :  public  INodelegationUnknown , public IModBusTCPServer , public IModBusTCPClient , public IModBusRTUServer , public IModBusRTUClient
{
public:
	CMyComObject(IUnknown* pUnknownOuter = NULL);
	~CMyComObject();
public:
	// IUnKnown  
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_opt_  REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppv);
	virtual ULONG   STDMETHODCALLTYPE AddRef(void);
	virtual ULONG   STDMETHODCALLTYPE Release(void);

	//INodelegationUnknown 
	virtual HRESULT STDMETHODCALLTYPE NodelegationQueryInyterface(const IID& iid, void** ppv);
	virtual ULONG   STDMETHODCALLTYPE NodelegationAddRef(void) ;
	virtual ULONG   STDMETHODCALLTYPE NodelegationRelease(void) ;

	//IModBusTCPServer
	/*�������յ�����Ϣ��������Ӧ����ĳ���*/
	virtual uint16_t STDMETHODCALLTYPE ParsingClientAccessCommand(uint8_t* receivedMessage, uint8_t* respondBytes) ;


	//IModBusTCPClient
	/*���ɷ��ʷ�����������*/
	virtual uint16_t STDMETHODCALLTYPE CreateAccessServerCommand(ObjAccessInfo objInfo, void* dataList, uint8_t* commandBytes) ;
	/*�����յ��ķ�������Ӧ��Ϣ*/
	virtual void    STDMETHODCALLTYPE ParsingServerRespondMessage(uint8_t* recievedMessage, void* dataList) ;

	//IModBusRTUServer
	/*�������յ�����Ϣ�������غϳɵĻظ���Ϣ����Ϣ���ֽڳ��ȣ�ͨ���ص�����*/
	virtual uint16_t STDMETHODCALLTYPE ParsingMasterAccessCommand(uint8_t* receivedMesasage, uint8_t* respondBytes, uint16_t rxLength) ;


	//IModBusRTUClient
		/*���ɷ��ʷ�����������*/
	virtual uint16_t STDMETHODCALLTYPE CreateAccessSlaveCommand(ObjAccessInfo objInfo, void* dataList, uint8_t* commandBytes);

	/*�����յ��ķ�������Ӧ��Ϣ*/
	virtual void STDMETHODCALLTYPE ParsingSlaveRespondMessage(uint8_t* recievedMessage, uint8_t* command) ;

private:
	ULONG  m_Ref;
	IUnknown* m_pUnknownOuter;
};

#endif //__CMYCOMOBJECT_H__