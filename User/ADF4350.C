#include "ADF4350.h"
/***************************************************************************
 @Function    	: 	
 @para         	:	 	the frequecy mutiply 10000  
 @return       	:	 	none
 @Description   :		VCO Divider is Outside loop and set as follows: 
										Start Freq	Stop Freq	VCO Divider	Channel Spacing 
										140MHz	 275MHz	16	62.5kHz 
										275MHz	 550MHz	8	 125kHz 
										550MHz	1.10GHz	4	 250kHz 
										1.10GHz	2.20GHz	2	 500kHz 
										2.20GHz	4.40GHz	1	1.00MHz 
***************************************************************************/


#define REF_F		(2600000)			//26MHz
#define MOD			(26)						//26
#define FREQ_A  (14000000)			//140MHz
#define FREQ_B  (27481250)   	//274.8125MHz
#define FREQ_C  (54962500)			//549.6250MHz
#define FREQ_D  (109925000)  	//1099.2500MHz
#define FREQ_E  (219850000)		//2198.5MHz
#define FREQ_F  (440000000)		//4400MHz

U8 get_vco_divider(U32 freq)
{
	U8 ret=0;
	if(freq<FREQ_A)
	{
		ret	=	0;
	}
	else if(freq<=FREQ_B)
	{
		ret = 16;
	}
	else if(freq<=FREQ_C)
	{
		ret = 8;
	}
	else if(freq<=FREQ_D)
	{
		ret = 4;
	}
	else if(freq<=FREQ_E)
	{
		ret = 2;
	}
	else if(freq<=FREQ_F)
	{
		ret = 1;
	}
	else
	{
		ret =0;
	}
	return ret;	
}
void set_pll(U8 vco_div,U16 int_value,U16 frac_value)
{
	U8 temp = 0;
	U8 buf[4];
	buf[3] = 0x00;
	buf[2] = 0x40;        //LD PIN Mode 01 DIGITAL LOCK DETECT
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress 
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);		

	buf[3] = 0x00;
	if(vco_div 	== 16)
	{
		buf[2] = 0xC1;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:1H)the RF divider is 2;(DB19-12:D8H)27
	}
	else if(vco_div 	== 8)
	{
		buf[2] = 0xB1;
	}
	else if(vco_div 	== 4)
	{
		buf[2] = 0xA1;
	}
	else if(vco_div 	== 2)
	{
		buf[2] = 0x91;
	}
	else if(vco_div 	== 1)
	{
		buf[2] = 0x81;
	}
	else
	{
		while(1);
	}
	buf[1] = 0xB0;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);
		
	buf[3] = 0x00;
	buf[2] = 0x00;				
	buf[1] = 0x04;				//CLOCK DIVIDER OFF
 	buf[0] = 0xB3;				
	WriteToADF4350(4,buf);	
	
	
	buf[3] = 0x04;
	buf[2] = 0x00;
	buf[1] = 0x5f;				//(DB14-3:96H)clock divider value is 1.
 	buf[0] = 0xc2;
	WriteToADF4350(4,buf);	

	buf[3] = 0x08;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x80;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0xD1;			//MOD =26	//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1
   

	buf[3] = int_value/256;
	buf[2] = ((int_value>>1)&0x00FF);
	buf[1] = ((frac_value>>5)&0x001f);
  if(int_value&0x0001)
	{
		buf[1]	|=0x80;
	}
	else
	{
		buf[1]	&=0x7f;
	}
 	buf[0] = ((frac_value<<3)&0x00f8); 
	WriteToADF4350(4,buf);	 
}
U8 set_pll_reg(U32 freq)
{
	U8  div = 0;
	U16 int_value		=	0;
	U32 frac_value	=	0;	
	U32 undiv_freq	=	0;
	
	div	=	get_vco_divider(freq);
	if(div ==0)
	{
		return 0;
	}
	else
	{
		//RFOUT = [INT + (FRAC/MOD)] * [fPFD]/RF divider
		undiv_freq	=	div * freq;
		int_value 	=	(U16)(undiv_freq/REF_F);
		frac_value	=	undiv_freq%REF_F;
		frac_value  = (frac_value/10000+5)/10; //Integer-valued
		set_pll(div,int_value,frac_value);	
	}
	return 1;
}
/*
void SetPllTo1000(void)
{
	U8 buf[4];
	buf[3] = 0x00;
	buf[2] = 0x40;        //LD PIN Mode 01 DIGITAL LOCK DETECT
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress 
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);		

	buf[3] = 0x00;
	buf[2] = 0xA1;						//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 4;(DB19-12:D8H)27
	buf[1] = 0xB0;						//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;						//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);
		
	buf[3] = 0x00;
	buf[2] = 0x00;				
	buf[1] = 0x04;						//CLOCK DIVIDER OFF
 	buf[0] = 0xB3;				
	WriteToADF4350(4,buf);	
	
	
	buf[3] = 0x04;
	buf[2] = 0x00;
	buf[1] = 0x5f;						//(DB14-3:96H)clock divider value is 1.
 	buf[0] = 0xc2;
	WriteToADF4350(4,buf);	

	buf[3] = 0x08;
	buf[2] = 0x00;						//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x80;						//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0xD1;						//MOD =26	//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1
   

	buf[3] = 0x00;
	buf[2] = 0x4c;
	buf[1] = 0x80;			  
 	buf[0] = 0xb0;			  
	WriteToADF4350(4,buf);	 
}
*/