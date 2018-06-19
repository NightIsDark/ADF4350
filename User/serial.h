#ifndef SERIAL_H
#define SERIAL_H

#include "compiler_defs.h"
#include "C8051F300_defs.h"

void InitSerial(void);

//extern U8 xdata CMD_ACK[10];   
////extern const U8 CMD_NAK[10]; 
//extern U8 xdata CMD_STF[10];
//extern U8 xdata CMD_GTF[10];
extern U8 data ReceiveFrame[20];  	//����Ϊ״̬,����,
extern U8 data SendFrame[20];  			//����Ϊ״̬,����,
extern U8 data SendCount;     				//�����ֽ���
extern U8 data TotalSendCount;				//��������
extern U8 data ReceiveCount; 		 		//�����ֽ���
extern U8 data TotalReceiveCount; 		//�����ֽ���
extern bit   bReceiveData;  //���յ����� 
#define SOF '$'
#define EOF '#'
#endif
