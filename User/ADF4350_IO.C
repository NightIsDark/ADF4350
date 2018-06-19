/***************************************************************************

 Author        : Mariah Nie - CAST

 Date          : Jun 12th, 2009

 File          : ADF4350_IO.c

 Hardware      : ADuC7026 and ADF4350

 Description   : Use the GPIO to simulate the SPI communication of ADF4350
	
***************************************************************************/
#include "ADF4350_IO.h"
#include "compiler_defs.h"
#include "C8051F300_defs.h"

#define SET_LE()		(P0 |= 0x04)         //P0.2->LE
#define CLR_LE()		(P0 &= 0xFB)

#define	SET_SCL()		(P0 |= 0x02)         //P0.1->SCL
#define	CLR_SCL()		(P0 &= 0xFD)

#define SET_DATA()	(P0 |= 0x08)         //P0.3->DATA
#define CLR_DATA()	(P0 &= 0xF7)

#define READ_DATA()	((P0 & 0x04)?1:0)    


#define SET_DATA_OUT()	NOP() 
#define SET_DATA_IN()	  NOP()

void delay (int length)
{
	while (length >0)
    	length--;
}

//---------------------------------
//void WriteToADF4350(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that writes to the ADF4350 via the SPI port. 
//--------------------------------------------------------------------------------
void WriteToADF4350(unsigned char count, unsigned char *buf)
{
	unsigned	char	ValueToWrite = 0;
  unsigned	char	i = 0;
	unsigned	char	j = 0;
	
	SET_DATA_OUT();
	
	delay(4);
	CLR_SCL();
	CLR_LE();
	delay(4);

	for(i=count;i>0;i--)
 	{
	 	ValueToWrite = *(buf + i - 1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				SET_DATA();	  //Send one to SDO pin
			}
			else
			{
				CLR_DATA();	  //Send zero to SDO pin
			}
			delay(4);
			SET_SCL();
			delay(4);
			ValueToWrite <<= 1;	//Rotate data
			CLR_SCL();
		}
	}
	CLR_DATA();
	delay(4);
	SET_LE();
	delay(4);
//	CLR_LE();
}


/*
//---------------------------------
void ReadFromADF4350(unsigned char count,unsigned char *buf)
---------------------------------
Function that reads from the ADF4350 via the SPI port. 
--------------------------------------------------------------------------------
void ReadFromADF4350(unsigned char count, unsigned char *buf)
{
	unsigned	char	i = 0;
	unsigned	char	j = 0;
	unsigned	int  	iTemp = 0;
	unsigned	char  	RotateData = 0;

	SET_DATA_IN();
	
	delay(4);
	CLR_SCL();
	CLR_LE();
	delay(4);

	for(j=count; j>0; j--)
	{
		for(i=0; i<8; i++)
		{
			RotateData <<= 1;		//Rotate data
			delay(4);           //Read DATA of ADF4350
			if(READ_DATA())
			{
				RotateData |= 1;	
			}			
			SET_SCL();	
			delay(4);
			CLR_SCL();
		}
		*(buf + j - 1)= RotateData;
	}	 
	delay(4);
	SET_LE();
	delay(4);
	CLR_LE();
} 

*/