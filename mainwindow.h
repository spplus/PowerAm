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
#include <qtimer.h>
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
#include "radiowidget.h"
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
	void		openFile(QString fname,QString stationId = "0",bool needRoot=true);
	void		showDownSvg();
	void		onToolButton();
	void		openOk();
	// 鎺ユ敹鏁版嵁
	void		recvdata(int msgtype,const char* msg,int msglength);

	// 璁剧疆鎺у埗寮€鍏?
	void		setControlEnable();

	// 璁剧疆澹伴煶鎶ヨ
	void		setAlarmEnable();

	// 璁剧疆鏍￠獙
	void		setCheckEnable();
	
	//鏄剧ず鐜矾鏌ヨDockwidget
	void		showCircleQueryDockwdg();
	//鏄剧ず鎸傜墝鏌ヨDockwidget
	void		showSignQueryDockwdg();
	//鏄剧ず鎺ュ湴鏌ヨDockwidget
	void		showGswitchQueryDockwdg();
	//鏄剧ず浜哄伐璁剧疆鏌ヨDockwidget
	void		showMsetQueryDockwdg();
	//鏄剧ず浜嬩欢鏌ヨDockwidget
	void		showEventQueryDockwdg();

	// 鏁寸珯鎷撴墤
	void		topoEntire();

	// 鎷熺エ鎬?
	void		ticketShow();

	// 璁剧疆鎷熺エ鎬?
	void			setRealState();
	void			setAnalogState();
	void			setTicketState();

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

	// 鏄剧ず鎿嶄綔缁撴灉
	void		showLineSetResult(const char* msg,int msglength);
	void		showPowerSetResult(const char* msg,int msglength);
	void		showTagOpResult(const char* msg,int msglength);
	void		showWriteSavingResult(const char* msg,int msglength);

	// 璁剧疆缃戠粶鐘舵€佹樉绀?
	void		setNetWorkStatus(int type);

	// 娑堟伅鎻愮ず妗?
	void		showMsg(QString msg);

	//鍒涘缓鐜矾鏌ヨDockwidget
	void	createCircleQueryDockwdg();
	//鏄剧ず鐜矾鏌ヨ缁撴灉
	void	showCircleQueryResult(const char* msg,int msglength);
	//鍒涘缓鎸傜墝鏌ヨDockwidget
	void	createSignQueryDockwdg();
	//鏄剧ず鎸傜墝鏌ヨ缁撴灉
	void	showSignQueryResult(const char* msg,int msglength);
	//鍒涘缓鎺ュ湴鏌ヨDockwidget
	void	createGswitchQueryDockwdg();
	//鏄剧ず鎺ュ湴鏌ヨ缁撴灉
	void	showGswitchQueryResult(const char* msg,int msglength);
	//鍒涘缓浜哄伐璁剧疆鏌ヨDockwidget
	void	createMsetQueryDockwdg();
	//鏄剧ず浜哄伐璁剧疆鏌ヨ缁撴灉
	void	showMsetQueryResult(const char* msg,int msglength);
	//鍒涘缓浜嬩欢鏌ヨDockwidget
	void	createEventQueryDockwdg();
	//鏄剧ず浜嬩欢鏌ヨ缁撴灉
	void	showEventQueryResult(const char* msg,int msglength);

