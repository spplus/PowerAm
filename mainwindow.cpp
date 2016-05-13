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
	
	m_title = ComUtil::instance()->getSysName();
	initWidget();
	initToolBar();
	initMenu();
	initStatusBar();
	setWindowTitle(m_title);
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

	setCentralWidget(m_spliter);
	this->showMaximized();
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
	connect(m_model,SIGNAL(openFile(QString)),this,SLOT(openFile(QString)));

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
	m_sysMenu = this->menuBar()->addMenu(tr("ϵͳ&"));
	m_sysMenu->addAction(m_downSvg);

	m_editMenu = this->menuBar()->addMenu(tr("����&"));
	m_editMenu->addAction(m_userMgrAction);
	m_editMenu->addAction(m_stationTypeAction);
	m_editMenu->addAction(m_stationRelaAction);
	
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

		m_contextMenu->addAction(m_onAction);
		m_contextMenu->addAction(m_offAction);
		m_contextMenu->addAction(m_signOnAction);
		m_contextMenu->addAction(m_signOffAction);

		break;
	default:
		break;
	}
}

void MainWindow::initActions()
{
	m_homeAction = new QAction(QIcon(":images/home.png"),tr("��ҳ"),this);
	m_openAction = new QAction(QIcon(":images/open.png"),tr("��"),this);
	m_zoutAction	 = new QAction(QIcon(":images/zoomout.png"),tr("�Ŵ�"),this);
	m_zinAction = new QAction(QIcon(":images/zoomin.png"),tr("��С"),this);
	m_nextAction = new QAction(QIcon(":images/next.png"),tr("ǰ��"),this);
	m_prevAction = new QAction(QIcon(":images/prev.png"),tr("����"),this);
	m_refreshAction = new QAction(QIcon(":images/refresh.png"),tr("ˢ��"),this);
	m_saveAction = new QAction(QIcon(":images/signquery.png"),tr("����"),this);
	m_readAction = new QAction(QIcon(":images/signquery.png"),tr("��ȡ"),this);
	m_controlAction = new QAction(QIcon(":images/signquery.png"),tr("����"),this);
	m_chekAction = new QAction(QIcon(":images/signquery.png"),tr("У��"),this);
	m_netAction = new QAction(QIcon(":images/prev.png"),tr("����"),this);
	m_soundAction = new QAction(QIcon(":images/prev.png"),tr("�����澯"),this);
	m_roleAction = new QAction(QIcon(":images/prev.png"),tr("��������"),this);
	m_circleQueryAction = new QAction(QIcon(":images/circlequery.png"),tr("��·��ѯ"),this);
	m_signQueryAction = new QAction(QIcon(":images/signquery.png"),tr("���Ʋ�ѯ"),this);
	m_gswitchQueryAction = new QAction(QIcon(":images/gswitchquery.png"),tr("�ӵز�ѯ"),this);
	m_opQueryAction = new QAction(QIcon(":images/gswitchquery.png"),tr("������ѯ"),this);
	m_eventQueryAction	= new QAction(QIcon(":images/download.png"),tr("�¼���ѯ"),this);
	m_modelAction = new QAction(QIcon(":images/model.png"),tr("����ģʽ"),this);

	m_userMgrAction = new QAction(QIcon(":images/usermgr.png"),tr("�û�����"),this);
	m_stationTypeAction = new QAction(QIcon(":images/stationmgr.png"),tr("��վ�������"),this);
	m_stationRelaAction = new QAction(QIcon(":images/stationrela.png"),tr("��վ����"),this);

	m_msetQueryAction = new QAction(QIcon(":images/msetquery.png"),tr("�˹����ò�ѯ"),this);
	m_cutQueryAction = new QAction(QIcon(":images/cutquery.png"),tr("��·�����ѯ"),this);
	m_onAction = new QAction(QIcon(":images/on.png"),tr("�ú�"),this);
	m_offAction = new QAction(QIcon(":images/off.png"),tr("�÷�"),this);
	m_signOnAction = new QAction(QIcon(":images/signon.png"),tr("����"),this);
	m_signOffAction = new QAction(QIcon(":images/signoff.png"),tr("ժ��"),this);
	m_viewModelAction = new QAction(QIcon(":images/pointer.png"),tr("ָ��"),this);
	m_originalAction = new QAction(QIcon(":images/zoom_original.png"),tr("ԭʼ�ߴ�"),this);
	m_downSvg = new QAction(QIcon(":images/download.png"),tr("����Svg"),this);


	m_roleQueryAction = new QAction(QIcon(":images/download.png"),tr("У�����"),this);
	m_topoQueryAction = new QAction(QIcon(":images/download.png"),tr("���˲�ѯ"),this);
	m_intervalQueryAction = new QAction(QIcon(":images/download.png"),tr("�����ѯ"),this);
	m_scadaLogQueryAction = new QAction(QIcon(":images/download.png"),tr("scada��־��ѯ"),this);
	


	// 1 ָ�� 2 ����
	m_viewModelAction->setData(QVariant(QGraphicsView::ScrollHandDrag));

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
	connect(m_downSvg,SIGNAL(triggered()),this,SLOT(showDownSvg()));
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::goHome()
{
	this->hide();
	HomeWindow::instance()->show();
}

void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("���ļ�"),"/", tr("*.svg"));
	openFile(fileName);


}

void MainWindow::openFile(QString fileName)
{
	if (fileName.length()== 0)
	{
		QMessageBox::warning(this,"ϵͳ��ʾ","�ļ�����Ϊ��");
		return;
	}

	m_sence->openSvgFile(fileName);
	int idx = fileName.lastIndexOf("/");
	fileName = fileName.right(fileName.length()-idx-1);
	fileName = m_title+"-"+fileName;
	this->setWindowTitle(fileName);


	// �����豸״̬����
	PBNS::DevStateMsg_Request req;
	req.set_saveid(1);
	req.set_stationid(1);
	string reqstr;
	req.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_DEV_STATE,reqstr.c_str(),reqstr.length());
}

void MainWindow::setViewModel()
{
	QGraphicsView::DragMode md;
	QIcon icon;
	if (m_viewModelAction->data().toInt() == QGraphicsView::NoDrag)
	{
		md = QGraphicsView::ScrollHandDrag;
		m_viewModelAction->setIcon(QIcon(":images/hand.png"));
	}
	else
	{
		md = QGraphicsView::NoDrag;
		m_viewModelAction->setIcon(QIcon(":images/pointer.png"));
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
	//�ޱ߿�����
	statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

}

void MainWindow::recvdata(int msgtype,const char* msg,int msglength)
{
	qDebug()<<msgtype;
	qDebug()<<msg;
	qDebug()<<msglength;
	
	switch (msgtype)
	{
	case CMD_DEV_STATE:
		m_sence->showDevState(msg,msglength);
		break;
	}

}

//�ر���Ӧ�¼�
void MainWindow::closeEvent(QCloseEvent *event)
{
	//pStaTypeDockWidget->close();
	return;
}