#include <cstddef>
#ifndef __MODBUS_H__
#define __MODBUS_H__
#pragma once

#ifndef uint8_t 
typedef unsigned char  uint8_t; 
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif


/*定义Modbus的操作功能码，支持01、02、03、04、05、06、15、16功能码*/
typedef enum {
	ReadCoilStatus = 0x01,       	/*读线圈状态（读多个输出位的状态）*/
	ReadInputStatus = 0x02,      	/*读输入位状态（读多个输入位的状态）*/
	ReadHoldingRegister = 0x03,  	/*读保持寄存器（读多个保持寄存器的数值）*/
	ReadInputRegister = 0x04,    	/*读输入寄存器（读多个输入寄存器的数值）*/
	WriteSingleCoil = 0x05,      	/*强制单个线圈（强制单个输出位的状态）*/
	WriteSingleRegister = 0x06,  	/*预制单个寄存器（设定一个寄存器的数值）*/
	WriteMultipleCoil = 0x0F,    	/*强制多个线圈（强制多个输出位的状态）*/
	WriteMultipleRegister = 0x10 	/*预制多个寄存器（设定多个寄存器的数值）*/
} FunctionCode;

/*定义接收到指令检测错误时的错误码 异常码*/
typedef enum {
	MODBUS_OK = 0x00,
	InvalidFunctionCode = 0x01,//不合法功能代码
	IllegalDataAddress = 0x02,//非法的数据地址
	IllegalDataValues = 0x03,//非法的数据值或者范围
	OperationFail = 0x04
}ModbusStatus;


/*定义用于传递要访问从站（服务器）的信息*/
typedef struct {
	uint8_t unitID;
	FunctionCode functionCode;
	uint16_t startingAddress;
	uint16_t quantity;
}ObjAccessInfo;


//定义用于Modbus寄存器数据存储的结构体
typedef struct rnode
{
	uint16_t index;     	//参数编号
	uint8_t hiByte;     	//数据值高字节
	uint8_t loByte;		//数据值低字节
	struct rnode* next; 	//下一个节点
	rnode()
	{
		index = 0;
		hiByte = 0;
		loByte = 0;
		next = NULL;
	}
	rnode(uint16_t idex, uint8_t hByte, uint8_t lByte, rnode* pNext)
	{
		index = idex;
		hiByte = hByte;
		loByte = lByte;
		next = pNext;
	}
}RegisterNode;

//定义用于Modbus状态量数据存储的结构体
typedef struct snode
{
	uint16_t index;     	//参数编号
	uint8_t statusByte;     //以字节的方式存储状态量
	struct snode* next; 	//下一个节点
	snode()
	{
		index = 0;
		statusByte = 0;
		next = NULL;
	}
	snode(uint16_t idex, uint8_t status, snode* pNext)
	{
		index = idex;
		statusByte = status;
		next = pNext;
	}
}StatusNode;


//定义数据对象，0线圈，1输入状态，3输入寄存器，4保持寄存器
typedef enum DataObjectType {
	Coil = 0,				//线圈对象（读写）
	InputStatus = 1,        //输入状态(只读)
	InputRegister = 2,		//输入寄存器(只读)
	HoldingRegister = 3		//保持寄存器（读写）
}DataObjectType;


//定义寄存器量的对象存储结构，包括初始节点的地址，数据对象的起始地址，总的数量
typedef struct {
	enum DataObjectType type;
	uint16_t startingAddress;
	uint16_t quantity;
}DataObject;

//定义状态量的对象存储结构，包括初始节点的地址，数据对象的起始地址，总的数量
typedef struct StatusObject {
	StatusNode* startNode; //存储第一个Node 位置
	uint16_t startingAddress;
	uint16_t quantity;
	StatusObject()
	{
		startNode = NULL;
		startingAddress = 0;
		quantity = 0;
	}
}StatusObject;

//定义寄存器量的对象存储结构，包括初始节点的地址，数据对象的起始地址，总的数量
typedef struct RegisterObject {
	RegisterNode* startNode;
	uint16_t startingAddress;
	uint16_t quantity;
	RegisterObject()
	{
		startNode = NULL;
		startingAddress = 0;
		quantity = 0;
	}
}RegisterObject;




#endif //__MODBUS_H__