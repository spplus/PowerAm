#include "netclient.h"

NetClient* NetClient::m_inst = NULL;

NetClient::NetClient()
{
	m_pTcpScoket = new QTcpSocket(this);
	//�ͻ��������ź�
	connect(m_pTcpScoket,SIGNAL(connected()),this,SIGNAL(connected()));
	//connect(m_pTcpScoket,SIGNAL(connected()),this,SLOT(testconn()));
	//�ͻ��˶Ͽ��ź�
	connect(m_pTcpScoket,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
	//�ͻ��˽��շ������˵�������Ӧ
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
	
	//ȡ�����е�����
	m_pTcpScoket->abort();
	//���ӷ�����
	m_pTcpScoket->connectToHost(srvName,port);
	//�ȴ��ͻ����Ƿ��������Ϸ����
	if(!m_pTcpScoket->waitForConnected(1000))
	{
		qDebug("�ͻ������ӷ�����ʧ��");
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
		qDebug("�ͻ��˶Ͽ�������ʧ��");
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
		qDebug("�������ݳ���С����С֡����");
		return;
	}

	int nmsgty = 0;

	QByteArray qBymsg = unpack(qbarecv,nmsgty,qbarecv.length());
	//���ݽ��
	const char* msg = qBymsg.data();

	//��Ӧ�ò㷢�ź�
	emit recvdata(nmsgty,msg,qbarecv.length());
	
	return;
}


QByteArray NetClient::pack(const char* msg,const int msgtype,const int msglength)
{

	QByteArray bpack;
	QByteArray bbpack;
	
	//��Ϣ����		 ��Ϣͷ		 ��Ϣ����		 ����������		 ��Ϣ��β
	// 4���ֽ�	  2�ֽ�(0x11)	  4���ֽ�						2�ֽ�(0x88)
	//������Ϣ����
	quint32 len =  2 + 4 + strlen(msg) + 2;

	//д����Ϣ����
	bpack.append(reinterpret_cast<const char*>(&len),4);
	//��Ϣͷ
	quint32 nhead = 0x11;
	bpack.append(reinterpret_cast<const char*>(&nhead),2);
	//��Ϣ����
	bpack.append(reinterpret_cast<const char*>(&msgtype),4);
	//������
	bpack.append(msg,strlen(msg));
	//��Ϣβ
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
	//д����Ϣ����
	//������Ļ��������ȳ�ʼֵ
	bpack.resize(len);
	memcpy(bpack.data(),strLen.toLatin1().data(),4);
	
	//��Ϣͷ
	quint32 nhead = 255;
	QString strHead = tr("%1").arg(nhead);
	memcpy(bpack.data()+4,strHead.toLatin1().data(),2);
	
	//��Ϣ����
	QString strType = tr("%1").arg(msgtype);
	memcpy(bpack.data()+6,strType.toLatin1().data(),4);
	
	//������
	memcpy(bpack.data()+10,msg,strlen(msg));
	
	//��Ϣβ
	quint32 ntail = 136;
	QString strTail = tr("%1").arg(ntail);
	memcpy(bpack.data()+10+strlen(msg),strTail.toLatin1().data(),2);
*/

/*
	// �����������Ϣ������
	char * buff = new char[len];
	int pos = 0;

	//д����Ϣ����
	size_t lens = 4;
	memcpy(buff+pos,&len,4);
	//memcpy(buff+pos,strLen.toLatin1().data(),4);
	pos += 4;

	// ��Ϣͷ
	memcpy(buff+pos,&nhead,2);
	//memcpy(buff+pos,strHead.toLatin1().data(),2);
	pos += 2;

	//��Ϣ����
	memcpy(buff+pos,&msgtype,4);
	pos += 4;

	//��Ϣ����
	memcpy(buff+pos,msg,strlen(msg));
	pos += strlen(msg);

	//��Ϣβ
	memcpy(buff+pos,&ntail,2);
	pos += 2;
*/

	/*
	//�����ݴ�Ҫ���͵�����  
	QByteArray block;  
	//ʹ��������д������  
	QDataStream out(&block,QIODevice::ReadWrite);  
	//�����������İ汾���ͻ��˺ͷ�������ʹ�õİ汾Ҫ��ͬ  
	out.setVersion(QDataStream::Qt_4_8);
	//���÷��ͳ��ȳ�ʼֵΪ0 
	out << quint32(0);
	//out << len;
	//��Ϣͷ
	//out << (quint32)255;
	//��Ϣ����
	//out << (quint32)msgtype;
	//��Ϣ��
	out << msg;
	//��Ϣβ
	//out << (quint32)136;
	
	//�ص��ֽ�����ʼλ��  
	out.device()->seek(0);  
	//�����ֽ�������  
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
		qDebug("�������ݳ���С����С֡����");
		return 0;
	}

	//��Ϣ����		 ��Ϣͷ		 ��Ϣ����		 ����������		 ��Ϣ��β
	// 4���ֽ�	  2�ֽ�(0x11)	  4���ֽ�						2�ֽ�(0x88)

	//ȡ����Ϣ�е�ǰ4���ֽڣ���Ϣ�ܳ���
	qint32 msgLen = qByte[0] & 0x000000FF;    
	msgLen |= ((qByte[1] << 8) & 0x0000FF00);    
	msgLen |= ((qByte[2] << 16) & 0x00FF0000);    
	msgLen |= ((qByte[3] << 24) & 0xFF000000);
	
	//��Ϣͷ
	qint16 msgHead = *reinterpret_cast<qint16*>(qByte.mid(4, 2).data());
	if (msgHead != 0x11)
	{
		qDebug("��������֡����Ϣͷ�ؼ���δ�ҵ�");
		return 0;
	}

	//��Ϣβ
	qint16 msgTail = *reinterpret_cast<qint16*>(qByte.mid(recvlen-2, 2).data());
	if (msgTail != 0x88)
	{
		qDebug("��������֡����Ϣβ�ؼ���δ�ҵ�");
		return 0;
	}

	//��Ϣ����
	qint32 msgType = *reinterpret_cast<qint32*>(qByte.mid(4+2, 4).data());
	msgtype = msgType;

	//��Ϣ��
	QByteArray qData = qByte.mid(4+2+4,recvlen-12);

	//��Ϣ��ת��Ϊchar
	char *rdata = qData.data();

	return qData;

}

/*
char* NetClient::unpack(const char* msg,const int msglength,int &msgtype)
{
	
	return NULL;
}
*/
