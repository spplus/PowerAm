#include "ticketmgr.h"
//#include "comutil.h"
//#include "define.h"

TicketMgr::TicketMgr(QWidget* parent /* = NULL */)
{
	setAttribute (Qt::WA_DeleteOnClose);

	m_ticketTable = NULL;
	m_missionTable = NULL;
	m_crateTime = NULL;
	m_tktActmgr = NULL;
	m_bdlgflag = false;

	switch (ComUtil::instance()->getCurUserRole())
	{
	case eManager://管理员
		initUi_Mission();
		break;
	case eDispatcher://调度
		initUi_Mission();
		break;
	case eAutomater://自动化
		break;
	case eMaintainers://运维
		initUi_Action();
		break;
	default:
		break;
	}

	//initUi_Action();
	//initSlots();
	initData();
}

void TicketMgr::initUi_Mission()
{
	//只显示关闭按钮
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	QVBoxLayout*		vbox = new QVBoxLayout;
	QGroupBox* gbox = new QGroupBox;
	gbox->setTitle(tr("操作任务"));
	m_missionTable = new QTableWidget;
	QHBoxLayout* hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_missionTable);
	gbox->setLayout(hbox0);
	//使行列头自适应宽度，所有列平均分来填充空白部分
	m_missionTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//行背景颜色变化
	m_missionTable->setAlternatingRowColors(true);
	//设置每行内容不可编辑
	m_missionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置只能选择一行，不能选择多行
	m_missionTable->setSelectionMode(QAbstractItemView::SingleSelection);
	//设置单击选择一行
	m_missionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

	QHBoxLayout* hbox1 = new QHBoxLayout;
	QLabel* doUser = new QLabel(tr("执行人"));
	m_userBox = new QComboBox;
	QLabel* doName = new QLabel(tr("操作任务名称"));
	m_nameLied = new QLineEdit;
	QLabel* doTime = new QLabel(tr("发布时间"));
	m_crateTime = new QDateTimeEdit;
	m_crateTime->setCalendarPopup(true);
	hbox1->addStretch();
	hbox1->addWidget(doUser);
	hbox1->addWidget(m_userBox);
	hbox1->addWidget(doName);
	hbox1->addWidget(m_nameLied);
	hbox1->addWidget(doTime);
	hbox1->addWidget(m_crateTime);
	hbox1->addStretch();

	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_addBtn = new QPushButton(tr("新增"));
	m_mdfBtn = new QPushButton(tr("修改"));
	m_delBtu = new QPushButton(tr("删除"));

	hbox2->addWidget(m_addBtn);
	hbox2->addWidget(m_delBtu);
	hbox2->addWidget(m_mdfBtn);

	vbox->addWidget(gbox);
	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2);

	connect(m_mdfBtn,SIGNAL(pressed()),this,SLOT(onMdf()));
	connect(m_addBtn,SIGNAL(pressed()),this,SLOT(onAdd()));
	connect(m_delBtu,SIGNAL(pressed()),this,SLOT(onDel()));

	//点击列表某行信号槽关联
	connect(m_missionTable,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getTicketMsionItem(QTableWidgetItem*)));

	//默认设置用户名,密码,角色,真实姓名框为只读
	m_userBox->setDisabled(true);
	m_nameLied->setDisabled(true);
	m_crateTime->setDisabled(true);

	//默认设置操作按钮状态，未选中用户列表时只可增加用户,增加按钮有效，删除和修改按钮无效
	m_addBtn->setEnabled(true);
	m_delBtu->setEnabled(false);
	m_mdfBtn->setEnabled(false);

	//默认添加用户标志位false
	baddflag = false;
	bmdfflag = false;

	setLayout(vbox);
}

void TicketMgr::initUi_Action()
{
	//只显示关闭按钮
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->setFixedWidth(680);
	//this->resize(650,430);

	QVBoxLayout*		vbox = new QVBoxLayout;
	QGroupBox* gbox = new QGroupBox;
	gbox->setTitle(tr("操作任务"));
	m_missionTable = new QTableWidget;
	QHBoxLayout* hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_missionTable);
	gbox->setLayout(hbox0);

	QHBoxLayout* hbox1 = new QHBoxLayout;
	
	QGroupBox* gbox2 = new QGroupBox;
	gbox2->setTitle(tr("操作票"));
	m_ticketTable = new QTableWidget;
	hbox1->addWidget(m_ticketTable);
	gbox2->setLayout(hbox1);
	//行背景颜色变化
	m_ticketTable->setAlternatingRowColors(true);
	//设置每行内容不可编辑
	m_ticketTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置只能选择一行，不能选择多行
	m_ticketTable->setSelectionMode(QAbstractItemView::SingleSelection);
	//设置单击选择一行
	m_ticketTable->setSelectionBehavior(QAbstractItemView::SelectRows);


	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_createBtn = new QPushButton(tr("创建操作票"));
	m_comitBtn = new QPushButton(tr("提 交"));
	m_queryBtn = new QPushButton(tr(("查询操作票")));
	//hbox2->addWidget(gbox);
	hbox2->addWidget(m_createBtn);
	hbox2->addWidget(m_comitBtn);
	hbox2->addWidget(m_queryBtn);

	QHBoxLayout* hbox3 = new QHBoxLayout;
	hbox3->addWidget(gbox2);

	vbox->addWidget(gbox);
	vbox->addLayout(hbox2);
	vbox->addLayout(hbox3);

	//connect(m_createBtn,SIGNAL(pressed()),this,SLOT(onSaveTicket()));
	setLayout(vbox);
}

