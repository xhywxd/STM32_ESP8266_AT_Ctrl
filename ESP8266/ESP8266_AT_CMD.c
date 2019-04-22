/***************(C)COPYRIGHT 2017 Vrunst������_WXD***************
*�ļ���         :ESP8266_AT_SEND_CMD.c
*����           :ESP8266ģ��ATָ��ϼ�
*ʵ��ƽ̨       :ESP8266ģ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 2017.11.17
*��ע           :�����Ͻ���ѧϰ����������˽��������ҵ��;
                 �����������ʼ���ϵ��xhywxd@126.com
**************************************************************************/
#include "ESP8266_AT_CMD.h"
#include "string.h"
/*******************��********************��****************************��**************************��**********************************/
/********************************************************************
*��������       :S_AT
*����˵��       : ��������AT
*����˵��       : ʵ���޲���                  
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:ESP8266����OK
*����           :����
*********************************************************************/
unsigned char S_AT(char data[])
{
	ESP8266_sendByte("AT\r\n",4);
	return 0;
}
/********************************************************************
*��������       :S_AT_RST
*����˵��       : ����ģ��
*����˵��       : ʵ���޲���                  
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:ESP8266����OK
*����           :����
*********************************************************************/
unsigned char S_AT_RST(char data[])
{
	ESP8266_sendByte("AT+RST\r\n",8);
	return 0;
}

