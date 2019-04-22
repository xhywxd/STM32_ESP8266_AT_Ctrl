/***************(C)COPYRIGHT 2017 Vrunst工作室_WXD***************
*文件名         :ESP8266_task.h
*描述           :ESP8266模块任务处理
*实验平台       :ESP8266模块
*库版本         :V1.0
*嵌入式系统     :无
*作者           :武旭东
*修改历史       :2017.11.17
*备注           :
**************************************************************************/
#ifndef _ESP8266_TASH_H
#define _ESP8266_TASH_H
#define SEND_STA 1
#define RECV_STA 2
#define IDLE_STA 0
/********************************************************************
*变量名称       : hash
*功能说明       : hash结构体
*成员说明       :key:hash码，我使用的是简单求和来代替hash运算
                 str[20]：这个hash码对应的字符串，最长为20
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
typedef struct hash
{
	unsigned char key;
	char str[40];
}hash;
/********************************************************************
*变量名称       : recvDecode
*功能说明       : 对ESP8266返回的数据解析后获得的数据
*成员说明       :count:解析出来的有用数据个数
                 recvList[12]:解析数据的hash列表
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
typedef struct hashDecode
{
	unsigned char count;
	hash     recvHashTable[12];
}hashDecode;
/********************************************************************
*变量名称       : TASK
*功能说明       : 任务结构体
*成员说明       :writeCmd:任务写命令
								 taskWaitTime：任务超时计时时间
								 taskTimeOut：任务超时时间
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
typedef struct TASK
{
	unsigned char cmd;
  unsigned short int cmdSta;/*命令执行的状态，从接收到回显开始到完成，中间根据不同命令会有多个状态*/
	unsigned char taskWaitTime;
	unsigned char taskTimeOut;
	unsigned char taskSta;/*对于一个命令任务，包括三个状态：发送，接收，空闲状态*/
	char *sendDataConf; /*需要发送的数据参数*/
	char *recvData;    /*接收的数据*/
	unsigned char (*ESP8266_writePro)(char data[]);/*命令发送函数*/
	unsigned short int (*ESP8266_readPro)(char data[],hashDecode *temp);/*接收数据处理函数指针*/
	hashDecode recvDecode;/*存储当前任务接收的数据*/
}TASK;
/*多个任务的时候用，暂时未写*/
typedef struct TASK_list
{
	TASK list[5];
	unsigned char taskReady;
	unsigned char nowTaskIndex;
	unsigned char taskLenth;
}TASK_list;
/*任务状态返回*/
#define getTaskRet(staCode,taskSta) (((staCode)<<4)|((taskSta)&0x0f))
#define getTaskStaCode(ret)  (((ret)&0xf0)>>4)
#define getTaskSta(ret)   ((ret)&0x0f)
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
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned char  ESP8266_taskProcess(TASK *ESP8266_task,unsigned char *recvFlag);
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
unsigned char recvStrDecode(char* str,hashDecode *recv);
/********************************************************************
*函数名称       : ESP8266_mulTaskProcess
*功能说明       : 多个任务执行函数，如中间某个函数执行失败整个任务将会失败
*参数说明       : TASK_list *ESP8266_task 任务列表                 
*函数返回				: 执行状况
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned char ESP8266_mulTaskProcess(TASK_list *ESP8266_task,unsigned char *recvFlag);
void ESP8266_taskInit(TASK *ESP8266_task);
#endif
