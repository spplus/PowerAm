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
#include "comutil.h"
#include "svgrenderer.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "navview.h"
#include "navmodel.h"
#include "ftputil.h"
#include "openthread.h"
#include "waitwidget.h"
#include "ticketmgr.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   static MainWindow* instance();

    ~MainWindow();

public:
	void		show();
	void		addContextMenuAction(eDeviceType type);
	bool		getIsCheck(){return m_isCheck;};

public slots:
	
	void		goHome();
    void		openFile();
	void		setViewModel();
	void		openFile(QString fname,QString stationId = 0,bool needRoot=true);
	void		showDownSvg();
	void		onToolButton();
	void		openOk();
	// 接收数据
	void		recvdata(int msgtype,const char* msg,int msglength);

	// 设置控制开关
	void		setControlEnable();

	// 设置声音报警
	void		setAlarmEnable();

	// 设置校验
	void		setCheckEnable();
	
	//显示环路查询Dockwidget
	void		showCircleQueryDockwdg();
	//显示挂牌查询Dockwidget
	void		showSignQueryDockwdg();
	//显示接地查询Dockwidget
	void		showGswitchQueryDockwdg();
	//显示人工设置查询Dockwidget
	void		showMsetQueryDockwdg();
	//显示事件查询Dockwidget
	void		showEventQueryDockwdg();

	// 整站拓扑
	void		topoEntire();

	// 拟票态
	void		ticketShow();

private:

	 MainWindow(QWidget *parent = 0);
	void		initWidget();
	void		initToolBar();
	void		initActions();
	void		initConnections();
	void		initMenu();
	void		initStatusBar();
	void		initNavView();
	void		initOpenThread();
	void		initDocWidget();
	void		updateModel();
	void		cleanScene();

	// 显示操作结果
	void		showLineSetResult(const char* msg,int msglength);
	void		showPowerSetResult(const char* msg,int msglength);
	void		showTagOpResult(const char* msg,int msglength);
	void		showWriteSavingResult(const char* msg,int msglength);

	// 设置网络状态显示
	void		setNetWorkStatus(int type);

	// 消息提示框
	void		showMsg(QString msg);

	//创建环路查询Dockwidget
	void	createCircleQueryDockwdg();
	//显示环路查询结果
	void	showCircleQueryResult(const char* msg,int msglength);
	//创建挂牌查询Dockwidget
	void	createSignQueryDockwdg();
	//显示挂牌查询结果
	void	showSignQueryResult(const char* msg,int msglength);
	//创建接地查询Dockwidget
	void	createGswitchQueryDockwdg();
	//显示接地查询结果
	void	showGswitchQueryResult(const char* msg,int msglength);
	//创建人工设置查询Dockwidget
	void	createMsetQueryDockwdg();
	//显示人工设置查询结果
	void	showMsetQueryResult(const char* msg,int msglength);
	//创建事件查询Dockwidget
	void	createEventQueryDockwdg();
	//显示事件查询结果
	void	showEventQueryResult(const char* msg,int msglength);

private:

	// 是否允许控制
	bool						m_isControl;

	// 是否允许声音告警
	bool						m_isAlarm;

	// 是否允许校验
	bool						m_isCheck;

	QMenu*						m_sysMenu;
	QMenu*						m_editMenu;
	QMenu*						m_queryMenu;
	QMenu*						m_contextMenu;             //右键菜单
    QToolBar*					m_toolBar;
	QToolButton*					m_drawerBtn;				// 抽屉按钮
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
	//QAction*						m_roleAction;				// 设置规则
	QAction*						m_circleQueryAction;	// 环路查询
	QAction*						m_signQueryAction;	// 挂牌查询
	QAction*						m_gswitchQueryAction;		// 接地查询
	//QAction*						m_opQueryAction;				// 操作查询
	QAction*						m_eventQueryAction;			// 事件查询

	QAction*						m_zoutAction;				// 放大
	QAction*						m_zinAction;				// 缩小
	QAction*						m_originalAction;		// 原始大小
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


	//状态栏标签
	QLabel*						m_pConnLabel;

	// 当前打开站点的CIMID
	QString						m_curStationId;

	NavView*					m_navview;
	NavModel*				m_model;
	QString					m_title;
    QSplitter*				m_spliter;
	GraphicsScene*			m_sence;
    GraphicsView*			m_view;
	WaitWidget				m_waitDlg;
	OpenThread*				m_openThread;
	static		MainWindow*		m_inst;

	//操作票管理界面
	TicketMgr*				m_TcktMgr;


	//设置环路查询QDockWidget
	QDockWidget*		m_pCircleQueryDockwdg;
	//设置环路查询的DOC中的tableWidget
	QTableWidget*		m_pCircleQueryTabWdgt;

	//设置挂牌查询QDockWidget
	QDockWidget*		m_pSignQueryDockwdg;
	//设置挂牌查询的DOC中的tableWidget
	QTableWidget*		m_pSignQueryTabWdgt;

	//设置接地查询QDockWidget
	QDockWidget*		m_pGswitchQueryDockwdg;
	//设置接地查询的DOC中的tableWidget
	QTableWidget*		m_pGswitchQueryTabWdgt;

	//人工设置查询QDockWidget
	QDockWidget*		m_pMsetQueryDockwdg;
	//人工设置查询的DOC中的tableWidget
	QTableWidget*		m_pMsetQueryTabWdgt;

	//事件查询QDockWidget
	QDockWidget*		m_pEventQueryDockwdg;
	//事件查询的DOC中的tableWidget
	QTableWidget*		m_pEventQueryTabWdgt;

};

#endif // MAINWINDOW_H