private:

	// 鏄惁鍏佽鎺у埗
	bool						m_isControl;

	// 鏄惁鍏佽澹伴煶鍛婅
	bool						m_isAlarm;

	// 鏄惁鍏佽鏍￠獙
	bool						m_isCheck;

	QMenu*						m_sysMenu;
	QMenu*						m_editMenu;
	QMenu*						m_queryMenu;
	QMenu*						m_contextMenu;             //鍙抽敭鑿滃崟
    QToolBar*					m_toolBar;
	QToolButton*					m_drawerBtn;				// 鎶藉眽鎸夐挳
	QAction*						m_homeAction;			// 杩斿洖涓婚〉
	QAction *					m_openAction;			// 鎵撳紑鍥惧舰鏂囦欢
	QAction*						m_nextAction;				// 鍓嶈繘
	QAction*						m_prevAction;				// 鍚庨€€
	QAction*						m_refreshAction;			// 鍒锋柊鏁版嵁
	QAction*						m_saveAction;				// 淇濆瓨
	QAction*						m_readAction;				// 璇诲彇
	QAction*						m_controlAction;			// 鎺у埗锛堢姝㈡帶鍒讹紝鍏佽鎺у埗锛?
	QAction*						m_chekAction;				// 鏍￠獙 锛堢姝㈡牎楠岋紝杩愯鏍￠獙锛?
	QAction*						m_netAction;				// 缃戠粶鐘舵€?
	QAction*						m_soundAction;			// 澹伴煶鍛婅
	//QAction*						m_roleAction;				// 璁剧疆瑙勫垯
	QAction*						m_circleQueryAction;	// 鐜矾鏌ヨ
	QAction*						m_signQueryAction;	// 鎸傜墝鏌ヨ
	QAction*						m_gswitchQueryAction;		// 鎺ュ湴鏌ヨ
	//QAction*						m_opQueryAction;				// 鎿嶄綔鏌ヨ
	QAction*						m_eventQueryAction;			// 浜嬩欢鏌ヨ

	QAction*						m_zoutAction;				// 鏀惧ぇ
	QAction*						m_zinAction;				// 缂╁皬
	QAction*						m_originalAction;		// 鍘熷澶у皬
	QAction*						m_modelAction;			// 鐘舵€侊紝瀹炴椂鎬侊紝妯℃嫙鎬?
	
	QAction*						m_msetQueryAction;			// 浜哄伐璁剧疆鏌ヨ
	QAction*						m_cutQueryAction;				// 鐢佃矾鎴潰鏌ヨ

	QAction*						m_roleQueryAction;			// 鏍￠獙瑙勫垯
	QAction*						m_topoQueryAction;			// 鎷撴墤淇℃伅鏌ヨ
	QAction*						m_intervalQueryAction;		// 闂撮殧鏌ヨ
	QAction*						m_scadaLogQueryAction;		// scada鏃ュ織鏌ヨ

	QAction*						m_onAction;						// 缃悎鎿嶄綔
	QAction*						m_offAction;					// 缃垎鎿嶄綔
	QAction*						m_signOnAction;					// 鎸傜墝
	QAction*						m_signOffAction;				// 鎽樼墝
	QAction*						m_powerSetAction;				// 璁剧疆鐢垫簮鐐?
	QAction*						m_inLineSetAction;				// 璁剧疆杩涘嚭绾?
	QAction*						m_viewModelAction;				// 璁剧疆瑙嗗浘閫夋嫨绫诲瀷


	//鐘舵€佹爮鏍囩
	QLabel*						m_pConnLabel;

	// 褰撳墠鎵撳紑绔欑偣鐨凜IMID
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

	//鎿嶄綔绁ㄧ鐞嗙晫闈?
	TicketMgr*				m_TcktMgr;

	// 宸ュ叿鏍忔寜閽獥鍙?
	RadioWidget*			m_radioWidget;

	//璁剧疆鐜矾鏌ヨQDockWidget
	QDockWidget*		m_pCircleQueryDockwdg;
	//璁剧疆鐜矾鏌ヨ鐨凞OC涓殑tableWidget
	QTableWidget*		m_pCircleQueryTabWdgt;

	//璁剧疆鎸傜墝鏌ヨQDockWidget
	QDockWidget*		m_pSignQueryDockwdg;
	//璁剧疆鎸傜墝鏌ヨ鐨凞OC涓殑tableWidget
	QTableWidget*		m_pSignQueryTabWdgt;

	//璁剧疆鎺ュ湴鏌ヨQDockWidget
	QDockWidget*		m_pGswitchQueryDockwdg;
	//璁剧疆鎺ュ湴鏌ヨ鐨凞OC涓殑tableWidget
	QTableWidget*		m_pGswitchQueryTabWdgt;

	//浜哄伐璁剧疆鏌ヨQDockWidget
	QDockWidget*		m_pMsetQueryDockwdg;
	//浜哄伐璁剧疆鏌ヨ鐨凞OC涓殑tableWidget
	QTableWidget*		m_pMsetQueryTabWdgt;

	//浜嬩欢鏌ヨQDockWidget
	QDockWidget*		m_pEventQueryDockwdg;
	//浜嬩欢鏌ヨ鐨凞OC涓殑tableWidget
	QTableWidget*		m_pEventQueryTabWdgt;

};

#endif // MAINWINDOW_H
