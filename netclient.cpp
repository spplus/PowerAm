#include "netclient.h"

NetClient* NetClient::m_inst = NULL;

NetClient::NetClient()
{

}

NetClient* NetClient::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new NetClient();
	}
	return m_inst;
}

void NetClient::connectToServer(const QString srvName,qint16 port)
{

}

void NetClient::close()
{

}

int NetClient::sendData(int msgtype,const char* msg,int msglength)
{
	return 0;
}

char* NetClient::unpack(const char* msg,const int msglength,int &msgtyhpe)
{
	return NULL;
}

char* NetClient::pack(const char* msg,const int msgtype,const int msglength)
{
	return NULL;
}