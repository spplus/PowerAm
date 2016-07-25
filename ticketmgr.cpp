#include "ticketmgr.h"
//#include "comutil.h"
//#include "define.h"

TicketMgr::TicketMgr(QWidget* parent /* = NULL */)
{
	//����¼��ɫΪ��ά��Աʱ,�Ի����Ƿ�ģ̬�ģ����ùرմ����Զ��ͷ�
	if (ComUtil::instance()->getCurUserRole() == eMaintainers)
	{
		setAttribute (Qt::WA_DeleteOnClose);
	}

	m_ticketTable = NULL;
	m_missionTable = NULL;
	m_crateTime = NULL;
	m_tktActmgr = NULL;
	m_bdlgflag = false;
	m_bcloseflag = true;

	switch (ComUtil::instance()->getCurUserRole())
	{
	case eManager://����Ա
		initUi_Mission();
		break;
	case eDispatcher://����
		initUi_Mission();
		break;
	case eAutomater://�Զ���
		break;
	case eMaintainers://��ά
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
	//ֻ��ʾ�رհ�ť
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	QVBoxLayout*		vbox = new QVBoxLayout;
	QGroupBox* gbox = new QGroupBox;
	gbox->setTitle(tr("��������"));
	m_missionTable = new QTableWidget;
	QHBoxLayout* hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_missionTable);
	gbox->setLayout(hbox0);
	//ʹ����ͷ����Ӧ��ȣ�������ƽ���������հײ���
	m_missionTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//�б�����ɫ�仯
	m_missionTable->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	m_missionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	m_missionTable->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	m_missionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

	QHBoxLayout* hbox1 = new QHBoxLayout;
	QLabel* doUser = new QLabel(tr("ִ����"));
	m_userBox = new QComboBox;
	QLabel* doName = new QLabel(tr("������������"));
	m_nameLied = new QLineEdit;
	QLabel* doTime = new QLabel(tr("����ʱ��"));
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
	m_addBtn = new QPushButton(tr("����"));
	m_mdfBtn = new QPushButton(tr("�޸�"));
	m_delBtu = new QPushButton(tr("ɾ��"));

	hbox2->addWidget(m_addBtn);
	hbox2->addWidget(m_delBtu);
	hbox2->addWidget(m_mdfBtn);

	vbox->addWidget(gbox);
	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2);

	connect(m_mdfBtn,SIGNAL(pressed()),this,SLOT(onMdf()));
	connect(m_addBtn,SIGNAL(pressed()),this,SLOT(onAdd()));
	connect(m_delBtu,SIGNAL(pressed()),this,SLOT(onDel()));

	//����б�ĳ���źŲ۹���
	connect(m_missionTable,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getTicketMsionItem(QTableWidgetItem*)));

	//Ĭ�������û���,����,��ɫ,��ʵ������Ϊֻ��
	m_userBox->setDisabled(true);
	m_nameLied->setDisabled(true);
	m_crateTime->setDisabled(true);

	//Ĭ�����ò�����ť״̬��δѡ���û��б�ʱֻ�������û�,���Ӱ�ť��Ч��ɾ�����޸İ�ť��Ч
	m_addBtn->setEnabled(true);
	m_delBtu->setEnabled(false);
	m_mdfBtn->setEnabled(false);

	//Ĭ������û���־λfalse
	baddflag = false;
	bmdfflag = false;

	setLayout(vbox);
}

void TicketMgr::initUi_Action()
{
	//ֻ��ʾ�رհ�ť
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->setFixedWidth(680);
	//this->resize(650,430);

	QVBoxLayout*		vbox = new QVBoxLayout;
	QGroupBox* gbox = new QGroupBox;
	gbox->setTitle(tr("��������"));
	m_missionTable = new QTableWidget;
	QHBoxLayout* hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_missionTable);
	gbox->setLayout(hbox0);

	QHBoxLayout* hbox1 = new QHBoxLayout;
	
	QGroupBox* gbox2 = new QGroupBox;
	gbox2->setTitle(tr("����Ʊ"));
	m_ticketTable = new QTableWidget;
	hbox1->addWidget(m_ticketTable);
	gbox2->setLayout(hbox1);
	//�б�����ɫ�仯
	m_ticketTable->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	m_ticketTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	m_ticketTable->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	m_ticketTable->setSelectionBehavior(QAbstractItemView::SelectRows);


	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_createBtn = new QPushButton(tr("��������Ʊ"));
	m_comitBtn = new QPushButton(tr("�� ��"));
	m_queryBtn = new QPushButton(tr(("��ѯ����Ʊ")));
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
		retTicketMsionList(msg,msglength);
		break;
	case CMD_TICKET_LIST:
		retTicketList(msg,msglength);
		break;
	case CMD_TICKET_COMMIT:
		{
			PBNS::TicketMgrMsg_Response resp;
			resp.ParseFromArray(msg,msglength);
			//�ύ�ɹ�ʱ�����²�ѯ����Ʊ�б�
			if (resp.rescode())
			{
				reqTicketList();
			}
		}
		break;
	case CMD_TICKET_QUERY:
		m_tktActmgr->instance()->retTicketActionsList(msg,msglength);
		//m_tktActmgr->retTicketActionsList(msg,msglength);
		break;
	case CMD_TICKET_CREATE:
		{
			PBNS::TicketActMgrMsg_Response resp;
			resp.ParseFromArray(msg,msglength);
			//�ύ�ɹ�ʱ�����²�ѯ����Ʊ�б�
			if (resp.rescode())
			{
				reqTicketList();
			}
		}
		break;
	}
}