void TicketMgr::initSlots()
{
	//connect(m_saveBtn,SIGNAL(pressed()),this,SLOT(onSaveTicket()));
	//connect(m_createBtn,SIGNAL(pressed()),this,SLOT(onCreateActions()));
}

void TicketMgr::recvdata(int msgtype,const char* msg,int msglength)
{
	switch (msgtype)
	{
	case CMD_TICKETMS_LIST:
		retTicketMsionList(msg);
		break;
	case CMD_TICKET_LIST:
		retTicketList(msg);
		break;
	case CMD_TICKET_COMMIT:
		{
			PBNS::TicketMgrMsg_Response resp;
			resp.ParseFromString(msg);
			//提交成功时，重新查询操作票列表
			if (resp.rescode())
			{
				reqTicketList();
			}
		}
		break;
	case CMD_TICKET_QUERY:
		m_tktActmgr->retTicketActionsList(msg);
		break;
	}
}

void TicketMgr::reqTicketMsionList()
{
	//非调管理员、度员角色、运维员不请求查询操作任务
	if (!(ComUtil::instance()->getCurUserRole() == eDispatcher || ComUtil::instance()->getCurUserRole() == eManager || ComUtil::instance()->getCurUserRole() == eMaintainers))
	{
		return;
	}

	PBNS::TicketMsionListMsg_Request tkmsreq;

	tkmsreq.set_userid(ComUtil::instance()->getCurUserId());
	tkmsreq.set_roleid(ComUtil::instance()->getCurUserRole());

	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_TICKETMS_LIST,tkmsreq.SerializeAsString().c_str(),tkmsreq.SerializeAsString().length());

	return;
}

void TicketMgr::retTicketMsionList(const char* msg)
{
	PBNS::TicketMsionListMsg_Response resp;
	resp.ParseFromString(msg);

	int nrow = resp.tktmsionlist_size();

	//设置行数
	m_missionTable->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::TicketMsionBean tkmsbean = resp.tktmsionlist(i);
		tkmsbean.userid();
		tkmsbean.actuserid();
		QString strid = QString("%1").arg(tkmsbean.id());
		QString struid = QString("%1").arg(tkmsbean.userid());
		QString strcatuid = QString("%1").arg(tkmsbean.actuserid());

		m_missionTable->setItem(i,0,new QTableWidgetItem(strid));
		m_missionTable->setItem(i,1,new QTableWidgetItem(struid));
		m_missionTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(tkmsbean.username())));
		m_missionTable->setItem(i,3,new QTableWidgetItem(strcatuid));	
		m_missionTable->setItem(i,4,new QTableWidgetItem(QString::fromStdString(tkmsbean.actusername())));	
		m_missionTable->setItem(i,5,new QTableWidgetItem(QString::fromStdString(tkmsbean.name())));
		m_missionTable->setItem(i,6,new QTableWidgetItem(QString::fromStdString(tkmsbean.publishtime())));

	}

	//调管理员、度员角色有效
	if (ComUtil::instance()->getCurUserRole() == eDispatcher || ComUtil::instance()->getCurUserRole() == eManager)
	{
		//未选中列表时只可增加用户,增加按钮有效，删除和修改按钮无效
		m_addBtn->setEnabled(true);
		m_delBtu->setEnabled(false);
		m_mdfBtn->setEnabled(false);
	}
	
}


void TicketMgr::reqTicketList()
{
	PBNS::TicketListMsg_Request tkreq;

	tkreq.set_reqtype(1);
	tkreq.set_reqid(ComUtil::instance()->getCurUserId());

	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_TICKET_LIST,tkreq.SerializeAsString().c_str(),tkreq.SerializeAsString().length());
}

void TicketMgr::retTicketList(const char* msg)
{
	PBNS::TicketListMsg_Response resp;
	resp.ParseFromString(msg);

	int nrow = resp.ticketlist_size();

	//设置行数
	m_ticketTable->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::TicketBean tkbean = resp.ticketlist(i);

		QString strid = QString("%1").arg(tkbean.id());
		QString struid = QString("%1").arg(tkbean.cuserid());
		QString strcatuid = QString("%1").arg(tkbean.auserid());
		QString strmsionid = QString("%1").arg(tkbean.missionid());

		m_ticketTable->setItem(i,0,new QTableWidgetItem(strid));
		m_ticketTable->setItem(i,1,new QTableWidgetItem(struid));
		m_ticketTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(tkbean.cusername())));
		m_ticketTable->setItem(i,3,new QTableWidgetItem(strcatuid));	
		m_ticketTable->setItem(i,4,new QTableWidgetItem(QString::fromStdString(tkbean.ausername())));	
		m_ticketTable->setItem(i,5,new QTableWidgetItem(strmsionid));
		m_ticketTable->setItem(i,6,new QTableWidgetItem(QString::fromStdString(tkbean.missionname())));
		m_ticketTable->setItem(i,7,new QTableWidgetItem(QString::fromStdString(tkbean.no())));
		m_ticketTable->setItem(i,8,new QTableWidgetItem(QString::fromStdString(tkbean.actiontype())));
		m_ticketTable->setItem(i,9,new QTableWidgetItem(QString::fromStdString(tkbean.info())));
		m_ticketTable->setItem(i,10,new QTableWidgetItem(QString::fromStdString(tkbean.actionperson())));
		m_ticketTable->setItem(i,11,new QTableWidgetItem(QString::fromStdString(tkbean.protectperson())));
		m_ticketTable->setItem(i,12,new QTableWidgetItem(QString::fromStdString(tkbean.chargeperson())));
		m_ticketTable->setItem(i,13,new QTableWidgetItem(QString::fromStdString(tkbean.starttime())));
		m_ticketTable->setItem(i,14,new QTableWidgetItem(QString::fromStdString(tkbean.endtime())));
		m_ticketTable->setItem(i,15,new QTableWidgetItem(QString::fromStdString(tkbean.publishtime())));
	}

}

