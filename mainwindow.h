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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   static MainWindow* instance();

    ~MainWindow();

public:
	void		show();
	void		addContextMenuAction(eDeviceType type);

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
	void		updateModel();
	void		cleanScene();

	// 显示操作结果
	void		showLineSetResult(const char* msg,int msglength);
	void		showPowerSetResult(const char* msg,int msglength);
	void		showTagOpResult(const char* msg,int msglength);

	// 设置网络状态显示
	void		setNetWorkStatus(int type);

	//关闭响应事件
	void		closeEvent(QCloseEvent *event);

	// 消息提示框
	void		showMsg(QString msg);
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
	QToolButton*				m_drawerBtn;				// 抽屉按钮
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

	FtpUtil					m_ftpUtil;
	NavView*				m_navview;
	NavModel*				m_model;
	QString					m_title;
    QSplitter*				m_spliter;
	GraphicsScene*			m_sence;
    GraphicsView*			m_view;
	WaitWidget				m_waitDlg;
	OpenThread*				m_openThread;
	static		MainWindow*		m_inst;


};

#endif // MAINWINDOW_H
