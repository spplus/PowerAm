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
#include "ticketmgr.h"

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
	initDocWidget();
	setWindowTitle(m_title); 

	connect(FtpUtil::instance(),SIGNAL(downloaded(QString ,QString,bool)),this,SLOT(openFile(QString,QString,bool)));
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

	m_radioWidget	= new RadioWidget;

	//this->showMaximized();
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

	// ����ɺ���Ҫ��������
	
	// �����豸״̬����
	m_sence->reqUnitState(m_curStationId);

	//PBNS::DevStateMsg_Request req;

	//// ע�⣡����������վ��ID��saveId����Ҫ���ݴ򿪵�ͼ�δ�����������
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
	//m_toolBar->addAction(m_roleAction);
	m_toolBar->addAction(m_circleQueryAction);
	m_toolBar->addAction(m_signQueryAction);
	m_toolBar->addAction(m_gswitchQueryAction);
	//m_toolBar->addAction(m_opQueryAction);
	m_toolBar->addAction(m_eventQueryAction);

	m_toolBar->addAction(m_modelAction);

	m_toolBar->setIconSize(QSize(24,24));
	m_toolBar->addSeparator();
	m_toolBar->addWidget(m_radioWidget);
	showMaximized();
}

void MainWindow::initMenu()
{
	m_sysMenu = this->menuBar()->addMenu(tr("ϵͳ&"));
	//m_sysMenu->addAction(m_downSvg);

	//m_editMenu = this->menuBar()->addMenu(tr("����&"));
	//m_editMenu->addAction(m_userMgrAction);
	//m_editMenu->addAction(m_stationTypeAction);
	//m_editMenu->addAction(m_stationRelaAction);
	
	m_queryMenu = this->menuBar()->addMenu(tr("��ѯ&"));
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
	//m_roleAction = new QAction(QIcon(ICON_ROLE),tr(MSG_TIP_RULE),this);
	m_circleQueryAction = new QAction(QIcon(ICON_CIRLCE),tr(MSG_TIP_CIRCLE),this);
	m_signQueryAction = new QAction(QIcon(ICON_TAG_QUERY),tr(MSG_TIP_TAG),this);
	m_gswitchQueryAction = new QAction(QIcon(ICON_GROUND),tr(MSG_TIP_GROUND),this);
	//m_opQueryAction = new QAction(QIcon(ICON_OPRATION),tr(MSG_TIP_OPRATION),this);
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

	// 1 ָ�� 2 ����
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
	connect(m_refreshAction,SIGNAL(triggered()),this,SLOT(topoEntire()));
	connect(m_controlAction,SIGNAL(triggered()),this,SLOT(setControlEnable()));
	connect(m_soundAction,SIGNAL(triggered()),this,SLOT(setAlarmEnable()));
	connect(m_chekAction,SIGNAL(triggered()),this,SLOT(setCheckEnable()));
	connect(m_powerSetAction,SIGNAL(triggered()),m_sence,SLOT(setPower()));
	connect(m_inLineSetAction,SIGNAL(triggered()),m_sence,SLOT(setLine()));
	connect(m_signOnAction,SIGNAL(triggered()),m_sence,SLOT(tagOn()));
	connect(m_signOffAction,SIGNAL(triggered()),m_sence,SLOT(tagOff()));
	connect(m_readAction,SIGNAL(triggered()),m_sence,SLOT(readSaving()));
	connect(m_saveAction,SIGNAL(triggered()),m_sence,SLOT(writeSaving()));
	connect(m_modelAction,SIGNAL(triggered()),this,SLOT(ticketShow()));
	connect(m_radioWidget,SIGNAL(realtime()),m_sence,SLOT(setRealState()));
	connect(m_radioWidget,SIGNAL(analog()),m_sence,SLOT(setAnalogState()));
	connect(m_radioWidget,SIGNAL(ticket()),m_sence,SLOT(setTicketState()));

	//��·��ѯ
	connect(m_circleQueryAction,SIGNAL(triggered()),this,SLOT(showCircleQueryDockwdg()));
	connect(m_signQueryAction,SIGNAL(triggered()),this,SLOT(showSignQueryDockwdg()));
	connect(m_gswitchQueryAction,SIGNAL(triggered()),this,SLOT(showGswitchQueryDockwdg()));
	//connect(m_msetQueryAction,SIGNAL(triggered()),this,SLOT(showMsetQueryDockwdg()));
	connect(m_eventQueryAction,SIGNAL(triggered()),this,SLOT(showEventQueryDockwdg()));
}

