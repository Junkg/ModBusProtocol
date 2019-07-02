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


/*����Modbus�Ĳ��������룬֧��01��02��03��04��05��06��15��16������*/
typedef enum {
	ReadCoilStatus = 0x01,       	/*����Ȧ״̬����������λ��״̬��*/
	ReadInputStatus = 0x02,      	/*������λ״̬�����������λ��״̬��*/
	ReadHoldingRegister = 0x03,  	/*�����ּĴ�������������ּĴ�������ֵ��*/
	ReadInputRegister = 0x04,    	/*������Ĵ��������������Ĵ�������ֵ��*/
	WriteSingleCoil = 0x05,      	/*ǿ�Ƶ�����Ȧ��ǿ�Ƶ������λ��״̬��*/
	WriteSingleRegister = 0x06,  	/*Ԥ�Ƶ����Ĵ������趨һ���Ĵ�������ֵ��*/
	WriteMultipleCoil = 0x0F,    	/*ǿ�ƶ����Ȧ��ǿ�ƶ�����λ��״̬��*/
	WriteMultipleRegister = 0x10 	/*Ԥ�ƶ���Ĵ������趨����Ĵ�������ֵ��*/
} FunctionCode;

/*������յ�ָ�������ʱ�Ĵ����� �쳣��*/
typedef enum {
	MODBUS_OK = 0x00,
	InvalidFunctionCode = 0x01,//���Ϸ����ܴ���
	IllegalDataAddress = 0x02,//�Ƿ������ݵ�ַ
	IllegalDataValues = 0x03,//�Ƿ�������ֵ���߷�Χ
	OperationFail = 0x04
}ModbusStatus;


/*�������ڴ���Ҫ���ʴ�վ��������������Ϣ*/
typedef struct {
	uint8_t unitID;
	FunctionCode functionCode;
	uint16_t startingAddress;
	uint16_t quantity;
}ObjAccessInfo;


//��������Modbus�Ĵ������ݴ洢�Ľṹ��
typedef struct rnode
{
	uint16_t index;     	//�������
	uint8_t hiByte;     	//����ֵ���ֽ�
	uint8_t loByte;		//����ֵ���ֽ�
	struct rnode* next; 	//��һ���ڵ�
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

//��������Modbus״̬�����ݴ洢�Ľṹ��
typedef struct snode
{
	uint16_t index;     	//�������
	uint8_t statusByte;     //���ֽڵķ�ʽ�洢״̬��
	struct snode* next; 	//��һ���ڵ�
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


//�������ݶ���0��Ȧ��1����״̬��3����Ĵ�����4���ּĴ���
typedef enum DataObjectType {
	Coil = 0,				//��Ȧ���󣨶�д��
	InputStatus = 1,        //����״̬(ֻ��)
	InputRegister = 2,		//����Ĵ���(ֻ��)
	HoldingRegister = 3		//���ּĴ�������д��
}DataObjectType;


//����Ĵ������Ķ���洢�ṹ��������ʼ�ڵ�ĵ�ַ�����ݶ������ʼ��ַ���ܵ�����
typedef struct {
	enum DataObjectType type;
	uint16_t startingAddress;
	uint16_t quantity;
}DataObject;

//����״̬���Ķ���洢�ṹ��������ʼ�ڵ�ĵ�ַ�����ݶ������ʼ��ַ���ܵ�����
typedef struct StatusObject {
	StatusNode* startNode; //�洢��һ��Node λ��
	uint16_t startingAddress;
	uint16_t quantity;
	StatusObject()
	{
		startNode = NULL;
		startingAddress = 0;
		quantity = 0;
	}
}StatusObject;

//����Ĵ������Ķ���洢�ṹ��������ʼ�ڵ�ĵ�ַ�����ݶ������ʼ��ַ���ܵ�����
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