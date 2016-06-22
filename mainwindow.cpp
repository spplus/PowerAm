#include "mainwindow.h"
#include "svgparser.h"
#include <string>
#include <QPushButton>
#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <qfile.h>
#include <QMessageBox>
#include <QDomDocument>
#include <QCoreApplication>
#include <QTextCodec>
#include "homewindow.h"
#include "navmodel.h"
#include "navdelegate.h"
#include "textitem.h"
#include "comutil.h"
#include "buff/msgbody.pb.h"
#include "include/commands.h"
#include "openwidget.h"

using namespace std;
MainWindow * MainWindow::m_inst = NULL;

MainWindow* MainWindow::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new MainWindow;
	}
	return m_inst;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
	m_isCheck = true;
	m_isControl = true;
	m_isAlarm = true;

	m_curStationId = "1";
	m_title = ComUtil::instance()->getSysName();
	initWidget();
	initOpenThread();
	initToolBar();
	initMenu();
	initStatusBar();
	initConnections();
	setWindowTitle(m_title); 

	connect(&m_ftpUtil,SIGNAL(downloaded(QString ,QString,bool)),this,SLOT(openFile(QString,QString,bool)));
	hide();
}

void MainWindow::initWidget()
{
	initNavView();
	m_spliter = new QSplitter(this);

	QWidget* widget = new QWidget(m_spliter);
	QHBoxLayout* hbox= new QHBoxLayout(widget);
	hbox->setMargin(0);
	m_drawerBtn= new QToolButton(widget);
	m_drawerBtn->setMaximumSize(QSize(10, 50));
	m_drawerBtn->setAutoRaise(true);
	m_drawerBtn->setObjectName("drawer");
	m_drawerBtn->setText(">");
	hbox->addWidget(m_drawerBtn);

	m_contextMenu = new QMenu(this);
	m_sence = new GraphicsScene(widget,m_contextMenu);
	m_sence->setBackgroundBrush(QBrush(Qt::white));
	m_view = new GraphicsView(m_sence);
	m_view->setFrameShape(QGraphicsView::NoFrame);
	hbox->addWidget(m_view);

	m_navview->setParent(m_spliter);
	m_spliter->addWidget(m_navview);
	m_spliter->addWidget(widget);
	
	m_navview->hide();
	connect(m_drawerBtn,SIGNAL(pressed()),this,SLOT(onToolButton()));
	m_spliter->setHandleWidth(1);
	
	QList<int> sizes;
	sizes.append(200);
	sizes.append(20);
	m_spliter->setSizes(sizes);
	m_spliter->setStretchFactor(1,1);

	m_waitDlg.setParent(this);
	m_waitDlg.hide();

	setCentralWidget(m_spliter);
	this->showMaximized();
}

void MainWindow::show()
{
	m_model->setData(ComUtil::instance()->getStationList());

	QMainWindow::show();
	this->activateWindow();
}

void MainWindow::openOk()
{
	m_waitDlg.hide();

	// 打开完成后需要做的事情
	
	// 加载设备状态数据
	m_sence->reqUnitState(m_curStationId);

	//PBNS::DevStateMsg_Request req;

	//// 注意！！！！！！站点ID和saveId，需要根据打开的图形传过来！！！
	//req.set_saveid(1);
	//req.set_stationcim(m_curStationId.toStdString());
	//string reqstr;
	//req.SerializeToString(&reqstr);
	//NetClient::instance()->sendData(CMD_DEV_STATE,reqstr.c_str(),reqstr.length());

}

void MainWindow::onToolButton()
{
	if (m_navview->isHidden())
	{
		m_navview->show();
		m_drawerBtn->setText("<");
	}
	else
	{
		m_navview->hide();
		m_drawerBtn->setText(">");
	}
}

void MainWindow::initNavView()
{
	m_navview = new NavView(this);
	m_model = new NavModel(this);
	//model->ReadDataFromConfig(QCoreApplication::applicationDirPath() + "/config.xml");
	m_model->setData(ComUtil::instance()->getStationList());
	NavDelegate* delegate = new NavDelegate(this);
	m_navview->setModel(m_model);
	m_navview->setItemDelegate(delegate);
	connect(m_navview, SIGNAL(doubleClicked(const QModelIndex &)), m_model, SLOT(Collapse(const QModelIndex&)));
	connect(m_model,SIGNAL(openFile(QString,QString,bool)),this,SLOT(openFile(QString,QString,bool)));

}

