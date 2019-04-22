/***************(C)COPYRIGHT 2017 Vrunst������_WXD***************
*�ļ���         :ESP8266_AT_SEND_CMD.h
*����           :
*ʵ��ƽ̨       :ESP8266ģ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 2017.11.17
*��ע           :�����Ͻ���ѧϰ����������˽��������ҵ��;
                 �����������ʼ���ϵ��xhywxd@126.com
**************************************************************************/
#ifndef _ESP8266_AT_CMD
#define _ESP8266_AT_CMD
#include "ESP8266_task.h"
/*���ͺ����궨��*/
#include "uart4.h"
/*�궨��ESP8266�ķ��ͺ����ͷ��ͻ����������Ը���ʵ������޸�I��*/
#define ESP8266_sendByte  UART4_sendStringByDma
#define ESP8266_sendBuff  UART4_sendBuff
/*----------ATָ������궨��----------*/
#define	AT	0x00
#define	AT_RST	0x01
#define	AT_GMR	0x02
#define	AT_GSLP	0x03
#define	ATE	0x04
#define	AT_RESTORE	0x05
#define	AT_UART	0x06
#define	AT_UART_CUR	0x07
#define	AT_UART_DEF	0x08
#define	AT_SLEEP	0x09
#define	AT_WAKEUPGPIO	0x0A
#define	AT_RFPOWER	0x0B
#define	AT_RFVDD	0x0C
#define	AT_CWMODE	0x0D
#define	AT_CWMODE_CUR	0x0E
#define	AT_CWMODE_DEF	0x0F
#define	AT_CWJAP	0x10
#define	AT_CWJAP_CUR	0x11
#define	AT_CWJAP_DEF	0x12
#define	AT_CWLAPOPT	0x13
#define	AT_CWLAP	0x14
#define	AT_CWQAP	0x15
#define	AT_CWSAP	0x16
#define	AT_CWSAP_CUR	0x17
#define	AT_CWSAP_DEF	0x18
#define	AT_CWLIF	0x19
#define	AT_CWDHCP	0x1A
#define	AT_CWDHCP_CUR	0x1B
#define	AT_CWDHCP_DEF	0x1C
#define	AT_CWDHCPS_CUR	0x1D
#define	AT_CWDHCPS_DEF	0x1E
#define	AT_CWAUTOCONN	0x1F
#define	AT_CIPSTAMAC	0x20
#define	AT_CIPSTAMAC_CUR	0x21
#define	AT_CIPSTAMAC_DEF	0x22
#define	AT_CIPAPMAC	0x23
#define	AT_CIPAPMAC_CUR	0x24
#define	AT_CIPAPMAC_DEF	0x25
#define	AT_CIPSTA	0x26
#define	AT_CIPSTA_CUR	0x27
#define	AT_CIPSTA_DEF	0x28
#define	AT_CIPAP	0x29
#define	AT_CIPAP_CUR	0x2A
#define	AT_CIPAP_DEF	0x2B
#define	AT_CWSTARTSMART	0x2C
#define	AT_CWSTOPSMART	0x2D
#define	AT_CWSTARTDISCOVER	0x2E
#define	AT_CWSTOPDISCOVER 	0x2F
#define	AT_WPS	0x30
#define	AT_MDNS	0x31
#define	AT_CIPSTATUS	0x32
#define	AT_CIPDOMAIN	0x33
#define	AT_CIPSTART	0x34
#define	AT_CIPSSLSIZE	0x35
#define	AT_CIPSEND	0x36
#define	AT_CIPSENDEX	0x37
#define	AT_CIPSENDBUF	0x38
#define	AT_CIPBUFRESET	0x39
#define	AT_CIPBUFSTATUS	0x3A
#define	AT_CIPCHECKSEQ 	0x3B
#define	AT_CIPCLOSE	0x3C
#define	AT_CIFSR	0x3D
#define	AT_CIPMUX	0x3E
#define	AT_CIPSERVER	0x3F
#define	AT_CIPMODE	0x40
#define	AT_SAVETRANSLINK 	0x41
#define	AT_CIPSTO	0x42
#define	AT_CIUPDATE	0x43
#define	AT_PING	0x44
#define	AT_CIPDINFO	0x45
/*----------��������궨��----------*/
#define WIFI_STATION_MODE '1'
#define WIFI_SOFTAP_MODE '2'
#define WIFI_STATION_SOFTAP_MODE '3'

#define WIFI_AUTO_CON_CLOSE '0'
#define WIFI_AUTO_CON_OPEN  '1'

#define WIFI_RETURN_SHOW_CLOSE '0'
#define WIFI_RETURN_SHOW_OPEN  '1'

#define WIFI_SMARTCONFIG_ESPTOCH  '1'
#define WIFI_SMARTCONFIG_AIRKISS  '2'
#define WIFI_SMARTCONFIG_ESPTOCH_AIRKISS '3'
/*----------�����ַ���ֵ----------*/
#define STR_OK    0x9A
#define STR_ERROR 0x8A


#define STR_AT    0x95

#define STR_AT_CWMODE_DEF1    0x1B
#define STR_AT_CWMODE_DEF2    0x1C
#define STR_AT_CWMODE_DEF3    0x1D

#define STR_AT_CWAUTOCONN0    0x2E
#define STR_AT_CWAUTOCONN1    0x2F

#define STR_AT_CWSTARTSMART1    0xDD
#define STR_AT_CWSTARTSMART2    0xDE
#define STR_AT_CWSTARTSMART3    0xDF
#define STR_WIFI_DISCONNECT     0x39
#define STR_smartconfig_type_AIRKISS 0xCF
#define STR_Smart_get_wifi_info 0x02
#define STR_WIFI_CONNECTED      0xE2
#define STR_WIFI_GOT_IP         0xF2
#define STR_smartconfig_connected_wifi 0x3F

