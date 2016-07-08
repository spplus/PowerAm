#include "stationmgr.h"

StationMgr::StationMgr(QWidget *parent)
	: QDialog(parent)
{
	//只显示关闭按钮
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->resize(720,900);

	QHBoxLayout *pstahlayt = new QHBoxLayout;
	QHBoxLayout *pbtnhlayt = new QHBoxLayout;
	QVBoxLayout *pmainvlyt = new QVBoxLayout;

	QGroupBox *pstalisgbox = new QGroupBox("厂站列表");
	ptabwdgStaList = new QTableWidget();
	//设置背景色为白色
	QPalette tabplt = ptabwdgStaList->palette();
	//tabplt.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));//设置透明
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	ptabwdgStaList->setPalette(tabplt);
	//设置列数
	ptabwdgStaList->setColumnCount(7);
	QStringList header;
	header<<"ID"<<"厂站类型"<<"类型名称"<<"厂站CIMID"<<"厂站名称"<<"厂站描述"<<"厂站图名";
	ptabwdgStaList->setHorizontalHeaderLabels(header);
	//使行列头自适应宽度，所有列平均分来填充空白部分
	//ptabwdgStaList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//行背景颜色变化
	ptabwdgStaList->setAlternatingRowColors(true);
	//设置每行内容不可编辑
	ptabwdgStaList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置只能选择一行，不能选择多行
	ptabwdgStaList->setSelectionMode(QAbstractItemView::SingleSelection);
	//设置单击选择一行
	ptabwdgStaList->setSelectionBehavior(QAbstractItemView::SelectRows);


	pbntStaType = new QPushButton("厂站类型");
	pbnCommit = new QPushButton("提  交");

	pbtnhlayt->addStretch();
	pbtnhlayt->addWidget(pbntStaType);
	pbtnhlayt->addStretch();
	pbtnhlayt->addWidget(pbnCommit);
	pbtnhlayt->addStretch();

	pstahlayt->addWidget(ptabwdgStaList);
	pstalisgbox->setLayout(pstahlayt);

	pmainvlyt->addWidget(pstalisgbox);
	pmainvlyt->addLayout(pbtnhlayt);

	pmainvlyt->setContentsMargins(8,5,8,10);
	this->setLayout(pmainvlyt);

	//设置厂站类型列隐藏
	ptabwdgStaList->setColumnHidden(1,true);
	//初始清空容器
	m_stationmdfvct.clear();
	pbnCommit->setEnabled(false);

	connect(pbntStaType,SIGNAL(pressed()),this,SLOT(createStaTypeMgrDlg()));
	connect(ptabwdgStaList,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(createStationMgrDlg(QTableWidgetItem*)));
	connect(pbnCommit,SIGNAL(pressed()),this,SLOT(commitStation()));

	loadStationList();

}

StationMgr::~StationMgr()
{

}

void StationMgr::loadStationList()
{
	m_stationvct.clear();
	ptabwdgStaList->clearContents();

	//获取厂站列表
	m_stationvct = ComUtil::instance()->getStationMgrList();

	int nrow = m_stationvct.size();

	//设置行数
	ptabwdgStaList->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		Station_S stalist = m_stationvct.at(i);
		
		QString strid = QString("%1").arg(stalist.id);
		QString strtypeid = QString("%1").arg(stalist.stypeid);

		ptabwdgStaList->setItem(i,0,new QTableWidgetItem(strid));
		ptabwdgStaList->setItem(i,1,new QTableWidgetItem(strtypeid));
		ptabwdgStaList->setItem(i,2,new QTableWidgetItem(stalist.stypename));
		ptabwdgStaList->setItem(i,3,new QTableWidgetItem(stalist.cimid));
		ptabwdgStaList->setItem(i,4,new QTableWidgetItem(stalist.name));
		ptabwdgStaList->setItem(i,5,new QTableWidgetItem(stalist.curname));
		ptabwdgStaList->setItem(i,6,new QTableWidgetItem(stalist.path));

	}

}


void StationMgr::createStaTypeMgrDlg()
{

	StationTypeMgr::instance()->setWindowIcon(QIcon(":images/stationmgr.png"));
	StationTypeMgr::instance()->setWindowTitle("厂站类型管理");

	StationTypeMgr::instance()->exec();


	return;
}