void TicketMgr::getTicketMsionItem(QTableWidgetItem* item)
{
	int itemrow = item->row();

	//调管理员、度员角色有效
	if (ComUtil::instance()->getCurUserRole() == eDispatcher || ComUtil::instance()->getCurUserRole() == eManager)
	{
		int nActusrid = m_missionTable->item(itemrow,3)->text().toInt();

		for (int i=0;i<m_userBox->count();i++)
		{
			if (nActusrid == m_userBox->itemData(i).toInt())
			{
				m_userBox->setCurrentIndex(i);
			}
		}


		m_nameLied->setText(m_missionTable->item(itemrow,5)->text());

		QString crteTime = m_missionTable->item(itemrow,6)->text();
		QDateTime time;
		time = QDateTime::fromString(crteTime, "yyyy-MM-dd hh:mm:ss");
		m_crateTime->setDateTime(time);

		m_delBtu->setEnabled(true);
		m_mdfBtn->setEnabled(true);
	}


	//运维角色有效
	if (ComUtil::instance()->getCurUserRole() == eMaintainers)
	{
		//已经提交状态时相应点击更新数据，否则不更新数据
		if (m_bcomitflag)
		{
			//设置创建操作票按钮可操作
			m_createBtn->setEnabled(true);

			//操作任务ID
			m_ticketMsion.id = m_missionTable->item(itemrow,0)->text().toInt();
			m_ticketMsion.userid = m_missionTable->item(itemrow,1)->text().toInt();
			m_ticketMsion.username = m_missionTable->item(itemrow,2)->text();
			m_ticketMsion.actuserid = m_missionTable->item(itemrow,3)->text().toInt();
			m_ticketMsion.actusername = m_missionTable->item(itemrow,4)->text();
			m_ticketMsion.name = m_missionTable->item(itemrow,5)->text();
			m_ticketMsion.publishtime = m_missionTable->item(itemrow,6)->text();

		}
		else
		{
			return;
		}
	}
	
}

void TicketMgr::getTicketItem(QTableWidgetItem* item)
{
	int itemrow = item->row();

	m_ticket.id = m_ticketTable->item(itemrow,0)->text().toInt();
	m_ticket.CuserId = m_ticketTable->item(itemrow,1)->text().toInt();
	m_ticket.CuserName = m_ticketTable->item(itemrow,2)->text();
	m_ticket.AuserId = m_ticketTable->item(itemrow,3)->text().toInt();
	m_ticket.AuserName = m_ticketTable->item(itemrow,4)->text();
	m_ticket.MissionId = m_ticketTable->item(itemrow,5)->text().toInt();
	m_ticket.MissionName = m_ticketTable->item(itemrow,6)->text();
	m_ticket.No = m_ticketTable->item(itemrow,7)->text();
	m_ticket.ActionType = m_ticketTable->item(itemrow,8)->text();
	m_ticket.info = m_ticketTable->item(itemrow,9)->text();
	m_ticket.ActionPerson = m_ticketTable->item(itemrow,10)->text();
	m_ticket.ProtectPerson = m_ticketTable->item(itemrow,11)->text();
	m_ticket.ChargePerson = m_ticketTable->item(itemrow,12)->text();
	m_ticket.StartTime = m_ticketTable->item(itemrow,13)->text();
	m_ticket.EndTime = m_ticketTable->item(itemrow,14)->text();
	m_ticket.PublishTime = m_ticketTable->item(itemrow,15)->text();

	m_queryBtn->setEnabled(true);

}

void TicketMgr::setUserCombox()
{
	m_userBox->clear();

	m_userList = ComUtil::instance()->getUserList();

	for (int i=0;i<m_userList.size();i++)
	{
		UserInfo_S userinfo = m_userList.at(i);

		QString strid = QString("%1").arg(userinfo.userid);
		QString strname = userinfo.username;

		m_userBox->insertItem(i,strname,userinfo.userid);
	}
}


