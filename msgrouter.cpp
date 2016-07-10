#include "msgrouter.h"
#include "mainwindow.h"
#include "homewindow.h"
#include "userlogindlg.h"
#include "stationmgr.h"
#include "rulemgrdlg.h"

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
	case CMD_DISCONNECTED:
	case CMD_DEV_STATE:
	case CMD_LINE_SET:
	case CMD_POWER_SET:
	case CMD_TAG_OP:
	case CMD_QUERY_CIRCLE_LIST:
	case CMD_QUERY_SIGN_LIST:
	case CMD_QUERY_GSWITCH_LIST:
	case CMD_QUERY_MSET_LIST:
	case CMD_QUERY_EVENT_LIST:
	case CMD_READ_SAVING:
	case CMD_WRITE_SAVING:
	case CMD_TOPO_BREAKER_CHANGE:
	case CMD_TRIGGER_RULES:
	case CMD_CHECK_PASS:
	case CMD_TOPO_ENTIRE:
	case CMD_TICKETMS_LIST:
	case CMD_TICKETMS_MANAGER:
	case CMD_TICKETMS_ADD:
	case CMD_TICKETMS_DEL:
	case CMD_TICKETMS_MODIFY:
		MainWindow::instance()->recvdata(msgtype,msg,msglength);
		break;
	case CMD_USER_MANAGER:
	case CMD_USER_LIST:
	case CMD_USER_ROLE:
	case CMD_USER_ADD:
	case CMD_USER_DEL:
	case CMD_ROLE_USER_LIST:
	case CMD_USER_MODIFY:
	case CMD_PWD_MODIFY:
	case CMD_STATION_TYPE:
	case CMD_STATION_LIST:
	case CMD_STATION_MANAGER:
	case CMD_STATION_RULE_LIST:
	case CMD_STATION_RULE_MGR:
	case CMD_COM_RULE_LIST:
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
	case CMD_RULE_LIST:
	case CMD_RULE_ADD:
	case CMD_RULE_DEL:
	case CMD_RULE_MODIFY:
		RuleTypeMgrdlg::instance()->recvdata(msgtype,msg,msglength);
		break;
	default:
		break;
	}
}