/***************(C)COPYRIGHT 2017 Vrunst工作室_WXD***************
*文件名         :ESP8266_AT_SEND_CMD.c
*描述           :ESP8266模块AT指令合集
*实验平台       :ESP8266模块
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 2017.11.17
*备注           :本资料仅供学习交流，请勿私自用于商业用途
                 如有问题请邮件联系：xhywxd@126.com
**************************************************************************/
#include "ESP8266_AT_CMD.h"
#include "string.h"
/*******************发********************送****************************相**************************关**********************************/
/********************************************************************
*函数名称       :S_AT
*功能说明       : 测试启动AT
*参数说明       : 实际无参数                  
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:ESP8266返回OK
*作者           :武旭东
*********************************************************************/
unsigned char S_AT(char data[])
{
	ESP8266_sendByte("AT\r\n",4);
	return 0;
}
/********************************************************************
*函数名称       :S_AT_RST
*功能说明       : 重启模块
*参数说明       : 实际无参数                  
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:ESP8266返回OK
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_RST(char data[])
{
	ESP8266_sendByte("AT+RST\r\n",8);
	return 0;
}

/********************************************************************
*函数名称       :S_AT_GMR
*功能说明       : 模块版本信息
*参数说明       : 实际无参数                  
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:这条指令发出后ESP8266会返回如下信息
                 <AT version info> AT版本信息
								 <SDK version info>基于SDK版本信息
								 <compile time> 编译生成时间
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_GMR(char data[])
{
	ESP8266_sendByte("AT+GMR\r\n",8);
  return 0;
}
/********************************************************************
*函数名称       :S_AT_E
*功能说明       :开关回显
*参数说明       :data[0]存参数，其他字节未用
                 0:关闭回显 1:打开回显              
*函数返回				: 无
*修改时间				:
*备注						:ESP8266返回OK
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_E(char data[])
{
	if(data[0]=='0')
	{
		ESP8266_sendByte("ATE0\r\n",6);
	}
	else if(data[0]=='1')
	{
		ESP8266_sendByte("ATE1\r\n",6);
	}	 
	else
	{
		return 1;
	}
	return 0;
}
/********************************************************************
*函数名称       :S_AT_RESTORE
*功能说明       : 恢复模块出厂设置
*参数说明       : 实际无参数                  
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:ESP8266返回OK，此命令会将flash参数恢复为默认值，机器将会重启
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_RESTORE(char data[])
{
	ESP8266_sendByte("AT+RESTORE\r\n",12);
	return 0;
}
/********************************************************************
*函数名称       :S_AT_CWMODE_DEF
*功能说明       :修改ESP8266模式
*参数说明       :data[0]存参数，其他字节未用
                 1:station模式
 					       2:softAP模式
                 3：softAP+station模式             
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:ESP8266返回OK，参数将会存储到flash区，掉电不丢失
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CWMODE_DEF(char data[])
{
	assert_param(IS_WIFI_MODE_ALL_PERIPH(data[0]));
	switch(data[0])
	{
		case WIFI_STATION_MODE: ESP8266_sendByte("AT+CWMODE_DEF=1\r\n",17);break;
		case WIFI_SOFTAP_MODE: ESP8266_sendByte("AT+CWMODE_DEF=2\r\n",17);break;
		case WIFI_STATION_SOFTAP_MODE: ESP8266_sendByte("AT+CWMODE_DEF=3\r\n",17);break;
		default : return 1; break;
	}
	return 0;
}
/********************************************************************
*函数名称       :S_AT_CWJAP_DEF
*功能说明       :连接wifi热点，保存到flash
*参数说明       :data[0]存参数，其他字节未用
								 参数存储格式为data依次存放  vrunbst;xhywxd;
                 vrunbst换为你自己的热点名称，xhywxd换为热点密码其他字符勿动
*函数返回				: 指令生成并发送成功返回0 否则返回1
*备注						:参数设置时候，模块需处于station模式
                 参数将会存储到flash区，掉电不丢失
                 热点名称不支持特殊字符及其中文，特殊字符处理未加，热点名称最长为30个字符
                 热点密码限制同名称限制
                 模块返回：1 连接超时
                           2 密码错误
                           3 找不到目标AP
                           4 连接失败
*修改时间				:                 
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CWJAP_DEF(char data[])
{
	unsigned char i;
	unsigned char ssidLenth=0;
	unsigned char pwdLenth=0;
	char buff[50]="AT+CWJAP_DEF=\"";
	for(i=0;i<14;i++)
	{
		ESP8266_sendBuff[i]=buff[i];
	}
	for(i=0;i<45;i++)
	{		
		if(data[i]==';')break;
		ESP8266_sendBuff[14+i]=data[i];	
		ssidLenth++;		
	}
	if(i==45)return 1;
	ESP8266_sendBuff[14+ssidLenth]='"';
	ESP8266_sendBuff[15+ssidLenth]=',';
	ESP8266_sendBuff[16+ssidLenth]='"';
	i=i+1;
	for(;i<45;i++)
	{		
		if(data[i]==';')break;
		ESP8266_sendBuff[16+i]=data[i];		
		pwdLenth++;
	}
	if(i==45)return 1;
	ESP8266_sendBuff[17+ssidLenth+pwdLenth]='"';
	ESP8266_sendBuff[18+ssidLenth+pwdLenth]='\r';
	ESP8266_sendBuff[19+ssidLenth+pwdLenth]='\n';

	ESP8266_sendByte(ESP8266_sendBuff,(unsigned int)(20+ssidLenth+pwdLenth));
	return 0;	
}
/********************************************************************
*函数名称       :S_AT_CWQAP
*功能说明       :断开与wifi连接
*参数说明       :实际无参数                  
*函数返回				:指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:ESP8266返回OK
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CWQAP(char data[])
{
	ESP8266_sendByte("AT+CWQAP\r\n",11);
	return 0;
}
/********************************************************************
*函数名称       :S_AT_CWAUTOCONN
*功能说明       :上电是否自动连接AP
*参数说明       :data[0]存参数，其他字节未用
                 0:上电不自动连接AP 1:上电自动连接AP       
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:参数保存到flash，掉电不丢失，ESP8266返回OK
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CWAUTOCONN(char data[])
{
	//assert_param(IS_WIFI_AUTO_CON_ALL_PERIPH(data[0]));
	*data ? ESP8266_sendByte("AT+CWAUTOCONN=1\r\n",17):ESP8266_sendByte("AT+CWAUTOCONN=0\r\n",17);
	return 0;
}
/********************************************************************
*函数名称       :S_AT_CWSTARTSMART
*功能说明       :开启smartConfig
*参数说明       :data[0]存参数，其他字节未用
                 1:ESPTOCH
 					       2:AIRKISS
                 3:ESPTOCH+AIRKISS      
*函数返回				: 指令生成并发送成功返回0 否则返1
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CWSTARTSMART(char data[])
{
	//assert_param(IS_WIFI_SMARTCONFIG_ALL_PERIPH(data[0]));
	switch(data[0])
	{
		case WIFI_SMARTCONFIG_ESPTOCH: ESP8266_sendByte("AT+CWSTARTSMART=1\r\n",19);break;
		case WIFI_SMARTCONFIG_AIRKISS: ESP8266_sendByte("AT+CWSTARTSMART=2\r\n",19);break;
		case WIFI_SMARTCONFIG_ESPTOCH_AIRKISS: ESP8266_sendByte("AT+CWSTARTSMART=3\r\n",19);break;
		default : return 1; break;
	}
	return 0;
}
/********************************************************************
*函数名称       :S_AT_CWSTOPSMART
*功能说明       :停止smartConfig
*参数说明       :实际无参数                  
*函数返回				:指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:ESP8266返回OK，不管有没有配网成功都要执行这条指令
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CWSTOPSMART(char data[])
{
	ESP8266_sendByte("AT+CWSTOPSMART\r\n",16);
	return 0;
}
/********************************************************************
*函数名称       :S_AT_CIPSTATUS
*功能说明       :查询当前网络状态
*参数说明       :实际无参数                  
*函数返回				:指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:模块返回：
                 2 ESP8266 station 已连接AP 获得ip地址
								 3 ESP8266 station 已经建立tcp或udp连接
								 4 ESP8266 station 断开网络连接
								 5 ESP8266 station 未连接ap
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CIPSTATUS(char data[])
{
	ESP8266_sendByte("AT+CIPSTATUS\r\n",14);
	return 0;
}
/********************************************************************
*函数名称       :S_AT_CIPSTART
*功能说明       :连接到指定tcp服务器
*参数说明       :data 中存放服务器地址与端口
                 存放格式为："192.168.111.111;54566; "，字符串ASCII格式
*函数返回				:指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:模块返回Ok 或 ERROR，如已经连接，模块返回ALREADY CONNECT
                 esp8266支持TCP、UDP、SSL连接，目前此函数默认为TCP，未支持其他连接
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CIPSTART(char data[])
{	
	unsigned char i;
	unsigned char addrLenth=0;
	unsigned char portLenth=0;
	char buff[50]="AT+CIPSTART=\"TCP\",\"";
	for(i=0;i<19;i++)
	{
		ESP8266_sendBuff[i]=buff[i];
	}
	for(i=0;i<30;i++)
	{		
		if(data[i]==';')break;
		ESP8266_sendBuff[19+i]=data[i];	
		addrLenth++;		
	}
	if(i==30)return 1;
	ESP8266_sendBuff[19+addrLenth]='"';
	ESP8266_sendBuff[20+addrLenth]=',';
	i=i+1;
	for(;i<30;i++)
	{		
		if(data[i]==';')break;
		ESP8266_sendBuff[20+i]=data[i];		
		portLenth++;
	}
	if(i==30)return 1;
	ESP8266_sendBuff[21+addrLenth+portLenth]='\r';
	ESP8266_sendBuff[22+addrLenth+portLenth]='\n';

	ESP8266_sendByte(ESP8266_sendBuff,(unsigned int)(23+addrLenth+portLenth));
	return 0;
}
/********************************************************************
*函数名称       :S_AT_CIPMODE
*功能说明       :设置传输模式
*参数说明       :data[0]存参数，其他字节未用
                 0:普通传输模式 1:透传模式    
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:ESP8266返回OK
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CIPMODE(char data[])
{
	switch(data[0])
	{
		case '0': ESP8266_sendByte("AT+CIPMODE=0\r\n",14);break;
		case '1': ESP8266_sendByte("AT+CIPMODE=1\r\n",14);break;
		default : return 1; break;
	}	
	return 0;
}
/********************************************************************
*函数名称       :S_AT_CIPSEND
*功能说明       :在透传模式下开启透传
*参数说明       :  
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_CIPSEND(char data[])
{
	 ESP8266_sendByte("AT+CIPSEND\r\n",12);
	 return 0;
}
/********************************************************************
*函数名称       :S_AT_QUITSEND
*功能说明       :退出透传指令
*参数说明       :  
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned char S_AT_QUITSEND(char data[])
{
	 ESP8266_sendByte("+++",3);
	 return 0;
}
/*******************接********************收****************************相**************************关**********************************/
/********************************************************************
*函数名称       : ESP8266_retStaTable
*功能说明       : ESP8266返回命令索引表
*参数说明       :                  
*函数返回				: 
*修改时间				:
*备注						:高三位表示数据，中间三位表示执行到哪个阶段，低两位表示命令状态
*作者           :武旭东
*********************************************************************/
unsigned short int ESP8266_retStaTable[255];
/**********************************************-**********************
*函数名称       : initRetStaTable
*功能说明       : 初始化ESP8266返回状态表
*参数说明       :
                  无
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
void initRetStaTable(void)
{
	for(int i=0;i<256;i++)
	{
		ESP8266_retStaTable[i]=getCmdRet(0,CMD_WAITE_AT_UNKNOWN,CMD_WAITE);
	}
	ESP8266_retStaTable[STR_OK]=getCmdRet(0,CMD_COMPLETE_OK,CMD_COMPLETE);
	ESP8266_retStaTable[STR_ERROR]=getCmdRet(0,CMD_COMPLETE_ERROR,CMD_COMPLETE);
	/*AT*/
	ESP8266_retStaTable[STR_AT]=getCmdRet(0,CMD_WAITE_AT_ECHO,CMD_WAITE);
	/*AT+CWMODE_DEF*/
	ESP8266_retStaTable[STR_AT_CWMODE_DEF1]=getCmdRet(1,CMD_WAITE_AT_ECHO,CMD_WAITE);
	ESP8266_retStaTable[STR_AT_CWMODE_DEF2]=getCmdRet(2,CMD_WAITE_AT_ECHO,CMD_WAITE);
	ESP8266_retStaTable[STR_AT_CWMODE_DEF3]=getCmdRet(3,CMD_WAITE_AT_ECHO,CMD_WAITE);
	/*AT+CWAUTOCONN*/
	ESP8266_retStaTable[STR_AT_CWAUTOCONN0]=getCmdRet(0,CMD_WAITE_AT_ECHO,CMD_WAITE);
	ESP8266_retStaTable[STR_AT_CWAUTOCONN1]=getCmdRet(1,CMD_WAITE_AT_ECHO,CMD_WAITE);
	/*AT+WSTARTSMART*/
	ESP8266_retStaTable[STR_AT_CWSTARTSMART1]=getCmdRet(1,CMD_WAITE_AT_ECHO,CMD_WAITE);
	ESP8266_retStaTable[STR_AT_CWSTARTSMART2]=getCmdRet(2,CMD_WAITE_AT_ECHO,CMD_WAITE);
	ESP8266_retStaTable[STR_AT_CWSTARTSMART3]=getCmdRet(3,CMD_WAITE_AT_ECHO,CMD_WAITE);
	ESP8266_retStaTable[STR_WIFI_DISCONNECT]=getCmdRet(0,CMD_WAITE_AT_SMARTCOnfig_STEP2,CMD_WAITE);
	ESP8266_retStaTable[STR_smartconfig_type_AIRKISS]=getCmdRet(0,CMD_WAITE_AT_SMARTCOnfig_STEP3,CMD_WAITE);
	ESP8266_retStaTable[STR_Smart_get_wifi_info]=getCmdRet(0,CMD_WAITE_AT_SMARTCOnfig_STEP4,CMD_WAITE);
	ESP8266_retStaTable[STR_WIFI_CONNECTED]=getCmdRet(0,CMD_WAITE_AT_SMARTCOnfig_STEP5,CMD_WAITE);
	ESP8266_retStaTable[STR_WIFI_GOT_IP]=getCmdRet(0,CMD_WAITE_AT_SMARTCOnfig_STEP6,CMD_WAITE);
	ESP8266_retStaTable[STR_smartconfig_connected_wifi]=getCmdRet(0,CMD_COMPLETE_OK,CMD_COMPLETE);
	/*AT+CIPSTATUS*/
	ESP8266_retStaTable[STR_STATUS1]=getCmdRet(1,CMD_WAITE_AT_DATA,CMD_WAITE);
	ESP8266_retStaTable[STR_STATUS2]=getCmdRet(2,CMD_WAITE_AT_DATA,CMD_WAITE);
	ESP8266_retStaTable[STR_STATUS3]=getCmdRet(3,CMD_WAITE_AT_DATA,CMD_WAITE);	
	ESP8266_retStaTable[STR_STATUS4]=getCmdRet(4,CMD_WAITE_AT_DATA,CMD_WAITE);	
	ESP8266_retStaTable[STR_STATUS5]=getCmdRet(5,CMD_WAITE_AT_DATA,CMD_WAITE);	
	/*AT+CIPSTART*/
	ESP8266_retStaTable[STR_CLOSED]=getCmdRet(0,CMD_COMPLETE_ERROR,CMD_COMPLETE);
	ESP8266_retStaTable[STR_AT_CIPSTART]=getCmdRet(0,CMD_WAITE_AT_ECHO,CMD_WAITE);
	ESP8266_retStaTable[STR_ALREAD_CONNECTED]=getCmdRet(0,CMD_WAITE_AT_CIPSTART_STEP1,CMD_WAITE);
	ESP8266_retStaTable[STR_CONNECT]=getCmdRet(0,CMD_WAITE_AT_CIPSTART_STEP1,CMD_WAITE);
	/*AT+CIIPSEND*/
	ESP8266_retStaTable[STR_AT_CIPSEND]=getCmdRet(0,CMD_WAITE_AT_ECHO,CMD_WAITE);
	/*AT+CIPMODE*/