void TicketMgr::reqTicketMsionList()
{
	//�ǵ�����Ա����Ա��ɫ����άԱ�������ѯ��������
	if (!(ComUtil::instance()->getCurUserRole() == eDispatcher || ComUtil::instance()->getCurUserRole() == eManager || ComUtil::instance()->getCurUserRole() == eMaintainers))
	{
		return;
	}

	PBNS::TicketMsionListMsg_Request tkmsreq;

	tkmsreq.set_userid(ComUtil::instance()->getCurUserId());
	tkmsreq.set_roleid(ComUtil::instance()->getCurUserRole());

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_TICKETMS_LIST,tkmsreq.SerializeAsString().c_str(),tkmsreq.SerializeAsString().length());

	return;
}

void TicketMgr::retTicketMsionList(const char* msg,int msglength)
{
	PBNS::TicketMsionListMsg_Response resp;
	resp.ParseFromArray(msg,msglength);

	int nrow = resp.tktmsionlist_size();

	//��������
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

	//������Ա����Ա��ɫ��Ч
	if (ComUtil::instance()->getCurUserRole() == eDispatcher || ComUtil::instance()->getCurUserRole() == eManager)
	{
		//δѡ���б�ʱֻ�������û�,���Ӱ�ť��Ч��ɾ�����޸İ�ť��Ч
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

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_TICKET_LIST,tkreq.SerializeAsString().c_str(),tkreq.SerializeAsString().length());
}

