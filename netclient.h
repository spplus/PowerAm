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
#include <QTcpSocket>
#include <QTimer>
#include <QTextCodec>

#define MSGHEAD "0xff"
#define MSGTAIL "0x88"

//定义数据包包头
const qint32 FRAM_HEAD = 0x11;

//定义数据包包未
const qint32 FRAM_TAIL = 0x88;

class NetClient :public QObject
{
	Q_OBJECT
public:

	// 获取对象
	static NetClient* instance();

	// 初始化网络
	bool	init();

	// 连接服务器
	bool	connectToServer(const QString srvName,quint16 port);

	// 向服务器发送数据
	int		sendData(int msgtype,const char* msg,int msglength);

	// 关闭连接
	void	close();

private:
	NetClient();

signals:
	
	// 连接建立
	void	connected();

	// 连接断开
	void	disconnected();

	// 接收数据完成，向应用层传递
	void	recvdata(int msgtype,const char* msg,int msglength);

public slots:

	// 连接建立
	void		connectOpen();

	// 连接断开
	void		connectLost();

	//接收服务器数据
	void		readMessage();

	//监测与服务器的连接
	void		checkConnect();

private:
	// 打包
	QByteArray	pack(const char* msg,const int msgtype,const int msglength);

	// 解包 
	QByteArray  unpack(QByteArray qByte,int &msgtype,int recvlen);


private:
	static NetClient*	m_inst;

	//Socket对象
	QTcpSocket*		m_pTcpScoket;

	//连接端口号
	QString			m_Port;

	//连接IP地址
	QString			m_IP;

	//检测与服务器端连接情况的定时器
	QTimer*			m_pTimer;

	//客户端连接服务器标志
	bool			m_netflag;
	
	//接收数据
	QByteArray	m_qbarecv;
	//接收数据总长度
	qint32		m_recvLen;
	//第一次接收标志
	bool		m_brecvflag;
};

#endif