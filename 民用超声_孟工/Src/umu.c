#include "umu.h"
#include "jiaoyan.h"
#include "uart4.h"
#include "test.h"

extern struct Test Test_result;

/*————————计量协议————————*/
uint8_t UMU_Agreement[18] = {0x02, 0x01, 0x86, 0xA0, 0x01, 0x08, 0x4C, 0x38, 0x01, 0x08, 0x4C, 0x38, 0x07, 0xD0, 0x1E, 0x1E, 0x03, 0xF1};

/*————————UMU接收数据解析————————*/
/*
计量协议发给主控板后会返回T 对应16进制0x54
正常来说板子上电后UMU的EXTI会动一下
但是由于测试时，如果生产误操作在开始测试后把板子拿走又重新开始测试
EXTI就会出现异常，故这边不测EXTI
*/
void UMU_Agreement_Analysis(uint8_t *Data, uint32_t lenth)
{
	uint8_t pHead = 0;
	while (1)
	{
		if (pHead + 1 > lenth)
			break;
		if (Data[pHead] == 0x54) //&& Test_result.UMU_EXTI == 1)
		{
			//Test_result.Agreement_receive = r_umu;
			Test_result.UMU = 1;
			//Test_result.UMU_EXTI = 0;
		}
		pHead++;
	}
}

/*————————UMU模拟协议发送————————*/
void UMU_Simulate(void)
{
	Uart4_Tx_Send(UMU_Agreement, 18);
}
