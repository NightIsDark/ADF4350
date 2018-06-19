/*----------------------------------------------------------------------
  File Name          : ADF4350_IO.h 
  Author             : Mariah Nie - CAST
  Version            : V1.0
  Date               : 12/6/2009
  Description        : ADF4350 Driver Declaration



  China Applications Support Team.
  china.support@analog.com

----------------------------------------------------------------------

  The present firmware which is for guidance only aims at providing
  customers with coding information regarding their products in order
  for them to save time.  As a result, Analog Devices shall not be
  held liable for any direct, indirect or consequential damages with
  respect to any claims arising from the content of such firmware and/or
  the use made by customers of the coding information contained herein
  in connection with their products.

----------------------------------------------------------------------*/

#ifndef AD4530_IO_H
#define AD4530_IO_H
//---------------------------------
//void WriteToADF4350(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that writes to the ADF4350 via the SPI port. 
//--------------------------------------------------------------------------------
void WriteToADF4350(unsigned char count, unsigned char *buf);

//---------------------------------
//void ReadFromADF4350(unsigned char count,unsigned char *buf)
//---------------------------------
//Function that reads from the ADF4350 via the SPI port. 
//--------------------------------------------------------------------------------
void ReadFromADF4350(unsigned char count, unsigned char *buf);

#endif