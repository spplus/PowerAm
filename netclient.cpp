
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

	//�ͻ��������ź�
	connect(m_pTcpScoket,SIGNAL(connected()),this,SLOT(connectOpen()));

	//�ͻ��˶Ͽ��ź�
	connect(m_pTcpScoket,SIGNAL(disconnected()),this,SLOT(connectLost()));

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

bool NetClient::init()
{
	
	QString strIP = ComUtil::instance()->getSvrAddr();
	QString strPort = ComUtil::instance()->getSvrPort();

	//��ֵ
	m_IP = strIP;
	m_Port = strPort;

	//�ж϶�ȡINI�����ļ��Ƿ���ȷ
	if(m_IP=="" || m_Port=="")
	{
		QMessageBox::information(NULL,tr("��ȡ�����ļ���ʾ:"),tr("���������ļ������Ƿ���ȷ���ļ��Ƿ����!"));
		return false;
	}

	//���������������뵱ǰ����ź���۵Ĺ�ϵ
	connect(this,SIGNAL(recvdata(int,const char* ,int)),MsgRouter::instance(),SLOT(recvdata(int,const char* ,int)));

	//��ʱ��
	m_pTimer = new QTimer(this);
	m_pTimer->setInterval(1);

	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(checkConnect()));

	//����ָ��IP�Ͷ˿ڵķ�����
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
	//��ʱ������ӷ�����Ƿ���������������������
	if(!m_netflag)
	{
		//��������ָ��IP�Ͷ˿ڵķ�����
		connectToServer(m_IP,m_Port.toUInt());
	}
}

void NetClient::connectOpen()
{
	m_netflag = true;
	m_pTimer->stop();

	// �������ӽ�����Ϣ
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
	
	//ȡ�����е�����
	m_pTcpScoket->abort();

	//���ӷ�����
	m_pTcpScoket->connectToHost(srvName,port);

	//�ȴ��ͻ����Ƿ��������Ϸ����
	/*if(!m_pTcpScoket->waitForConnected(1000))
	{
		qDebug("�ͻ������ӷ�����ʧ��");
		return false;
	}*/

	return true;

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
	while(1)
	{
		m_qbarecv.append(m_pTcpScoket->readAll());

		//����һ�ν��յ������еĳ�����ȡ������Ϊ�Ժ���մ������ݼ��㳤��
		if (m_brecvflag)
		{
			//ȡ����Ϣ�е�ǰ4���ֽڣ���Ϣ�ܳ���
			qint32 msgLen = m_qbarecv[0] & 0x000000FF;    
			msgLen |= ((m_qbarecv[1] << 8) & 0x0000FF00);    
			msgLen |= ((m_qbarecv[2] << 16) & 0x00FF0000);    
			msgLen |= ((m_qbarecv[3] << 24) & 0xFF000000);

			m_recvLen = msgLen;

			m_brecvflag = false;
		}

		//�����ճ���С����С֡��ʱֱ�ӷ���
		if (m_qbarecv.length() < 12)
		{
			qDebug("�������ݳ���С����С֡����");
			return;
		}

		//���������ݳ������ʱ����ѭ��
		if (m_qbarecv.length() == m_recvLen)
		{
			break;
		}

		//�ȴ������ÿ�η�������
		if(!(m_pTcpScoket->waitForReadyRead(10000)))
		{
			qDebug()<<"m_pTcpScoket->errorString()2: "<< m_pTcpScoket->errorString();
			//return;
		}

	}
	
	QString recstr = QString("������Ϣ:");
	qDebug()<<recstr;
	QString strrecv=QVariant(m_qbarecv).toString();
	qDebug()<<strrecv;

	if (m_qbarecv.length() < 12)
	{
		qDebug("�������ݳ���С����С֡����");
		return;
	}

	int nmsgty = 0;

	QByteArray qBymsg = unpack(m_qbarecv,nmsgty,m_qbarecv.length());
	//���ݽ��
	const char* msg = qBymsg.data();

	//��Ӧ�ò㷢�ź�
	emit recvdata(nmsgty,msg,m_qbarecv.length());
	
	m_brecvflag = true;
	m_recvLen = 0;
	m_qbarecv.clear();

	return;
}


QByteArray NetClient::pack(const char* msg,const int msgtype,const int msglength)
{

	QByteArray bpack;

	//��Ϣ����		 ��Ϣͷ		 ��Ϣ����		 ����������		 ��Ϣ��β
	// 4���ֽ�	  2�ֽ�(0x11)	  4���ֽ�						2�ֽ�(0x88)
	//������Ϣ������
	quint32 len = 2 + 4 + msglength + 2;

	//д����Ϣ����
	bpack.append(reinterpret_cast<const char*>(&len),4);
	//��Ϣͷ
	quint32 nhead = 0x11;
	bpack.append(reinterpret_cast<const char*>(&FRAM_HEAD),2);
	//��Ϣ����
	bpack.append(reinterpret_cast<const char*>(&msgtype),4);
	//������
	bpack.append(msg,msglength);
	//��Ϣβ
	quint32 ntail = 0x88;
	bpack.append(reinterpret_cast<const char*>(&FRAM_TAIL),2);

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
	if (msgHead != FRAM_HEAD)
	{
		qDebug("��������֡����Ϣͷ�ؼ���δ�ҵ�");
		return 0;
	}

	//��Ϣβ
	qint16 msgTail = *reinterpret_cast<qint16*>(qByte.mid(recvlen-2, 2).data());
	if (msgTail != FRAM_TAIL)
	{
		qDebug("��������֡����Ϣβ�ؼ���δ�ҵ�");
		return 0;
	}

	//��Ϣ����
	qint32 msgType = *reinterpret_cast<qint32*>(qByte.mid(4+2, 4).data());
	msgtype = msgType;

	//��Ϣ��
	QByteArray qData = qByte.mid(4+2+4,recvlen-12);

	return qData;

}