#define STR_STATUS1 0x4F
#define STR_STATUS2 0x50
#define STR_STATUS3 0x51
#define STR_STATUS4 0x52
#define STR_STATUS5 0x53

#define STR_CLOSED 0xBA
#define STR_AT_CIPSTART 0x8E /**/
#define STR_CONNECT 0x0A /**/
#define STR_ALREAD_CONNECTED 0xB5 /**/

#define STR_AT_CIPMODE0  0x2F
#define STR_AT_CIPMODE1  0x30
#define STR_AT_CIPSEND   0XC6

/*----------����ִ��״̬----------*/
#define CMD_COMPLETE 0 /*�������*/
#define CMD_SEND     1/*�����״̬*/
#define CMD_WAITE    2 /*����ȴ���*/
/*----------�������״̬��----------*/
#define CMD_COMPLETE_OK    0 /*ָ��ִ�гɹ�*/
#define CMD_COMPLETE_ERROR 1 /*ָ��ִ��ʧ��*/
#define CMD_COMPLETE_UNKNOWN 2/*δ֪���*/
/*----------����ȴ�״̬��----------*/
#define CMD_WAITE_NO_AT  0 /*��ǰû�н��յ�ATָ��*/
#define CMD_WAITE_NO_CMD  1 /*��ǰ���ز���OK����ERROR*/

#define CMD_WAITE_AT_ECHO  2/*���յ�����*/
#define CMD_WAITE_AT_DATA  3/*���յ�����*/
#define CMD_WAITE_AT_UNKNOWN  4/*���յ�����*/
#define CMD_WAITE_AT_EMPTY    5/*δ�յ�����*/ 

#define CMD_WAITE_AT_SMARTCOnfig_STEP1  3
#define CMD_WAITE_AT_SMARTCOnfig_STEP2  4
#define CMD_WAITE_AT_SMARTCOnfig_STEP3  5
#define CMD_WAITE_AT_SMARTCOnfig_STEP4  6
#define CMD_WAITE_AT_SMARTCOnfig_STEP5  7
#define CMD_WAITE_AT_SMARTCOnfig_STEP6  8

#define CMD_WAITE_AT_CIPSTART_STEP1  3
#define CMD_WAITE_AT_CIPSTART_STEP2  4
#define CMD_WAITE_AT_CIPSTART_STEP3  5
/*----------���ݲ�ֺ궨�壬�ɽ�32λ��16λ���ֲ��Ϊ���ֽ�----------*/
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
/*----------����״̬����ֵ�궨��----------*/
#define getCmdRet(data,staCode,cmdSta)    ((((data)<<9)&0xFE00)|(((staCode)<<2)&0x01FC)|((cmdSta)&0x03))
#define getCmdData(ret)        ((ret)>>9)
#define getCmdStaCode(ret)     (((ret)&0x1FC)>>2)
#define getCmdSta(ret)      ((ret)&0x03)
/*----------�������궨��----------*/
#define IS_WIFI_MODE_ALL_PERIPH(PERIPH) (((PERIPH) == WIFI_STATION_MODE) || \
                                        ((PERIPH) == WIFI_SOFTAP_MODE) || \
                                        ((PERIPH) == WIFI_STATION_SOFTAP_MODE))  

#define IS_WIFI_AUTO_CON_ALL_PERIPH(PERIPH) (((PERIPH) == WIFI_AUTO_CON_CLOSE) || \
                                            ((PERIPH) == WIFI_AUTO_CON_OPEN)) 

#define IS_WIFI_RETURN_SHOW_ALL_PERIPH(PERIPH) (((PERIPH) == WIFI_RETURN_SHOW_CLOSE) || \
                                               ((PERIPH) == WIFI_RETURN_SHOW_OPEN))

#define IS_WIFI_SMARTCONFIG_ALL_PERIPH(PERIPH) (((PERIPH) == WIFI_SMARTCONFIG_ESPTOCH) || \
                                               ((PERIPH) == WIFI_SMARTCONFIG_AIRKISS)\
																							 ((PERIPH) == WIFI_SMARTCONFIG_ESPTOCH_AIRKISS))
/********************************************************************
*��������       : ESP8266_retStaTable
*����˵��       : ESP8266��������������
*����˵��       :                  
*��������				: 
*�޸�ʱ��				:
*��ע						:����λ��ʾ���ݣ��м���λ��ʾִ�е��ĸ��׶Σ�����λ��ʾ����״̬
*����           :����
*********************************************************************/
extern unsigned short int ESP8266_retStaTable[255];
/**********************************************-**********************
*��������       : initRetStaTable
*����˵��       : ��ʼ��ESP8266����״̬��
*����˵��       :
                  ��
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
void initRetStaTable(void);
/********************************************************************
*��������       : strSum
*����˵��       : ��ָ���ַ���ָ��λ��ָ�����ȵ��ַ���
*����˵��       :
                  ��
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned char strSum(char* str,unsigned int start,unsigned int lenth);

extern TASK AT_CWAUTOCONN_task;
extern TASK AT_CWMODE_DEF_task;
extern TASK AT_CWSTARTSMART_task;
extern TASK AT_CIPSTATUS_task;
extern TASK AT_CWSTOPSMART_task;
extern TASK AT_CIPSTART_task;
extern TASK AT_task;
extern TASK AT_CIPMODE_task;
extern TASK AT_CIPSEND_task;
extern TASK_list ESP8266_taskList;
#endif
