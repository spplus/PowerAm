#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <Qtimer>
#include <QtGui>
#include "netclient.h"

#include "svgrenderer.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "navview.h"
#include "ftputil.h"

//定义配置文件路径的常量D:\work\qt\PowerAm_1604241106\conf
static QString PAMCFGINI = "D:\\work\\qt\\PowerAm_1604241106\\conf\\PAMConfig.ini";


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	static MainWindow * m_self;

public:
	void		addContextMenuAction(eDeviceType type);

public slots:
    void		openFile();
	void		setViewModel();
	void		openFile(QString fname);
	void		showDownSvg();

	// 连接服务器成功
	void		connected();
	void		disconnected();

	//监测与服务器的连接
	void		checkConnect();

	// 接收数据
	void		recvdata(int msgtype,const char* msg,int msglength);

private:
	void		initWidget();
	void		initToolBar();
	void		initActions();
	void		initMenu();
	void		initStatusBar();
	void		initNavView();

	//初始化网络连接
	void	initNet();
	//读取连接服务器IP和端口
	bool	readIpPort();
	//关闭响应事件
	void closeEvent(QCloseEvent *event);

private:
	QMenu*						m_sysMenu;
	QMenu*						m_editMenu;
	QMenu*						m_queryMenu;
	QMenu*						m_contextMenu;             //右键菜单
    QToolBar*					m_toolBar;
   
	QAction*						m_homeAction;			// 返回主页
	QAction *					m_openAction;			// 打开图形文件
	QAction*						m_nextAction;				// 前进
	QAction*						m_prevAction;				// 后退
	QAction*						m_refreshAction;			// 刷新数据
	QAction*						m_saveAction;				// 保存
	QAction*						m_readAction;				// 读取
	QAction*						m_controlAction;			// 控制（禁止控制，允许控制）
	QAction*						m_chekAction;				// 校验 （禁止校验，运行校验）
	QAction*						m_netAction;				// 网络状态
	QAction*						m_soundAction;			// 声音告警
	QAction*						m_roleAction;				// 设置规则
	QAction*						m_circleQueryAction;	// 环路查询
	QAction*						m_signQueryAction;	// 挂牌查询
	QAction*						m_gswitchQueryAction;		// 接地查询
	QAction*						m_opQueryAction;				// 操作查询
	QAction*						m_eventQueryAction;			// 事件查询

	QAction*						m_zoutAction;				// 放大
	QAction*						m_zinAction;				// 缩小
	QAction*						m_originalAction;		// 原始大小

	QAction*						m_userMgrAction;		// 用户管理
	QAction*						m_stationTypeAction;	// 电站分类设置
	QAction*						m_stationRelaAction;	// 电站类别、图形绑定
	QAction*						m_modelAction;			// 状态，实时态，模拟态
	
	QAction*						m_msetQueryAction;			// 人工设置查询
	QAction*						m_cutQueryAction;				// 电路截面查询

	QAction*						m_roleQueryAction;			// 校验规则
	QAction*						m_topoQueryAction;			// 拓扑信息查询
	QAction*						m_intervalQueryAction;		// 间隔查询
	QAction*						m_scadaLogQueryAction;		// scada日志查询

	QAction*						m_onAction;						// 置合操作
	QAction*						m_offAction;					// 置分操作
	QAction*						m_signOnAction;					// 挂牌
	QAction*						m_signOffAction;				// 摘牌
	QAction*						m_powerSetAction;				// 设置电源点
	QAction*						m_inLineSetAction;				// 设置进出线
	QAction*						m_viewModelAction;				// 设置视图选择类型
	QAction*						m_downSvg;						// 下载svg文件

	//状态栏标签
	QLabel*				m_pConnLabel;

	//连接端口号
	QString m_Port;
	//连接IP地址
	QString m_IP;

	//检测与服务器端连接情况的定时器
	QTimer*		pTimer;

	//客户端连接服务器标志
	bool bNetflag;

	//客户端连接对象
	NetClient*		m_pNetClient;

	FtpUtil					m_ftpUtil;
	NavView*				m_navview;
	QString					m_title;
    QSplitter*				m_spliter;
	GraphicsScene*			m_sence;
    GraphicsView*			m_view;


};

#endif // MAINWINDOW_H