void StationMgr::createStationMgrDlg(QTableWidgetItem * item)
{
	pStationEditMgrDlg = new StationEditMgr(this);

	pStationEditMgrDlg->setWindowIcon(QIcon(":images/stationmgr.png"));
	pStationEditMgrDlg->setWindowTitle("厂站管理");
	
	setStationStruct(item);
	pStationEditMgrDlg->loadStationEditData();

	pStationEditMgrDlg->exec();

	//如果选中的厂站进行了修改则得到修改后的值
	if (pStationEditMgrDlg->getConfirmStationFlag())
	{
		pbnCommit->setEnabled(true);
		m_station = pStationEditMgrDlg->getStationSelectVal();

		//更新当前选中的厂站列表中的值,此步只是将修改的对应项显示而已
		int ncrow = item->row();
		QString strtypeid = QString("%1").arg(m_station.stypeid);
		ptabwdgStaList->setItem(ncrow,1,new QTableWidgetItem(strtypeid));
		ptabwdgStaList->setItem(ncrow,2,new QTableWidgetItem(m_station.stypename));
		ptabwdgStaList->setItem(ncrow,5,new QTableWidgetItem(m_station.curname));
		ptabwdgStaList->setItem(ncrow,6,new QTableWidgetItem(m_station.path));

		//设置容器中是否有修改的厂站标志
		bool bfind = false;

		//遍历容器查找编辑的厂站是否已经编辑过存储在容器中
		for (int i=0;i<m_stationmdfvct.size();i++)
		{
			//若容器中已有此编辑的厂站，需要更新厂站结构体各修改值
			if (m_station.id == m_stationmdfvct.at(i).id)
			{
				m_stationmdfvct.at(i).stypeid = m_station.stypeid;
				m_stationmdfvct.at(i).stypename = m_station.stypename;
				m_stationmdfvct.at(i).curname = m_station.curname;
				m_stationmdfvct.at(i).path = m_station.path;

				bfind = true;
				break;
			}
		}

		if (!bfind)
		{
			m_stationmdfvct.push_back(m_station);
		}

	}

	delete pStationEditMgrDlg;

	return;

}


void StationMgr::setStationStruct(QTableWidgetItem * item)
{
	int ncurrow = item->row();

	m_station.id = ptabwdgStaList->item(ncurrow,0)->text().toInt();
	m_station.stypeid = ptabwdgStaList->item(ncurrow,1)->text().toInt();
	m_station.stypename = ptabwdgStaList->item(ncurrow,2)->text();
	m_station.cimid = ptabwdgStaList->item(ncurrow,3)->text();
	m_station.name = ptabwdgStaList->item(ncurrow,4)->text();
	m_station.curname = ptabwdgStaList->item(ncurrow,5)->text();
	m_station.path = ptabwdgStaList->item(ncurrow,6)->text();

	//设置当前选中厂站值
	pStationEditMgrDlg->setStationSelectVal(m_station);

}


void StationMgr::commitStation()
{
	int ncount = m_stationmdfvct.size();
	if (ncount <= 0 )
	{
		QMessageBox::information(this,tr("厂站设置"),tr("未对厂站信息进行修改,不需提交!"));
		return;
	}

	PBNS::StationMgrMsg_Request stareq;
	for (int i=0;i<m_stationmdfvct.size();i++)
	{
		PBNS::StationBean *stabean = stareq.add_stationlist();
		stabean->set_id(m_stationmdfvct.at(i).id);
		stabean->set_categoryid(m_stationmdfvct.at(i).stypeid);
		stabean->set_cimid(m_stationmdfvct.at(i).cimid.toStdString());
		stabean->set_name(m_stationmdfvct.at(i).name.toStdString());
		stabean->set_currentname(m_stationmdfvct.at(i).curname.toStdString());
		stabean->set_path(m_stationmdfvct.at(i).path.toStdString());
	}

	string reqstr;
	stareq.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_STATION_MANAGER,reqstr.c_str(),reqstr.length());

	//this->close();

}