void MainWindow::initDocWidget()
{
	createCircleQueryDockwdg();
	createSignQueryDockwdg();
	createGswitchQueryDockwdg();
	createMsetQueryDockwdg();
	createEventQueryDockwdg();
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
	m_sence->clearItem();
	m_sence->setBackgroundBrush(QBrush(Qt::white));
}
void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("���ļ�"),"/", tr("*.svg"));
	openFile(fileName,m_curStationId,false);
}

void MainWindow::openFile(QString fileName,QString stationId /* = 0 */,bool needRoot/* =true */)
{
	QString tempName = fileName;
	if (fileName.length()== 0)
	{
		QMessageBox::warning(this,MSG_TITLE,"�ļ�����Ϊ��");
		return;
	}
	// ���浱ǰվ��ID
	m_curStationId = stationId;

	// ��־�Ƿ��Ĭ��Ŀ¼�µ��ļ�
	if (needRoot)
	{
		QString svgRoot = ComUtil::instance()->getSvgRoot();
		if (svgRoot.length() == 0)
		{
			QMessageBox::warning(this,MSG_TITLE,"Svg·��Ϊ��");
			return;
		}

		fileName = svgRoot+"/"+fileName;
	}

	// �жϸ��ļ��Ƿ���ڱ���Ŀ¼����������ڣ���ͨ��FTP���ظ��ļ�
	QFile file(fileName);
	if (!file.exists())
	{
		file.close();
		FtpUtil::instance()->show();
		FtpUtil::instance()->getFile(tempName);
		return;
	}
	file.close();

	// ��ճ���
	cleanScene();

	// ��ʾ�ȴ�����
	m_waitDlg.show();

	// ��������ͼ���ļ��߳�
	m_openThread->open(fileName);

	//m_sence->openSvgFile(fileName);

	int idx = fileName.lastIndexOf("/");
	fileName = fileName.right(fileName.length()-idx-1);
	fileName = m_title+"-"+fileName;

	QString titleName;
	vector<Station_S> stationList = ComUtil::instance()->getStationMgrList();
	for (int i = 0;i<stationList.size();i++)
	{
		Station_S st = stationList.at(i);
		if (st.cimid == m_curStationId)
		{
			titleName = st.curname.length()>0?st.curname:st.name;
			break;
		}
	}

	this->setWindowTitle(titleName);

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
	FtpUtil::instance()->exec();
}

