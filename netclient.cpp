
#include <QSettings>
#include <QCoreApplication>
#include <QMessageBox>
#include "netclient.h"
#include "mainwindow.h"
#include "homewindow.h"
#include "msgrouter.h"

NetClient* NetClient::m_inst = NULL;

NetClient::NetClient()
{
	m_netflag = false;

	m_brecvflag = true;

	m_recvLen = 0;

	m_pTcpScoket = new QTcpSocket(this);

	//客户端连接信号
	connect(m_pTcpScoket,SIGNAL(connected()),this,SLOT(connectOpen()));

	//客户端断开信号
	connect(m_pTcpScoket,SIGNAL(disconnected()),this,SLOT(connectLost()));

	//客户端接收服务器端的数据响应
	connect(m_pTcpScoket,SIGNAL(readyRead()),this,SLOT(readMessage()));

}

/**/
NetClient* NetClient::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new NetClient();
	}
	return m_inst;
}

bool NetClient::init()
{
	
	QString strIP = ComUtil::instance()->getSvrAddr();
	QString strPort = ComUtil::instance()->getSvrPort();

	//赋值
	m_IP = strIP;
	m_Port = strPort;

	//判断读取INI配置文件是否正确
	if(m_IP=="" || m_Port=="")
	{
		QMessageBox::information(NULL,tr("读取配置文件提示:"),tr("请检查配置文件内容是否正确或文件是否存在!"));
		return false;
	}

	//建立网络连接类与当前类的信号与槽的关系
	connect(this,SIGNAL(recvdata(int,const char* ,int)),MsgRouter::instance(),SLOT(recvdata(int,const char* ,int)));

	//定时器
	m_pTimer = new QTimer(this);
	m_pTimer->setInterval(1);

	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(checkConnect()));

	//连接指定IP和端口的服务器
	connectToServer(m_IP,m_Port.toUInt());
	m_pTcpScoket->waitForConnected();

	if (!m_netflag)
	{
		m_pTimer->start();
	}
	return m_netflag;
}

void NetClient::checkConnect()
{
	//定时检测连接服务端是否正常，不正常进行重连
	if(!m_netflag)
	{
		//重新连接指定IP和端口的服务器
		connectToServer(m_IP,m_Port.toUInt());
	}
}

void NetClient::connectOpen()
{
	m_netflag = true;
	m_pTimer->stop();

	// 发送连接建立消息
	emit recvdata(CMD_CONNECTED,0,0);
}

void NetClient::connectLost()
{
	m_netflag = false;
	m_pTimer->start();
	emit recvdata(CMD_DISCONNECTED,0,0);
}
bool NetClient::connectToServer(const QString srvName,quint16 port)
{
	
	//取消已有的连接
	m_pTcpScoket->abort();

	//连接服务器
	m_pTcpScoket->connectToHost(srvName,port);

	//等待客户端是否真正连上服务端
	/*if(!m_pTcpScoket->waitForConnected(1000))
	{
		qDebug("客户端连接服务器失败");
		return false;
	}*/

	return true;

}



void NetClient::close()
{
	m_pTcpScoket->disconnectFromHost();
	if (!m_pTcpScoket->waitForDisconnected(1000))
	{
		qDebug("客户端断开服务器失败");
	}
}


int NetClient::sendData(int msgtype,const char* msg,int msglength)
{
	
	QByteArray byteary = pack(msg,msgtype,msglength);
	QString strbyte = tr("%1").arg(byteary.data());
	QString strbyt = QString::fromAscii(byteary.data());
	m_pTcpScoket->write(byteary);


	return 0;
}

