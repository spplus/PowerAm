#include "msgrouter.h"
#include "mainwindow.h"
#include "homewindow.h"


MsgRouter* MsgRouter::m_inst = NULL;

MsgRouter* MsgRouter::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new MsgRouter;
	}
	return m_inst;
}

MsgRouter::MsgRouter()
{

}

void MsgRouter::recvdata(int msgtype,const char* msg,int msglength)
{
	switch(msgtype)
	{
	case CMD_CONNECTED:
		qDebug("connected ok!");
		break;
	case CMD_DISCONNECTED:
		qDebug("disconnected!");
		break;
	case CMD_DEV_STATE:
		MainWindow::instance()->recvdata(msgtype,msg,msglength);
		break;
	default:
		break;
	}
}