void StationMgr::retStationMgr(int msgtype,const char* msg)
{
	switch (msgtype)
	{
	case CMD_STATION_MANAGER:
		{
			PBNS::StationMgrMsg_Response starep;
			starep.ParseFromString(msg);
			if (starep.rescode() > 0)
			{
				
				QMessageBox::information(this,tr("厂站设置"),tr("厂站修改提交成功!"));
				ComUtil::instance()->getStationType();
				ComUtil::instance()->getStation();
				pbnCommit->setEnabled(false);
			}
			else
			{
				QMessageBox::information(this,tr("厂站设置"),tr("厂站修改提交失败!"));
				pbnCommit->setEnabled(true);
			}
		}
		break;
	}
}


/**************************************************************************************************************************/
/**************************************************************************************************************************/
StationTypeMgr* StationTypeMgr::m_inst = NULL;

StationTypeMgr* StationTypeMgr::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new StationTypeMgr;

	}
	return m_inst;
}

StationTypeMgr::StationTypeMgr()
	: QDialog()
{
	//只显示关闭按钮
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->resize(430,500);

	QHBoxLayout *ptypehlayt = new QHBoxLayout;
	QHBoxLayout *plinedhlayt = new QHBoxLayout;
	QHBoxLayout *pbtnhlayt = new QHBoxLayout;
	QVBoxLayout *pmainvlyt = new QVBoxLayout;

	QGroupBox *ptypelisgbox = new QGroupBox("厂站类型列表");
	ptabwdgStaTypeList = new QTableWidget();
	//设置背景色为白色
	QPalette tabplt = ptabwdgStaTypeList->palette();
	//tabplt.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));//设置透明
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	ptabwdgStaTypeList->setPalette(tabplt);
	//设置列数
	ptabwdgStaTypeList->setColumnCount(3);
	QStringList header;
	header<<"ID"<<"顺序号"<<"厂站类型名称";
	ptabwdgStaTypeList->setHorizontalHeaderLabels(header);
	//使行列头自适应宽度，所有列平均分来填充空白部分
	//ptabwdgStaList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//行背景颜色变化
	ptabwdgStaTypeList->setAlternatingRowColors(true);
	//设置每行内容不可编辑
	ptabwdgStaTypeList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置只能选择一行，不能选择多行
	ptabwdgStaTypeList->setSelectionMode(QAbstractItemView::SingleSelection);
	//设置单击选择一行
	ptabwdgStaTypeList->setSelectionBehavior(QAbstractItemView::SelectRows);

	QGroupBox *pgbox = new QGroupBox("类型显示");
	porderlab = new QLabel("顺序号");
	porderlab->setFixedWidth(40);
	porderspbox = new QSpinBox;
	porderspbox->setFixedWidth(135);
	porderspbox->setFixedHeight(25);

	porderlined = new QLineEdit;
	porderlined->setFixedWidth(135);
	porderlined->setFixedHeight(25);

	ptypelab = new QLabel("类型名称");
	ptypelab->setFixedWidth(50);
	ptypelined = new QLineEdit;
	ptypelined->setFixedWidth(135);
	ptypelined->setFixedHeight(25);

	QGroupBox *pbtngbox = new QGroupBox("操作");
	pbntadd = new QPushButton("增  加");
	pbntadd->setFixedHeight(25);
	pbntdel = new QPushButton("删  除");
	pbntdel->setFixedHeight(25);
	pbntmdf = new QPushButton("修  改");
	pbntmdf->setFixedHeight(25);

	ptypehlayt->addWidget(ptabwdgStaTypeList);
	ptypelisgbox->setLayout(ptypehlayt);

	plinedhlayt->addWidget(porderlab);
	plinedhlayt->addWidget(porderspbox);
	plinedhlayt->addStretch();
	plinedhlayt->addWidget(ptypelab);
	plinedhlayt->addWidget(ptypelined);
	pgbox->setLayout(plinedhlayt);


	pbtnhlayt->addWidget(pbntadd);
	pbtnhlayt->addWidget(pbntdel);
	pbtnhlayt->addWidget(pbntmdf);
	pbtngbox->setLayout(pbtnhlayt);

	pmainvlyt->addWidget(ptypelisgbox);
	pmainvlyt->addSpacing(20);
	pmainvlyt->addWidget(pgbox);
	pmainvlyt->addSpacing(20);
	pmainvlyt->addWidget(pbtngbox);
	pmainvlyt->addSpacing(5);

	pmainvlyt->setContentsMargins(8,8,8,15);

	this->setLayout(pmainvlyt);

	porderspbox->setEnabled(false);
	ptypelined->setEnabled(false);
	pbntmdf->setEnabled(false);

	bflgadd = false;
	bflgmdf = false;

	//加载厂站类型列表
	loadStationTypeList();
	
	//点击用户列表某行信号槽关联
	connect(ptabwdgStaTypeList,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getStationTypeItem(QTableWidgetItem*)));
	//操作按钮信号槽关联
	connect(pbntadd,SIGNAL(clicked()),this,SLOT(addStationType()));
	connect(pbntdel,SIGNAL(clicked()),this,SLOT(delStationType()));
	connect(pbntmdf,SIGNAL(clicked()),this,SLOT(mdfStationType()));


}