void TicketMgr::onMdf()
{
	if (!bmdfflag)
	{
		m_userBox->setDisabled(false);
		m_nameLied->setDisabled(false);
		m_crateTime->setDisabled(false);

		//设置操作按钮状态
		m_addBtn->setEnabled(false);
		m_delBtu->setEnabled(false);

		bmdfflag = true;
	} 
	else
	{
		if (m_nameLied->text()=="")
		{
			QMessageBox::information(this,tr("增加操作票任务提示:"),tr("操作票任务名不可为空称,请输入操作票任务名称!"));
			return;
		}

		if (m_userBox->currentText()=="")
		{
			QMessageBox::information(this,tr("增加操作票任务提示:"),tr("执行人不可为空,请选择执行人!"));
			return;
		}


		int ncurrow = m_missionTable->currentRow();
		//取操作票任务列表中选中行的第一列操作票任务id号
		int nTktMsid = m_missionTable->item(ncurrow,0)->text().toInt();

		//取操作票任务列表中选中行的第四列操作票任务执行人ID
		int nActUserid = m_missionTable->item(ncurrow,3)->text().toInt();

		//取当前设置的创建时间
		QString strCrtTime = m_crateTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");  

		//UPDATE ticket_missions SET ActionUserId=13 , Name='操作票任务测试11' , PublishTime='2016-07-10 17:20:33' where ID=1
		QString sql = QString("UPDATE ticket_missions SET ActionUserId=%1 , Name='%2' , PublishTime='%3' where ID=%4 ;").arg(nActUserid).arg(m_nameLied->text()).arg(strCrtTime).arg(nTktMsid);

		PBNS::TicketMgrMsg_Request mdfreq;
		mdfreq.set_mgrsql(sql.toStdString());

		string sdata = mdfreq.SerializeAsString();
		NetClient::instance()->sendData(CMD_TICKETMS_MODIFY,sdata.c_str(),sdata.length());

		bmdfflag = false;
		m_userBox->setDisabled(true);
		m_nameLied->setDisabled(true);
		m_crateTime->setDisabled(true);

		//修改操作票任务完成后,请求操作票任务列表
		reqTicketMsionList();

	}
}

void TicketMgr::onAdd()
{
	if (!baddflag)
	{
		m_nameLied->clear();
		m_crateTime->clear();

		m_crateTime->setDateTime(QDateTime::currentDateTime());
		m_userBox->setCurrentIndex(0);

		m_nameLied->setDisabled(false);
		m_crateTime->setDisabled(false);
		m_userBox->setDisabled(false);

		//设置操作按钮状态
		m_delBtu->setEnabled(false);
		m_mdfBtn->setEnabled(false);

		baddflag = true;
	}
	else
	{
		if (m_nameLied->text()=="")
		{
			QMessageBox::information(this,tr("增加操作票任务提示:"),tr("操作票任务名不可为空称,请输入操作票任务名称!"));
			return;
		}

		if (m_userBox->currentText()=="")
		{
			QMessageBox::information(this,tr("增加操作票任务提示:"),tr("执行人不可为空,请选择执行人!"));
			return;
		}

		int nActUserid = m_userBox->itemData(m_userBox->currentIndex()).toInt();

		//取当前设置的创建时间
		QString strCrtTime = m_crateTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
		//获取当前登录用户Id
		int nUserid = ComUtil::instance()->getCurUserId();
		
		QString sql = QString("insert into ticket_missions (UserId,ActionUserId,Name,PublishTime) VALUES ('%1','%2','%3','%4');").arg(nUserid).arg(nActUserid).arg(m_nameLied->text()).arg(strCrtTime);

		PBNS::TicketMgrMsg_Request addreq;
		addreq.set_mgrsql(sql.toStdString());

		string sdata = addreq.SerializeAsString();

		NetClient::instance()->sendData(CMD_TICKETMS_ADD,sdata.c_str(),sdata.length());

		baddflag = false;
		m_nameLied->setDisabled(true);
		m_crateTime->setDisabled(true);
		m_userBox->setDisabled(true);

		//增加操作票任务完成后,请求操作票任务列表
		reqTicketMsionList();
	}
}

void TicketMgr::onDel()
{
	int ncurrow = m_missionTable->currentRow();

	//取操作票任务列表中选中行的第一列操作票任务id号
	int nTktMsid = m_missionTable->item(ncurrow,0)->text().toInt();

	QString sql = QString("delete from ticket_missions where id=%1").arg(nTktMsid);

	PBNS::TicketMgrMsg_Request delreq;
	delreq.set_mgrsql(sql.toStdString());

	string sdata = delreq.SerializeAsString();

	NetClient::instance()->sendData(CMD_TICKETMS_DEL,sdata.c_str(),sdata.length());

	//设置操作按钮状态
	m_delBtu->setEnabled(false);
	m_mdfBtn->setEnabled(false);

	//删除操作票任务完成后,请求操作票任务列表
	reqTicketMsionList();
}