void TicketMgr::retTicketList(const char* msg,int msglength)
{
	PBNS::TicketListMsg_Response resp;
	resp.ParseFromArray(msg,msglength);

	int nrow = resp.ticketlist_size();

	//��������
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

	//������Ա����Ա��ɫ��Ч
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


	//��ά��ɫ��Ч
	if (ComUtil::instance()->getCurUserRole() == eMaintainers)
	{
		//�Ӵ���δ�رգ�ֱ�ӷ���
		if (!m_bcloseflag)
		{
			return;
		}

		//�Ѿ��ύ״̬ʱ��Ӧ����������ݣ����򲻸�������
		if (m_bcomitflag)
		{
			//���ô�������Ʊ��ť�ɲ���
			m_createBtn->setEnabled(true);

			//��������ID
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
	//�Ӵ���δ�رգ�ֱ�ӷ���
	if (!m_bcloseflag)
	{
		return;
	}

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

		//���ò�����ť״̬
		m_addBtn->setEnabled(false);
		m_delBtu->setEnabled(false);

		bmdfflag = true;
	} 
	else
	{
		if (m_nameLied->text()=="")
		{
			QMessageBox::information(this,tr("���Ӳ���Ʊ������ʾ:"),tr("����Ʊ����������Ϊ�ճ�,���������Ʊ��������!"));
			return;
		}

		if (m_userBox->currentText()=="")
		{
			QMessageBox::information(this,tr("���Ӳ���Ʊ������ʾ:"),tr("ִ���˲���Ϊ��,��ѡ��ִ����!"));
			return;
		}


		int ncurrow = m_missionTable->currentRow();
		//ȡ����Ʊ�����б���ѡ���еĵ�һ�в���Ʊ����id��
		int nTktMsid = m_missionTable->item(ncurrow,0)->text().toInt();

		//ȡ����Ʊ�����б���ѡ���еĵ����в���Ʊ����ִ����ID
		int nActUserid = m_userBox->itemData(m_userBox->currentIndex()).toInt();

		//ȡ��ǰ���õĴ���ʱ��
		QString strCrtTime = m_crateTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");  

		//UPDATE ticket_missions SET ActionUserId=13 , Name='����Ʊ�������11' , PublishTime='2016-07-10 17:20:33' where ID=1
		QString sql = QString("UPDATE ticket_missions SET ActionUserId=%1 , Name='%2' , PublishTime='%3' where ID=%4 ;").arg(nActUserid).arg(m_nameLied->text()).arg(strCrtTime).arg(nTktMsid);

		PBNS::TicketMgrMsg_Request mdfreq;
		mdfreq.set_mgrsql(sql.toStdString());

		string sdata = mdfreq.SerializeAsString();
		NetClient::instance()->sendData(CMD_TICKETMS_MODIFY,sdata.c_str(),sdata.length());

		bmdfflag = false;
		m_userBox->setDisabled(true);
		m_nameLied->setDisabled(true);
		m_crateTime->setDisabled(true);

		//�޸Ĳ���Ʊ������ɺ�,�������Ʊ�����б�
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

		//���ò�����ť״̬
		m_delBtu->setEnabled(false);
		m_mdfBtn->setEnabled(false);

		baddflag = true;
	}
	else
	{
		if (m_nameLied->text()=="")
		{
			QMessageBox::information(this,tr("���Ӳ���Ʊ������ʾ:"),tr("����Ʊ����������Ϊ�ճ�,���������Ʊ��������!"));
			return;
		}

		if (m_userBox->currentText()=="")
		{
			QMessageBox::information(this,tr("���Ӳ���Ʊ������ʾ:"),tr("ִ���˲���Ϊ��,��ѡ��ִ����!"));
			return;
		}

		int nActUserid = m_userBox->itemData(m_userBox->currentIndex()).toInt();

		//ȡ��ǰ���õĴ���ʱ��
		QString strCrtTime = m_crateTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
		//��ȡ��ǰ��¼�û�Id
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

		//���Ӳ���Ʊ������ɺ�,�������Ʊ�����б�
		reqTicketMsionList();
	}
}

void TicketMgr::onDel()
{
	int ncurrow = m_missionTable->currentRow();

	//ȡ����Ʊ�����б���ѡ���еĵ�һ�в���Ʊ����id��
	int nTktMsid = m_missionTable->item(ncurrow,0)->text().toInt();

	QString sql = QString("delete from ticket_missions where id=%1").arg(nTktMsid);

	PBNS::TicketMgrMsg_Request delreq;
	delreq.set_mgrsql(sql.toStdString());

	string sdata = delreq.SerializeAsString();

	NetClient::instance()->sendData(CMD_TICKETMS_DEL,sdata.c_str(),sdata.length());

	//���ò�����ť״̬
	m_delBtu->setEnabled(false);
	m_mdfBtn->setEnabled(false);

	//ɾ������Ʊ������ɺ�,�������Ʊ�����б�
	reqTicketMsionList();
}


void TicketMgr::initData()
{
	if (m_ticketTable != NULL)
	{
		m_ticketTable->setColumnCount(16);
		m_ticketTable->resizeColumnsToContents();
		QStringList tickets ;
		//tickets<<"���"<<"��������"<<"������"<<"�໤��"<<"ֵ�ฺ����"<<"������ʼʱ��"<<"��������ʱ��"<<"��ע";
		tickets<<"����ƱID"<<"������ID"<<"������"<<"ִ����ID"<<"ִ����"<<"��������ID"<<"��������"<<"���"<<"��������"<<"��ע"<<"������"<<"�໤��"<<"ֵ�ฺ����"<<"������ʼʱ��"<<"��������ʱ��"<<"����ʱ��";
		m_ticketTable->setHorizontalHeaderLabels(tickets);
		//ʹ����ͷ����Ӧ��ȣ�������ƽ���������հײ���
		m_ticketTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
		//�б�����ɫ�仯
		m_ticketTable->setAlternatingRowColors(true);
		//����ÿ�����ݲ��ɱ༭
		m_ticketTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		//����ֻ��ѡ��һ�У�����ѡ�����
		m_ticketTable->setSelectionMode(QAbstractItemView::SingleSelection);
		//���õ���ѡ��һ��
		m_ticketTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		
		//������
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

		//����б�ĳ���źŲ۹���
		connect(m_ticketTable,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getTicketItem(QTableWidgetItem*)));
		

		reqTicketList();
	}
	

	if (m_missionTable != NULL)
	{
		//����Ա�����Ա��ɫ
		if (ComUtil::instance()->getCurUserRole()==eManager || ComUtil::instance()->getCurUserRole()==eDispatcher )
		{
			m_missionTable->setColumnCount(7);
			m_missionTable->resizeColumnsToContents();
			QStringList missions;
			missions<<"��������ID"<<"������ID"<<"������"<<"ִ����ID"<<"ִ����"<<"��������"<<"����ʱ��";
			m_missionTable->setHorizontalHeaderLabels(missions);
			//ʹ����ͷ����Ӧ��ȣ�������ƽ���������հײ���
			m_missionTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
			//�б�����ɫ�仯
			m_missionTable->setAlternatingRowColors(true);
			//����ÿ�����ݲ��ɱ༭
			m_missionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			//����ֻ��ѡ��һ�У�����ѡ�����
			m_missionTable->setSelectionMode(QAbstractItemView::SingleSelection);
			//���õ���ѡ��һ��
			m_missionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

			//���ص�һ������
			m_missionTable->setColumnHidden(0,true);
			m_missionTable->setColumnHidden(1,true);
			m_missionTable->setColumnHidden(3,true);

			setUserCombox();
			reqTicketMsionList();
		}

		//��ά��ɫ
		if (ComUtil::instance()->getCurUserRole()==eMaintainers)
		{
			m_missionTable->setColumnCount(7);
			m_missionTable->resizeColumnsToContents();
			QStringList missions;
			//missions<<"ִ����"<<"����ʱ��";
			missions<<"��������ID"<<"������ID"<<"������"<<"ִ����ID"<<"ִ����"<<"��������"<<"����ʱ��";
			m_missionTable->setHorizontalHeaderLabels(missions);
			//ʹ����ͷ����Ӧ��ȣ�������ƽ���������հײ���
			m_missionTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
			//�б�����ɫ�仯
			m_missionTable->setAlternatingRowColors(true);
			//����ÿ�����ݲ��ɱ༭
			m_missionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			//����ֻ��ѡ��һ�У�����ѡ�����
			m_missionTable->setSelectionMode(QAbstractItemView::SingleSelection);
			//���õ���ѡ��һ��
			m_missionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

			//���ص�һ������
			m_missionTable->setColumnHidden(0,true);
			m_missionTable->setColumnHidden(1,true);
			m_missionTable->setColumnHidden(3,true);

			//����б�ĳ���źŲ۹���
			connect(m_missionTable,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getTicketMsionItem(QTableWidgetItem*)));

			connect(m_createBtn,SIGNAL(pressed()),this,SLOT(onCreateActions()));
			connect(m_comitBtn,SIGNAL(pressed()),this,SLOT(onCommitActions()));
			connect(m_queryBtn,SIGNAL(pressed()),this,SLOT(onQueryActions()));

			m_createBtn->setEnabled(false);
			m_comitBtn->setEnabled(false);
			m_queryBtn->setEnabled(false);

			//�Ѿ��ύ״̬
			m_bcomitflag = true;

			reqTicketMsionList();

		}
	}

	if (m_crateTime != NULL)
	{
		m_crateTime->setDateTime(QDateTime::currentDateTime());
	}

}

