#include "jiliang_xieyi_Ctrl.h"
#include "GPIO.h"
#include "Test_List.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
//��������ź���ͨ��

uint8_t jiliang_xieyi_zuzhen[200];
uint8_t jiliang_kongshujuyu[1];
uint8_t jiliang_moren_biaohao[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

//��ͨ�ŵ�ַ  2031H  C=82H  ��������
//

void JILIAN_TONGXIN_xieyijiexi(uint8_t zufuchua[], uint16_t lenth) {
  uint16_t pHead = 0;
  uint16_t zhen_lenth = 0;
  uint8_t jiaoyanhe_jiaoyan = 0;
  uint8_t jiaoyanhe_jiaoyan_jisuan = 0;
  while (1) {
    //Э��֡������СΪ18
    if (pHead + 6 > lenth) {
      break;
    }
    if (zufuchua[pHead] == 0x68) {
      //ȷ������֡�����ԣ���ȡ֡����
      zhen_lenth = 0;
      zhen_lenth = zufuchua[pHead + 2];
      //������֡���Ȳ����򲻽������
      if (pHead + zhen_lenth <= lenth) {
        //У�����һ���ֽ��Ƿ���֡������
        if (zufuchua[pHead + zhen_lenth - 1] == 0x16) {
          //У��У����Ƿ�ΪԤ��У���
          jiaoyanhe_jiaoyan = 0;
          for (jiaoyanhe_jiaoyan_jisuan = 0;
               jiaoyanhe_jiaoyan_jisuan < zhen_lenth - 2;
               jiaoyanhe_jiaoyan_jisuan++) {
            jiaoyanhe_jiaoyan += zufuchua[pHead + jiaoyanhe_jiaoyan_jisuan];
          }
          //��У��ͨ������ΪЭ������
          if (jiaoyanhe_jiaoyan == zufuchua[pHead + zhen_lenth - 2]) {
            switch (zufuchua[pHead + 3]) {
            case 0:
              //��ˮ״̬
              test_xieyi_jilu_Rec = w_get_kongshui;
              break;
            case 1:
              //��ˮ׮��
              test_xieyi_jilu_Rec = w_get_manshui;
              break;
            case 2:
              //��ˮ״̬
              test_xieyi_jilu_Rec = w_get_zoushui;
              break;
            }
            //Э�������ɺ󣬽���֡��ǰ��
            pHead += zhen_lenth;
          }
        }
      }
    }
    pHead++;
  }
}
//���ݱ�ʶ ������ ���� ����������
//�����򳤶�
void JILIAN_TONGXIN_xieyifasong(uint8_t zhuangtai, uint8_t zoushuidaxiao) {
  uint16_t xieyi_zongchangdu = 0;
  uint16_t hejianyan_changdu = 0;
  //֡��ʼ��
  jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = 0x68;
  //��ַ��
  jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = 0xAA;
  //Э�鳤��
  jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = 0x07;
  //�����״̬ ��ˮ����ˮ����ˮ
  jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = zhuangtai;
  //ˮ����С
  jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = zoushuidaxiao;
  jiliang_xieyi_zuzhen[xieyi_zongchangdu] = 0;
  //У����
  for (hejianyan_changdu = 0; hejianyan_changdu < 5; hejianyan_changdu++) {
    jiliang_xieyi_zuzhen[xieyi_zongchangdu] +=
        jiliang_xieyi_zuzhen[hejianyan_changdu];
  }
  xieyi_zongchangdu++;
  //֡������
  jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = 0x16;
  protocol_debug_print(jiliang_xieyi_zuzhen, xieyi_zongchangdu);
  Uart3_Tx_Send(jiliang_xieyi_zuzhen, xieyi_zongchangdu);
}
//���ļ������� �չ�
void JILIAN_wushui_xieyi() { JILIAN_TONGXIN_xieyifasong(0, 1); }
//���ļ������� ��ˮ
void JILIAN_manshui_xieyi() { JILIAN_TONGXIN_xieyifasong(1, 1); }
//���ļ������� ��ˮ
void JILIAN_zoushui_xieyi() { JILIAN_TONGXIN_xieyifasong(2, 4); }
