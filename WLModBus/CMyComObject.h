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
	/*检查功能吗是否正确*/
	ModbusStatus CheckFunctionCode(FunctionCode fc);

	/*作为RTU主站或TCP客户端）时，生成读写RTU从站或TCP服务器）对象的命令*/
	uint16_t GenerateReadWriteCommand(ObjAccessInfo objInfo, bool* statusList, uint16_t* registerList, uint8_t* commandBytes);

	/*解析RTU主站或 TCP客户端）从服务器读取的数据 */
	void TransformClientReceivedData(uint8_t* receivedMessage, uint16_t quantity, bool* statusList, uint16_t* registerLister);

	//RTU 
	/*通过CRC校验校验接收的信息是否正确*/
	bool CheckRTUMessageIntegrity(uint8_t* message, uint16_t length);

	/*生成CRC-16/MODBUS 校验码  多项式： x16 + x15 + x2 + 1  */
	uint16_t  GenerateCRC16CheckCode(uint8_t* puckData, uint16_t usDataLen);

	//COMMON 

	/*将接收到的写单个Coil值转化为布尔量，对应0x05功能码*/
	bool CovertSingleCommandCoilToBoolStatus(uint16_t coilValue, bool value);

	/*将布尔量（线圈和输入状态）数组转化为MB字节数组,返回最终数组的长度  */
	static uint8_t ConvertBoolArrayToMBByteArray(bool* sData, uint16_t length, uint8_t* oData);
	/*将寄存器（输入寄存器和保持寄存器）数组转化为MB字节数组,返回最终数组的长度*/
	static uint8_t ConvertRegisterArrayToMBByteArray(uint16_t* sData, uint16_t length, uint8_t* oData);

	/*将接收到的写Coil字节数组转化为布尔数组*/
	static void ConvertMBByteArrayTotBoolArray(uint8_t* sData, bool* oData);
	/*将接收到的写保持寄存器的字节数组专为寄存器数组*/
	static void ConvertMBByteArrayToRegisterArray(uint8_t* sData, uint16_t* oData);

	/*获取字节中某一个位的值 从0开始*/
	bool  GetBitValue(uint8_t nData, char nChar);

	/*设置某一位上的值，返回原来的值*/
	void SetBitValue(uint8_t& nData, char nChar, bool value);
protected:

public:
	ModbusStatus  m_status;

};

class CModBusStorage
{
public:

	/*初始化数据存储区域（创建线圈量、输入状态量、保持寄存器、输入寄存器的存储区域） 输入为结构体数组，在应用程序中定义*/
	void InitializeDataStorageStructure(DataObject dataObject[], int length);

protected:
	/*获取想要读取的Coil量的值*/
	void GetCoilStatus(uint16_t startAddress, uint16_t quantity, bool* statusList);

	/*获取想要读取的InputStatus量的值*/
	void GetInputStatus(uint16_t startAddress, uint16_t quantity, bool* statusValue);

	/*获取想要读取的保持寄存器的值*/
	void GetHoldingRegister(uint16_t startAddress, uint16_t quantity, uint16_t* registerValue);

	/*获取想要读取的输入寄存器的值*/
	void GetInputRegister(uint16_t startAddress, uint16_t quantity, uint16_t* registerValue);

	/*设置单个线圈的值*/
	bool SetSingleCoil(uint16_t coilAddress, bool coilValue);

	/*设置单个寄存器的值*/
	void SetSingleRegister(uint16_t registerAddress, uint16_t registerValue);

	/*设置多个线圈的值*/
	void SetMultipleCoil(uint16_t startAddress, uint16_t quantity, bool* statusValue);

	/*设置多个寄存器的值*/
	void SetMultipleRegister(uint16_t startAddress, uint16_t quantity, uint16_t* registerValue);
private:

	/*重置线圈对象的值*/
	void ResetCoilStructure();
	/*重置输入状态对象的值*/
	void ResetInputStatusStructure();
	/*重置输入寄存器对象的值*/
	void ResetInputRegisterStructure();
	/*重置保持对象的值*/
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
	/*解析接收到的信息，返回响应命令的长度*/
	virtual uint16_t STDMETHODCALLTYPE ParsingClientAccessCommand(uint8_t* receivedMessage, uint8_t* respondBytes) ;


	//IModBusTCPClient
	/*生成访问服务器的命令*/
	virtual uint16_t STDMETHODCALLTYPE CreateAccessServerCommand(ObjAccessInfo objInfo, void* dataList, uint8_t* commandBytes) ;
	/*解析收到的服务器相应信息*/
	virtual void    STDMETHODCALLTYPE ParsingServerRespondMessage(uint8_t* recievedMessage, void* dataList) ;

	//IModBusRTUServer
	/*解析接收到的信息，并返回合成的回复信息和信息的字节长度，通过回调函数*/
	virtual uint16_t STDMETHODCALLTYPE ParsingMasterAccessCommand(uint8_t* receivedMesasage, uint8_t* respondBytes, uint16_t rxLength) ;


	//IModBusRTUClient
		/*生成访问服务器的命令*/
	virtual uint16_t STDMETHODCALLTYPE CreateAccessSlaveCommand(ObjAccessInfo objInfo, void* dataList, uint8_t* commandBytes);

	/*解析收到的服务器相应信息*/
	virtual void STDMETHODCALLTYPE ParsingSlaveRespondMessage(uint8_t* recievedMessage, uint8_t* command) ;

private:
	ULONG  m_Ref;
	IUnknown* m_pUnknownOuter;
};

#endif //__CMYCOMOBJECT_H__