void TicketMgr::closeTicketActionsMgr()
{
	//�Ӵ��ڹر�״̬����Ϊtrue
	m_bcloseflag = true;
	//�ύ��ť��Ϊ��
	m_comitBtn->setEnabled(false);
	//�ύ��׼��Ϊtrue
	m_bcomitflag = true;
}

void TicketMgr::onCreateActions()
{
	//�Ӵ���δ�رգ�ֱ�ӷ���
	if (!m_bcloseflag)
	{
		return;
	}

/*	
	m_tktActmgr = new TicketActionsMgr(this);
	m_tktActmgr->setWindowTitle(tr("����Ʊ"));
	m_tktActmgr->setAttribute(Qt::WA_DeleteOnClose);
	connect(m_tktActmgr,SIGNAL(destroyed()),this,SLOT(closeTicketActionsMgr()));

	m_tktActmgr->setTicketMsion(m_ticketMsion);
	m_tktActmgr->initDataByTicketMsion();
	QString strTest1 = QString(tr("����1��λ�ɷֵ���"));
	QString strTest2 = QString(tr("����2��λ�ɷֵ���"));
	m_tktActmgr->setTicketActions(strTest1);
	m_tktActmgr->setTicketActions(strTest2);
	m_tktActmgr->setModal(false);
	m_tktActmgr->show();
*/	

	m_tktActmgr->instance()->setWindowTitle(tr("����Ʊ"));
	//m_tktActmgr->instance()->setAttribute(Qt::WA_DeleteOnClose);
	connect(m_tktActmgr->instance(),SIGNAL(sendCloseTicketActionsMgr()),this,SLOT(closeTicketActionsMgr()));

	m_tktActmgr->instance()->setTicketMsion(m_ticketMsion);
	m_tktActmgr->instance()->initDataByTicketMsion();
	m_tktActmgr->instance()->setModal(false);
	m_tktActmgr->instance()->show();

	ComUtil::instance()->setActionFlag(true);

	m_bcreateflag = true;
	m_bqueryflag = false;
	m_bcomitflag = false;
	m_createBtn->setEnabled(false);
	m_comitBtn->setEnabled(true);

	m_bcloseflag = false;

}


