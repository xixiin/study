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

//msg net to ctrl
#define MsgLen 64

enum{
	_e_ntc_type_on = 0,		//启动订单消息
	_e_ntc_type_off,		//结束订单消息

};

typedef struct{
	uint32_t type;		//消息类型
	uint32_t road;			//描述第几路插座，从零开始数
	uint32_t time;          //充电时长
	uint32_t rec[5];
	
	char ddh[32];			//订单号


}order_dat_t;

typedef struct{
	void (*init)(void);
	void (*put_order)( int type,int road,uint32_t time,char* ddh );
	int (*get)( void* data,int len );
		
}msg_ntc_opt_t;

extern msg_ntc_opt_t* msg_ntc_opt;

//msg ctrl to net
typedef struct{
	void (*init)(void);
	void (*put_curr_report)( char* ddh,int num ,int road,float curr,float pwr,float vol,uint32_t usetime);
	void (*put_over_report)( char* ddh,float ekwh,uint32_t time,uint32_t event );
	void (*work)(void *ndev);
		
}msg_ctn_opt_t;

extern msg_ctn_opt_t* msg_ctn_opt;
extern int cat_get_time_flag();


#endif