StationTypeMgr::~StationTypeMgr()
{
	if (m_inst != NULL)
	{
		delete m_inst;

		m_inst = NULL;
	}
}


void StationTypeMgr::loadStationTypeList()
{
	m_stationtype.clear();
	//ptabwdgStaTypeList->clear();
	ptabwdgStaTypeList->clearContents();

	m_stationtype = ComUtil::instance()->getStationTypeMgrList();

	int nrow = m_stationtype.size();
	//设置行数
	ptabwdgStaTypeList->setRowCount(nrow);

	for (int i=0; i<nrow; i++)
	{
		StationType_S stationtype = m_stationtype.at(i);
		QString strid = QString("%1").arg(stationtype.id);
		QString stroder = QString("%1").arg(stationtype.odernum);

		ptabwdgStaTypeList->setItem(i,0,new QTableWidgetItem(strid));
		ptabwdgStaTypeList->setItem(i,1,new QTableWidgetItem(stroder));

		ptabwdgStaTypeList->setItem(i,2,new QTableWidgetItem(stationtype.name));
	}

	if (nrow > 0)
	{
		ptabwdgStaTypeList->selectRow(0);
		porderspbox->setValue(ptabwdgStaTypeList->item(0,1)->text().toInt());
		ptypelined->setText(ptabwdgStaTypeList->item(0,2)->text());

		pbntmdf->setEnabled(true);

	}

	return;
}

void StationTypeMgr::recvdata(int msgtype,const char* msg,int msglength)
{
	switch (msgtype)
	{
	case CMD_STATION_TYPE_ADD:
	case CMD_STATION_TYPE_DEL:
	case CMD_STATION_TYPE_MODIFY:
		{
			PBNS::StationTypeMgrMsg_Response rep;
			rep.ParseFromString(msg);

			if (rep.rescode()>0)
			{
				//修改厂站类型成功后，对主界面中厂站类型重新加载
				ComUtil::instance()->getStationType();
				//loadStationTypeList();
			}
		}
		break;

	case CMD_STATION_TYPE_LIST:
		retStationTypeList(msgtype,msg);
		break;
	}

	return;
}

void StationTypeMgr::reqStationTypeList()
{
	PBNS::StationTypeMsg_Request statypereq;

	statypereq.set_saveid(1);

	QString strData;
	strData.append("1");


	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_STATION_TYPE_LIST,statypereq.SerializeAsString().c_str(),statypereq.SerializeAsString().length());

	return;
}