void TicketMgr::onCommitActions()
{
	//�Ӵ���δ�ر�ʱ�ύ����Ч
	if (!m_bcloseflag)
	{
		if (m_bqueryflag)
		{
			Ticket_S mticket = m_tktActmgr->instance()->getTicketInfo();		
			//Ticket_S mticket = m_tktActmgr->getTicketInfo();

			//QString sql = QString(tr("update tickets set UserId=%1,MissionId=%2,No='%3',ActionType='%4',info='%5',ActionPerson='%6',ProtectPerson='%7',ChargePerson='%8',StartTime='%9',EndTime='%10' where ID=%11;")).arg(mticket.AuserId).arg(mticket.MissionId).arg(mticket.No).arg(mticket.ActionType).arg(mticket.info).arg(mticket.ActionPerson).arg(mticket.ProtectPerson).arg(mticket.ChargePerson).arg(mticket.StartTime).arg(mticket.EndTime).arg(m_ticket.id);
			QString sql = QString(tr("update tickets set UserId=%1,MissionId=%2,No='%3',ActionType='%4',info='%5',ActionPerson='%6',ProtectPerson='%7',ChargePerson='%8',StartTime='%9',EndTime='%10' where ID=%11;")).arg(m_ticket.AuserId).arg(mticket.MissionId).arg(mticket.No).arg(mticket.ActionType).arg(mticket.info).arg(mticket.ActionPerson).arg(mticket.ProtectPerson).arg(mticket.ChargePerson).arg(mticket.StartTime).arg(mticket.EndTime).arg(m_ticket.id);

			PBNS::TicketMgrMsg_Request req;
			req.set_mgrsql(sql.toStdString());

			//���䷢������������Ϣ�ź�
			NetClient::instance()->sendData(CMD_TICKET_COMMIT,req.SerializeAsString().c_str(),req.SerializeAsString().length());

		}
		
		if (m_bcreateflag)
		{
			Ticket_S mticket = m_tktActmgr->instance()->getTicketInfo();	
			vector<TicketActions_S> tketActionvect = m_tktActmgr->instance()->getTicketActions();

			//Ticket_S mticket = m_tktActmgr->getTicketInfo();	
			//vector<TicketActions_S> tketActionvect = m_tktActmgr->getTicketActions();

			QString sql = QString(tr("insert into tickets (UserId,MissionId,No,ActionType,info,ActionPerson,ProtectPerson,ChargePerson,StartTime,EndTime) VALUES (%1,%2,'%3','%4','%5','%6','%7','%8','%9','%10');")).arg(mticket.AuserId).arg(mticket.MissionId).arg(mticket.No).arg(mticket.ActionType).arg(mticket.info).arg(mticket.ActionPerson).arg(mticket.ProtectPerson).arg(mticket.ChargePerson).arg(mticket.StartTime).arg(mticket.EndTime);

			int uid = mticket.AuserId;
			int mid = mticket.MissionId;
			QString sttime = mticket.StartTime;
			QString endtime = mticket.EndTime;

			PBNS::TicketActMgrMsg_Request req;
			req.set_requid(uid);
			req.set_reqmid(mid);
			req.set_startt(sttime.toStdString());
			req.set_endt(endtime.toStdString());
			req.set_mgrsql(sql.toStdString());

			for (int i = 0;i<tketActionvect.size();i++)
			{
				PBNS::TicketActBean* tketactbean = req.add_ticketactlist();
				tketactbean->set_ticketid(tketActionvect.at(i).Ticketid);
				tketactbean->set_ordernum(tketActionvect.at(i).OrderNum);
				tketactbean->set_systemcontent(tketActionvect.at(i).SystemContent.toStdString());
				tketactbean->set_content(tketActionvect.at(i).Content.toStdString());
			}

			//���䷢������������Ϣ�ź�
			NetClient::instance()->sendData(CMD_TICKET_CREATE,req.SerializeAsString().c_str(),req.SerializeAsString().length());

		}

		m_bcomitflag = true;
		m_comitBtn->setEnabled(false);
	}
	
}

void TicketMgr::onQueryActions()
{
	//�Ӵ���δ�رգ�ֱ�ӷ���
	if (!m_bcloseflag)
	{
		return;
	}

	/*
	m_tktActmgr = new TicketActionsMgr(this);
	m_tktActmgr->setWindowTitle(tr("����Ʊ��ѯ"));
	m_tktActmgr->setAttribute(Qt::WA_DeleteOnClose);
	connect(m_tktActmgr,SIGNAL(destroyed()),this,SLOT(closeTicketActionsMgr()));

	m_tktActmgr->setTicket(m_ticket);
	m_tktActmgr->initDataByTicket();

	m_tktActmgr->setModal(false);
	m_tktActmgr->show();
*/

	m_tktActmgr->instance()->setWindowTitle(tr("����Ʊ��ѯ"));
	//m_tktActmgr->instance()->setAttribute(Qt::WA_DeleteOnClose);
	connect(m_tktActmgr->instance(),SIGNAL(sendCloseTicketActionsMgr()),this,SLOT(closeTicketActionsMgr()));

	m_tktActmgr->instance()->setTicket(m_ticket);
	m_tktActmgr->instance()->initDataByTicket();

	m_tktActmgr->instance()->setModal(false);
	m_tktActmgr->instance()->show();

	m_bqueryflag = true;
	m_bcreateflag = false;
	m_bcomitflag = false;
	m_queryBtn->setEnabled(false);
	m_comitBtn->setEnabled(true);

	m_bcloseflag = false;
}


