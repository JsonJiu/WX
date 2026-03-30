#ifndef _MCP4561_H_
#define _MCP4561_H_
#include "r_cg_macrodriver.h"
#include "MyDefine.h"

///////////////FengZhi
#define MCP4561_SCL_Out             (P2)
#define MCP4561_SCL_Port            (BIT7)
#define MCP4561_SCL_HI              (MCP4561_SCL_Out |=MCP4561_SCL_Port)
#define MCP4561_SCL_LO              (MCP4561_SCL_Out &=~MCP4561_SCL_Port)
#define MCP4561_SCL(a)              (a)?(MCP4561_SCL_HI):(MCP4561_SCL_LO)

#define MCP4561_SDA_DIR             (PM2)
#define MCP4561_SDA_Out             (P2)
#define MCP4561_SDA_Port            (BIT5)
#define MCP4561_SDA_OUT_Dir         (MCP4561_SDA_DIR &=~MCP4561_SDA_Port)
#define MCP4561_SDA_IN_Dir          (MCP4561_SDA_DIR |=MCP4561_SDA_Port)
#define MCP4561_SDA_HI              (MCP4561_SDA_Out |=MCP4561_SDA_Port)
#define MCP4561_SDA_LO              (MCP4561_SDA_Out &=~MCP4561_SDA_Port)
#define MCP4561_SDA(a)              (a)?(MCP4561_SDA_HI):(MCP4561_SDA_LO)
#define MCP4561_SDA_InPut           (MCP4561_SDA_Out&MCP4561_SDA_Port)
void MCP4561_IIC_Init();
unsigned char MCP4561_Write_Data(unsigned char Choice,unsigned int MCP_Data);
#endif