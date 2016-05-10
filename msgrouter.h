/************************************************************************/
/* 
		DESC:	消息路由，处理来自服务器的消息，根据消息类型，分发不同的
				业务处理模块进行处理。
		DATE:	2016-05-10
		AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __MSGROUTER_H__
#define __MSGROUTER_H__
#include <QObject>
#include "include/commands.h"

class MsgRouter	:public QObject
{
	Q_OBJECT
public:
	static MsgRouter* instance();

public slots:
	void	recvdata(int msgtype,const char* msg,int msglength);

private:
	MsgRouter();
private:

	static MsgRouter*	m_inst;
};

#endif