/************************************************************************/
/*                           ����Ʊ��ϸ                                 */
/************************************************************************/
/**/
TicketActionsMgr* TicketActionsMgr::m_inst = NULL;

TicketActionsMgr* TicketActionsMgr::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new TicketActionsMgr;

	}
	return m_inst;
}


TicketActionsMgr::TicketActionsMgr(QWidget* parent /* = NULL */)
{
	//setAttribute (Qt::WA_DeleteOnClose);
	m_viewType = 1;

	initUi_Operator();

}

/*
TicketActionsMgr::~TicketActionsMgr()
{
	
	deleteInstance();
}

void TicketActionsMgr::deleteInstance()
{
	if (m_inst != NULL)
	{
		delete m_inst;

		m_inst = NULL;
	}
}
*/

void TicketActionsMgr::initUi_Operator()
{
	//ֻ��ʾ�رհ�ť
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

	QLabel *companylab = new QLabel(tr("��λ:"));
	companylab->setFixedSize(30,25);
	QLabel *nolab = new QLabel(tr("���:"));
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

	QLabel *crtlab = new QLabel(tr("������:"));
	crtlab->setFixedSize(40,25);
	QLabel *actlab = new QLabel(tr("������:"));
	actlab->setFixedSize(40,25);
	QLabel *crttimelab = new QLabel(tr("    ����ʱ��:"));
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


	QLabel *oprstalab = new QLabel(tr("������ʼʱ��:"));
	oprstalab->setFixedSize(80,25);
	QLabel *oprendlab = new QLabel(tr("��������ʱ��:"));
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
	QLabel *gndlab = new QLabel(tr("�໤�²���"));
	QLabel *onelab = new QLabel(tr("���˲���"));
	QLabel *ohlab = new QLabel(tr("������Ա����"));
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

	QLabel *mslab = new QLabel(tr("��������"));
	mslab->setFixedSize(50,25);
	m_missionLied = new QLineEdit;
	m_missionLied->setFixedSize(600,25);
	hbox5->addWidget(mslab);
	hbox5->addWidget(m_missionLied);
	hbox5->addStretch();


	m_ticketActTable = new QTableWidget();
	//���ñ���ɫΪ��ɫ
	QPalette tabplt = m_ticketActTable->palette();
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	m_ticketActTable->setPalette(tabplt);

	//ʹ����ͷ����Ӧ��ȣ�������ƽ���������հײ���
	//ptabwdgStaList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//�б�����ɫ�仯
	m_ticketActTable->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	//m_ticketActTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	m_ticketActTable->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	//m_ticketActTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	hbox6->addWidget(m_ticketActTable);


	QLabel *remklab = new QLabel(tr("��ע:"));
	hbox7->addWidget(remklab);
	hbox7->addStretch();

	m_remarksLied = new QLineEdit;
	m_remarksLied->setFixedSize(650,75);
	hbox8->addWidget(m_remarksLied);


	QLabel *oprlab = new QLabel(tr("������:"));
	QLabel *gdlab = new QLabel(tr("�໤��:"));
	QLabel *ohalab = new QLabel(tr("ֵ�ฺ����:"));
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

	//connect(m_ticketActTable,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(setDesc(QTableWidgetItem*)));
	
	m_createLied->setEnabled(false);
	m_actLied->setEnabled(false);
	m_createrTime->setEnabled(false);
	m_missionLied->setEnabled(false);
}


void TicketActionsMgr::initDataByTicketMsion()
{
	//���ù�˾����:ÿ�����й̶����������ļ�������
	m_companyLied->setText(ComUtil::instance()->getCompanyName());
	//��������ID
	m_Msionid = m_tketMsion.id;
	//������
	m_createLied->setText(m_tketMsion.username);
	//������
	m_actLied->setText(m_tketMsion.actusername);
	//����ʱ��
	QString crteTime = m_tketMsion.publishtime;
	QDateTime time;
	time = QDateTime::fromString(crteTime, "yyyy-MM-dd hh:mm:ss");
	m_createrTime->setDateTime(time);
	//������ʼʱ��
	m_opstartTime->setDateTime(QDateTime::currentDateTime());
	//��������ʱ��
	m_opendTime->setDateTime(QDateTime::currentDateTime());
	//��������
	m_missionLied->setText(m_tketMsion.name);

	//��������
	m_ticketActTable->setColumnCount(6);
	QStringList header;
	header<<"����ƱID"<<"˳��"<<"������Ŀ(ϵͳ)"<<"������Ŀ(����)"<<"��"<<"ʱ��";
	m_ticketActTable->setHorizontalHeaderLabels(header);

	//���ص�һ��:����ƱID
	m_ticketActTable->setColumnHidden(0,true);

}

