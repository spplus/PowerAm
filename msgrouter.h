/************************************************************************/
/* 
		DESC:	��Ϣ·�ɣ��������Է���������Ϣ��������Ϣ���ͣ��ַ���ͬ��
				ҵ����ģ����д���
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