void TicketMgr::initData()
{
	if (m_ticketTable != NULL)
	{
		m_ticketTable->setColumnCount(16);
		m_ticketTable->resizeColumnsToContents();
		QStringList tickets ;
		//tickets<<"编号"<<"操作类型"<<"操作人"<<"监护人"<<"值班负责人"<<"操作开始时间"<<"操作结束时间"<<"备注";
		tickets<<"操作票ID"<<"创建人ID"<<"创建人"<<"执行人ID"<<"执行人"<<"操作任务ID"<<"任务名称"<<"编号"<<"操作类型"<<"备注"<<"操作人"<<"监护人"<<"值班负责人"<<"操作开始时间"<<"操作结束时间"<<"发布时间";
		m_ticketTable->setHorizontalHeaderLabels(tickets);
		//使行列头自适应宽度，所有列平均分来填充空白部分
		m_ticketTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
		//行背景颜色变化
		m_ticketTable->setAlternatingRowColors(true);
		//设置每行内容不可编辑
		m_ticketTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		//设置只能选择一行，不能选择多行
		m_ticketTable->setSelectionMode(QAbstractItemView::SingleSelection);
		//设置单击选择一行
		m_ticketTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		
		//隐藏列
		m_ticketTable->setColumnHidden(0,true);
		m_ticketTable->setColumnHidden(1,true);
		m_ticketTable->setColumnHidden(3,true);
		m_ticketTable->setColumnHidden(5,true);
		m_ticketTable->setColumnHidden(8,true);
		m_ticketTable->setColumnHidden(10,true);
		m_ticketTable->setColumnHidden(11,true);
		m_ticketTable->setColumnHidden(12,true);
		m_ticketTable->setColumnHidden(13,true);
		m_ticketTable->setColumnHidden(14,true);

		//点击列表某行信号槽关联
		connect(m_ticketTable,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getTicketItem(QTableWidgetItem*)));

		reqTicketList();
	}
	

	if (m_missionTable != NULL)
	{
		//管理员或调度员角色
		if (ComUtil::instance()->getCurUserRole()==eManager || ComUtil::instance()->getCurUserRole()==eDispatcher )
		{
			m_missionTable->setColumnCount(7);
			m_missionTable->resizeColumnsToContents();
			QStringList missions;
			missions<<"操作任务ID"<<"创建人ID"<<"创建人"<<"执行人ID"<<"执行人"<<"任务名称"<<"发布时间";
			m_missionTable->setHorizontalHeaderLabels(missions);
			//使行列头自适应宽度，所有列平均分来填充空白部分
			m_missionTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
			//行背景颜色变化
			m_missionTable->setAlternatingRowColors(true);
			//设置每行内容不可编辑
			m_missionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			//设置只能选择一行，不能选择多行
			m_missionTable->setSelectionMode(QAbstractItemView::SingleSelection);
			//设置单击选择一行
			m_missionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

			//隐藏第一二四列
			m_missionTable->setColumnHidden(0,true);
			m_missionTable->setColumnHidden(1,true);
			m_missionTable->setColumnHidden(3,true);

			setUserCombox();
			reqTicketMsionList();
		}

		//运维角色
		if (ComUtil::instance()->getCurUserRole()==eMaintainers)
		{
			m_missionTable->setColumnCount(7);
			m_missionTable->resizeColumnsToContents();
			QStringList missions;
			//missions<<"执行人"<<"发令时间";
			missions<<"操作任务ID"<<"创建人ID"<<"创建人"<<"执行人ID"<<"执行人"<<"任务名称"<<"发布时间";
			m_missionTable->setHorizontalHeaderLabels(missions);
			//使行列头自适应宽度，所有列平均分来填充空白部分
			m_missionTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
			//行背景颜色变化
			m_missionTable->setAlternatingRowColors(true);
			//设置每行内容不可编辑
			m_missionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			//设置只能选择一行，不能选择多行
			m_missionTable->setSelectionMode(QAbstractItemView::SingleSelection);
			//设置单击选择一行
			m_missionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

			//隐藏第一二四列
			m_missionTable->setColumnHidden(0,true);
			m_missionTable->setColumnHidden(1,true);
			m_missionTable->setColumnHidden(3,true);

			//点击列表某行信号槽关联
			connect(m_missionTable,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getTicketMsionItem(QTableWidgetItem*)));

			connect(m_createBtn,SIGNAL(pressed()),this,SLOT(onCreateActions()));
			connect(m_comitBtn,SIGNAL(pressed()),this,SLOT(onCommitActions()));
			connect(m_queryBtn,SIGNAL(pressed()),this,SLOT(onQueryActions()));

			m_createBtn->setEnabled(false);
			m_comitBtn->setEnabled(false);
			m_queryBtn->setEnabled(false);

			//已经提交状态
			m_bcomitflag = true;

			reqTicketMsionList();

		}
	}

	if (m_crateTime != NULL)
	{
		m_crateTime->setDateTime(QDateTime::currentDateTime());
	}

}


void TicketMgr::onCreateActions()
{
	//已经创建，直接返回
	if (m_tktActmgr != NULL)
	{
		return;
	}

	m_tktActmgr = new TicketActionsMgr(this);
	m_tktActmgr->setWindowTitle(tr("操作票"));

	m_tktActmgr->setTicketMsion(m_ticketMsion);
	m_tktActmgr->initDataByTicketMsion();

	m_tktActmgr->setModal(false);
	m_tktActmgr->show();

	m_bcomitflag = false;
	m_createBtn->setEnabled(false);
	m_comitBtn->setEnabled(true);

	//delete m_tktActmgr;

}


void TicketMgr::onCommitActions()
{
	if (m_tktActmgr)
	{
		m_ticket = m_tktActmgr->getTicketInfo();		

		QString sql = QString(tr("insert into tickets (UserId,MissionId,No,ActionType,info,ActionPerson,ProtectPerson,ChargePerson,StartTime,EndTime) VALUES (%1,%2,'%3','%4','%5','%6','%7','%8','%9','%10');")).arg(m_ticket.AuserId).arg(m_ticket.MissionId).arg(m_ticket.No).arg(m_ticket.ActionType).arg(m_ticket.info).arg(m_ticket.ActionPerson).arg(m_ticket.ProtectPerson).arg(m_ticket.ChargePerson).arg(m_ticket.StartTime).arg(m_ticket.EndTime);

		PBNS::TicketMgrMsg_Request req;
		req.set_mgrsql(sql.toStdString());

		//发射发送数据请求消息信号
		NetClient::instance()->sendData(CMD_TICKET_COMMIT,req.SerializeAsString().c_str(),req.SerializeAsString().length());


		m_bcomitflag = true;
		m_comitBtn->setEnabled(false);
	}
	
}