void StationTypeMgr::retStationTypeList(int msgtype,const char* msg)
{
	PBNS::StationTypeMsg_Response statyperep;
	statyperep.ParseFromString(msg);

	//记录请求前选择的行
	int ncurrow = ptabwdgStaTypeList->currentRow();

	//ptabwdgStaTypeList->clear();
	ptabwdgStaTypeList->clearContents();
	int nrow = statyperep.typelist_size();

	//设置行数
	ptabwdgStaTypeList->setRowCount(nrow);

	for (int i=0; i<nrow; i++)
	{
		PBNS::StationTypeBean typebean = statyperep.typelist(i);

		QString strid = QString("%1").arg(typebean.id());
		QString stroder = QString("%1").arg(typebean.ordernum());

		ptabwdgStaTypeList->setItem(i,0,new QTableWidgetItem(strid));
		ptabwdgStaTypeList->setItem(i,1,new QTableWidgetItem(stroder));
		ptabwdgStaTypeList->setItem(i,2,new QTableWidgetItem(QString::fromStdString(typebean.name())));

	}

	if (nrow > 0)
	{
		int nitemrow = ncurrow;

		if (msgtype == CMD_STATION_TYPE_ADD)
		{
			nitemrow = nrow;
		}

		if (msgtype == CMD_STATION_TYPE_DEL)
		{
			if (ncurrow > 0)
			{
				nitemrow = ncurrow - 1;
			}
	
		}

		if (msgtype == CMD_STATION_TYPE_MODIFY)
		{
			nitemrow = ncurrow;
		}

		ptabwdgStaTypeList->selectRow(nitemrow);

		porderspbox->setValue(ptabwdgStaTypeList->item(nitemrow,1)->text().toInt());
		ptypelined->setText(ptabwdgStaTypeList->item(nitemrow,2)->text());

		pbntadd->setEnabled(true);
		pbntdel->setEnabled(true);
		pbntmdf->setEnabled(true);
	}

	porderspbox->setEnabled(false);
	ptypelined->setEnabled(false);

	bflgadd = false;
	bflgmdf = false;

	return;
}

void StationTypeMgr::getStationTypeItem(QTableWidgetItem *item)
{
	int itemrow = item->row();

	porderspbox->setValue(ptabwdgStaTypeList->item(itemrow,1)->text().toInt());

	ptypelined->setText(ptabwdgStaTypeList->item(itemrow,2)->text());

	pbntdel->setEnabled(true);
	pbntmdf->setEnabled(true);

}


void StationTypeMgr::addStationType()
{
	if (!bflgadd)
	{
		porderspbox->clear();
		ptypelined->clear();

		porderspbox->setEnabled(true);
		ptypelined->setEnabled(true);

		pbntdel->setEnabled(false);
		pbntmdf->setEnabled(false);

		bflgadd = true;
	} 
	else
	{
		if (porderspbox->value()<=0)
		{
			QMessageBox::information(this,tr("厂站类型设置"),tr("顺序号要大于0，请输入大于0值!"));
			return;
		}

		if (ptypelined->text() == "")
		{
			QMessageBox::information(this,tr("厂站类型设置"),tr("厂站类型名称不可为空，请输入厂站类型名称!"));
			return;
		}

		int norder = porderspbox->value();
		QString strtype = ptypelined->text();

		QString sql = QString("INSERT INTO station_category (ordernum,name) VALUES (%1,'%2')").arg(norder).arg(strtype);

		PBNS::StationTypeMgrMsg_Request addreq;
		addreq.set_mgrsql(sql.toStdString());

		NetClient::instance()->sendData(CMD_STATION_TYPE_ADD,addreq.SerializeAsString().c_str(),addreq.SerializeAsString().length());

		bflgadd = false;
		porderspbox->setEnabled(false);
		ptypelined->setEnabled(false);

		//添加完成后,请求用户列表
		reqStationTypeList();
	}

}

void StationTypeMgr::delStationType()
{
	int ncurrow = ptabwdgStaTypeList->currentRow();
	//取用户列表中选中行的第一列用户id号
	int nStatypeid = ptabwdgStaTypeList->item(ncurrow,0)->text().toInt();

	QString sql = QString("delete from station_category where id=%1").arg(nStatypeid);

	PBNS::StationTypeMgrMsg_Request delureq;
	delureq.set_mgrsql(sql.toStdString());

	NetClient::instance()->sendData(CMD_STATION_TYPE_DEL,delureq.SerializeAsString().c_str(),delureq.SerializeAsString().length());

	//设置操作按钮状态
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

	//删除完成后,请求用户列表
	reqStationTypeList();

	return;
}