//	ESP8266_retStaTable[STR_AT_CIPMODE0]=getCmdRet(0,CMD_WAITE_AT_ECHO,CMD_WAITE);
 	ESP8266_retStaTable[STR_AT_CIPMODE1]=getCmdRet(1,CMD_WAITE_AT_ECHO,CMD_WAITE);
}
/********************************************************************
*函数名称       : R_pro
*功能说明       : 发送完指令 后对接收到的数据处理,通用接收处理，特殊的特殊处理
*参数说明       : 接收到的数据                
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned short int R_pro(char data[],hashDecode* taskHash)
{
	unsigned short int taskData=0;
	unsigned short int taskSta=CMD_WAITE;/*当前指令执行状态*/
	unsigned short int staCode=0;/*等待+模块返回状态下的命令状态*/
	unsigned short int ret=0;
	unsigned char lenth;
	unsigned short int temp;
	unsigned char hashStar=taskHash->count;	
	recvStrDecode(data,taskHash);
	lenth=taskHash->count-hashStar;
	if(lenth)
	{
		for(int i=hashStar;i<taskHash->count;i++)
		{
			ret=ESP8266_retStaTable[taskHash->recvHashTable[i].key];
			temp=getCmdData(ret);
			if(temp)taskData=temp;
		}
	}
	else
	{
		ret=getCmdRet(taskData,staCode,taskSta);	
	}
	return ret|((taskData<<9)&0xFE00);	
}
/********************************************************************
*函数名称       :R_AT_CWSTARTSMART
*功能说明       :发送完AT+CWSTARTSMART命令后，对接收到的数据进行处理
*参数说明       :接收到的数据            
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned short int R_AT_CWSTARTSMART(char data[],hashDecode* taskHash)
{
	unsigned short int taskData=0;
	unsigned short int taskSta=CMD_WAITE;/*当前指令执行状态*/
	unsigned short int staCode=0;/*等待+模块返回状态下的命令状态*/
	unsigned short int ret=0;
	unsigned char lenth;
	unsigned char hashStar=taskHash->count;	
	recvStrDecode(data,taskHash);
	lenth=taskHash->count-hashStar;
	if(lenth)
	{
		for(int i=hashStar;i<taskHash->count;i++)
		{
			if(taskHash->recvHashTable[i].key!=STR_OK&&taskHash->recvHashTable[i].key!=STR_ERROR)
			{
				ret=ESP8266_retStaTable[taskHash->recvHashTable[i].key];
			}
			else
			{
				switch(taskHash->recvHashTable[i].key)
				{
					case STR_OK:ret=getCmdRet(0,CMD_WAITE_AT_SMARTCOnfig_STEP1,CMD_WAITE);break;
					case STR_ERROR:ret=getCmdRet(0,CMD_COMPLETE_ERROR,CMD_COMPLETE);break;
					default: ret=getCmdRet(0,CMD_WAITE_AT_UNKNOWN,CMD_WAITE);break;
				}				 
			}			
		}
	}
	else
	{
		ret=getCmdRet(taskData,staCode,taskSta);	
	}
	return ret ;	
}
/********************************************************************
*函数名称       :R_AT_CIPSTART
*功能说明       :发送完AT+CIPSTART命令后，对接收到的数据进行处理
*参数说明       :接收到的数据            
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned short int R_AT_CIPSTART(char data[],hashDecode* taskHash)
{
	unsigned short int taskData=0;
	unsigned short int taskSta=CMD_WAITE;/*当前指令执行状态*/
	unsigned short int staCode=0;/*等待+模块返回状态下的命令状态*/
	unsigned short int ret=0;
	unsigned char lenth;
	unsigned char hashStart=taskHash->count;	
	recvStrDecode(data,taskHash);
	lenth=taskHash->count-hashStart;
	if(lenth)
	{
		for(int i=hashStart;i<taskHash->count;i++)
		{
			if(taskHash->recvHashTable[i].key==STR_ERROR)
			{
				if(taskHash->recvHashTable[i-1].key==STR_AT_CIPSTART)
				{
					ret=getCmdRet(0,CMD_WAITE_AT_CIPSTART_STEP1,CMD_WAITE);
				}
				else if(taskHash->recvHashTable[i-1].key==STR_ALREAD_CONNECTED)
				{ 					
					ret=getCmdRet(0,CMD_COMPLETE_OK,CMD_COMPLETE);
				}
			}
			else
			{
				ret=ESP8266_retStaTable[taskHash->recvHashTable[i].key];
			}		
		}
	}
	else
	{
		ret=getCmdRet(taskData,staCode,taskSta);	
	}
	return ret ;	
}
/********************************************************************
*函数名称       :R_AT_CIPMODE
*功能说明       :发送完AT+CIPMODE命令后，对接收到的数据进行处理
*参数说明       :接收到的数据            
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned short int R_AT_CIPMODE(char data[],hashDecode* taskHash)
{
	unsigned short int taskData=0;
	unsigned short int taskSta=CMD_WAITE;/*当前指令执行状态*/
	unsigned short int staCode=0;/*等待+模块返回状态下的命令状态*/
	unsigned short int ret=0;
	unsigned char lenth;
	unsigned char hashStart=taskHash->count;	
	recvStrDecode(data,taskHash);
	lenth=taskHash->count-hashStart;
	if(lenth)
	{
		for(int i=hashStart;i<taskHash->count;i++)
		{
			switch(taskHash->recvHashTable[i].key)
			{
				case STR_ERROR: ret=ESP8266_retStaTable[STR_ERROR];break;
				case STR_OK: ret=ESP8266_retStaTable[STR_OK];break;
				case STR_AT_CIPMODE0: ret=getCmdRet(0,CMD_WAITE_AT_ECHO,CMD_WAITE);break;
				case STR_AT_CIPMODE1:ret=getCmdRet(1,CMD_WAITE_AT_ECHO,CMD_WAITE); break;
				default:ret=getCmdRet(0,CMD_WAITE_AT_UNKNOWN,CMD_WAITE);break;
			}				
		}
	}
	else
	{
		ret=getCmdRet(taskData,staCode,taskSta);	
	}
	return ret ;	
}
/********************************************************************
*函数名称       :R_AT_QUITSEND
*功能说明       :退出透传指令回复处理
*参数说明       :  
*函数返回				: 指令生成并发送成功返回0 否则返回1
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned char R_AT_QUITSEND(char data[])
{
	static  unsigned char waitTime=0;
	unsigned short int ret=0;
	if(waitTime<25)/**/
	{
		ret=getCmdRet(0,CMD_WAITE_AT_ECHO,CMD_WAITE);
	}
	else
	{
		waitTime=0;
		ret=getCmdRet(0,CMD_COMPLETE_OK,CMD_COMPLETE);
	}
	return ret;
}
/********************************************************************
*函数名称       : strSum
*功能说明       : 求指定字符串指定位置指定长度的字符和
*参数说明       :
                  无
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
unsigned char strSum(char* str,unsigned int start,unsigned int lenth)
{
	unsigned char sum=0;
	int i=start;
	for(;i<lenth+start;i++)
	{
		sum=sum+str[i];
	}
	for(;;)
	{
		str[i-start-lenth]=str[i];
		i++;
		if(str[i]=='\0') break;
	}
	return sum;
}

TASK AT_CWAUTOCONN_task={AT_CWMODE_DEF,CMD_SEND,0,0,SEND_STA,"1",UART4_receiveBuff,S_AT_CWAUTOCONN,R_pro};
TASK AT_CWMODE_DEF_task={AT_CWMODE_DEF,CMD_SEND,0,0,SEND_STA,"1",UART4_receiveBuff,S_AT_CWMODE_DEF,R_pro};
TASK AT_CWSTARTSMART_task={AT_CWSTARTSMART,CMD_SEND,0,0,SEND_STA,"3",UART4_receiveBuff,S_AT_CWSTARTSMART,R_AT_CWSTARTSMART};
TASK AT_CIPSTATUS_task={AT_CIPSTATUS,CMD_SEND,0,0,SEND_STA,"0",UART4_receiveBuff,S_AT_CIPSTATUS,R_pro};
TASK AT_CWSTOPSMART_task={AT_CWSTOPSMART,CMD_SEND,0,0,SEND_STA,"0",UART4_receiveBuff,S_AT_CWSTOPSMART,R_pro};
TASK AT_task={AT,CMD_SEND,0,0,SEND_STA,"0",UART4_receiveBuff,S_AT,R_pro};
TASK AT_CIPSTART_task={AT_CIPSTART,CMD_SEND,0,0,SEND_STA,"192.168.1.104;1883; ",UART4_receiveBuff,S_AT_CIPSTART,R_AT_CIPSTART};

TASK AT_CIPMODE_task={AT_CIPMODE,CMD_SEND,0,0,SEND_STA,"1",UART4_receiveBuff,S_AT_CIPMODE,R_AT_CIPMODE};
TASK AT_CIPSEND_task={AT_CIPSEND,CMD_SEND,0,0,SEND_STA,"0",UART4_receiveBuff,S_AT_CIPSEND,R_pro};
TASK_list ESP8266_taskList={0,0,0,0};


