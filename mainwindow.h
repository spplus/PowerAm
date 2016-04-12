#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <QMenu>
#include <QMenuBar>

#include "svgrenderer.h"
#include "graphicsscene.h"
#include "graphicsview.h"

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
	
	// 连接服务器成功
	void		connected();
	void		disconnected();

	// 接收数据
	void		recvdata(int msgtype,const char* msg,int msglength);

private:
	void		initWidget();
	void		initToolBar();
	void		initActions();
	void		initMenu();
	void		initStatusBar();

private:
	QMenu*						m_sysMenu;
	QMenu*						m_editMenu;
	QMenu*						m_queryMenu;
	QMenu*						m_contextMenu;             //右键菜单
    QToolBar*					m_toolBar;
   
	QAction *					m_openAction;			// 打开图形文件
	QAction*						m_zoutAction;				// 放大
	QAction*						m_zinAction;				// 缩小
	QAction*						m_nextAction;				// 前进
	QAction*						m_prevAction;				// 后退
	QAction*						m_userMgrAction;		// 用户管理
	QAction*						m_stationTypeAction;	// 电站分类设置
	QAction*						m_stationRelaAction;	// 电站类别、图形绑定
	QAction*						m_modelAction;			// 状态，实时态，模拟态
	QAction*						m_refreshAction;			// 刷新数据

	QAction*						m_circleQueryAction;	// 环路查询
	QAction*						m_signQueryAction;	// 挂牌查询
	QAction*						m_gswitchQueryAction;		// 地刀查询
	QAction*						m_msetQueryAction;			// 人工设置查询
	QAction*						m_cutQueryAction;				// 电路截面查询

	QAction*						m_onAction;						// 置合操作
	QAction*						m_offAction;						// 置分操作
	QAction*						m_signOnAction;					// 挂牌
	QAction*						m_signOffAction;				// 摘牌
	QAction*						m_viewModelAction;				// 设置视图选择类型

	QString					m_title;
    GraphicsScene*		m_sence;
    GraphicsView*			m_view;

};

#endif // MAINWINDOW_H
