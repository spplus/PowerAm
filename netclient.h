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


class NetClient :public QObject
{
	Q_OBJECT
public:

	static NetClient* instance();

	// ���ӷ�����
	void	connectToServer(const QString srvName,qint16 port);

	// ���������������
	int		sendData(int msgtype,const char* msg,int msglength);

	// �ر�����
	void	close();

private:
	NetClient();

public slots:

signals:
	
	// ���ӽ���
	void	connected();

	// ���ӶϿ�
	void	disconnected();

	// ����������ɣ���Ӧ�ò㴫��
	void	recvdata(int msgtype,const char* msg,int msglength);

private:
	// ���
	char*	pack(const char* msg,const int msgtype,const int msglength);

	// ��� 
	char*	unpack(const char* msg,const int msglength,int &msgtyhpe);

private:
	static NetClient*	m_inst;

};

#endif