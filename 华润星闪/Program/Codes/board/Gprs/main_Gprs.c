#include "timer_manage.h"
#include "user_manage.h"
#include "task_manage.h"
#include "include.h"
#include "lib_rs004.h"
//在两个Start和End之间写代码   
void main(void)
{
   MCU_IntEn();
   User_Init();
   /* Start user code. Do not edit comment generated here */
   //SystemSelfTest();  //建议在此增加一个执行自检的程序
	//申请任务ID
	
	
	//注册
   St_SAVESTRUCT temp;
   temp.fun = DataSvrDefault;
   temp.data = (unsigned char *)&Meter;
   temp.len = sizeof(Meter);
   DataSvrReg(0, 20, &temp, (St_RegTxt *)&MeterRegTag);
   
   DataSvrReadAll();

   while(1)
     SettingTask();
   Example();           //例程
    /* End user code. Do not edit comment generated here */
   while (1U)
   {
      User_TimerManage();
      User_TaskManage();
      
      /* Start user code. Do not edit comment generated here */
      //User_Manage();
      /* End user code. Do not edit comment generated here */
      LowPowerConsManage();
   }
   
}