void MainWindow::initOpenThread()
{
	m_openThread = new OpenThread(m_sence);

	connect(m_openThread,SIGNAL(drawSvgGraph(SvgGraph*)),m_sence,SLOT(drawSvgGraph(SvgGraph*)));
	connect(m_openThread,SIGNAL(finished()),this,SLOT(openOk()));

}

void MainWindow::initToolBar()
{
	initActions();
	m_toolBar =  this->addToolBar(tr("toolbar"));
	m_toolBar->addAction(m_homeAction);
	m_toolBar->addAction(m_viewModelAction);
	m_toolBar->addAction(m_openAction);
	m_toolBar->addAction(m_prevAction);
	m_toolBar->addAction(m_nextAction);
	m_toolBar->addAction(m_originalAction);
	m_toolBar->addAction(m_zoutAction);
	m_toolBar->addAction(m_zinAction);
	m_toolBar->addAction(m_refreshAction);
	m_toolBar->addAction(m_saveAction);
	m_toolBar->addAction(m_readAction);
	m_toolBar->addAction(m_controlAction);
	m_toolBar->addAction(m_chekAction);
	m_toolBar->addAction(m_netAction);
	m_toolBar->addAction(m_soundAction);
	m_toolBar->addAction(m_roleAction);
	m_toolBar->addAction(m_circleQueryAction);
	m_toolBar->addAction(m_signQueryAction);
	m_toolBar->addAction(m_gswitchQueryAction);
	m_toolBar->addAction(m_opQueryAction);
	m_toolBar->addAction(m_eventQueryAction);

	m_toolBar->addAction(m_modelAction);

	m_toolBar->setIconSize(QSize(24,24));
}

void MainWindow::initMenu()
{
	m_sysMenu = this->menuBar()->addMenu(tr("系统&"));
	//m_sysMenu->addAction(m_downSvg);

	//m_editMenu = this->menuBar()->addMenu(tr("设置&"));
	//m_editMenu->addAction(m_userMgrAction);
	//m_editMenu->addAction(m_stationTypeAction);
	//m_editMenu->addAction(m_stationRelaAction);
	
	m_queryMenu = this->menuBar()->addMenu(tr("查询&"));
	m_queryMenu->addAction(m_circleQueryAction);
	m_queryMenu->addAction(m_cutQueryAction);
	m_queryMenu->addAction(m_gswitchQueryAction);
	m_queryMenu->addAction(m_msetQueryAction);
	m_queryMenu->addAction(m_signQueryAction);
	m_queryMenu->addAction(m_roleQueryAction);
	m_queryMenu->addAction(m_topoQueryAction);
	m_queryMenu->addAction(m_scadaLogQueryAction);
	m_queryMenu->addAction(m_eventQueryAction);
	m_queryMenu->addAction(m_intervalQueryAction);


}

void MainWindow::addContextMenuAction(eDeviceType type)
{
	m_contextMenu->clear();
	switch (type)
	{
	
	case eSWITCH:
	case eBREAKER:
	case eGROUNDSWITCH:
		if (m_isControl)
		{
			m_contextMenu->addAction(m_onAction);
			m_contextMenu->addAction(m_offAction);
		}
		
		m_contextMenu->addAction(m_signOnAction);
		m_contextMenu->addAction(m_signOffAction);
		m_contextMenu->addAction(m_inLineSetAction);
		break;
	case eLINE:
		m_contextMenu->addAction(m_powerSetAction);
		m_contextMenu->addAction(m_inLineSetAction);
		break;
	default:
		break;
	}
}

