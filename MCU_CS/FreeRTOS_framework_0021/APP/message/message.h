#ifndef __message__H
#define __message__H

#include "framework.h"
#include "network.h"

//enum{
//	_srv_ret_Logon = 0,   //服务器返回登录信息 
//	_srv_ret_unknow,	  //服务器返回unknow，需要重新登录
//	_srv_ret_time,       //服务器返回时间
//	_srv_ret_pong,       //服务器返回心跳
//	_srv_ret_over,       //服务器返回over,结束事件

//	_srv_cmd_update,     //服务器下发命令 ota
//	_srv_cmd_on,    	//服务器下发开始订单
//	_srv_cmd_off,     	//服务器下发结束订单
//	
//	
//	_srv_cmd_max,
//	
//	
//};


typedef int (*cmd_cb_t)(void* ndev,int argc,char** argv);

typedef struct{
	char* name;					//字符串
	cmd_cb_t entry;		//操作函数

}net_cmd_t;

extern int msg_cycle(void* ndev,int argc,char** argv);
extern int net_cycle(void* ndev);


#endif
