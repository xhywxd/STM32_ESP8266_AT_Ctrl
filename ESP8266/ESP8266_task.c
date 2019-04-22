/***************(C)COPYRIGHT 2017 Vrunst工作室_WXD***************
*文件名         :ESP8266_task.c
*描述           :ESP8266模块任务处理
*实验平台       :ESP8266模块
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 2017.11.17
*备注           :
**************************************************************************/
#include "system.h"
/********************************************************************
*函数名称       : ESP8266_taskRst
*功能说明       : 复位某个AT任务
*参数说明       : TASK *ESP8266_task  需要复位的任务指针                   
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
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
*函数名称       : ESP8266_taskProcess
*功能说明       : ESP8266AT任务处理
*参数说明       : TASK *ESP8266_task  需要处理的任务                   
*函数返回				: 0 AT指令生成并发送成功
                  1 AT指令生成失败，请检查传入数据格式是否正确
                  2 AT任务等待超时，指令发送出去了，但是接收模块回复超时，如果超时时间设置为0，为不设超时，不返回此值
                  3 AT指令发送成功后，成功收到模块正确返回值，这里还未对返回值进行判断，可能会细分
                  4 AT指令成功发送，在超时时间以内等待模块回复
                  5 当前任务空闲
                  6 不可预期结果
*修改时间				:
*备注						:
*作者           :武旭东
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
				retSta=1;/*发送失败，复位*/
				ESP8266_taskInit(ESP8266_task);
			}			
		} break;
		case RECV_STA: 
		{
			if(ESP8266_task->taskTimeOut!=0)/*为0时候表示不设超时*/
			{
				ESP8266_task->taskWaitTime++;
				if(ESP8266_task->taskWaitTime>ESP8266_task->taskTimeOut)
				{
					retSta=2;/*超时*/
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
					retSta=3;/*成功*/	
				}
				else if(getCmdStaCode(ESP8266_task->cmdSta)==CMD_COMPLETE_ERROR)
				{
					retSta=4;	/*失败*/
					ESP8266_taskInit(ESP8266_task);
				}
				else
				{
					retSta=5;
				}	
			}
			else
			{		
        /*这里可以对返回的阶段进行判断*/				 
				retSta=5;/*等待命令阶段*/
			}			
		}break;
		case IDLE_STA:/*任务空闲，外部设置进入*/
		{
			retSta= 6;
		}break;
		default: 
		{
			retSta= 7;
			ESP8266_taskInit(ESP8266_task);
		}break;/*不可预期状况*/
	}
	return retSta;
}
/********************************************************************
*函数名称       : recvStrDecode
*功能说明       : 解析str字符串中的有用信息并存储
*参数说明       :char* str 需要解析的字符串
                 hashDecode *recv 解析完的数据存储
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
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
			case 0x00: /*没有\r\n出现*/
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
			case 0x01:/*出现\r*/
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
					recv->count=countTemp;/*不是正常帧数据*/
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
*函数名称       : ESP8266_mulTaskProcess
*功能说明       : 多个任务执行函数，如中间某个函数执行失败整个任务将会失败
*参数说明       : TASK_list *ESP8266_task 任务列表                 
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned char ESP8266_addTask(TASK_list *ESP8266_taskList,TASK *ESP8266_task)
{
}
/********************************************************************
*函数名称       : ESP8266_mulTaskProcess
*功能说明       : 多个任务执行函数，如中间某个函数执行失败整个任务将会失败
*参数说明       : TASK_list *ESP8266_task 任务列表                 
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
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
					retSta=ESP8266_task->nowTaskIndex<<4|0;/*当前任务发送成功*/
				}break;
				case 1:
				{
					/*某个任务发送失败*/
					retSta=ESP8266_task->nowTaskIndex<<4|1;
					ESP8266_task->taskLenth=0;
					ESP8266_task->nowTaskIndex=0;
					ESP8266_task->taskReady=0;					
				}break;
				case 2:
				{
					/*某个任务发送超时*/
					retSta=ESP8266_task->nowTaskIndex<<4|2;
					ESP8266_task->taskLenth=0;
					ESP8266_task->nowTaskIndex=0;
					ESP8266_task->taskReady=0;						
				}break;
				case 3:
				{
          /*当前任务成功,并返回OK*/				
					retSta=ESP8266_task->nowTaskIndex<<4|3;
					ESP8266_taskInit(&ESP8266_task->list[ESP8266_task->nowTaskIndex]);
					ESP8266_task->nowTaskIndex++;
				}break;
				case 4:
				{
					/*当前任务成,并返回ERROR*/
					
						retSta=ESP8266_task->nowTaskIndex<<4|4;					
						ESP8266_task->taskLenth=0;
						ESP8266_task->nowTaskIndex=0;
						ESP8266_task->taskReady=0;	
				}break;
				case 5:
				{
					/*当前任务等待中*/
					retSta=ESP8266_task->nowTaskIndex<<4|5;
					
				}break;
				case 6:
				{
					/*某个任务不可预期*/
					retSta=ESP8266_task->nowTaskIndex<<4|6;
					ESP8266_task->taskLenth=0;
					ESP8266_task->nowTaskIndex=0;
					ESP8266_task->taskReady=0;					
				}break;
				default:
				{
					/*某个任务不可预期*/
					retSta=ESP8266_task->nowTaskIndex<<4|7;
					ESP8266_task->taskLenth=0;
					ESP8266_task->nowTaskIndex=0;
					ESP8266_task->taskReady=0;						
				}break;
			}
		}
		else
		{
			/*任务列表全部成功完成*/
			retSta=ESP8266_task->nowTaskIndex<<4|9;
			ESP8266_task->taskLenth=0;
			ESP8266_task->nowTaskIndex=0;
			ESP8266_task->taskReady=0;		
		}		
	}
	else
	{
		/*任务空*/
		retSta=ESP8266_task->nowTaskIndex<<4|10;		
	}
	return retSta;
}

