/***************(C)COPYRIGHT 2017 Vrunst������_WXD***************
*�ļ���         :ESP8266_task.h
*����           :ESP8266ģ��������
*ʵ��ƽ̨       :ESP8266ģ��
*��汾         :V1.0
*Ƕ��ʽϵͳ     :��
*����           :����
*�޸���ʷ       :2017.11.17
*��ע           :
**************************************************************************/
#ifndef _ESP8266_TASH_H
#define _ESP8266_TASH_H
#define SEND_STA 1
#define RECV_STA 2
#define IDLE_STA 0
/********************************************************************
*��������       : hash
*����˵��       : hash�ṹ��
*��Ա˵��       :key:hash�룬��ʹ�õ��Ǽ����������hash����
                 str[20]�����hash���Ӧ���ַ������Ϊ20
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
typedef struct hash
{
	unsigned char key;
	char str[40];
}hash;
/********************************************************************
*��������       : recvDecode
*����˵��       : ��ESP8266���ص����ݽ������õ�����
*��Ա˵��       :count:�����������������ݸ���
                 recvList[12]:�������ݵ�hash�б�
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
typedef struct hashDecode
{
	unsigned char count;
	hash     recvHashTable[12];
}hashDecode;
/********************************************************************
*��������       : TASK
*����˵��       : ����ṹ��
*��Ա˵��       :writeCmd:����д����
								 taskWaitTime������ʱ��ʱʱ��
								 taskTimeOut������ʱʱ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
typedef struct TASK
{
	unsigned char cmd;
  unsigned short int cmdSta;/*����ִ�е�״̬���ӽ��յ����Կ�ʼ����ɣ��м���ݲ�ͬ������ж��״̬*/
	unsigned char taskWaitTime;
	unsigned char taskTimeOut;
	unsigned char taskSta;/*����һ���������񣬰�������״̬�����ͣ����գ�����״̬*/
	char *sendDataConf; /*��Ҫ���͵����ݲ���*/
	char *recvData;    /*���յ�����*/
	unsigned char (*ESP8266_writePro)(char data[]);/*����ͺ���*/
	unsigned short int (*ESP8266_readPro)(char data[],hashDecode *temp);/*�������ݴ�����ָ��*/
	hashDecode recvDecode;/*�洢��ǰ������յ�����*/
}TASK;
/*��������ʱ���ã���ʱδд*/
typedef struct TASK_list
{
	TASK list[5];
	unsigned char taskReady;
	unsigned char nowTaskIndex;
	unsigned char taskLenth;
}TASK_list;
/*����״̬����*/
#define getTaskRet(staCode,taskSta) (((staCode)<<4)|((taskSta)&0x0f))
#define getTaskStaCode(ret)  (((ret)&0xf0)>>4)
#define getTaskSta(ret)   ((ret)&0x0f)
/********************************************************************
*��������       : ESP8266_taskProcess
*����˵��       : ESP8266AT������
*����˵��       : TASK *ESP8266_task  ��Ҫ���������                   
*��������				: 0 ATָ�����ɲ����ͳɹ�
                  1 ATָ������ʧ�ܣ����鴫�����ݸ�ʽ�Ƿ���ȷ
                  2 AT����ȴ���ʱ��ָ��ͳ�ȥ�ˣ����ǽ���ģ��ظ���ʱ�������ʱʱ������Ϊ0��Ϊ���賬ʱ�������ش�ֵ
                  3 ATָ��ͳɹ��󣬳ɹ��յ�ģ����ȷ����ֵ�����ﻹδ�Է���ֵ�����жϣ����ܻ�ϸ��
                  4 ATָ��ɹ����ͣ��ڳ�ʱʱ�����ڵȴ�ģ��ظ�
                  5 ��ǰ�������
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned char  ESP8266_taskProcess(TASK *ESP8266_task,unsigned char *recvFlag);
/********************************************************************
*��������       : recvStrDecode
*����˵��       : ����str�ַ����е�������Ϣ���洢
*����˵��       :char* str ��Ҫ�������ַ���
                 hashDecode *recv ����������ݴ洢
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned char recvStrDecode(char* str,hashDecode *recv);
/********************************************************************
*��������       : ESP8266_mulTaskProcess
*����˵��       : �������ִ�к��������м�ĳ������ִ��ʧ���������񽫻�ʧ��
*����˵��       : TASK_list *ESP8266_task �����б�                 
*��������				: ִ��״��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned char ESP8266_mulTaskProcess(TASK_list *ESP8266_task,unsigned char *recvFlag);
void ESP8266_taskInit(TASK *ESP8266_task);
#endif
