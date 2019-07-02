#ifndef __MBINTERFACE_H__
#define __MBINTERFACE_H__
#pragma once
#ifndef __unknwn_h__
#include <Unknwn.h>
#endif 

#include "ModBus.h"


// {61A3E0F7-E657-4F1F-8453-DD2C7EF2E934}
extern "C" const  IID  IID_ModBusTCPServer =
{ 0x61a3e0f7, 0xe657, 0x4f1f, { 0x84, 0x53, 0xdd, 0x2c, 0x7e, 0xf2, 0xe9, 0x34 } };
class IModBusTCPServer : public IUnknown
{
public:
	/*解析接收到的信息，返回响应命令的长度*/
	virtual uint16_t  ParsingClientAccessCommand(uint8_t* receivedMessage, uint8_t* respondBytes) = 0;
};

// {866560FE-7648-4135-A3BC-F0EB0C243215}
extern "C" const IID  IID_ModBusTCPClient =
{ 0x866560fe, 0x7648, 0x4135, { 0xa3, 0xbc, 0xf0, 0xeb, 0xc, 0x24, 0x32, 0x15 } };
class IModBusTCPClient : public IUnknown
{
public:
	/*生成访问服务器的命令*/
	virtual uint16_t CreateAccessServerCommand(ObjAccessInfo objInfo, void* dataList, uint8_t* commandBytes) =0 ;

	/*解析收到的服务器相应信息*/
	virtual void ParsingServerRespondMessage(uint8_t* recievedMessage, void* dataList) = 0 ;
};

// {8A6EEDFB-9484-4C9E-A020-673EECC8D481}
extern "C" const IID  IID_ModBusRTUServer =
{ 0x8a6eedfb, 0x9484, 0x4c9e, { 0xa0, 0x20, 0x67, 0x3e, 0xec, 0xc8, 0xd4, 0x81 } };

class IModBusRTUServer :  public IUnknown
{
public:
	/*解析接收到的信息，并返回合成的回复信息和信息的字节长度，通过回调函数*/
	virtual uint16_t ParsingMasterAccessCommand(uint8_t* receivedMesasage, uint8_t* respondBytes, uint16_t rxLength)= 0;

};

// {C0B2A2CE-FC68-4CBD-AFD9-026DEBAF6D76}
extern "C" const IID  IID_ModBusRTUClient =
{ 0xc0b2a2ce, 0xfc68, 0x4cbd, { 0xaf, 0xd9, 0x2, 0x6d, 0xeb, 0xaf, 0x6d, 0x76 } };
class IModBusRTUClient :  public IUnknown
{
public:

	/*生成访问服务器的命令*/
	virtual uint16_t CreateAccessSlaveCommand(ObjAccessInfo objInfo, void* dataList, uint8_t* commandBytes) = 0 ;

	/*解析收到的服务器相应信息*/
	virtual void ParsingSlaveRespondMessage(uint8_t* recievedMessage, uint8_t* command) = 0 ;

};

#endif // __MBINTERFACE_H__