void TicketMgr::onQueryActions()
{
	//已经创建，直接返回
	if (m_tktActmgr != NULL)
	{
		return;
	}

	m_tktActmgr = new TicketActionsMgr(this);
	m_tktActmgr->setWindowTitle(tr("操作票"));

	m_tktActmgr->setTicket(m_ticket);
	m_tktActmgr->initDataByTicket();

	m_tktActmgr->setModal(false);
	m_tktActmgr->show();

	m_bcomitflag = false;
	m_queryBtn->setEnabled(false);
	//m_comitBtn->setEnabled(true);
}


/************************************************************************/
/*                           操作票明细                                 */
/************************************************************************/
TicketActionsMgr::TicketActionsMgr(QWidget* parent /* = NULL */)
{
	setAttribute (Qt::WA_DeleteOnClose);
	m_viewType = 1;

	initUi_Operator();

}


void TicketActionsMgr::initUi_Operator()
{
	//只显示关闭按钮
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->resize(680,780);

	QHBoxLayout *hbox1 = new QHBoxLayout;
	QHBoxLayout *hbox2 = new QHBoxLayout;
	QHBoxLayout *hbox3 = new QHBoxLayout;
	QHBoxLayout *hbox4 = new QHBoxLayout;
	QHBoxLayout *hbox5 = new QHBoxLayout;
	QHBoxLayout *hbox6 = new QHBoxLayout;
	QHBoxLayout *hbox7 = new QHBoxLayout;
	QHBoxLayout *hbox8 = new QHBoxLayout;
	QHBoxLayout *hbox9 = new QHBoxLayout;
	QVBoxLayout *pmainvlyt = new QVBoxLayout;

	QLabel *companylab = new QLabel(tr("单位:"));
	companylab->setFixedSize(30,25);
	QLabel *nolab = new QLabel(tr("编号:"));
	nolab->setFixedSize(30,25);
	m_companyLied = new QLineEdit;
	m_companyLied->setFixedSize(300,25);
	m_noLied = new QLineEdit;
	m_noLied->setFixedSize(230,25);
	hbox1->addStretch();
	hbox1->addWidget(companylab);
	hbox1->addWidget(m_companyLied);
	hbox1->addSpacing(50);
	hbox1->addWidget(nolab);
	hbox1->addWidget(m_noLied);
	hbox1->addStretch();

	QLabel *crtlab = new QLabel(tr("发令人:"));
	crtlab->setFixedSize(40,25);
	QLabel *actlab = new QLabel(tr("受令人:"));
	actlab->setFixedSize(40,25);
	QLabel *crttimelab = new QLabel(tr("    发令时间:"));
	crttimelab->setFixedSize(80,25);
	m_createLied = new QLineEdit;
	m_createLied->setFixedSize(100,25);
	m_actLied = new QLineEdit;
	m_actLied->setFixedSize(100,25);
	m_createrTime = new QDateTimeEdit;
	m_createrTime->setFixedSize(180,25);
	m_createrTime->setCalendarPopup(true);
	hbox2->addWidget(crtlab);
	hbox2->addWidget(m_createLied);
	hbox2->addSpacing(20);
	hbox2->addWidget(actlab);
	hbox2->addWidget(m_actLied);
	hbox2->addSpacing(50);
	hbox2->addStretch();
	hbox2->addWidget(crttimelab);
	hbox2->addWidget(m_createrTime);


	QLabel *oprstalab = new QLabel(tr("操作开始时间:"));
	oprstalab->setFixedSize(80,25);
	QLabel *oprendlab = new QLabel(tr("操作结束时间:"));
	oprendlab->setFixedSize(80,25);
	m_opstartTime = new QDateTimeEdit;
	m_opstartTime->setFixedSize(180,25);
	m_opstartTime->setCalendarPopup(true);
	m_opendTime = new QDateTimeEdit;
	m_opendTime->setFixedSize(180,25);
	m_opendTime->setCalendarPopup(true);
	hbox3->addWidget(oprstalab);
	hbox3->addWidget(m_opstartTime);
	hbox3->addStretch();
	hbox3->addWidget(oprendlab);
	hbox3->addWidget(m_opendTime);


	QLabel *l1lab = new QLabel(tr("( "));
	QLabel *l2lab = new QLabel(tr("( "));
	QLabel *l3lab = new QLabel(tr("( "));
	QLabel *r1lab = new QLabel(tr(" )"));
	QLabel *r2lab = new QLabel(tr(" )"));
	QLabel *r3lab = new QLabel(tr(" )"));
	QLabel *gndlab = new QLabel(tr("监护下操作"));
	QLabel *onelab = new QLabel(tr("单人操作"));
	QLabel *ohlab = new QLabel(tr("检修人员操作"));
	m_guardianChbox = new QCheckBox;
	m_onerChbox = new QCheckBox;
	m_overhaulChbox = new QCheckBox;
	hbox4->addStretch();
	hbox4->addWidget(l1lab);
	hbox4->addWidget(m_guardianChbox);
	hbox4->addWidget(r1lab);
	hbox4->addWidget(gndlab);
	hbox4->addStretch();
	hbox4->addWidget(l2lab);
	hbox4->addWidget(m_onerChbox);
	hbox4->addWidget(r2lab);
	hbox4->addWidget(onelab);
	hbox4->addStretch();
	hbox4->addWidget(l3lab);
	hbox4->addWidget(m_overhaulChbox);
	hbox4->addWidget(r3lab);
	hbox4->addWidget(ohlab);
	hbox4->addStretch();

	QLabel *mslab = new QLabel(tr("操作任务"));
	mslab->setFixedSize(50,25);
	m_missionLied = new QLineEdit;
	m_missionLied->setFixedSize(600,25);
	hbox5->addWidget(mslab);
	hbox5->addWidget(m_missionLied);
	hbox5->addStretch();


	m_ticketActTable = new QTableWidget();
	//设置背景色为白色
	QPalette tabplt = m_ticketActTable->palette();
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	m_ticketActTable->setPalette(tabplt);

	//使行列头自适应宽度，所有列平均分来填充空白部分
	//ptabwdgStaList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//行背景颜色变化
	m_ticketActTable->setAlternatingRowColors(true);
	//设置每行内容不可编辑
	m_ticketActTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置只能选择一行，不能选择多行
	m_ticketActTable->setSelectionMode(QAbstractItemView::SingleSelection);
	//设置单击选择一行
	m_ticketActTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	hbox6->addWidget(m_ticketActTable);


	QLabel *remklab = new QLabel(tr("备注:"));
	hbox7->addWidget(remklab);
	hbox7->addStretch();

	m_remarksLied = new QLineEdit;
	m_remarksLied->setFixedSize(650,75);
	hbox8->addWidget(m_remarksLied);


	QLabel *oprlab = new QLabel(tr("操作人:"));
	QLabel *gdlab = new QLabel(tr("监护人:"));
	QLabel *ohalab = new QLabel(tr("值班负责人:"));
	m_operatorLied = new QLineEdit;
	m_operatorLied->setFixedSize(100,25);
	m_guardianLied = new QLineEdit;
	m_guardianLied->setFixedSize(100,25);
	m_watcherLied = new QLineEdit;
	m_watcherLied->setFixedSize(100,25);
	hbox9->addStretch();
	hbox9->addWidget(oprlab);
	hbox9->addWidget(m_operatorLied);
	hbox9->addStretch();
	hbox9->addWidget(gdlab);
	hbox9->addWidget(m_guardianLied);
	hbox9->addStretch();
	hbox9->addWidget(ohalab);
	hbox9->addWidget(m_watcherLied);
	hbox9->addStretch();


	pmainvlyt->addLayout(hbox1);
	pmainvlyt->addLayout(hbox2);
	pmainvlyt->addLayout(hbox3);
	pmainvlyt->addLayout(hbox4);
	pmainvlyt->addLayout(hbox5);
	pmainvlyt->addLayout(hbox6);
	pmainvlyt->addLayout(hbox7);
	pmainvlyt->addLayout(hbox8);
	pmainvlyt->addLayout(hbox9);

	pmainvlyt->setContentsMargins(8,8,8,15);

	this->setLayout(pmainvlyt);

	m_createLied->setEnabled(false);
	m_actLied->setEnabled(false);
	m_createrTime->setEnabled(false);
	m_missionLied->setEnabled(false);
}