void TicketActionsMgr::initDataByTicket()
{
	//���ù�˾����:ÿ�����й̶����������ļ�������
	m_companyLied->setText(ComUtil::instance()->getCompanyName());
	//���
	m_noLied->setText(m_tket.No);
	//��������ID
	m_Msionid = m_tket.MissionId;
	//������
	m_createLied->setText(m_tket.CuserName);
	//������
	m_actLied->setText(m_tket.AuserName);
	//����ʱ��
	QString crteTime = m_tket.PublishTime;
	QDateTime time;
	time = QDateTime::fromString(crteTime, "yyyy-MM-dd hh:mm:ss");
	m_createrTime->setDateTime(time);
	//������ʼʱ��
	m_opstartTime->setDateTime(QDateTime::fromString(m_tket.StartTime, "yyyy-MM-dd hh:mm:ss"));
	//��������ʱ��
	m_opendTime->setDateTime(QDateTime::fromString(m_tket.EndTime, "yyyy-MM-dd hh:mm:ss"));
	//��������
	setActionsType();
	//��������
	m_missionLied->setText(m_tket.MissionName);
	//��ע
	m_remarksLied->setText(m_tket.info);
	//������
	m_operatorLied->setText(m_tket.ActionPerson);
	//�໤��
	m_guardianLied->setText(m_tket.ProtectPerson);
	//ֵ�ฺ����
	m_watcherLied->setText(m_tket.ChargePerson);

	//��������
	m_ticketActTable->setColumnCount(5);
	QStringList header;
	header<<"����ƱID"<<"˳��"<<"������Ŀ"<<"��"<<"ʱ��";
	m_ticketActTable->setHorizontalHeaderLabels(header);

	//���ص�һ��:����ƱID
	m_ticketActTable->setColumnHidden(0,true);

	m_companyLied->setEnabled(false);
	m_guardianChbox->setEnabled(false);
	m_onerChbox->setEnabled(false);
	m_overhaulChbox->setEnabled(false);
	m_operatorLied->setEnabled(false);
	m_guardianLied->setEnabled(false);
	m_watcherLied->setEnabled(false);

	//�����ѯ����Ʊ��ϸ�б�
	reqTicketActionsList();

}

void TicketActionsMgr::reqTicketActionsList()
{
	PBNS::TicketActListMsg_Request req;
	req.set_reqid(m_tket.id);

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_TICKET_QUERY,req.SerializeAsString().c_str(),req.SerializeAsString().length());
}

void TicketActionsMgr::retTicketActionsList(const char* msg,int msglength)
{
	PBNS::TicketActListMsg_Response resp;
	resp.ParseFromArray(msg,msglength);

	int nrow = resp.ticketactlist_size();

	//��������
	m_ticketActTable->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::TicketActBean tkacbean = resp.ticketactlist(i);

		QString strTicketid = QString(tr("%1")).arg(tkacbean.ticketid());
		QString strOrderNum = QString(tr("%1")).arg(tkacbean.ordernum());

		QString strInfo = QString(tr("%1%2")).arg(QString::fromStdString(tkacbean.systemcontent())).arg(QString::fromStdString(tkacbean.content()));
		QString strGou = QString(tr(""));
		QString strTime = QString(tr(""));

		m_ticketActTable->setItem(i,0,new QTableWidgetItem(strTicketid));
		m_ticketActTable->setItem(i,1,new QTableWidgetItem(strOrderNum));
		m_ticketActTable->setItem(i,2,new QTableWidgetItem(strInfo));
		m_ticketActTable->setItem(i,3,new QTableWidgetItem(strGou));
		m_ticketActTable->setItem(i,4,new QTableWidgetItem(strTime));

		//Ĭ����ȫ���ɱ༭������Ϊ���ɱ༭
		m_ticketActTable->item(i,0)->setFlags(Qt::NoItemFlags);
		m_ticketActTable->item(i,1)->setFlags(Qt::NoItemFlags);
		m_ticketActTable->item(i,2)->setFlags(Qt::NoItemFlags);
		m_ticketActTable->item(i,3)->setFlags(Qt::NoItemFlags);
		m_ticketActTable->item(i,4)->setFlags(Qt::NoItemFlags);

	}
}

