/************************************************************************/
/* 
	DESC:			TCP客户端数据转发层，实现业务层数据和TCP网络层之间的数据中转
						数据的打包，解包.
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

	// 连接服务器
	void	connectToServer(const QString srvName,qint16 port);

	// 向服务器发送数据
	int		sendData(int msgtype,const char* msg,int msglength);

	// 关闭连接
	void	close();

private:
	NetClient();

public slots:

signals:
	
	// 连接建立
	void	connected();

	// 连接断开
	void	disconnected();

	// 接收数据完成，向应用层传递
	void	recvdata(int msgtype,const char* msg,int msglength);

private:
	// 打包
	char*	pack(const char* msg,const int msgtype,const int msglength);

	// 解包 
	char*	unpack(const char* msg,const int msglength,int &msgtyhpe);

private:
	static NetClient*	m_inst;

};

#endif