void MainWindow::initActions()
{
	m_homeAction = new QAction(QIcon(ICON_HOME),tr(MSG_TIP_HOME),this);
	m_openAction = new QAction(QIcon(ICON_OPEN),tr(MSG_TIP_OPEN),this);
	m_zoutAction	 = new QAction(QIcon(ICON_ZOOM_OUT),tr(MSG_TIP_ZOOMOUT),this);
	m_zinAction = new QAction(QIcon(ICON_ZOOM_IN),tr(MSG_TIP_ZOOMIN),this);
	m_nextAction = new QAction(QIcon(ICON_NEXT),tr(MSG_TIP_NEXT),this);
	m_prevAction = new QAction(QIcon(ICON_PREV),tr(MSG_TIP_PREV),this);
	m_refreshAction = new QAction(QIcon(ICON_REFRESH),tr(MSG_TIP_REFRESH),this);
	m_saveAction = new QAction(QIcon(ICON_SAVE),tr(MSG_TIP_SAVE),this);
	m_readAction = new QAction(QIcon(ICON_READ),tr(MSG_TIP_READ),this);
	m_controlAction = new QAction(QIcon(ICON_CONTROL_ON),tr(MSG_TIP_CONTROL_ON),this);
	m_chekAction = new QAction(QIcon(ICON_CHECK_ON),tr(MSG_TIP_CHECK_ON),this);
	m_netAction = new QAction(QIcon(ICON_NETWORK_ON),tr(MSG_TIP_NETWORK_ON),this);
	m_soundAction = new QAction(QIcon(ICON_ALARM_ON),tr(MSG_TIP_ALARM_ON),this);
	m_roleAction = new QAction(QIcon(ICON_ROLE),tr(MSG_TIP_RULE),this);
	m_circleQueryAction = new QAction(QIcon(ICON_CIRLCE),tr(MSG_TIP_CIRCLE),this);
	m_signQueryAction = new QAction(QIcon(ICON_TAG_ON),tr(MSG_TIP_TAG),this);
	m_gswitchQueryAction = new QAction(QIcon(ICON_GROUND),tr(MSG_TIP_GROUND),this);
	m_opQueryAction = new QAction(QIcon(ICON_OPRATION),tr(MSG_TIP_OPRATION),this);
	m_eventQueryAction	= new QAction(QIcon(ICON_EVENT),tr(MSG_TIP_EVENT),this);
	m_modelAction = new QAction(QIcon(ICON_RUN),tr(MSG_TIP_RUN_ANALOG),this);

	m_msetQueryAction = new QAction(QIcon(ICON_SETQUERY),tr(MSG_TIP_SETTING),this);
	m_cutQueryAction = new QAction(QIcon(ICON_CUTTING),tr(MSG_TIP_SAVING),this);

	m_onAction = new QAction(QIcon(ICON_SWITCH_ON),tr(MSG_TIP_SWITCH_ON),this);
	m_offAction = new QAction(QIcon(ICON_SWITCH_OFF),tr(MSG_TIP_SWITCH_OFF),this);
	m_signOnAction = new QAction(QIcon(ICON_TAG_ON),tr(MSG_TIP_TAG_ON),this);
	m_signOffAction = new QAction(QIcon(ICON_TAG_OFF),tr(MSG_TIP_TAG_OFF),this);
	m_viewModelAction = new QAction(QIcon(ICON_POINTER),tr(MSG_TIP_POINTER),this);
	m_originalAction = new QAction(QIcon(ICON_ZOOM_ORG),tr(MSG_TIP_ZOOM_ORG),this);

	m_roleQueryAction = new QAction(QIcon(ICON_ROLE),tr(MSG_TIP_RUN_ANALOG),this);
	m_topoQueryAction = new QAction(QIcon(ICON_TOPO),tr(MSG_TIP_TOPO),this);
	m_intervalQueryAction = new QAction(QIcon(ICON_BAY),tr(MSG_TIP_BAY),this);
	m_scadaLogQueryAction = new QAction(QIcon(ICON_LOG),tr(MSG_TIP_LOG),this);
	m_powerSetAction = new QAction(QIcon(ICON_POWERSET),tr(MSG_TIP_POWERSET),this);
	m_inLineSetAction = new QAction(QIcon(ICON_LINESET),tr(MSG_TIP_LINESET),this);

	// 1 指针 2 手掌
	m_viewModelAction->setData(QVariant(QGraphicsView::ScrollHandDrag));

}

