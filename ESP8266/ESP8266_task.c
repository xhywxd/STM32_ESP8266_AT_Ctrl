/***************(C)COPYRIGHT 2017 Vrunst������_WXD***************
*�ļ���         :ESP8266_task.c
*����           :ESP8266ģ��������
*ʵ��ƽ̨       :ESP8266ģ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 2017.11.17
*��ע           :
**************************************************************************/
#include "system.h"
/********************************************************************
*��������       : ESP8266_taskRst
*����˵��       : ��λĳ��AT����
*����˵��       : TASK *ESP8266_task  ��Ҫ��λ������ָ��                   
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
void ESP8266_taskRst(TASK *ESP8266_task)
{
	ESP8266_task->taskSta=IDLE_STA;
	ESP8266_task->taskWaitTime=0;
}
void ESP8266_taskInit(TASK *ESP8266_task)
{
	ESP8266_task->cmdSta=CMD_SEND;
	ESP8266_task->taskSta=SEND_STA;
	ESP8266_task->taskWaitTime=0;
}
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
                  6 ����Ԥ�ڽ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned char  ESP8266_taskProcess(TASK *ESP8266_task,unsigned char *recvFlag)
{
	int retSta=0;
	switch (ESP8266_task->taskSta)
	{
		case SEND_STA:
		{
			if(!ESP8266_task->ESP8266_writePro(ESP8266_task->sendDataConf))
			{
				ESP8266_task->taskSta=RECV_STA;
				retSta= 0;				
			}
			else
			{				
				retSta=1;/*����ʧ�ܣ���λ*/
				ESP8266_taskInit(ESP8266_task);
			}			
		} break;
		case RECV_STA: 
		{
			if(ESP8266_task->taskTimeOut!=0)/*Ϊ0ʱ���ʾ���賬ʱ*/
			{
				ESP8266_task->taskWaitTime++;
				if(ESP8266_task->taskWaitTime>ESP8266_task->taskTimeOut)
				{
					retSta=2;/*��ʱ*/
					ESP8266_taskInit(ESP8266_task);
				}			
			}
			if(*recvFlag)
			{
				ESP8266_task->cmdSta=ESP8266_task->ESP8266_readPro(ESP8266_task->recvData,&(ESP8266_task->recvDecode));
			 *recvFlag=0;
			}			
			if(!getCmdSta(ESP8266_task->cmdSta))			
			{
				/**/
				if(getCmdStaCode(ESP8266_task->cmdSta)==CMD_COMPLETE_OK)
				{
					retSta=3;/*�ɹ�*/	
				}
				else if(getCmdStaCode(ESP8266_task->cmdSta)==CMD_COMPLETE_ERROR)
				{
					retSta=4;	/*ʧ��*/
					ESP8266_taskInit(ESP8266_task);
				}
				else
				{
					retSta=5;
				}	
			}
			else
			{		
        /*������ԶԷ��صĽ׶ν����ж�*/				 
				retSta=5;/*�ȴ�����׶�*/
			}			
		}break;
		case IDLE_STA:/*������У��ⲿ���ý���*/
		{
			retSta= 6;
		}break;
		default: 
		{
			retSta= 7;
			ESP8266_taskInit(ESP8266_task);
		}break;/*����Ԥ��״��*/
	}
	return retSta;
}
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
unsigned char recvStrDecode(char* str,hashDecode *recv)
{
	unsigned char countTemp=recv->count;
	unsigned char drFlag=0x00;
	unsigned char i=0,j=0;;
	while(str[i]!='\0')
	{
		switch (drFlag)
		{
			case 0x00: /*û��\r\n����*/
			{
				if(str[i]=='\r')
				{
					drFlag=0x01;
					if(recv->recvHashTable[recv->count].key)
					{
						recv->count++;
						j=0;
					}					   
					break;
				}
				recv->recvHashTable[recv->count].str[j++]=str[i];
				recv->recvHashTable[recv->count].key+=str[i];
			}break;
			case 0x01:/*����\r*/
			{				
				if(str[i]=='\n')
				{
					drFlag=0x00;
				}
				else if(str[i]=='\r')
				{				
				}
				else
				{
					recv->count=countTemp;/*��������֡����*/
					return 1;
				}
			}break;
			default: break;
		}
		i++;
	}
	return 0;
}
/********************************************************************
*��������       : ESP8266_mulTaskProcess
*����˵��       : �������ִ�к��������м�ĳ������ִ��ʧ���������񽫻�ʧ��
*����˵��       : TASK_list *ESP8266_task �����б�                 
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned char ESP8266_addTask(TASK_list *ESP8266_taskList,TASK *ESP8266_task)
{
}
/********************************************************************
*��������       : ESP8266_mulTaskProcess
*����˵��       : �������ִ�к��������м�ĳ������ִ��ʧ���������񽫻�ʧ��
*����˵��       : TASK_list *ESP8266_task �����б�                 
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned char ESP8266_mulTaskProcess(TASK_list *ESP8266_task,unsigned char *recvFlag)
{
	unsigned char retSta=0;
	if(ESP8266_task->taskReady)
	{
		if(ESP8266_task->nowTaskIndex<ESP8266_task->taskLenth)
		{
			switch(ESP8266_taskProcess(&ESP8266_task->list[ESP8266_task->nowTaskIndex],recvFlag))
			{
				case 0:
				{
					retSta=ESP8266_task->nowTaskIndex<<4|0;/*��ǰ�����ͳɹ�*/
				}break;
				case 1:
				{
					/*ĳ��������ʧ��*/
					retSta=ESP8266_task->nowTaskIndex<<4|1;
					ESP8266_task->taskLenth=0;
					ESP8266_task->nowTaskIndex=0;
					ESP8266_task->taskReady=0;					
				}break;
				case 2:
				{
					/*ĳ�������ͳ�ʱ*/
					retSta=ESP8266_task->nowTaskIndex<<4|2;
					ESP8266_task->taskLenth=0;
					ESP8266_task->nowTaskIndex=0;
					ESP8266_task->taskReady=0;						
				}break;
				case 3:
				{
          /*��ǰ����ɹ�,������OK*/				
					retSta=ESP8266_task->nowTaskIndex<<4|3;
					ESP8266_taskInit(&ESP8266_task->list[ESP8266_task->nowTaskIndex]);
					ESP8266_task->nowTaskIndex++;
				}break;
				case 4:
				{
					/*��ǰ�����,������ERROR*/
					
						retSta=ESP8266_task->nowTaskIndex<<4|4;					
						ESP8266_task->taskLenth=0;
						ESP8266_task->nowTaskIndex=0;
						ESP8266_task->taskReady=0;	
				}break;
				case 5:
				{
					/*��ǰ����ȴ���*/
					retSta=ESP8266_task->nowTaskIndex<<4|5;
					
				}break;
				case 6:
				{
					/*ĳ�����񲻿�Ԥ��*/
					retSta=ESP8266_task->nowTaskIndex<<4|6;
					ESP8266_task->taskLenth=0;
					ESP8266_task->nowTaskIndex=0;
					ESP8266_task->taskReady=0;					
				}break;
				default:
				{
					/*ĳ�����񲻿�Ԥ��*/
					retSta=ESP8266_task->nowTaskIndex<<4|7;
					ESP8266_task->taskLenth=0;
					ESP8266_task->nowTaskIndex=0;
					ESP8266_task->taskReady=0;						
				}break;
			}
		}
		else
		{
			/*�����б�ȫ���ɹ����*/
			retSta=ESP8266_task->nowTaskIndex<<4|9;
			ESP8266_task->taskLenth=0;
			ESP8266_task->nowTaskIndex=0;
			ESP8266_task->taskReady=0;		
		}		
	}
	else
	{
		/*�����*/
		retSta=ESP8266_task->nowTaskIndex<<4|10;		
	}
	return retSta;
}