void StationTypeMgr::mdfStationType()
{
	if (!bflgmdf)
	{
		porderspbox->setEnabled(true);
		ptypelined->setEnabled(true);

		pbntadd->setEnabled(false);
		pbntdel->setEnabled(false);

		bflgmdf = true;
	} 
	else
	{
		if (porderspbox->value()<=0)
		{
			QMessageBox::information(this,tr("厂站类型设置"),tr("顺序号要大于0，请输入大于0值!"));
			return;
		}

		if (ptypelined->text() == "")
		{
			QMessageBox::information(this,tr("厂站类型设置"),tr("厂站类型名称不可为空，请输入厂站类型名称!"));
			return;
		}

		int currow = ptabwdgStaTypeList->currentRow();
		//取厂站类型ID(厂站类型表主键)
		int nid = ptabwdgStaTypeList->item(currow,0)->text().toInt();

		int norder = porderspbox->value();
		QString strtype = ptypelined->text();

		QString sql = QString("UPDATE station_category SET ordernum=%1 , name='%2' where id=%3 ").arg(norder).arg(strtype).arg(nid);

		PBNS::StationTypeMgrMsg_Request mdfreq;
		mdfreq.set_mgrsql(sql.toStdString());

		NetClient::instance()->sendData(CMD_STATION_TYPE_MODIFY,mdfreq.SerializeAsString().c_str(),mdfreq.SerializeAsString().length());

		bflgmdf = false;
		porderspbox->setEnabled(false);
		ptypelined->setEnabled(false);

		//修改完成后,请求用户列表
		reqStationTypeList();

		return;
	}
}


/**************************************************************************************************************************/
/*                                                  站点编辑管理类                                                        */
/**************************************************************************************************************************/

StationEditMgr::StationEditMgr(QWidget *parent)
{
	//只显示关闭按钮
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->resize(370,400);
	this->setMaximumWidth(400);

	QHBoxLayout *phlayt1 = new QHBoxLayout;
	QHBoxLayout *phlayt2 = new QHBoxLayout;
	QHBoxLayout *phlayt3 = new QHBoxLayout;
	QHBoxLayout *phlayt4 = new QHBoxLayout;
	QHBoxLayout *phlayt5 = new QHBoxLayout;
	QHBoxLayout *phlayt6 = new QHBoxLayout;
	QHBoxLayout *phlayt7 = new QHBoxLayout;

	QHBoxLayout *pbtnhlayt = new QHBoxLayout;
	QVBoxLayout *pmainvlyt = new QVBoxLayout;

	//"ID"<<"厂站类型"<<"类型名称"<<"厂站CIMID"<<"厂站名称"<<"厂站描述"<<"厂站图名";

	QLabel *pidlab = new QLabel("厂站ID");
	pidlab->setFixedSize(50,25);
	QLabel *ptypelab = new QLabel("类型名称");
	ptypelab->setFixedSize(50,25);
	QLabel *pcimidlab = new QLabel("厂站CIMID");
	pcimidlab->setFixedSize(55,25);
	QLabel *pstanamelab = new QLabel("厂站名称");
	pstanamelab->setFixedSize(50,25);
	QLabel *pstadislab = new QLabel("厂站描述");
	pstadislab->setFixedSize(50,25);
	QLabel *ppathlab = new QLabel("厂站图名");
	ppathlab->setFixedSize(50,25);
	

	pidlined = new QLineEdit;
	pidlined->setFixedSize(230,25);
	pstatypecbox = new QComboBox;
	pstatypecbox->setFixedSize(230,25);
	pcimidlined = new QLineEdit;
	pcimidlined->setFixedSize(230,25);
	pnamelined = new QLineEdit;
	pnamelined->setFixedSize(230,25);
	pstadisclined = new QLineEdit;
	pstadisclined->setFixedSize(230,25);
	ppathcbox = new QComboBox;
	ppathcbox->setFixedSize(230,25);
	pbntok = new QPushButton("确  认");
	pbntok->setFixedSize(150,25);

	phlayt1->addWidget(pidlab);
	phlayt1->addWidget(pidlined);
	
	phlayt2->addWidget(ptypelab);
	phlayt2->addWidget(pstatypecbox);

	phlayt3->addWidget(pcimidlab);
	phlayt3->addWidget(pcimidlined);

	phlayt4->addWidget(pstanamelab);
	phlayt4->addWidget(pnamelined);

	phlayt5->addWidget(pstadislab);
	phlayt5->addWidget(pstadisclined);

	phlayt6->addWidget(ppathlab);
	//phlayt6->addWidget(ppathlined);
	phlayt6->addWidget(ppathcbox);

	phlayt7->addStretch();
	phlayt7->addWidget(pbntok);
	phlayt7->addStretch();

	pmainvlyt->addLayout(phlayt1);
	pmainvlyt->addSpacing(15);
	pmainvlyt->addLayout(phlayt2);
	pmainvlyt->addSpacing(15);
	pmainvlyt->addLayout(phlayt3);
	pmainvlyt->addSpacing(15);
	pmainvlyt->addLayout(phlayt4);
	pmainvlyt->addSpacing(15);
	pmainvlyt->addLayout(phlayt5);
	pmainvlyt->addSpacing(15);
	pmainvlyt->addLayout(phlayt6);
	pmainvlyt->addSpacing(15);
	pmainvlyt->addStretch();

	pmainvlyt->addLayout(phlayt7);

	pmainvlyt->setContentsMargins(22,30,22,30);

	this->setLayout(pmainvlyt);

	bcfmflag = false;
	pidlined->setEnabled(false);
	pcimidlined->setEnabled(false);
	pnamelined->setEnabled(false);

	connect(pbntok,SIGNAL(pressed()),this,SLOT(confirmStation()));

}


