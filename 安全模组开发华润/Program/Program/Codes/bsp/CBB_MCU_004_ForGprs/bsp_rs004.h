#include 	"r_cg_macrodriver.h"
#include    "lib_rs004.h"

//定义各个IO口
#define IO_BEEP_DIR_OUT      PORT4OutputConfig(BIT7);PFSEG3ClearBit(BIT4)
#define IO_BEEP_1            PORT4SetBit(BIT7)
#define IO_BEEP_O            PORT4ClearBit(BIT7)

#define IO_M_SLEEP_DIR_OUT          PORT0OutputConfig(BIT3);PFSEG5ClearBit(BIT7)
#define IO_M_SLEEP_1                PORT0SetBit(BIT3)
#define IO_M_SLEEP_0                PORT0ClearBit(BIT3)
#define IO_M_CTRL1_DIR_OUT          PORT12OutputConfig(BIT6)
#define IO_M_CTRL1_1                PORT12SetBit(BIT6)
#define IO_M_CTRL1_0                PORT12ClearBit(BIT6)
#define IO_M_CTRL2_DIR_OUT          PORT12OutputConfig(BIT7)
#define IO_M_CTRL2_1                PORT12SetBit(BIT7)
#define IO_M_CTRL2_0                PORT12ClearBit(BIT7)

#define IO_HALL1_READ               PORT3ReadBit(BIT1)
#define IO_HALL2_READ               PORT13ReadBit(BIT7)
#define IO_KEY_USER_READ                 PORT0ReadBit(BIT2)
#define IO_KEY_PROM_READ                 PORT3ReadBit(BIT3)

#define IO_VALVE_CHK_POW_DIR_OUT    PORT0OutputConfig(BIT5);PFSEG6ClearBit(BIT1)
#define IO_VALVE_CHK_POW_1          PORT0SetBit(BIT5)
#define IO_VALVE_CHK_POW_0          PORT0ClearBit(BIT5)
#define IO_VALVE_IN_DIR_IN          PORT0InputConfig(BIT4);PFSEG6ClearBit(BIT0)
#define IO_VALVE_IN_READ            PORT0ReadBit(BIT4)
#define IO_GD_EN_DIR_OUT            PORT4OutputConfig(BIT6);PFSEG3ClearBit(BIT3)
#define IO_GD_EN_DIR_IN             PORT4InputConfig(BIT6);PFSEG3ClearBit(BIT3)
#define IO_GD_EN_1                  PORT4SetBit(BIT6)
#define IO_GD_EN_0                  PORT4ClearBit(BIT6)
#define IO_GD_EN_PullUp             PORT4PullUpBit(BIT6)

#define IO_EEP_POW_DIR_OUT          PORT1OutputConfig(BIT4);PFSEG5ClearBit(BIT0)
#define IO_EEP_POW_1                PORT1SetBit(BIT4)
#define IO_EEP_POW_0                PORT1ClearBit(BIT4)
#define IO_EEP_POW_PullUp           PORT1PullUpBit(BIT4)
#define IO_EEP_IIC_SCL_DIR_OUT      PORT1OutputConfig(BIT6);PFSEG5ClearBit(BIT2)
#define IO_EEP_IIC_SCL_1            PORT1SetBit(BIT6)
#define IO_EEP_IIC_SCL_0            PORT1ClearBit(BIT6)
#define IO_EEP_IIC_SDA_DIR_OUT      PORT1OutputConfig(BIT5);PFSEG5ClearBit(BIT1)
#define IO_EEP_IIC_SDA_DIR_IN       PORT1InputConfig(BIT5);PFSEG5ClearBit(BIT1)
#define IO_EEP_IIC_SDA_1            PORT1SetBit(BIT5) 
#define IO_EEP_IIC_SDA_0            PORT1ClearBit(BIT5)
#define IO_EEP_IIC_SDA_READ         PORT1ReadBit(BIT5)

//#define IO_VBB_CHK_EN_DIR_OUT       PORT12OutputConfig(BIT5)
//#define IO_VBB_CHK_EN_1             PORT12SetBit(BIT5)
//#define IO_VBB_CHK_EN_0             PORT12ClearBit(BIT5)

#define R_LED_OUT                   PORT1OutputConfig(BIT2);PFSEG4ClearBit(BIT6);PMC1ClearBit(BIT2)
#define R_LED_0                     PORT1SetBit(BIT2) 
#define R_LED_1                     PORT1ClearBit(BIT2)
#define G_LED_OUT                   PORT1OutputConfig(BIT3);PFSEG4ClearBit(BIT7);PMC1ClearBit(BIT3)
#define G_LED_0                     PORT1SetBit(BIT3) 
#define G_LED_1                     PORT1ClearBit(BIT3)

