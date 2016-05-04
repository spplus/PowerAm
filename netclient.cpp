#include "netclient.h"

NetClient* NetClient::m_inst = NULL;

NetClient::NetClient()
{
	m_pTcpScoket = new QTcpSocket(this);
	//客户端连接信号
	connect(m_pTcpScoket,SIGNAL(connected()),this,SIGNAL(connected()));
	//connect(m_pTcpScoket,SIGNAL(connected()),this,SLOT(testconn()));
	//客户端断开信号
	connect(m_pTcpScoket,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
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


bool NetClient::connectToServer(const QString srvName,quint16 port)
{
	
	//取消已有的连接
	m_pTcpScoket->abort();
	//连接服务器
	m_pTcpScoket->connectToHost(srvName,port);
	//等待客户端是否真正连上服务端
	if(!m_pTcpScoket->waitForConnected(1000))
	{
		qDebug("客户端连接服务器失败");
		return false;
	}

	return true;

}

void NetClient::testconn()
{
	qDebug("==================testconn=====================");

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
	QByteArray qbarecv = m_pTcpScoket->readAll();
	qDebug()<<qbarecv;
	QString strrecv=QVariant(qbarecv).toString();

	if (qbarecv.length() < 12)
	{
		qDebug("接收数据长度小于最小帧长度");
		return;
	}

	int nmsgty = 0;

	QByteArray qBymsg = unpack(qbarecv,nmsgty,qbarecv.length());
	//数据解包
	const char* msg = qBymsg.data();

	//向应用层发信号
	emit recvdata(nmsgty,msg,qbarecv.length());
	
	return;
}


QByteArray NetClient::pack(const char* msg,const int msgtype,const int msglength)
{

	QByteArray bpack;
	QByteArray bbpack;
	
	//消息长度		 消息头		 消息类型		 数据区内容		 消息结尾
	// 4个字节	  2字节(0x11)	  4个字节						2字节(0x88)
	//计算消息长度
	quint32 len =  2 + 4 + strlen(msg) + 2;

	//写入消息长度
	bpack.append(reinterpret_cast<const char*>(&len),4);
	//消息头
	quint32 nhead = 0x11;
	bpack.append(reinterpret_cast<const char*>(&nhead),2);
	//消息类型
	bpack.append(reinterpret_cast<const char*>(&msgtype),4);
	//数据体
	bpack.append(msg,strlen(msg));
	//消息尾
	quint32 ntail = 0x88;
	bpack.append(reinterpret_cast<const char*>(&ntail),2);

/**/

	//int i = 200;
	QByteArray bytes;
	bytes.resize(4);
	bytes[0] = (unsigned char)(0x000000ff & len);
	bytes[1] = (unsigned char)((0x0000ff00 & len) >> 8);
	bytes[2] = (unsigned char)((0x00ff0000 & len) >> 16);
	bytes[3] = (unsigned char)((0xff000000 & len) >> 24);

/*
	QString strLen = tr("%1").arg(len);
	//写入消息长度
	//计算出的缓存区长度初始值
	bpack.resize(len);
	memcpy(bpack.data(),strLen.toLatin1().data(),4);
	
	//消息头
	quint32 nhead = 255;
	QString strHead = tr("%1").arg(nhead);
	memcpy(bpack.data()+4,strHead.toLatin1().data(),2);
	
	//消息类型
	QString strType = tr("%1").arg(msgtype);
	memcpy(bpack.data()+6,strType.toLatin1().data(),4);
	
	//数据体
	memcpy(bpack.data()+10,msg,strlen(msg));
	
	//消息尾
	quint32 ntail = 136;
	QString strTail = tr("%1").arg(ntail);
	memcpy(bpack.data()+10+strlen(msg),strTail.toLatin1().data(),2);
*/

/*
	// 分配打包后的消息缓冲区
	char * buff = new char[len];
	int pos = 0;

	//写入消息长度
	size_t lens = 4;
	memcpy(buff+pos,&len,4);
	//memcpy(buff+pos,strLen.toLatin1().data(),4);
	pos += 4;

	// 消息头
	memcpy(buff+pos,&nhead,2);
	//memcpy(buff+pos,strHead.toLatin1().data(),2);
	pos += 2;

	//消息类型
	memcpy(buff+pos,&msgtype,4);
	pos += 4;

	//消息内容
	memcpy(buff+pos,msg,strlen(msg));
	pos += strlen(msg);

	//消息尾
	memcpy(buff+pos,&ntail,2);
	pos += 2;
*/

	/*
	//用于暂存要发送的数据  
	QByteArray block;  
	//使用数据流写入数据  
	QDataStream out(&block,QIODevice::ReadWrite);  
	//设置数据流的版本，客户端和服务器端使用的版本要相同  
	out.setVersion(QDataStream::Qt_4_8);
	//设置发送长度初始值为0 
	out << quint32(0);
	//out << len;
	//消息头
	//out << (quint32)255;
	//消息类型
	//out << (quint32)msgtype;
	//消息体
	out << msg;
	//消息尾
	//out << (quint32)136;
	
	//回到字节流起始位置  
	out.device()->seek(0);  
	//重置字节流长度  
	out << len <<(quint32) (block.size()-sizeof(quint32));

	QString stt = tr("testststts");
	QString strbf = tr("%1").arg(block.data());
	*/

	//;return block.data();
	//return bpack;
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
	if (msgHead != 0x11)
	{
		qDebug("接收数据帧中消息头关键字未找到");
		return 0;
	}

	//消息尾
	qint16 msgTail = *reinterpret_cast<qint16*>(qByte.mid(recvlen-2, 2).data());
	if (msgTail != 0x88)
	{
		qDebug("接收数据帧中消息尾关键字未找到");
		return 0;
	}

	//消息类型
	qint32 msgType = *reinterpret_cast<qint32*>(qByte.mid(4+2, 4).data());
	msgtype = msgType;

	//消息体
	QByteArray qData = qByte.mid(4+2+4,recvlen-12);

	//消息体转换为char
	char *rdata = qData.data();

	return qData;

}

/*
char* NetClient::unpack(const char* msg,const int msglength,int &msgtype)
{
	
	return NULL;
}
*/