/********************************************************************
*��������       :S_AT_GMR
*����˵��       : ģ��汾��Ϣ
*����˵��       : ʵ���޲���                  
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:����ָ�����ESP8266�᷵��������Ϣ
                 <AT version info> AT�汾��Ϣ
								 <SDK version info>����SDK�汾��Ϣ
								 <compile time> ��������ʱ��
*����           :����
*********************************************************************/
unsigned char S_AT_GMR(char data[])
{
	ESP8266_sendByte("AT+GMR\r\n",8);
  return 0;
}
/********************************************************************
*��������       :S_AT_E
*����˵��       :���ػ���
*����˵��       :data[0]������������ֽ�δ��
                 0:�رջ��� 1:�򿪻���              
*��������				: ��
*�޸�ʱ��				:
*��ע						:ESP8266����OK
*����           :����
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
*��������       :S_AT_RESTORE
*����˵��       : �ָ�ģ���������
*����˵��       : ʵ���޲���                  
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:ESP8266����OK��������Ὣflash�����ָ�ΪĬ��ֵ��������������
*����           :����
*********************************************************************/
unsigned char S_AT_RESTORE(char data[])
{
	ESP8266_sendByte("AT+RESTORE\r\n",12);
	return 0;
}
/********************************************************************
*��������       :S_AT_CWMODE_DEF
*����˵��       :�޸�ESP8266ģʽ
*����˵��       :data[0]������������ֽ�δ��
                 1:stationģʽ
 					       2:softAPģʽ
                 3��softAP+stationģʽ             
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:ESP8266����OK����������洢��flash�������粻��ʧ
*����           :����
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
*��������       :S_AT_CWJAP_DEF
*����˵��       :����wifi�ȵ㣬���浽flash
*����˵��       :data[0]������������ֽ�δ��
								 �����洢��ʽΪdata���δ��  vrunbst;xhywxd;
                 vrunbst��Ϊ���Լ����ȵ����ƣ�xhywxd��Ϊ�ȵ����������ַ���
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*��ע						:��������ʱ��ģ���账��stationģʽ
                 ��������洢��flash�������粻��ʧ
                 �ȵ����Ʋ�֧�������ַ��������ģ������ַ�����δ�ӣ��ȵ������Ϊ30���ַ�
                 �ȵ���������ͬ��������
                 ģ�鷵�أ�1 ���ӳ�ʱ
                           2 �������
                           3 �Ҳ���Ŀ��AP
                           4 ����ʧ��
*�޸�ʱ��				:                 
*����           :����
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
*��������       :S_AT_CWQAP
*����˵��       :�Ͽ���wifi����
*����˵��       :ʵ���޲���                  
*��������				:ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:ESP8266����OK
*����           :����
*********************************************************************/
unsigned char S_AT_CWQAP(char data[])
{
	ESP8266_sendByte("AT+CWQAP\r\n",11);
	return 0;
}
/********************************************************************
*��������       :S_AT_CWAUTOCONN
*����˵��       :�ϵ��Ƿ��Զ�����AP
*����˵��       :data[0]������������ֽ�δ��
                 0:�ϵ粻�Զ�����AP 1:�ϵ��Զ�����AP       
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:�������浽flash�����粻��ʧ��ESP8266����OK
*����           :����
*********************************************************************/
unsigned char S_AT_CWAUTOCONN(char data[])
{
	//assert_param(IS_WIFI_AUTO_CON_ALL_PERIPH(data[0]));
	*data ? ESP8266_sendByte("AT+CWAUTOCONN=1\r\n",17):ESP8266_sendByte("AT+CWAUTOCONN=0\r\n",17);
	return 0;
}
/********************************************************************
*��������       :S_AT_CWSTARTSMART
*����˵��       :����smartConfig
*����˵��       :data[0]������������ֽ�δ��
                 1:ESPTOCH
 					       2:AIRKISS
                 3:ESPTOCH+AIRKISS      
*��������				: ָ�����ɲ����ͳɹ�����0 ����1
*�޸�ʱ��				:
*��ע						:
*����           :����
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
*��������       :S_AT_CWSTOPSMART
*����˵��       :ֹͣsmartConfig
*����˵��       :ʵ���޲���                  
*��������				:ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:ESP8266����OK��������û�������ɹ���Ҫִ������ָ��
*����           :����
*********************************************************************/
unsigned char S_AT_CWSTOPSMART(char data[])
{
	ESP8266_sendByte("AT+CWSTOPSMART\r\n",16);
	return 0;
}
/********************************************************************
*��������       :S_AT_CIPSTATUS
*����˵��       :��ѯ��ǰ����״̬
*����˵��       :ʵ���޲���                  
*��������				:ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:ģ�鷵�أ�
                 2 ESP8266 station ������AP ���ip��ַ
								 3 ESP8266 station �Ѿ�����tcp��udp����
								 4 ESP8266 station �Ͽ���������
								 5 ESP8266 station δ����ap
*����           :����
*********************************************************************/
unsigned char S_AT_CIPSTATUS(char data[])
{
	ESP8266_sendByte("AT+CIPSTATUS\r\n",14);
	return 0;
}
/********************************************************************
*��������       :S_AT_CIPSTART
*����˵��       :���ӵ�ָ��tcp������
*����˵��       :data �д�ŷ�������ַ��˿�
                 ��Ÿ�ʽΪ��"192.168.111.111;54566; "���ַ���ASCII��ʽ
*��������				:ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:ģ�鷵��Ok �� ERROR�����Ѿ����ӣ�ģ�鷵��ALREADY CONNECT
                 esp8266֧��TCP��UDP��SSL���ӣ�Ŀǰ�˺���Ĭ��ΪTCP��δ֧����������
*����           :����
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
*��������       :S_AT_CIPMODE
*����˵��       :���ô���ģʽ
*����˵��       :data[0]������������ֽ�δ��
                 0:��ͨ����ģʽ 1:͸��ģʽ    
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:ESP8266����OK
*����           :����
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
*��������       :S_AT_CIPSEND
*����˵��       :��͸��ģʽ�¿���͸��
*����˵��       :  
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned char S_AT_CIPSEND(char data[])
{
	 ESP8266_sendByte("AT+CIPSEND\r\n",12);
	 return 0;
}
/********************************************************************
*��������       :S_AT_QUITSEND
*����˵��       :�˳�͸��ָ��
*����˵��       :  
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned char S_AT_QUITSEND(char data[])
{
	 ESP8266_sendByte("+++",3);
	 return 0;
}
/*******************��********************��****************************��**************************��**********************************/
/********************************************************************
*��������       : ESP8266_retStaTable
*����˵��       : ESP8266��������������
*����˵��       :                  
*��������				: 
*�޸�ʱ��				:
*��ע						:����λ��ʾ���ݣ��м���λ��ʾִ�е��ĸ��׶Σ�����λ��ʾ����״̬
*����           :����
*********************************************************************/
unsigned short int ESP8266_retStaTable[255];
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
*��������       : R_pro
*����˵��       : ������ָ�� ��Խ��յ������ݴ���,ͨ�ý��մ�����������⴦��
*����˵��       : ���յ�������                
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned short int R_pro(char data[],hashDecode* taskHash)
{
	unsigned short int taskData=0;
	unsigned short int taskSta=CMD_WAITE;/*��ǰָ��ִ��״̬*/
	unsigned short int staCode=0;/*�ȴ�+ģ�鷵��״̬�µ�����״̬*/
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
*��������       :R_AT_CWSTARTSMART
*����˵��       :������AT+CWSTARTSMART����󣬶Խ��յ������ݽ��д���
*����˵��       :���յ�������            
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned short int R_AT_CWSTARTSMART(char data[],hashDecode* taskHash)
{
	unsigned short int taskData=0;
	unsigned short int taskSta=CMD_WAITE;/*��ǰָ��ִ��״̬*/
	unsigned short int staCode=0;/*�ȴ�+ģ�鷵��״̬�µ�����״̬*/
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
*��������       :R_AT_CIPSTART
*����˵��       :������AT+CIPSTART����󣬶Խ��յ������ݽ��д���
*����˵��       :���յ�������            
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned short int R_AT_CIPSTART(char data[],hashDecode* taskHash)
{
	unsigned short int taskData=0;
	unsigned short int taskSta=CMD_WAITE;/*��ǰָ��ִ��״̬*/
	unsigned short int staCode=0;/*�ȴ�+ģ�鷵��״̬�µ�����״̬*/
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
*��������       :R_AT_CIPMODE
*����˵��       :������AT+CIPMODE����󣬶Խ��յ������ݽ��д���
*����˵��       :���յ�������            
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
unsigned short int R_AT_CIPMODE(char data[],hashDecode* taskHash)
{
	unsigned short int taskData=0;
	unsigned short int taskSta=CMD_WAITE;/*��ǰָ��ִ��״̬*/
	unsigned short int staCode=0;/*�ȴ�+ģ�鷵��״̬�µ�����״̬*/
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
*��������       :R_AT_QUITSEND
*����˵��       :�˳�͸��ָ��ظ�����
*����˵��       :  
*��������				: ָ�����ɲ����ͳɹ�����0 ���򷵻�1
*�޸�ʱ��				:
*��ע						:
*����           :����
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
*��������       : strSum
*����˵��       : ��ָ���ַ���ָ��λ��ָ�����ȵ��ַ���
*����˵��       :
                  ��
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
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