void NetClient::readMessage()
{
	while(1)
	{
		m_qbarecv.append(m_pTcpScoket->readAll());

		//将第一次接收的数据中的长度提取出来，为以后接收大量数据计算长度
		if (m_brecvflag)
		{
			//取出消息中的前4个字节：消息总长度
			qint32 msgLen = m_qbarecv[0] & 0x000000FF;    
			msgLen |= ((m_qbarecv[1] << 8) & 0x0000FF00);    
			msgLen |= ((m_qbarecv[2] << 16) & 0x00FF0000);    
			msgLen |= ((m_qbarecv[3] << 24) & 0xFF000000);

			m_recvLen = msgLen;

			m_brecvflag = false;
		}

		//当接收长度小于最小帧长时直接返回
		if (m_qbarecv.length() < 12)
		{
			qDebug("接收数据长度小于最小帧长度");
			return;
		}

		//当接收数据长度相等时跳出循环
		if (m_qbarecv.length() == m_recvLen)
		{
			break;
		}

		//等待服务端每次发送数据
		if(!(m_pTcpScoket->waitForReadyRead(20000)))
		{
			qDebug()<<"m_pTcpScoket->errorString()2: "<< m_pTcpScoket->errorString();
			//return;
		}

	}

	QString strrecv=QVariant(m_qbarecv).toString();

	if (m_qbarecv.length() < 12)
	{
		qDebug("接收数据长度小于最小帧长度");
		return;
	}

	int nmsgty = 0;

	QByteArray qBymsg = unpack(m_qbarecv,nmsgty,m_qbarecv.length());
	//数据解包
	const char* msg = qBymsg.data();

	//向应用层发信号
	emit recvdata(nmsgty,msg,m_qbarecv.length());
	
	m_brecvflag = true;
	m_recvLen = 0;
	m_qbarecv.clear();

	return;
}


QByteArray NetClient::pack(const char* msg,const int msgtype,const int msglength)
{

	QByteArray bpack;

	//消息长度		 消息头		 消息类型		 数据区内容		 消息结尾
	// 4个字节	  2字节(0x11)	  4个字节						2字节(0x88)
	//计算消息区长度
	quint32 len = 2 + 4 + msglength + 2;

	//写入消息长度
	bpack.append(reinterpret_cast<const char*>(&len),4);
	//消息头
	quint32 nhead = 0x11;
	bpack.append(reinterpret_cast<const char*>(&FRAM_HEAD),2);
	//消息类型
	bpack.append(reinterpret_cast<const char*>(&msgtype),4);
	//数据体
	bpack.append(msg,msglength);
	//消息尾
	quint32 ntail = 0x88;
	bpack.append(reinterpret_cast<const char*>(&FRAM_TAIL),2);

	return bpack;

}


QByteArray NetClient::unpack(QByteArray qByte,int &msgtype,int recvlen)
{
	if (qByte.length() < 12)
	{
		qDebug("接收数据长度小于最小帧长度");
		return 0;
	}

	//消息长度		 消息头		 消息类型		 数据区内容		 消息结尾
	// 4个字节	  2字节(0x11)	  4个字节						2字节(0x88)

	//取出消息中的前4个字节：消息总长度
	qint32 msgLen = qByte[0] & 0x000000FF;    
	msgLen |= ((qByte[1] << 8) & 0x0000FF00);    
	msgLen |= ((qByte[2] << 16) & 0x00FF0000);    
	msgLen |= ((qByte[3] << 24) & 0xFF000000);

	//消息头
	qint16 msgHead = *reinterpret_cast<qint16*>(qByte.mid(4, 2).data());
	if (msgHead != FRAM_HEAD)
	{
		qDebug("接收数据帧中消息头关键字未找到");
		return 0;
	}

	//消息尾
	qint16 msgTail = *reinterpret_cast<qint16*>(qByte.mid(recvlen-2, 2).data());
	if (msgTail != FRAM_TAIL)
	{
		qDebug("接收数据帧中消息尾关键字未找到");
		return 0;
	}

	//消息类型
	qint32 msgType = *reinterpret_cast<qint32*>(qByte.mid(4+2, 4).data());
	msgtype = msgType;

	//消息体
	QByteArray qData = qByte.mid(4+2+4,recvlen-12);

	return qData;

}