void TicketActionsMgr::setTicketActions(QString strActions)
{

	//����ϸ�б����������
	addTicketActionsToTable(strActions);

}

void TicketActionsMgr::addTicketActionsToTable(QString strAct)
{
	//��ǰ�б��¼��
	int tktRow = m_ticketActTable->rowCount();
	//���ò�����ϸ�б�����
	m_ticketActTable->setRowCount(tktRow+1);
	
	//����ƱID��δ�ύ��ʱ�ò���������Ĭ��Ϊ0���������
	QString strTktid = QString(tr("0"));					//����ƱID
	QString strNum = QString(tr("%1")).arg(tktRow+1);			//˳���
	QString strDesc = QString(tr(""));						//��Ŀ����
	QString strGou = QString(tr(""));
	QString strTime = QString(tr(""));

	m_ticketActTable->setItem(tktRow,0,new QTableWidgetItem(strTktid));
	m_ticketActTable->setItem(tktRow,1,new QTableWidgetItem(strNum));
	m_ticketActTable->setItem(tktRow,2,new QTableWidgetItem(strAct));
	m_ticketActTable->setItem(tktRow,3,new QTableWidgetItem(strDesc));
	m_ticketActTable->setItem(tktRow,4,new QTableWidgetItem(strGou));
	m_ticketActTable->setItem(tktRow,5,new QTableWidgetItem(strTime));

	//Ĭ����ȫ���ɱ༭������Ϊ���ɱ༭
	m_ticketActTable->item(tktRow,0)->setFlags(Qt::NoItemFlags);
	m_ticketActTable->item(tktRow,1)->setFlags(Qt::NoItemFlags);
	m_ticketActTable->item(tktRow,2)->setFlags(Qt::NoItemFlags);
	m_ticketActTable->item(tktRow,4)->setFlags(Qt::NoItemFlags);
	m_ticketActTable->item(tktRow,5)->setFlags(Qt::NoItemFlags);

	
	/*
	QTableWidgetItem *item = m_ticketActTable->item(tktRow,3);
	if(item) 
	{
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
	} 
	else 
	{
		item = new QTableWidgetItem;
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		m_ticketActTable->setItem(tktRow, 3, item);
	}
	*/
}

void TicketActionsMgr::setDesc(QTableWidgetItem* item)
{
	
}

vector<TicketActions_S> TicketActionsMgr::getTicketActions()
{
	m_tketActions.clear();

	int nRow = m_ticketActTable->rowCount();

	for (int i=0;i<nRow;i++)
	{
		TicketActions_S tketActions;

		tketActions.Ticketid = m_ticketActTable->item(i,0)->text().toInt();
		tketActions.OrderNum = m_ticketActTable->item(i,1)->text().toInt();
		tketActions.SystemContent = m_ticketActTable->item(i,2)->text();
		tketActions.Content = m_ticketActTable->item(i,3)->text();

		//tketActbean.set_ticketid(m_ticketActTable->item(i,0)->text().toInt());
		//tketActbean.set_ordernum(m_ticketActTable->item(i,1)->text().toInt());
		//tketActbean.set_systemcontent(m_ticketActTable->item(i,2)->text().toStdString());
		//tketActbean.set_content(m_ticketActTable->item(i,3)->text().toStdString());
		
		m_tketActions.push_back(tketActions);
	}

	return m_tketActions;
}

void TicketActionsMgr::setActionsType()
{
	QString strType = m_tket.ActionType;

	if (strType.length()>0)
	{
		//�໤�˲�������
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

		//���˲�������
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

		//������Ա��������
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


void TicketActionsMgr::closeEvent(QCloseEvent *event)
{
	//��Ϊ��ȫ�־�̬�����Թر�ʱ������б����ͻ�ԭ���пؼ�״̬
	m_companyLied->setEnabled(true);
	m_guardianChbox->setEnabled(true);
	m_onerChbox->setEnabled(true);
	m_overhaulChbox->setEnabled(true);
	m_operatorLied->setEnabled(true);
	m_guardianLied->setEnabled(true);
	m_watcherLied->setEnabled(true);

	m_operatorLied->clear();
	m_guardianLied->clear();
	m_watcherLied->clear();
	m_noLied->clear();
	m_remarksLied->clear();

	TicketMsion_S tktmsn;
	m_tketMsion = tktmsn;

	Ticket_S tkts;
	m_tket = tkts;
	m_tketReturn = tkts;

	m_ticketActTable->clear();
	m_ticketActTable->setRowCount(0);
	m_ticketActTable->clearContents();
	ComUtil::instance()->setActionFlag(false);
	emit sendCloseTicketActionsMgr();

}











