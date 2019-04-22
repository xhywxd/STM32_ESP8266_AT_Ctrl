/***************(C)COPYRIGHT 2017 Vrunst工作室_WXD***************
*文件名         :WIFI.h
*描述           :系统wifi连接相关
*实验平台       :
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 2017.11.17
*备注           :本资料仅供学习交流，请勿私自用于商业用途
                 如有问题请邮件联系：xhywxd@126.com
**************************************************************************/
#ifndef _WIFI_H
#define _WIFI_H
/*系统网络连接状态宏定义*/ 
typedef enum NET_STA
{
	WIFI_DIS_CONNECT =0,/*未连接wifi,红灯慢闪烁*/
	WIFI_CONNECTING_AP=1,/*正在连接AP，智能配网中，红灯快闪烁*/
	WIFI_CONNECTED_AP=2,/*已连接wifi AP，未连接到tcp服务器，正在连接服务器，绿灯快闪，红灯灭*/
	WIFI_CONNECT_TCP_SERVER=3,/*已 连接TCP服务，绿灯慢闪*/ 
	WIFI_SERIAL=4
}NET_STA;


#endif
