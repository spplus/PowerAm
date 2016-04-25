#include "mainwindow.h"
#include "svgparser.h"

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

#include "navmodel.h"
#include "navdelegate.h"
#include "textitem.h"


MainWindow * MainWindow::m_self = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
	m_self = this;
	m_title = "��������ϵͳ";
	initWidget();
	initToolBar();
	initMenu();
	setWindowTitle(m_title);
}

void MainWindow::initWidget()
{
	initNavView();

	m_spliter = new QSplitter(this);
	
	m_contextMenu = new QMenu(this);
	m_sence = new GraphicsScene(this,m_contextMenu);

	m_sence->setBackgroundBrush(QBrush(Qt::white));
	m_view = new GraphicsView(m_sence);

	this->setGeometry(100, 100, 800, 500);
	m_spliter->addWidget(m_navview);
	m_spliter->addWidget(m_view);

	QList<int> sizes;
	sizes.append(200);
	sizes.append(20);
	m_spliter->setSizes(sizes);
	m_spliter->setStretchFactor(1,1);

	setCentralWidget(m_spliter);
	this->showMaximized();
}

void MainWindow::initNavView()
{
	m_navview = new NavView(this);
	NavModel* model = new NavModel(this);
	model->ReadDataFromConfig(QCoreApplication::applicationDirPath() + "/config.xml");
	NavDelegate* delegate = new NavDelegate(this);
	m_navview->setModel(model);
	m_navview->setItemDelegate(delegate);
	connect(m_navview, SIGNAL(doubleClicked(const QModelIndex &)), model, SLOT(Collapse(const QModelIndex&)));
	connect(model,SIGNAL(openFile(QString)),this,SLOT(openFile(QString)));
}

void MainWindow::initToolBar()
{
	initActions();
	m_toolBar =  this->addToolBar(tr("toolbar"));
	m_toolBar->addAction(m_viewModelAction);
	m_toolBar->addAction(m_openAction);
	m_toolBar->addAction(m_prevAction);
	m_toolBar->addAction(m_nextAction);
	m_toolBar->addAction(m_zoutAction);
	m_toolBar->addAction(m_zinAction);
	m_toolBar->addAction(m_originalAction);
	m_toolBar->addAction(m_refreshAction);
	m_toolBar->addAction(m_modelAction);

	m_toolBar->setIconSize(QSize(36,36));
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
	
	m_openAction = new QAction(QIcon(":images/open.png"),tr("��"),this);
	m_zoutAction	 = new QAction(QIcon(":images/zoomout.png"),tr("�Ŵ�"),this);
	m_zinAction = new QAction(QIcon(":images/zoomin.png"),tr("��С"),this);
	m_nextAction = new QAction(QIcon(":images/next.png"),tr("ǰ��"),this);
	m_prevAction = new QAction(QIcon(":images/prev.png"),tr("����"),this);
	m_userMgrAction = new QAction(QIcon(":images/usermgr.png"),tr("�û�����"),this);
	m_stationTypeAction = new QAction(QIcon(":images/stationmgr.png"),tr("��վ�������"),this);
	m_stationRelaAction = new QAction(QIcon(":images/stationrela.png"),tr("��վ����"),this);
	m_modelAction = new QAction(QIcon(":images/model.png"),tr("����ģʽ"),this);
	m_refreshAction = new QAction(QIcon(":images/refresh.png"),tr("ˢ������"),this);
	m_circleQueryAction = new QAction(QIcon(":images/circlequery.png"),tr("��·��ѯ"),this);
	m_signQueryAction = new QAction(QIcon(":images/signquery.png"),tr("���Ʋ�ѯ"),this);
	m_gswitchQueryAction = new QAction(QIcon(":images/gswitchquery.png"),tr("�ص���ѯ"),this);
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
	m_eventQueryAction	= new QAction(QIcon(":images/download.png"),tr("�¼���ѯ"),this);


	// 1 ָ�� 2 ����
	m_viewModelAction->setData(QVariant(QGraphicsView::ScrollHandDrag));

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

void MainWindow::recvdata(int msgtype,const char* msg,int msglength)
{

}

void MainWindow::connected()
{

}

void MainWindow::disconnected()
{

}