void TicketActionsMgr::initDataByTicketMsion()
{
	//设置公司名称:每个地市固定，在配置文件中配置
	m_companyLied->setText(ComUtil::instance()->getCompanyName());
	//操作任务ID
	m_Msionid = m_tketMsion.id;
	//发令人
	m_createLied->setText(m_tketMsion.username);
	//受令人
	m_actLied->setText(m_tketMsion.actusername);
	//发令时间
	QString crteTime = m_tketMsion.publishtime;
	QDateTime time;
	time = QDateTime::fromString(crteTime, "yyyy-MM-dd hh:mm:ss");
	m_createrTime->setDateTime(time);
	//操作开始时间
	m_opstartTime->setDateTime(QDateTime::currentDateTime());
	//操作结束时间
	m_opendTime->setDateTime(QDateTime::currentDateTime());
	//操作任务
	m_missionLied->setText(m_tketMsion.name);

	//设置列数
	m_ticketActTable->setColumnCount(6);
	QStringList header;
	header<<"操作票ID"<<"顺序"<<"操作项目(系统)"<<"操作项目(描述)"<<"√"<<"时间";
	m_ticketActTable->setHorizontalHeaderLabels(header);

}

void TicketActionsMgr::initDataByTicket()
{
	//设置公司名称:每个地市固定，在配置文件中配置
	m_companyLied->setText(ComUtil::instance()->getCompanyName());
	//编号
	m_noLied->setText(m_tket.No);
	//操作任务ID
	m_Msionid = m_tket.MissionId;
	//发令人
	m_createLied->setText(m_tket.CuserName);
	//受令人
	m_actLied->setText(m_tket.AuserName);
	//发令时间
	QString crteTime = m_tket.PublishTime;
	QDateTime time;
	time = QDateTime::fromString(crteTime, "yyyy-MM-dd hh:mm:ss");
	m_createrTime->setDateTime(time);
	//操作开始时间
	m_opstartTime->setDateTime(QDateTime::fromString(m_tket.StartTime, "yyyy-MM-dd hh:mm:ss"));
	//操作结束时间
	m_opendTime->setDateTime(QDateTime::fromString(m_tket.EndTime, "yyyy-MM-dd hh:mm:ss"));
	//操作类型
	setActionsType();
	//操作任务
	m_missionLied->setText(m_tket.MissionName);
	//备注
	m_remarksLied->setText(m_tket.info);
	//操作人
	m_operatorLied->setText(m_tket.ActionPerson);
	//监护人
	m_guardianLied->setText(m_tket.ProtectPerson);
	//值班负责人
	m_watcherLied->setText(m_tket.ChargePerson);

	//设置列数
	m_ticketActTable->setColumnCount(5);
	QStringList header;
	header<<"操作票ID"<<"顺序"<<"操作项目"<<"√"<<"时间";
	m_ticketActTable->setHorizontalHeaderLabels(header);

	m_companyLied->setEnabled(false);
	m_guardianChbox->setEnabled(false);
	m_onerChbox->setEnabled(false);
	m_overhaulChbox->setEnabled(false);
	m_operatorLied->setEnabled(false);
	m_guardianLied->setEnabled(false);
	m_watcherLied->setEnabled(false);

	//请求查询操作票明细列表
	reqTicketActionsList();

}

