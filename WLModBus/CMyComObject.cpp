#include "pch.h"
#include "CMyComObject.h"

//¼ÓÈë¹²Ïí×Ö¶Î
#pragma data_seg("DLLSharedSection")
StatusObject   m_coilObject;
StatusObject   m_inputStatusObject;
RegisterObject m_inputRegisterObject;
RegisterObject m_holdingRegisterObject;
#pragma data_seg();


ULONG    g_ObjNumber = 0;
extern "C" CLSID  CLSID_MODBUS =
{ 0x33c2dae3, 0x6f89, 0x4318, { 0x9f, 0x6, 0x98, 0x20, 0xc8, 0x7d, 0xe2, 0x2 } };

CMyComObject::CMyComObject(IUnknown* pUnknownOuter)
{
	m_pUnknownOuter = pUnknownOuter;
}

CMyComObject::~CMyComObject()
{
}

HRESULT __stdcall CMyComObject::QueryInterface(REFIID riid, void** ppv)
{
	if (m_pUnknownOuter != NULL)
		return m_pUnknownOuter->QueryInterface(riid,ppv);
	else
		return NodelegationQueryInyterface(riid,ppv);
}

ULONG __stdcall CMyComObject::AddRef(void)
{
	if (m_pUnknownOuter != NULL)
		return m_pUnknownOuter->AddRef();
	else
		return NodelegationAddRef();
}

ULONG __stdcall CMyComObject::Release(void)
{
	if (m_pUnknownOuter != NULL)
		return m_pUnknownOuter->Release();
	else
		return NodelegationRelease();
}

HRESULT __stdcall CMyComObject::NodelegationQueryInyterface(const IID& iid, void** ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = (IUnknown*)((IModBusTCPServer*)this);
		((IUnknown*)(*ppv))->AddRef();

	}else if (iid == IID_ModBusTCPServer)
	{
		*ppv = (IModBusTCPServer*)this;
		((IModBusTCPServer*)(*ppv))->AddRef();
	}
	else if (iid == IID_ModBusTCPClient)
	{
		*ppv = (IModBusTCPClient*)this;
		((IModBusTCPClient*)(*ppv))->AddRef();
	}
	else if (iid == IID_ModBusRTUServer)
	{
		*ppv = (IModBusRTUServer*)this;
		((IModBusRTUServer*)(*ppv))->AddRef();
	}
	else if (iid == IID_ModBusRTUClient)
	{
		*ppv = (IModBusRTUClient*)this;
		((IModBusRTUClient*)(*ppv))->AddRef();
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG __stdcall CMyComObject::NodelegationAddRef(void)
{
	m_Ref++;
	return m_Ref;
}

ULONG __stdcall CMyComObject::NodelegationRelease(void)
{
	m_Ref--;
	if (m_Ref == 0)
	{
		g_ObjNumber--;
		delete this;
	}
	return m_Ref;
}

uint16_t __stdcall CMyComObject::ParsingClientAccessCommand(uint8_t* receivedMessage, uint8_t* respondBytes)
{
	m_status = MODBUS_OK;

	FunctionCode fc = (FunctionCode)(*(receivedMessage + 7));
	if (CheckFunctionCode(fc) != MODBUS_OK)
	{
		m_status = InvalidFunctionCode;
		return 0;
	}

	uint16_t length = 0;

	uint16_t startAddress = (uint16_t)(*(receivedMessage + 8));
	startAddress = (startAddress << 8) + (uint16_t)(*(receivedMessage + 9));

	uint16_t quantity = (uint16_t)(*(receivedMessage + 10));
	quantity = (quantity << 8) + (uint16_t)(*(receivedMessage + 11));

	switch (fc)
	{
	case ReadCoilStatus:
	{
		length = HandleReadCoilStatusCommand(startAddress, quantity, receivedMessage, respondBytes);
	}
	break;
	case ReadInputStatus:
	{
		length = HandleReadInputStatusCommand(startAddress, quantity, receivedMessage, respondBytes);
	}
	break;
	case ReadHoldingRegister:
	{
		length = HandleReadHoldingRegisterCommand(startAddress, quantity, receivedMessage, respondBytes);
	}
	break;
	case ReadInputRegister:
	{
		length = HandleReadInputRegisterCommand(startAddress, quantity, receivedMessage, respondBytes);
	}
	break;
	case WriteSingleCoil:
	{
		length = HandleWriteSingleCoilCommand(startAddress, quantity, receivedMessage, respondBytes);
	}
	break;
	case WriteSingleRegister:
	{
		length = HandleWriteSingleRegisterCommand(startAddress, quantity, receivedMessage, respondBytes);
	}
	break;
	case WriteMultipleCoil:
	{
		length = HandleWriteMultipleCoilCommand(startAddress, quantity, receivedMessage, respondBytes);
	}
	break;
	case WriteMultipleRegister:
	{
		length = HandleWriteMultipleRegisterCommand(startAddress, quantity, receivedMessage, respondBytes);
	}
	break;
	default:
		break;
	}

	return length;
}

uint16_t __stdcall CMyComObject::CreateAccessServerCommand(ObjAccessInfo objInfo, void* dataList, uint8_t* commandBytes)
{
	return uint16_t();
}

void __stdcall CMyComObject::ParsingServerRespondMessage(uint8_t* recievedMessage, void* dataList)
{
}

uint16_t __stdcall CMyComObject::ParsingMasterAccessCommand(uint8_t* receivedMesasage, uint8_t* respondBytes, uint16_t rxLength)
{
	return uint16_t();
}

uint16_t __stdcall CMyComObject::CreateAccessSlaveCommand(ObjAccessInfo objInfo, void* dataList, uint8_t* commandBytes)
{
	return uint16_t();
}

void __stdcall CMyComObject::ParsingSlaveRespondMessage(uint8_t* recievedMessage, uint8_t* command)
{
}
