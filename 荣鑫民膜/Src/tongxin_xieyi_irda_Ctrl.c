#include "tongxin_xieyi_irda_Ctrl.h"
#include "GPIO.h"
#include "Test_List.h"
#include "uart1.h"
#include "uart4.h"
#include "utility.h"
//��������ź���ͨ��

uint8_t irad_xieyi_zuzhen[200];
uint8_t irad_kongshujuyu[1];
uint8_t irad_moren_biaohao[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

//����д�Ƚ��ַ�����Ҫ��ԭ���Ƚϻ�ʹ\0���ǽ�������
uint8_t irad_bijiao_zifuchuan(uint8_t bijiao1[], uint8_t bijiao2[],
                              uint16_t lenth) {
  uint16_t bijiao_lenth = 0;
  for (bijiao_lenth = 0; bijiao_lenth < lenth; bijiao_lenth++) {
    if (bijiao1[bijiao_lenth] != bijiao2[bijiao_lenth]) {
      return 0;
    }
  }
  return 1;
}

//��ͨ�ŵ�ַ  2031H  C=82H  ��������
//

void irad_TONGXIN_xieyijiexi(uint8_t zufuchua[], uint16_t lenth) {
  uint16_t pHead = 0;
  uint16_t zhen_lenth = 0;
  uint16_t CRC_jiaoyan = 0;
  uint16_t CRC_jiaoyan_jisuan = 0;
  uint16_t shujubiaoshi = 0;
  while (1) {
    //Э��֡������СΪ18
    if (pHead + 17 > lenth) {
      break;
    }
    if (zufuchua[pHead] == 0x68) {
      //ȷ������֡�����ԣ���ȡ֡����
      zhen_lenth = 0;
      zhen_lenth = zufuchua[pHead + 11];
      zhen_lenth = zhen_lenth << 8;
      zhen_lenth += zufuchua[pHead + 10];
      //������֡���Ȳ����򲻽������
      if (pHead + zhen_lenth <= lenth) {
        //У�����һ���ֽ��Ƿ���֡������
        if (zufuchua[pHead + zhen_lenth - 1] == 0x16) {
          //��ȡЭ���е�CRCУ��ֵ
          CRC_jiaoyan = 0;
          CRC_jiaoyan = zufuchua[pHead + zhen_lenth - 2];
          CRC_jiaoyan = CRC_jiaoyan << 8;
          CRC_jiaoyan += zufuchua[pHead + zhen_lenth - 3];
          //ִ��CRCУ��
          CRC_jiaoyan_jisuan =
              util_crc16_ccitt(&zufuchua[pHead], zhen_lenth - 3);
          // CRCУ��ͨ������ΪЭ������
          if (CRC_jiaoyan == CRC_jiaoyan_jisuan) {
            //�ȼ�¼��ַ�����ں�����Э��ͨ��
            memcpy(Test_jiejuo_jilu.dangqian_biaohao, &zufuchua[pHead + 1], 6);
            //Э������ѡ��
            switch (zufuchua[pHead + 7]) {
            case 0x00:
              //Э��汾��ѡ��
              switch (zufuchua[pHead + 8]) {
              case 0x00:
                //������У��
                switch (zufuchua[pHead + 9]) {
                //��������������־������
                //�ϴ����ݷ���(Ҳ��ֱ�ӻظ���ָ��)
                case 0x81:
                  break;
                //�����ݷ���
                case 0x82:
                  //�������ݱ�ʶ����
                  shujubiaoshi = 0;
                  shujubiaoshi = zufuchua[pHead + 13];
                  shujubiaoshi = shujubiaoshi << 8;
                  shujubiaoshi += zufuchua[pHead + 12];
                  // pHead+15
                  switch (shujubiaoshi) {
                  //��ȡ����
                  case 0x2031:
                    //������������
                    Test_quanju_canshu_L.time_softdelay_ms = 0;
                    test_xieyi_jilu_irda_rec = h_get_biaohao_chuankou;
                    break;
                  default:
                    break;
                  }
                  break;
                //����������
                case 0x83:
                  break;
                //д����
                case 0x84:
                  //�������ݱ�ʶ����
                  shujubiaoshi = 0;
                  shujubiaoshi = zufuchua[pHead + 13];
                  shujubiaoshi = shujubiaoshi << 8;
                  shujubiaoshi += zufuchua[pHead + 12];
                  // pHead+15
                  switch (shujubiaoshi) {
                  default:
                    break;
                  }
                  break;
                //Զ������
                case 0x86:
                  break;
                default:
                  break;
                }
                break;
              default:
                break;
              }
              break;
            default:
              break;
            }
            //Э�������ɺ󣬽���ͬһ֡��һ��Э�����
            pHead += zhen_lenth;
          }
        }
      }
    }
    pHead++;
  }
}
// 固定帧开销: 帧头(1) + 表号(6) + 协议类型(1) + 版本(1) + 控制码(1) +
//            帧长度(2) + 数据标识(2) + 消息序号(1) + CRC(2) + 帧尾(1) = 18字节
#define IRAD_FRAME_OVERHEAD 18
#define IRAD_MAX_DATA_LEN (sizeof(irad_xieyi_zuzhen) - IRAD_FRAME_OVERHEAD)

//数据标识 控制码 表号 数据域长度 帧长总长度
void irad_TONGXIN_xieyifasong(uint16_t shujubiaoshi, uint8_t kongzhima,
                              uint8_t w_biaohao[], uint8_t shujuyu[],
                              uint16_t shujuyu_changdu) {
  // 缓冲区溢出保护
  if (shujuyu_changdu > IRAD_MAX_DATA_LEN) {
    return;
  }

  uint16_t xieyi_zongchangdu = 0;
  uint16_t CRCjianyan = 0;
  //֡��ʼ��
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x68;
  //��ַ��
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[0];
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[1];
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[2];
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[3];
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[4];
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[5];
  //Э������
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
  //Э��汾��
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
  //������
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = kongzhima;
  //֡����
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = (shujuyu_changdu + 18) & 0xFF;
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = ((shujuyu_changdu + 18) >> 8) & 0xFF;
  //���ݱ�ʶ
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = (shujubiaoshi & 0xFF);
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = ((shujubiaoshi >> 8) & 0xFF);
  //��Ϣ��ţ���Զ��0
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
  //������
  memcpy(&irad_xieyi_zuzhen[xieyi_zongchangdu], shujuyu, shujuyu_changdu);
  xieyi_zongchangdu += shujuyu_changdu;
  //У����
  CRCjianyan = util_crc16_ccitt(irad_xieyi_zuzhen, xieyi_zongchangdu);
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = CRCjianyan & 0xFF;
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = (CRCjianyan >> 8) & 0xFF;
  //֡������
  irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x16;
  Uart4_Tx_Send_ok(irad_xieyi_zuzhen, xieyi_zongchangdu);
}
//��ȡ��ǰ����
void irad_find_biaohao_xieyi() {
  irad_TONGXIN_xieyifasong(0x2031, 0x02, irad_moren_biaohao, irad_kongshujuyu,
                           0);
}