void TicketActionsMgr::reqTicketActionsList()
{
	PBNS::TicketActListMsg_Request req;
	req.set_reqid(m_tket.id);

	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_TICKET_QUERY,req.SerializeAsString().c_str(),req.SerializeAsString().length());
}

void TicketActionsMgr::retTicketActionsList(const char* msg)
{
	PBNS::TicketActListMsg_Response resp;
	resp.ParseFromString(msg);

	int nrow = resp.ticketactlist_size();

	//设置行数
	m_ticketActTable->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::TicketActBean tkacbean = resp.ticketactlist(i);

		QString strTicketid = QString(tr("%1")).arg(tkacbean.ticketid());
		QString strOrderNum = QString(tr("%1")).arg(tkacbean.ordernum());

		QString strInfo = QString(tr("%1%2")).arg(QString::fromStdString(tkacbean.systemcontent())).arg(QString::fromStdString(tkacbean.content()));
		QString strGou = QString(tr("√"));

		m_ticketActTable->setItem(i,0,new QTableWidgetItem(strTicketid));
		m_ticketActTable->setItem(i,1,new QTableWidgetItem(strOrderNum));
		m_ticketActTable->setItem(i,2,new QTableWidgetItem(strInfo));
		m_ticketActTable->setItem(i,3,new QTableWidgetItem(strGou));	

	}
}

void TicketActionsMgr::setActionsType()
{
	QString strType = m_tket.ActionType;

	if (strType.length()>0)
	{
		//监护人操作类型
		QString strGuandian = strType.left(1);
		int nGuandian = atoi(strGuandian.toStdString().c_str());
		if (nGuandian == 1)
		{
			m_guardianChbox->setChecked(true);
		}
		else
		{
			m_guardianChbox->setChecked(false);
		}

		//单人操作类型
		QString strOner = strType.mid(1,1);
		int nOner = atoi(strOner.toStdString().c_str());
		if (nOner == 1)
		{
			m_onerChbox->setChecked(true);
		}
		else
		{
			m_onerChbox->setChecked(false);
		}

		//检修人员操作类型
		QString strOverhaul = strType.right(1);
		int nOverhaul = atoi(strOverhaul.toStdString().c_str());
		if (nOverhaul == 1)
		{
			m_overhaulChbox->setChecked(true);
		}
		else
		{
			m_overhaulChbox->setChecked(false);
		}
	}
}

QString TicketActionsMgr::getActionsType()
{
	if (m_guardianChbox->checkState() == Qt::Checked)
	{
		m_strGuandian = QString(tr("1"));
	}
	else
	{
		m_strGuandian = QString(tr("0"));
	}

	if (m_onerChbox->checkState() == Qt::Checked)
	{
		m_strOner = QString(tr("1"));
	}
	else
	{
		m_strOner = QString(tr("0"));
	}

	if (m_overhaulChbox->checkState() == Qt::Checked)
	{
		m_strOverhaul = QString(tr("1"));
	}
	else
	{
		m_strOverhaul = QString(tr("0"));
	}

	m_strActType = QString(tr("%1%2%3")).arg(m_strGuandian).arg(m_strOner).arg(m_strOverhaul);

	return m_strActType;
}

Ticket_S TicketActionsMgr::getTicketInfo()
{
	m_tketReturn.id = 0;

	m_tketReturn.CuserId = m_tketMsion.userid;
	m_tketReturn.CuserName = m_tketMsion.username;
	m_tketReturn.AuserId = m_tketMsion.actuserid;
	m_tketReturn.AuserName = m_tketMsion.actusername;
	m_tketReturn.MissionId = m_Msionid;
	m_tketReturn.MissionName =m_tketMsion.name;
	m_tketReturn.No = m_noLied->text();
	m_tketReturn.ActionType = getActionsType();
	m_tketReturn.info = m_remarksLied->text();
	m_tketReturn.ActionPerson = m_operatorLied->text();
	m_tketReturn.ProtectPerson = m_guardianLied->text();
	m_tketReturn.ChargePerson = m_watcherLied->text();
	m_tketReturn.StartTime = m_opstartTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");
	m_tketReturn.EndTime = m_opendTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");
	m_tketReturn.PublishTime = m_createrTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");

	return m_tketReturn;
}














