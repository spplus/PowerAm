#include "ticketmgr.h"
//#include "comutil.h"
//#include "define.h"

TicketMgr::TicketMgr(QWidget* parent /* = NULL */)
{
	m_ticketTable = NULL;
	m_missionTable = NULL;
	m_crateTime = NULL;

	switch (ComUtil::instance()->getCurUserRole())
	{
	case eManager://����Ա
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
	initSlots();
	initData();
}

void TicketMgr::initUi_Mission()
{
	QVBoxLayout*		vbox = new QVBoxLayout;
	QGroupBox* gbox = new QGroupBox;
	gbox->setTitle(tr("��������"));
	m_missionTable = new QTableWidget;
	QHBoxLayout* hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_missionTable);
	gbox->setLayout(hbox0);

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


	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_createBtn = new QPushButton(tr("��������Ʊ"));
	hbox2->addWidget(gbox);
	hbox2->addWidget(m_createBtn);

	QHBoxLayout* hbox3 = new QHBoxLayout;
	hbox3->addWidget(gbox2);

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

void TicketMgr::onCreateActions()
{

}

void TicketMgr::recvdata(int msgtype,const char* msg,int msglength)
{
	switch (msgtype)
	{
	case CMD_TICKETMS_LIST:
		retTicketMsionList(msg);
		break;
	}
}

void TicketMgr::reqTicketMsionList()
{
	PBNS::TicketMsionListMsg_Request tkmsreq;

	tkmsreq.set_userid(ComUtil::instance()->getCurUserId());

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_TICKETMS_LIST,tkmsreq.SerializeAsString().c_str(),tkmsreq.SerializeAsString().length());

	return;
}

void TicketMgr::retTicketMsionList(const char* msg)
{
	PBNS::TicketMsionListMsg_Response resp;
	resp.ParseFromString(msg);

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

	//δѡ���б�ʱֻ�������û�,���Ӱ�ť��Ч��ɾ�����޸İ�ť��Ч
	m_addBtn->setEnabled(true);
	m_delBtu->setEnabled(false);
	m_mdfBtn->setEnabled(false);
}


void TicketMgr::getTicketMsionItem(QTableWidgetItem* item)
{
	int itemrow = item->row();

	int nActusrid = m_missionTable->item(itemrow,3)->text().toInt();

	for (int i=0;i<m_userBox->count();i++)
	{
		if (nActusrid == m_userBox->itemData(i).toInt())
		{
			m_userBox->setCurrentIndex(i);
		}
	}


	m_nameLied->setText(m_missionTable->item(itemrow,5)->text());

	//m_crateTime->setDateTime();

	m_delBtu->setEnabled(true);
	m_mdfBtn->setEnabled(true);
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
		int nActUserid = m_missionTable->item(ncurrow,3)->text().toInt();

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
		m_ticketTable->setColumnCount(8);
		m_ticketTable->resizeColumnsToContents();
		QStringList tickets ;
		tickets<<"���"<<"��������"<<"������"<<"�໤��"<<"ֵ�ฺ����"<<"������ʼʱ��"<<"��������ʱ��"<<"��ע";
		m_ticketTable->setHorizontalHeaderLabels(tickets);

	}
	

	if (m_missionTable != NULL)
	{
		//����Ա��ɫ
		if (ComUtil::instance()->getCurUserRole()==eDispatcher)
		{
			m_missionTable->setColumnCount(7);
			m_missionTable->resizeColumnsToContents();
			QStringList missions;
			missions<<"��������ID"<<"������ID"<<"������"<<"ִ����ID"<<"ִ����"<<"��������"<<"����ʱ��";
			m_missionTable->setHorizontalHeaderLabels(missions);

			//���ص�һ������
			m_missionTable->setColumnHidden(0,true);
			m_missionTable->setColumnHidden(1,true);
			m_missionTable->setColumnHidden(3,true);

			setUserCombox();
		}

		if (ComUtil::instance()->getCurUserRole()==eMaintainers)
		{
			m_missionTable->setColumnCount(2);
			m_missionTable->resizeColumnsToContents();
			QStringList missions;
			missions<<"ִ����"<<"����ʱ��";
			m_missionTable->setHorizontalHeaderLabels(missions);
		}
	}

	if (m_crateTime != NULL)
	{
		m_crateTime->setDateTime(QDateTime::currentDateTime());
	}

}


