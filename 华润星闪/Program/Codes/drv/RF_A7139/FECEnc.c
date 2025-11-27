#include "string.h"


unsigned short fecEncodeTable[] = {
    0, 3, 1, 2,
    3, 0, 2, 1,
    3, 0, 2, 1,
    0, 3, 1, 2   
};


//unsigned short input[20] = {0x07, 0x56, 0x78, 0x00, 0x00, 0x1B, 0x10};//{0x0A, 0x00, 0x0B, 0xFE, 0x12, 0x34, 0x56, 0x78, 0xdc, 0x35}; 
//unsigned short input[20] = {0x03, 0x01, 0x02, 0x03, 0x30, 0x3A};
//unsigned char input[] = {0x01, 0x02, 0x03, 0x04, 0x05};

unsigned long intOutput;
unsigned short length;
unsigned char FECEnc(unsigned char * input, unsigned char len)
{
  unsigned char fec[100];
  unsigned short inputNum = 0, fecNum;
  unsigned short i, j, fecReg, fecOutput;
  unsigned char k;
  inputNum = len;
  memset(fec, 0, 30);
  input[inputNum] = 0x0B;
  input[inputNum + 1] = 0x0B;
  //inputNum += 2;
  
  fecNum = 2*((inputNum / 2) + 1);
  
  // FEC encode
  fecReg = 0; 
  for (i = 0; i < fecNum; i++) {
      fecReg = (fecReg & 0x700) | (input[i] & 0xFF);
      fecOutput = 0;
      for (j = 0; j < 8; j++) {
          fecOutput = (fecOutput << 2) | fecEncodeTable[fecReg >> 7];
          fecReg = (fecReg << 1) & 0x7FF;
      }
      fec[i * 2] = fecOutput >> 8;
      fec[i * 2 + 1] = fecOutput & 0xFF;
  }  


  for (i = 0; i < fecNum * 2; i += 4) {
    intOutput = 0;
    for (j = 0; j < 4*4; j++)
    {
      k = ~j;
        intOutput = (intOutput << 2) | ((fec[i +(k & 0x03)] >> (2 * ((j & 0x0C) >> 2))) & 0x03);      
    }
    input[i]      = (intOutput >> 24) & 0xFF;
    input[i + 1]  = (intOutput >> 16) & 0xFF;
    input[i + 2]  = (intOutput >> 8)  & 0xFF;
    input[i + 3]  = (intOutput >> 0)  & 0xFF;
  }
  return fecNum * 2;
}