StationEditMgr::~StationEditMgr()
{

}


void StationEditMgr::loadStationEditData()
{
	setStationTypeCbox();
	setStationPathCbox();

	QString strid = QString("%1").arg(m_station.id);
	QString strtypeid = QString("%1").arg(m_station.stypeid);

	for (int i=0;i<pstatypecbox->count();i++)
	{
		if (m_station.stypeid == pstatypecbox->itemData(i).toInt())
		{
			pstatypecbox->setCurrentIndex(i);
		}
	}

	pidlined->setText(strid);
	pcimidlined->setText(m_station.cimid);
	pnamelined->setText(m_station.name);
	pstadisclined->setText(m_station.curname);

	//默认设置svg选择框中为第一个显示值
	ppathcbox->setCurrentIndex(0);

	for (int i=0;i<ppathcbox->count();i++)
	{
		if (m_station.path == ppathcbox->itemText(i))
		{
			ppathcbox->setCurrentIndex(i);
		}
	}

}


void StationEditMgr::setStationTypeCbox()
{
	m_stationtype = ComUtil::instance()->getStationTypeMgrList();

	for (int i=0;i<m_stationtype.size();i++)
	{
		StationType_S stationtype = m_stationtype.at(i);
		QString strid = QString("%1").arg(stationtype.id);
		QString stroder = QString("%1").arg(stationtype.odernum);
		QString strname = stationtype.name;

		pstatypecbox->insertItem(i,strname,stationtype.id);

	}
}

void StationEditMgr::setStationPathCbox()
{
	m_svgflist = ComUtil::instance()->getSvgPathName();
	//m_svgflist<<"10kV城关开关站.fac.svg"<<"35kV敖城变电站.fac.svg"<<"35kV白云山电站.fac.svg"<<"35kV大冲变电站.fac.svg"<<"35kV高塘变电站.fac.svg"<<"35kV高塘变电站1.fac.svg"<<"35kV固江变电站.fac.svg"<<"35kV盘田变电站.fac.svg";

	for (int i=0;i<m_svgflist.size();i++)
	{
		QString strsvgpath = m_svgflist.at(i);

		ppathcbox->insertItem(i,strsvgpath);
	}

}

void StationEditMgr::confirmStation()
{
	bcfmflag = true;

	m_station.stypeid = pstatypecbox->itemData(pstatypecbox->currentIndex()).toInt();
	m_station.stypename = pstatypecbox->currentText();
	m_station.curname = pstadisclined->text();

	m_station.path = ppathcbox->currentText();

	this->close();
}


bool StationEditMgr::getConfirmStationFlag()
{
	return bcfmflag;

}

















