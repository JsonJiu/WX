#include "Mydefine.h"
void Delay_us(unsigned int us)
{
  unsigned int i;
  us=us/2;
  for(i=0;i<us;i++)
  {
    NOP();
  }
}
void Delay_ms(unsigned int ms)
{
  unsigned int i,j;
  for(i=0;i<ms;i++)
  {

		for(j=0;j<950;j++)
    {			
       NOP();
    }
  }
}