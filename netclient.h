/************************************************************************/
/* 
	DESC:			TCP�ͻ�������ת���㣬ʵ��ҵ������ݺ�TCP�����֮���������ת
						���ݵĴ�������.
	DATE:			2016-04-03
	AUTHOR:		YUANLS
*/
/************************************************************************/

#ifndef	__NetClient_H__
#define	__NetClient_H__


#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QTextCodec>

#define MSGHEAD "0xff"
#define MSGTAIL "0x88"

//�������ݰ���ͷ
const qint32 FRAM_HEAD = 0x11;

//�������ݰ���δ
const qint32 FRAM_TAIL = 0x88;

class NetClient :public QObject
{
	Q_OBJECT
public:

	// ��ȡ����
	static NetClient* instance();

	// ��ʼ������
	bool	init();

	// ���ӷ�����
	bool	connectToServer(const QString srvName,quint16 port);

	// ���������������
	int		sendData(int msgtype,const char* msg,int msglength);

	// �ر�����
	void	close();

private:
	NetClient();

signals:
	
	// ���ӽ���
	void	connected();

	// ���ӶϿ�
	void	disconnected();

	// ����������ɣ���Ӧ�ò㴫��
	void	recvdata(int msgtype,const char* msg,int msglength);

public slots:

	// ���ӽ���
	void		connectOpen();

	// ���ӶϿ�
	void		connectLost();

	//���շ���������
	void		readMessage();

	//����������������
	void		checkConnect();

private:
	// ���
	QByteArray	pack(const char* msg,const int msgtype,const int msglength);

	// ��� 
	QByteArray  unpack(QByteArray qByte,int &msgtype,int recvlen);


private:
	static NetClient*	m_inst;

	//Socket����
	QTcpSocket*		m_pTcpScoket;

	//���Ӷ˿ں�
	QString			m_Port;

	//����IP��ַ
	QString			m_IP;

	//��������������������Ķ�ʱ��
	QTimer*			m_pTimer;

	//�ͻ������ӷ�������־
	bool			m_netflag;
	
	//��������
	QByteArray	m_qbarecv;
	//���������ܳ���
	qint32		m_recvLen;
	//��һ�ν��ձ�־
	bool		m_brecvflag;
};

#endif