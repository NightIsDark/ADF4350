#include "key.h"
#include "serial.h"
#include "ADF4350.h"
#include "F300_FlashPrimitives.h"
#include "F300_FlashUtils.h"

typedef enum 
{
	CMD_ERROR,
	PAR_ERROR,
	STF_CMD,
	GTF_CMD,
}MESSAGE_T;

extern void Init_Device(void);
void DataProcess(void);
void ReportMessage(MESSAGE_T type);


FLADDR code FlashAddr	=	0x1000;
U32 frequecy	=	0;

void delay_ms(U16 ms)
{
	U16 i;
	while(ms)
	{
		for(i=0;i<1000;i++);
		ms--; 
	}
}
int main(void)
{
	Init_Device();		
	InitSerial();
	FLASH_Read((char *)&frequecy,FlashAddr,4);	
	if(frequecy	==	0)
	{
		if((P0 & 0x80)?1:0)
		{
			frequecy	=	100000000; //1000M
		}
		else
		{
			frequecy	=	200000000; //2000M			
		}
	}
	set_pll_reg(frequecy);
	ReportMessage(GTF_CMD);
	while(1)
{
		DataProcess();
		delay_ms(1000);
	  
		if((P0 & 0x40)?1:0)
		{
			P0 &= 0xbf;
		}
		else
		{
			P0|= 0x40;
		}	
		
		if((P0 & 0x80)?1:0)
		{
			P0 &= 0x7F;
		}
		else
		{
			P0|= 0x80;
		}
	}
}
void ReportMessage(MESSAGE_T type)
{
	U32 tmp = 0;
	unsigned char i;
	tmp =	frequecy;
	switch(type)
	{
		case CMD_ERROR:
		{
			SendFrame[0]=SOF;
			SendFrame[1]='E';
			SendFrame[2]='R';
			SendFrame[3]='R';
			SendFrame[4]='O';
			SendFrame[5]='R';	
			SendFrame[6]='1';	
			SendFrame[7]='!';				
			SendFrame[8]=EOF;
			SendCount=0;
			TotalSendCount=9;
			SCON0 |=0x02;	
			break;
		}
		case PAR_ERROR:
		{
			SendFrame[0]=SOF;
			SendFrame[1]='E';
			SendFrame[2]='R';
			SendFrame[3]='R';
			SendFrame[4]='O';
			SendFrame[5]='R';	
			SendFrame[6]='2';	
			SendFrame[7]='!';				
			SendFrame[8]=EOF;
			SendCount=0;
			TotalSendCount=9;
			SCON0 |=0x02;	
			break;
		}
		case 	STF_CMD:
		{
			SendFrame[0]=SOF;
			SendFrame[1]='S';
			SendFrame[2]='T';
			SendFrame[3]='F';
			SendFrame[4]=':';
			SendFrame[5]='O';	
			SendFrame[6]='K';	
			SendFrame[7]='!';				
			SendFrame[8]=EOF;
			SendCount=0;
			TotalSendCount=9;
			SCON0 |=0x02;	
			break;
		}
		case 	GTF_CMD:
		{
			SendFrame[0]=SOF;
			SendFrame[1]='F';
			SendFrame[2]='R';
			SendFrame[3]='Q';
			SendFrame[4]=':';
			for(i = 13;i>4;i--)
			{				
				SendFrame[i]	=	tmp%10 + 0x30;		
				tmp	/=	10;
			}	
			SendFrame[14]=0x30;			
			SendFrame[15]=EOF;
			SendCount=0;
			TotalSendCount=16;
			SCON0 |=0x02;	
			break;
		}
		default:
			break;
	}
}
void DataProcess(void)
{
	U32 freq=0;
	U8 i	=	0;
	if(bReceiveData)
	{
		bReceiveData	=	0;
		if((ReceiveFrame[1]=='S')&&(ReceiveFrame[2]=='T')&&(ReceiveFrame[3]=='F'))  //set the frequecy
		{
			for(i=5;i<13;i++)
			{
				freq	+=	ReceiveFrame[i]-0x30;
				freq  *=	10;
			}
			freq 	+=	(ReceiveFrame[13]-0x30);
			frequecy	=	freq;
			FLASH_Update(FlashAddr,(char *)&frequecy,4);	
			set_pll_reg(frequecy);
			ReportMessage(STF_CMD);			
		}
		else if((ReceiveFrame[1]=='G')&&(ReceiveFrame[2]=='T')&&(ReceiveFrame[3]=='F'))  //get the frequecy
		{
			//todo
			ReportMessage(GTF_CMD);
		}
		else
		{
			//error processing
			ReportMessage(CMD_ERROR);
		}	
	}
}
