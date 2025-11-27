#include "lib_public.h"
#include "drv_ad.h"

/**
 * @declare 获取ADC转换结果
 * 
 * @author ZYX (2014/11/9)
 *
 * requirements 
 * 
 * @param AdcChannel ADC转换源
 * 
 * @return unsigned short ADC转换结果
 */
unsigned short OA_ADC_GetValue(ADCChannel_ET AdcChannel)
{
    unsigned short TempAd = 0;
    if( AdcChannel == ADCCHANNEL_ONLINE )
    {
        IO_VBB_CHK_EN_DIR_OUT();
        IO_VBB_CHK_EN_1();
        OA_ADC_SetConversionChannel(ADCCHANNEL_ONLINE);
    }
    else  if( AdcChannel == ADCCHANNEL_RTC )
        OA_ADC_SetConversionChannel(ADCCHANNEL_RTC);
    else if(AdcChannel == ADCCHANNEL_BAT)
        OA_ADC_SetConversionChannel(ADCCHANNEL_BAT);

    else
        OA_ADC_SetConversionChannel(ADCCHANNEL_3V3);

    OA_ADC_CMD(ENABLE);
    TempAd = OA_ADC_GetConversionValue();
    OA_ADC_CMD(DISABLE);
    return (TempAd);
}