void MainWindow::initStatusBar()
{
	m_pConnLabel = new QLabel(this);
	this->statusBar()->addPermanentWidget(m_pConnLabel);
	//�ޱ߿�����
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
	case CMD_QUERY_CIRCLE_LIST:
		break;
	case CMD_QUERY_SIGN_LIST:
		showSignQueryResult(msg,msglength);
		break;
	case CMD_QUERY_GSWITCH_LIST:
		showGswitchQueryResult(msg,msglength);
		break;
	case CMD_QUERY_MSET_LIST:
		showMsetQueryResult(msg,msglength);
		break;
	case CMD_QUERY_EVENT_LIST:
		showEventQueryResult(msg,msglength);
		break;
	case CMD_TICKETMS_LIST:
	case CMD_TICKETMS_MANAGER:
	case CMD_TICKETMS_ADD:
	case CMD_TICKETMS_DEL:
	case CMD_TICKETMS_MODIFY:
	case CMD_TICKET_LIST:
	case CMD_TICKET_CREATE:
	case CMD_TICKET_QUERY:
	case CMD_TICKET_COMMIT:
		m_TcktMgr->recvdata(msgtype,msg,msglength);
		break;
	case CMD_READ_SAVING:
		m_sence->showSavingList(msg,msglength);
		break;
	case CMD_WRITE_SAVING:
		showWriteSavingResult(msg,msglength);
		break;

	case CMD_TRIGGER_RULES:
		// ���򴥷�����ʾ�����Ĺ����б�
		m_sence->showRuleList(msg,msglength);
		break;
	case CMD_TOPO_BREAKER_CHANGE:
	case CMD_CHECK_PASS:
		// ��λ�ͻ��˲������豸
		// ���ر�λ��̨ҵ���߼���ɣ����ش����豸�б��Դ����豸����ѹ�ȼ��������ɫ��ɫ
		m_sence->recvBreakerOpRes(msg,msglength);

		break;
	case CMD_TOPO_ENTIRE:
		showMsg("���˷�������ɣ������������ͻ���");
		break;
	case CMD_SERVER_BUSY:
		showMsg("���������ڽ������˷��������Ժ�����...");
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
		showMsg("����ɹ�");
	}
	else
	{
		showMsg("����ʧ��");
	}
}
void MainWindow::showPowerSetResult(const char* msg,int msglength)
{
	PBNS::PowerSetMsg_Response res;
	res.ParseFromArray(msg,msglength);
	if (res.rescode() == eSUCCESS)
	{
		showMsg("�����ɹ�");
	}
	else
	{
		showMsg("����ʧ��");
	}
}

void MainWindow::showTagOpResult(const char* msg,int msglength)
{
	PBNS::TagMsg_Response res;
	res.ParseFromArray(msg,msglength);
	if (res.rescode() == eSUCCESS)
	{
		showMsg("�����ɹ�");
	}
	else
	{
		showMsg("����ʧ��");
	}
}