void MainWindow::initConnections()
{
	connect(m_homeAction,SIGNAL(triggered()),this,SLOT(goHome()));
	connect(m_openAction,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(m_zinAction,SIGNAL(triggered()),m_view,SLOT(zoomIn()));
	connect(m_zoutAction,SIGNAL(triggered()),m_view,SLOT(zoomOut()));
	connect(m_prevAction,SIGNAL(triggered()),m_sence,SLOT(goPrev()));
	connect(m_nextAction,SIGNAL(triggered()),m_sence,SLOT(goNext()));
	connect(m_originalAction,SIGNAL(triggered()),m_view,SLOT(zoomHome()));
	connect(m_viewModelAction,SIGNAL(triggered()),this,SLOT(setViewModel()));
	connect(m_offAction,SIGNAL(triggered()),m_sence,SLOT(setOpen()));
	connect(m_onAction,SIGNAL(triggered()),m_sence,SLOT(setClose()));
	connect(m_refreshAction,SIGNAL(triggered()),m_sence,SLOT(startAnimation()));
	connect(m_controlAction,SIGNAL(triggered()),this,SLOT(setControlEnable()));
	connect(m_soundAction,SIGNAL(triggered()),this,SLOT(setAlarmEnable()));
	connect(m_chekAction,SIGNAL(triggered()),this,SLOT(setCheckEnable()));
	connect(m_powerSetAction,SIGNAL(triggered()),m_sence,SLOT(setPower()));
	connect(m_inLineSetAction,SIGNAL(triggered()),m_sence,SLOT(setLine()));
	connect(m_signOnAction,SIGNAL(triggered()),m_sence,SLOT(tagOn()));
	connect(m_signOffAction,SIGNAL(triggered()),m_sence,SLOT(tagOff()));
	connect(m_readAction,SIGNAL(triggered()),m_sence,SLOT(readSaving()));
	connect(m_saveAction,SIGNAL(triggered()),m_sence,SLOT(writeSaving()));
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::goHome()
{
	this->hide();
	cleanScene();
	HomeWindow::instance()->show();
}

void MainWindow::cleanScene()
{
	m_sence->clear();
	m_sence->setBackgroundBrush(QBrush(Qt::white));
}
void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"),"/", tr("*.svg"));
	openFile(fileName,m_curStationId,false);
}

void MainWindow::openFile(QString fileName,QString stationId /* = 0 */,bool needRoot/* =true */)
{
	QString tempName = fileName;
	if (fileName.length()== 0)
	{
		QMessageBox::warning(this,MSG_TITLE,"文件名称为空");
		return;
	}
	// 保存当前站点ID
	m_curStationId = stationId;

	// 标志是否打开默认目录下的文件
	if (needRoot)
	{
		QString svgRoot = ComUtil::instance()->getSvgRoot();
		if (svgRoot.length() == 0)
		{
			QMessageBox::warning(this,MSG_TITLE,"Svg路径为空");
			return;
		}

		fileName = svgRoot+"/"+fileName;
	}

	// 判断该文件是否存在本地目录，如果不存在，则通过FTP下载该文件
	QFile file(fileName);
	if (!file.exists())
	{
		file.close();
		m_ftpUtil.show();
		m_ftpUtil.getFile(tempName);
		return;
	}
	file.close();

	// 清空场景
	cleanScene();

	// 显示等待窗口
	m_waitDlg.show();

	// 启动解析图形文件线程
	m_openThread->open(fileName);

	//m_sence->openSvgFile(fileName);

	int idx = fileName.lastIndexOf("/");
	fileName = fileName.right(fileName.length()-idx-1);
	fileName = m_title+"-"+fileName;
	this->setWindowTitle(fileName);

}

void MainWindow::setViewModel()
{
	QGraphicsView::DragMode md;
	QIcon icon;
	if (m_viewModelAction->data().toInt() == QGraphicsView::NoDrag)
	{
		md = QGraphicsView::ScrollHandDrag;
		m_viewModelAction->setIcon(QIcon(ICON_HAND));
	}
	else
	{
		md = QGraphicsView::NoDrag;
		m_viewModelAction->setIcon(QIcon(ICON_POINTER));
	}
	m_viewModelAction->setData(md);
	m_view->setDragMode(md);
}

void MainWindow::showDownSvg()
{
	m_ftpUtil.exec();
}

void MainWindow::initStatusBar()
{
	m_pConnLabel = new QLabel(this);
	this->statusBar()->addPermanentWidget(m_pConnLabel);
	//无边框设置
	statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

}

void MainWindow::recvdata(int msgtype,const char* msg,int msglength)
{

	switch (msgtype)
	{
	case CMD_DEV_STATE:
		m_sence->showDevState(msg,msglength);
		break;
	case CMD_LINE_SET:
		showLineSetResult(msg,msglength);
		break;
	case CMD_POWER_SET:
		showPowerSetResult(msg,msglength);
		break;
	case CMD_TAG_OP:
		showTagOpResult(msg,msglength);
		break;
	case CMD_CONNECTED:
	case CMD_DISCONNECTED:
		setNetWorkStatus(msgtype);
		break;
	case CMD_READ_SAVING:
		m_sence->showSavingList(msg,msglength);
		break;
	case CMD_WRITE_SAVING:
		showWriteSavingResult(msg,msglength);
		break;
	default:
		break;
	}

}

void MainWindow::showMsg(QString msg)
{
	QMessageBox box;
	box.setMinimumWidth(300);
	box.information(this,MSG_TITLE,msg);
}

void MainWindow::showWriteSavingResult(const char* msg,int msglength)
{
	PBNS::WriteSavingMsg_Response res;
	res.ParseFromArray(msg,msglength);
	if (res.rescode() == eSUCCESS)
	{
		showMsg("保存成功");
	}
	else
	{
		showMsg("保存失败");
	}
}
void MainWindow::showPowerSetResult(const char* msg,int msglength)
{
	PBNS::PowerSetMsg_Response res;
	res.ParseFromArray(msg,msglength);
	if (res.rescode() == eSUCCESS)
	{
		showMsg("操作成功");
	}
	else
	{
		showMsg("操作失败");
	}
}

void MainWindow::showTagOpResult(const char* msg,int msglength)
{
	PBNS::TagMsg_Response res;
	res.ParseFromArray(msg,msglength);
	if (res.rescode() == eSUCCESS)
	{
		showMsg("操作成功");
	}
	else
	{
		showMsg("操作失败");
	}
}

void MainWindow::showLineSetResult(const char* msg,int msglength)
{
	PBNS::LineSetMsg_Response res;
	res.ParseFromArray(msg,msglength);
	if (res.rescode() == eSUCCESS)
	{
		showMsg("操作成功");
	}
	else
	{
		showMsg("操作失败");
	}
}

//关闭响应事件
void MainWindow::closeEvent(QCloseEvent *event)
{
	//pStaTypeDockWidget->close();
	return;
}

void MainWindow::setNetWorkStatus(int type)
{
	if (type == CMD_CONNECTED)
	{
		m_netAction->setIcon(QIcon(ICON_NETWORK_ON));
		m_netAction->setToolTip(MSG_TIP_NETWORK_ON);
	}
	else
	{
		m_netAction->setIcon(QIcon(ICON_NETWORK_OFF));
		m_netAction->setToolTip(MSG_TIP_NETWORK_OFF);
	}
}

void MainWindow::setControlEnable()
{
	QString icon,msg;
	if (m_isControl)
	{
		icon = ICON_CONTROL_OFF;
		msg = MSG_TIP_CONTROL_OFF;
	}
	else
	{
		icon = ICON_CONTROL_ON;
		msg = MSG_TIP_CONTROL_ON;
	}
	m_isControl = !m_isControl;

	m_controlAction->setIcon(QIcon(icon));
	m_controlAction->setToolTip(msg);

}

void MainWindow::setAlarmEnable()
{
	QString icon,msg;
	if (m_isAlarm)
	{
		icon = ICON_ALARM_OFF;
		msg = MSG_TIP_ALARM_OFF;
	}
	else
	{
		icon = ICON_ALARM_ON;
		msg = MSG_TIP_ALARM_ON;
	}
	m_isAlarm = !m_isAlarm;

	m_soundAction->setIcon(QIcon(icon));
	m_soundAction->setToolTip(msg);
}

void MainWindow::setCheckEnable()
{
	QString icon,msg;
	if (m_isCheck)
	{
		icon = ICON_CHECK_OFF;
		msg = MSG_TIP_CHECK_OFF;
	}
	else
	{
		icon = ICON_CHECK_ON;
		msg = MSG_TIP_CHECK_ON;
	}
	m_isCheck = !m_isCheck;

	m_chekAction->setIcon(QIcon(icon));
	m_chekAction->setToolTip(msg);
}
