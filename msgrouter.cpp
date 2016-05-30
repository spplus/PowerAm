#include "msgrouter.h"
#include "mainwindow.h"
#include "homewindow.h"
#include "userlogindlg.h"
#include "stationmgr.h"

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
	case CMD_USER_MANAGER:
	case CMD_USER_ROLE:
	case CMD_USER_ADD:
	case CMD_USER_DEL:
	case CMD_USER_MODIFY:
	case CMD_PWD_MODIFY:
	case CMD_STATION_TYPE:
	case CMD_STATION_LIST:
	case CMD_STATION_MANAGER:
		HomeWindow::instance()->recvdata(msgtype,msg,msglength);
		break;
	case CMD_USER_LONGIN:
		UserLogindlg::instance()->recvdata(msgtype,msg,msglength);
		break;
	case CMD_STATION_TYPE_ADD:
	case CMD_STATION_TYPE_DEL:
	case CMD_STATION_TYPE_MODIFY:
	case CMD_STATION_TYPE_LIST:
		StationTypeMgr::instance()->recvdata(msgtype,msg,msglength);
		break;
	default:
		break;
	}
}