void MainWindow::showLineSetResult(const char* msg,int msglength)
{
	PBNS::LineSetMsg_Response res;
	res.ParseFromArray(msg,msglength);
	if (res.rescode() == eSUCCESS)
	{
		showMsg("�����ɹ�");
	}
	else
	{
		showMsg("����ʧ��");
	}
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

void MainWindow::topoEntire()
{
	PBNS::OprationMsg_Request req; 
	req.set_saveid(m_sence->getSaveId());
	string msg=req.SerializeAsString();
	NetClient::instance()->sendData(CMD_TOPO_ENTIRE,msg.c_str(),msg.length());
	showMsg("���˷�������ͳɹ�");
}


void MainWindow::createCircleQueryDockwdg()
{
	m_pCircleQueryDockwdg = new QDockWidget;//(tr("��·��ѯ"),this);
	//����QDockWidget�Ĵ�������  
	m_pCircleQueryDockwdg->setWindowTitle(tr("��·��ѯ"));  
	//���õ�վ���QDockWidget�Ŀ�ͣ����������ȫ����ͣ��    
	m_pCircleQueryDockwdg->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	//����dock�е�tabwidget
	m_pCircleQueryTabWdgt = new QTableWidget;//(m_pCircleQueryDockwdg);

	m_pCircleQueryTabWdgt->setColumnCount(4);
	//m_pCircleQueryTabWdgt->setRowCount(5);
	//���tab�б�ͷ����
	QStringList headers;
	headers << "�豸����" << "��վ����" << "�豸ID" << "��վID" ;
	m_pCircleQueryTabWdgt->setHorizontalHeaderLabels(headers);
	//�б�����ɫ�仯
	m_pCircleQueryTabWdgt->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	m_pCircleQueryTabWdgt->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	m_pCircleQueryTabWdgt->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	m_pCircleQueryTabWdgt->setSelectionBehavior(QAbstractItemView::SelectRows);
	//��tabwidget��ӵ�dock��
	m_pCircleQueryDockwdg->setWidget(m_pCircleQueryTabWdgt);
	//��dockwidget��ӵ��������в��Ҳ�ͣ��
	this->addDockWidget(Qt::RightDockWidgetArea, m_pCircleQueryDockwdg);

	//��dock��������,����Ҫ�����õ�ʱ������ʾ��������tab�����ֵ
	m_pCircleQueryDockwdg->hide();

	//�˵������ѡ��
	//m_queryMenu->addAction(m_pCircleQueryDockwdg->toggleViewAction());

	return;
}

void MainWindow::showCircleQueryDockwdg()
{
	if (m_pCircleQueryDockwdg->isHidden())
	{
		PBNS::CircleListMsg_Request req;
		req.set_saveid(m_sence->getSaveId());

		//���䷢������������Ϣ�ź�
		NetClient::instance()->sendData(CMD_QUERY_CIRCLE_LIST,req.SerializeAsString().c_str(),req.SerializeAsString().length());

		m_pCircleQueryDockwdg->show();
	} 
}

void MainWindow::showCircleQueryResult(const char* msg,int msglength)
{
	PBNS::CircleListMsg_Response resp;
	resp.ParseFromArray(msg,msglength);

	int nrow = resp.circlelist_size();

	//��������
	m_pCircleQueryTabWdgt->setRowCount(nrow);

	for (int i=0;i<resp.circlelist_size();i++)
	{
		PBNS::CircleQueryBean cleBean = resp.circlelist(i);

		m_pCircleQueryTabWdgt->setItem(i,0,new QTableWidgetItem(QString::fromStdString(cleBean.name())));		
		m_pCircleQueryTabWdgt->setItem(i,1,new QTableWidgetItem(QString::fromStdString(cleBean.stationname())));	
		m_pCircleQueryTabWdgt->setItem(i,2,new QTableWidgetItem(QString::fromStdString(cleBean.unitcim())));
		m_pCircleQueryTabWdgt->setItem(i,3,new QTableWidgetItem(QString::fromStdString(cleBean.stationcim())));		
	}
}

void MainWindow::createSignQueryDockwdg()
{
	m_pSignQueryDockwdg = new QDockWidget;
	//����QDockWidget�Ĵ�������  
	m_pSignQueryDockwdg->setWindowTitle(tr("���Ʋ�ѯ"));  
	//���õ�վ���QDockWidget�Ŀ�ͣ����������ȫ����ͣ��    
	m_pSignQueryDockwdg->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	//����dock�е�tabwidget
	m_pSignQueryTabWdgt = new QTableWidget;

	m_pSignQueryTabWdgt->setColumnCount(5);
	//���tab�б�ͷ����
	QStringList headers;
	headers <<"�豸����"<<"��վ����"<<"�豸����"<<"�豸ID"<<"��վID" ;
	m_pSignQueryTabWdgt->setHorizontalHeaderLabels(headers);
	//�б�����ɫ�仯
	m_pSignQueryTabWdgt->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	m_pSignQueryTabWdgt->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	m_pSignQueryTabWdgt->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	m_pSignQueryTabWdgt->setSelectionBehavior(QAbstractItemView::SelectRows);
	//��tabwidget��ӵ�dock��
	m_pSignQueryDockwdg->setWidget(m_pSignQueryTabWdgt);
	//��dockwidget��ӵ��������в��Ҳ�ͣ��
	this->addDockWidget(Qt::RightDockWidgetArea, m_pSignQueryDockwdg);

	//��dock��������,����Ҫ�����õ�ʱ������ʾ��������tab�����ֵ
	m_pSignQueryDockwdg->hide();


	return;
}


void MainWindow::showSignQueryDockwdg()
{
	if (m_pSignQueryDockwdg->isHidden())
	{
		PBNS::SignListMsg_Request req;
		req.set_reqdate("1");

		//���䷢������������Ϣ�ź�
		NetClient::instance()->sendData(CMD_QUERY_SIGN_LIST,req.SerializeAsString().c_str(),req.SerializeAsString().length());

		m_pSignQueryDockwdg->show();
	} 
}

void MainWindow::showSignQueryResult(const char* msg,int msglength)
{
	PBNS::SignListMsg_Response resp;
	resp.ParseFromArray(msg,msglength);

	int nrow = resp.signlist_size();

	//��������
	m_pSignQueryTabWdgt->setRowCount(nrow);

	for (int i=0;i<resp.signlist_size();i++)
	{
		PBNS::SignQueryBean sgBean = resp.signlist(i);

		m_pSignQueryTabWdgt->setItem(i,0,new QTableWidgetItem(QString::fromStdString(sgBean.unitcimname())));		
		m_pSignQueryTabWdgt->setItem(i,1,new QTableWidgetItem(QString::fromStdString(sgBean.stationname())));
		m_pSignQueryTabWdgt->setItem(i,2,new QTableWidgetItem(QString::fromStdString(sgBean.unittype())));		
		m_pSignQueryTabWdgt->setItem(i,3,new QTableWidgetItem(QString::fromStdString(sgBean.unitcim())));
		m_pSignQueryTabWdgt->setItem(i,4,new QTableWidgetItem(QString::fromStdString(sgBean.stationcim())));		
	}

}

void MainWindow::createGswitchQueryDockwdg()
{
	m_pGswitchQueryDockwdg = new QDockWidget;
	//����QDockWidget�Ĵ�������  
	m_pGswitchQueryDockwdg->setWindowTitle(tr("�ӵز�ѯ"));  
	//���õ�վ���QDockWidget�Ŀ�ͣ����������ȫ����ͣ��    
	m_pGswitchQueryDockwdg->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	//����dock�е�tabwidget
	m_pGswitchQueryTabWdgt = new QTableWidget;

	m_pGswitchQueryTabWdgt->setColumnCount(5);
	//���tab�б�ͷ����
	QStringList headers;
	headers <<"�豸����"<<"��վ����"<<"����"<<"�豸ID"<<"��վID" ;
	m_pGswitchQueryTabWdgt->setHorizontalHeaderLabels(headers);
	//�б�����ɫ�仯
	m_pGswitchQueryTabWdgt->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	m_pGswitchQueryTabWdgt->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	m_pGswitchQueryTabWdgt->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	m_pGswitchQueryTabWdgt->setSelectionBehavior(QAbstractItemView::SelectRows);
	//��tabwidget��ӵ�dock��
	m_pGswitchQueryDockwdg->setWidget(m_pGswitchQueryTabWdgt);
	//��dockwidget��ӵ��������в��Ҳ�ͣ��
	this->addDockWidget(Qt::RightDockWidgetArea, m_pGswitchQueryDockwdg);

	//��dock��������,����Ҫ�����õ�ʱ������ʾ��������tab�����ֵ
	m_pGswitchQueryDockwdg->hide();

	return;
}

void MainWindow::showGswitchQueryDockwdg()
{
	if (m_pGswitchQueryDockwdg->isHidden())
	{
		PBNS::GswitchListMsg_Request req;
		req.set_reqdate("1");

		//���䷢������������Ϣ�ź�
		NetClient::instance()->sendData(CMD_QUERY_GSWITCH_LIST,req.SerializeAsString().c_str(),req.SerializeAsString().length());

		m_pGswitchQueryDockwdg->show();
	} 
}

void MainWindow::showGswitchQueryResult(const char* msg,int msglength)
{
	PBNS::GswitchListMsg_Response resp;
	resp.ParseFromArray(msg,msglength);

	int nrow = resp.gswitchlist_size();

	//��������
	m_pGswitchQueryTabWdgt->setRowCount(nrow);

	for (int i=0;i<resp.gswitchlist_size();i++)
	{
		PBNS::GswitchQueryBean gsBean = resp.gswitchlist(i);

		m_pGswitchQueryTabWdgt->setItem(i,0,new QTableWidgetItem(QString::fromStdString(gsBean.unitcimname())));		
		m_pGswitchQueryTabWdgt->setItem(i,1,new QTableWidgetItem(QString::fromStdString(gsBean.stationname())));
		m_pGswitchQueryTabWdgt->setItem(i,2,new QTableWidgetItem(QString::fromStdString(gsBean.unittype())));		
		m_pGswitchQueryTabWdgt->setItem(i,3,new QTableWidgetItem(QString::fromStdString(gsBean.unitcim())));
		m_pGswitchQueryTabWdgt->setItem(i,4,new QTableWidgetItem(QString::fromStdString(gsBean.stationcim())));		
	}
}

void MainWindow::createMsetQueryDockwdg()
{
	m_pMsetQueryDockwdg = new QDockWidget;
	//����QDockWidget�Ĵ�������  
	m_pMsetQueryDockwdg->setWindowTitle(tr("�˹����ò�ѯ"));  
	//���õ�վ���QDockWidget�Ŀ�ͣ����������ȫ����ͣ��    
	m_pMsetQueryDockwdg->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	//����dock�е�tabwidget
	m_pMsetQueryTabWdgt = new QTableWidget;
	//�б�����ɫ�仯
	m_pMsetQueryTabWdgt->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	m_pMsetQueryTabWdgt->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	m_pMsetQueryTabWdgt->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	m_pMsetQueryTabWdgt->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pMsetQueryTabWdgt->setColumnCount(5);
	m_pMsetQueryTabWdgt->setRowCount(5);
	//���tab�б�ͷ����
	QStringList headers;
	headers <<"�豸����"<<"��վ����"<<"����"<<"�豸ID"<<"��վID" ;
	m_pMsetQueryTabWdgt->setHorizontalHeaderLabels(headers);

	//��tabwidget��ӵ�dock��
	m_pMsetQueryDockwdg->setWidget(m_pMsetQueryTabWdgt);
	//��dockwidget��ӵ��������в��Ҳ�ͣ��
	this->addDockWidget(Qt::RightDockWidgetArea, m_pMsetQueryDockwdg);

	//��dock��������,����Ҫ�����õ�ʱ������ʾ��������tab�����ֵ
	m_pMsetQueryDockwdg->hide();


	return;
}

void MainWindow::showMsetQueryDockwdg()
{
	if (m_pMsetQueryDockwdg->isHidden())
	{
		/*
		PBNS::MsetListMsg_Request req;
		req.set_reqdate("1");

		//���䷢������������Ϣ�ź�
		NetClient::instance()->sendData(CMD_QUERY_MSET_LIST,req.SerializeAsString().c_str(),req.SerializeAsString().length());
		*/
		m_pMsetQueryDockwdg->show();
	} 
}

void MainWindow::showMsetQueryResult(const char* msg,int msglength)
{
	PBNS::MsetListMsg_Response resp;
	resp.ParseFromArray(msg,msglength);

	int nrow = resp.msetlist_size();

	//��������
	m_pMsetQueryTabWdgt->setRowCount(nrow);

	for (int i=0;i<resp.msetlist_size();i++)
	{
		PBNS::MsetQueryBean mstBean = resp.msetlist(i);

		m_pMsetQueryTabWdgt->setItem(i,0,new QTableWidgetItem(QString::fromStdString(mstBean.unitcimname())));		
		m_pMsetQueryTabWdgt->setItem(i,1,new QTableWidgetItem(QString::fromStdString(mstBean.stationname())));
		m_pMsetQueryTabWdgt->setItem(i,2,new QTableWidgetItem(QString::fromStdString(mstBean.unittype())));		
		m_pMsetQueryTabWdgt->setItem(i,3,new QTableWidgetItem(QString::fromStdString(mstBean.unitcim())));
		m_pMsetQueryTabWdgt->setItem(i,4,new QTableWidgetItem(QString::fromStdString(mstBean.stationcim())));		
	}
}

void MainWindow::createEventQueryDockwdg()
{
	m_pEventQueryDockwdg = new QDockWidget;
	//����QDockWidget�Ĵ�������  
	m_pEventQueryDockwdg->setWindowTitle(tr("�¼���ѯ"));  
	//���õ�վ���QDockWidget�Ŀ�ͣ����������ȫ����ͣ��    
	m_pEventQueryDockwdg->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	//����dock�е�tabwidget
	m_pEventQueryTabWdgt = new QTableWidget;

	m_pEventQueryTabWdgt->setColumnCount(4);
	//m_pEventQueryTabWdgt->setRowCount(5);
	//���tab�б�ͷ����
	QStringList headers;
	headers <<"��վ����"<<"�豸����"<<"����"<<"ʱ��" ;
	m_pEventQueryTabWdgt->setHorizontalHeaderLabels(headers);
	//�б�����ɫ�仯
	m_pEventQueryTabWdgt->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	m_pEventQueryTabWdgt->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	m_pEventQueryTabWdgt->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	m_pEventQueryTabWdgt->setSelectionBehavior(QAbstractItemView::SelectRows);
	//��tabwidget��ӵ�dock��
	m_pEventQueryDockwdg->setWidget(m_pEventQueryTabWdgt);
	//��dockwidget��ӵ��������в��Ҳ�ͣ��
	this->addDockWidget(Qt::RightDockWidgetArea, m_pEventQueryDockwdg);

	//��dock��������,����Ҫ�����õ�ʱ������ʾ��������tab�����ֵ
	m_pEventQueryDockwdg->hide();


	return;
}

void MainWindow::showEventQueryDockwdg()
{

	if (m_pEventQueryDockwdg->isHidden())
	{
		PBNS::EventListMsg_Request req;
		req.set_reqdate("1");

		//���䷢������������Ϣ�ź�
		NetClient::instance()->sendData(CMD_QUERY_EVENT_LIST,req.SerializeAsString().c_str(),req.SerializeAsString().length());

		m_pEventQueryDockwdg->show();
	} 
}

void MainWindow::showEventQueryResult(const char* msg,int msglength)
{
	PBNS::EventListMsg_Response resp;
	resp.ParseFromArray(msg,msglength);

	int nrow = resp.eventlist_size();

	//��������
	m_pEventQueryTabWdgt->setRowCount(nrow);

	for (int i=0;i<resp.eventlist_size();i++)
	{
		PBNS::EventQueryBean evtBean = resp.eventlist(i);

		m_pEventQueryTabWdgt->setItem(i,0,new QTableWidgetItem(QString::fromStdString(evtBean.stationname())));
		m_pEventQueryTabWdgt->setItem(i,1,new QTableWidgetItem(QString::fromStdString(evtBean.unitcimname())));		
		m_pEventQueryTabWdgt->setItem(i,2,new QTableWidgetItem(QString::fromStdString(evtBean.eventvalue())));		
		m_pEventQueryTabWdgt->setItem(i,3,new QTableWidgetItem(QString::fromStdString(evtBean.eventtime())));
		
	}
}

void MainWindow::ticketShow()
{
	//���ݲ�ͬ��ɫ������ͬ����Ʊ����
	if (ComUtil::instance()->getCurUserRole() == eDispatcher || ComUtil::instance()->getCurUserRole() == eManager)
	{
		m_TcktMgr = new TicketMgr(this);

		m_TcktMgr->setWindowTitle("�����������");

		m_TcktMgr->exec();

		delete m_TcktMgr;
	}

	if (ComUtil::instance()->getCurUserRole() == eMaintainers)
	{
		m_TcktMgr = new TicketMgr(this);
		m_TcktMgr->setWindowTitle("����Ʊ����");

		m_TcktMgr->setModal(false);
		m_TcktMgr->show();
	}
	

	//��ģʽ�����£�����������ǰ����ʾ
	//m_TcktMgr->setWindowFlags(Qt::WindowStaysOnTopHint);
	//m_TcktMgr->exec();

	//delete m_TcktMgr;

	return;
}