#include "rulemgrdlg.h"


RuleMgrdlg::RuleMgrdlg(QWidget *parent)
	: QDialog(parent)
{
	//ֻ��ʾ�رհ�ť
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->resize(600,800);


	QHBoxLayout *phlyt = new QHBoxLayout;
	QHBoxLayout *phlayt = new QHBoxLayout;
	QHBoxLayout *poprlyt1 = new QHBoxLayout;
	QHBoxLayout *poprlyt2 = new QHBoxLayout;
	QHBoxLayout *poprlyt = new QHBoxLayout;
	QHBoxLayout *psrhlayt = new QHBoxLayout;
	QHBoxLayout *poprhlayt = new QHBoxLayout;

	QVBoxLayout *pmainvlyt = new QVBoxLayout;

	QGroupBox *pstalisgbox = new QGroupBox("����վ����");
	pstachbox = new QCheckBox;
	pstacbox = new QComboBox;
	pstacbox->setFixedWidth(250);
	pstacbox->setFixedHeight(25);
	pbtnstause = new QPushButton("����վ����");
	pbtnstause->setFixedHeight(30);
	pbtnstanouse = new QPushButton("����վ����");
	pbtnstanouse->setFixedHeight(30);

	QGroupBox *prulelisgbox = new QGroupBox("���������");
	prulechbox = new QCheckBox;
	prulecbox = new QComboBox;
	prulecbox->setFixedWidth(250);
	prulecbox->setFixedHeight(25);
	pbtnruleuse = new QPushButton("����������");
	pbtnruleuse->setFixedHeight(30);
	pbtnrulenouse = new QPushButton("���������");
	pbtnrulenouse->setFixedHeight(30);

	QGroupBox *poprgbox = new QGroupBox("����");
	pstaallchbox = new QCheckBox;
	pbtnalluse = new QPushButton("ȫվ��������");
	pbtnalluse->setFixedHeight(30);

	pcmitchbox = new QCheckBox;
	pbtncommit = new QPushButton("��  ��");
	pbtncommit->setFixedHeight(30);


	QGroupBox *psrlisgbox = new QGroupBox("��վ���������ϵ�б�");
	ptabwdgsrlist = new QTableWidget();
	//���ñ���ɫΪ��ɫ
	QPalette tabplt = ptabwdgsrlist->palette();
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	ptabwdgsrlist->setPalette(tabplt);
	//��������
	ptabwdgsrlist->setColumnCount(6);
	QStringList header;
	header<<"��վCIMID"<<"��վ����"<<"����ID"<<"��������"<<"�Ƿ�����ֵ"<<"�Ƿ�����";
	ptabwdgsrlist->setHorizontalHeaderLabels(header);
	//ʹ����ͷ����Ӧ��ȣ�������ƽ���������հײ���
	ptabwdgsrlist->horizontalHeader()->setResizeMode(QHeaderView::Stretch); //QHeaderView::ResizeToContents QHeaderView::Stretch
	//�б�����ɫ�仯
	ptabwdgsrlist->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	ptabwdgsrlist->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	ptabwdgsrlist->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	ptabwdgsrlist->setSelectionBehavior(QAbstractItemView::SelectRows);

	//ȥ��ÿ�е��к�
	//QHeaderView *headerView = ptabwdgsrlist->verticalHeader();
	//headerView->setHidden(true);

	//����վ����������ϵ�б��һ����������
	ptabwdgsrlist->setColumnHidden(0,true);
	ptabwdgsrlist->setColumnHidden(2,true);
	ptabwdgsrlist->setColumnHidden(4,true);

	pbtnruletype = new QPushButton("��������");
	pbtnruletype->setFixedHeight(30);

	

	//����վ����
	phlyt->addWidget(pstachbox);
	phlyt->addWidget(pstacbox);
	phlyt->addSpacing(100);
	phlyt->addWidget(pbtnstause);
	phlyt->addWidget(pbtnstanouse);
	pstalisgbox->setLayout(phlyt);

	//���������
	phlayt->addWidget(prulechbox);
	phlayt->addWidget(prulecbox);
	phlayt->addSpacing(100);
	phlayt->addWidget(pbtnruleuse);
	phlayt->addWidget(pbtnrulenouse);
	prulelisgbox->setLayout(phlayt);

	//����
	poprlyt1->addWidget(pstaallchbox);
	poprlyt1->addWidget(pbtnalluse);
	poprlyt2->addWidget(pcmitchbox);
	poprlyt2->addWidget(pbtncommit);
	poprlyt->addLayout(poprlyt1);
	poprlyt->addStretch();
	poprlyt->addLayout(poprlyt2);
	poprlyt->addSpacing(120);
	poprlyt->addStretch();
	poprlyt->addWidget(pbtnruletype);
	poprgbox->setLayout(poprlyt);

	//��վ���������ϵ�б�
	psrhlayt->addWidget(ptabwdgsrlist);
	psrlisgbox->setLayout(psrhlayt);

	pmainvlyt->addWidget(pstalisgbox);
	pmainvlyt->addWidget(prulelisgbox);
	pmainvlyt->addWidget(poprgbox);
	pmainvlyt->addWidget(psrlisgbox);

	pmainvlyt->addSpacing(5);

	pmainvlyt->setContentsMargins(20,10,20,20);

	this->setLayout(pmainvlyt);

	//���س�վ�б�͹��������б�
	loadSationList();
	loadRuleTypeList();

	//������ť�źŲ۹���
	connect(pstachbox,SIGNAL(clicked()),this,SLOT(checkStachbox()));
	connect(prulechbox,SIGNAL(clicked()),this,SLOT(checkRulechbox()));

	connect(pstacbox, SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeStaCbox(int)));
	connect(prulecbox, SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeRuleCbox(int)));

	connect(pbtnstause,SIGNAL(clicked()),this,SLOT(checkStationRuleUse()));
	connect(pbtnstanouse,SIGNAL(clicked()),this,SLOT(checkStationRuleNoUse()));

	connect(pbtnruleuse,SIGNAL(clicked()),this,SLOT(checkRuleUse()));
	connect(pbtnrulenouse,SIGNAL(clicked()),this,SLOT(checkRuleNoUse()));

	connect(pstaallchbox,SIGNAL(clicked()),this,SLOT(checkAllstachbox()));
	connect(pcmitchbox,SIGNAL(clicked()),this,SLOT(checkCommitchbox()));
	//ȫվ����
	connect(pbtnalluse,SIGNAL(clicked()),this,SLOT(allStationRuleUse()));
	connect(pbtncommit,SIGNAL(clicked()),this,SLOT(commitStationRule()));

	connect(pbtnruletype,SIGNAL(clicked()),this,SLOT(createRuleTypeMgrDlg()));
	connect(ptabwdgsrlist,SIGNAL(cellChanged(int,int)),this,SLOT(cellChangeIsUse(int,int)));
	

	//Ĭ��ѡ�񰴳�վ��ʾ
	pstachbox->setChecked(true);
	checkStachbox();

	pstacbox->setCurrentIndex(0);

	pbtnalluse->setEnabled(false);
	pbtncommit->setEnabled(false);
	//��ʼ�������
	m_strulemdfvct.clear();

	bcellchageflag = false;

}

RuleMgrdlg::~RuleMgrdlg()
{

}

void RuleMgrdlg::checkStachbox()
{
	if (pstachbox->isChecked())
	{
		pstachbox->setChecked(true);

		pstachbox->setEnabled(true);
		pstacbox->setEnabled(true);
		pbtnstause->setEnabled(true);
		pbtnstanouse->setEnabled(true);

		//����ǰ�����õĳ�վ��ʾ�б�
		ChangeStaCbox(pstacbox->currentIndex());


		prulechbox->setChecked(false);

		prulechbox->setEnabled(false);
		prulecbox->setEnabled(false);
		pbtnruleuse->setEnabled(false);
		pbtnrulenouse->setEnabled(false);
	}
	else
	{
		pstachbox->setChecked(false);

		pstachbox->setEnabled(false);
		pstacbox->setEnabled(false);
		pbtnstause->setEnabled(false);
		pbtnstanouse->setEnabled(false);


		prulechbox->setChecked(true);

		prulechbox->setEnabled(true);
		prulecbox->setEnabled(true);
		pbtnruleuse->setEnabled(true);
		pbtnrulenouse->setEnabled(true);

		//����ǰ���õĹ�����ʾ�б�
		ChangeRuleCbox(prulecbox->currentIndex());
	}
	
}

void RuleMgrdlg::checkRulechbox()
{
	if (prulechbox->isChecked())
	{
		prulechbox->setChecked(true);

		prulechbox->setEnabled(true);
		prulecbox->setEnabled(true);
		pbtnruleuse->setEnabled(true);
		pbtnrulenouse->setEnabled(true);

		//����ǰ���õĹ�����ʾ�б�
		ChangeRuleCbox(prulecbox->currentIndex());

		pstachbox->setChecked(false);

		pstachbox->setEnabled(false);
		pstacbox->setEnabled(false);
		pbtnstause->setEnabled(false);
		pbtnstanouse->setEnabled(false);
	}
	else
	{
		prulechbox->setChecked(false);

		prulechbox->setEnabled(false);
		prulecbox->setEnabled(false);
		pbtnruleuse->setEnabled(false);
		pbtnrulenouse->setEnabled(false);


		pstachbox->setChecked(true);

		pstachbox->setEnabled(true);
		pstacbox->setEnabled(true);
		pbtnstause->setEnabled(true);
		pbtnstanouse->setEnabled(true);

		//����ǰ�����õĳ�վ��ʾ�б�
		ChangeStaCbox(pstacbox->currentIndex());
	}

}

void RuleMgrdlg::ChangeStaCbox(int idx)
{
	QString stacimid = pstacbox->itemData(idx).toString();
	m_strstacimid = stacimid;
	vector<StationRule_S> strulevect;

	if (stacimid == "ȫ����վ")
	{
		ptabwdgsrlist->clearContents();
		int nrow = m_starulevct.size();
		ptabwdgsrlist->setRowCount(nrow);

		for (int i=0;i<m_starulevct.size();i++)
		{
			StationRule_S strule = m_starulevct.at(i);
			int nIsUse = strule.isUse.toInt();
			ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(strule.stationCim));										//�����и�ֵ
			ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(strule.stationName));
			ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(QString("%1").arg(strule.ruleId)));							//�����и�ֵ
			ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(strule.ruleName));
			ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(strule.isUse));												//�����и�ֵ
			
			//�ڵ�6����Ƕ��checkbox������״ֵ̬
			QTableWidgetItem *chkitem = new QTableWidgetItem();
			if (nIsUse == 1)
			{
				chkitem->setCheckState(Qt::Checked);
			}
			else
			{
				chkitem->setCheckState(Qt::Unchecked);
			}

			ptabwdgsrlist->setItem(i,5,chkitem);

			/*
			QCheckBox *chboxUse = new QCheckBox;
			if (nIsUse == 1)
			{
				chboxUse->setChecked(true);
			}
			else
			{
				chboxUse->setChecked(false);
			}
			
			QHBoxLayout *hlay = new QHBoxLayout;
			hlay->addWidget(chboxUse);
			hlay->setMargin(0);
			hlay->setAlignment(chboxUse,Qt::AlignCenter);
			QWidget *pwidget = new QWidget;
			pwidget->setLayout(hlay);

			//ptabwdgsrlist->setCellWidget(i,5,pwidget);
			ptabwdgsrlist->setCellWidget(i,5,chboxUse);
			*/

			/*
			QComboBox *cboxUse = new QComboBox;
			cboxUse->insertItem(0,"��",0);
			cboxUse->insertItem(1,"��",1);
			cboxUse->insertItem(2,"δ֪",2);

			cboxUse->setCurrentIndex(nIsUse);
			if (nIsUse == 1 || nIsUse == 0)
			{
				cboxUse->setCurrentIndex(nIsUse);
			}
			else
			{
				cboxUse->setCurrentIndex(2);
			}
			ptabwdgsrlist->setCellWidget(i,5,cboxUse);
			*/

			//ptabwdgsrlist->setItem(i,5,new QTableWidgetItem(strule.isUseName));

		}

		strulevect = m_starulevct;
	}
	
	if (stacimid != "ȫ����վ")
	{
		ptabwdgsrlist->clearContents();

		for (int i=0;i<m_starulevct.size();i++)
		{
			StationRule_S strule = m_starulevct.at(i);
			if (strule.stationCim == stacimid)
			{
				strulevect.push_back(strule);
			}
			
		}

		int nrow = strulevect.size();
		ptabwdgsrlist->setRowCount(nrow);

		for (int i=0;i<nrow;i++)
		{
			StationRule_S strl = strulevect.at(i);
			int nIsUse = strl.isUse.toInt();
			ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(strl.stationCim));											//�����и�ֵ
			ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(strl.stationName));
			ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(QString("%1").arg(strl.ruleId)));							//�����и�ֵ
			ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(strl.ruleName));
			ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(strl.isUse));												//�����и�ֵ

			//�ڵ�6����Ƕ��checkbox������״ֵ̬
			QTableWidgetItem *chkitem = new QTableWidgetItem();
			if (nIsUse == 1)
			{
				chkitem->setCheckState(Qt::Checked);
			}
			else
			{
				chkitem->setCheckState(Qt::Unchecked);
			}
			ptabwdgsrlist->setItem(i,5,chkitem);
		}
	}

	//����վ���泧վ�����ϵ�б�����
	m_stationrvct = strulevect;

}

void RuleMgrdlg::ChangeRuleCbox(int idx)
{
	int ruleid = prulecbox->itemData(idx).toInt();
	m_ruleid = ruleid;
	vector<StationRule_S> strulevect;

	if (ruleid == 0)
	{
		ptabwdgsrlist->clearContents();
		int nrow = m_starulevct.size();
		ptabwdgsrlist->setRowCount(nrow);

		for (int i=0;i<m_starulevct.size();i++)
		{
			StationRule_S strule = m_starulevct.at(i);
			int nIsUse = strule.isUse.toInt();
			ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(strule.stationCim));										//�����и�ֵ
			ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(strule.stationName));
			ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(QString("%1").arg(strule.ruleId)));							//�����и�ֵ
			ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(strule.ruleName));
			ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(strule.isUse));												//�����и�ֵ
			
			//�ڵ�6����Ƕ��checkbox������״ֵ̬
			QTableWidgetItem *chkitem = new QTableWidgetItem();
			if (nIsUse == 1)
			{
				chkitem->setCheckState(Qt::Checked);
			}
			else
			{
				chkitem->setCheckState(Qt::Unchecked);
			}
			ptabwdgsrlist->setItem(i,5,chkitem);

		}

		strulevect = m_starulevct;
	}

	if (ruleid != 0)
	{
		ptabwdgsrlist->clearContents();

		for (int i=0;i<m_starulevct.size();i++)
		{
			StationRule_S strule = m_starulevct.at(i);
			if (strule.ruleId == ruleid)
			{
				strulevect.push_back(strule);
			}
		}

		int nrow = strulevect.size();
		ptabwdgsrlist->setRowCount(nrow);

		for (int i=0;i<nrow;i++)
		{
			StationRule_S strl = strulevect.at(i);
			int nIsUse = strl.isUse.toInt();
			ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(strl.stationCim));											//�����и�ֵ
			ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(strl.stationName));
			ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(QString("%1").arg(strl.ruleId)));							//�����и�ֵ
			ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(strl.ruleName));
			ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(strl.isUse));												//�����и�ֵ
			
			//�ڵ�6����Ƕ��checkbox������״ֵ̬
			QTableWidgetItem *chkitem = new QTableWidgetItem();
			if (nIsUse == 1)
			{
				chkitem->setCheckState(Qt::Checked);
			}
			else
			{
				chkitem->setCheckState(Qt::Unchecked);
			}
			ptabwdgsrlist->setItem(i,5,chkitem);
		}
	}
	
	//�����򱣴泧վ�����ϵ�б�����
	m_rulestvct = strulevect;
}

void RuleMgrdlg::loadSationList()
{
	m_stationvct.clear();
	pstacbox->clear();

	m_stationvct = ComUtil::instance()->getStationMgrList();

	int nrow = m_stationvct.size();

	pstacbox->insertItem(0,QString("ȫ��"),QString("ȫ����վ"));

	for (int i = 0; i < nrow; i++)
	{
		Station_S stalist = m_stationvct.at(i);

		pstacbox->insertItem(i+1,stalist.name,stalist.cimid);

	}

}

void RuleMgrdlg::loadRuleTypeList()
{
	m_ruletypevct.clear();
	prulecbox->clear();

	m_ruletypevct = ComUtil::instance()->getRuleTypeList();

	int nrow = m_ruletypevct.size();
	prulecbox->insertItem(0,QString("ȫ��"),QString("ȫ������"));

	for (int i=0;i<nrow;i++)
	{
		RuleType_S rulelist = m_ruletypevct.at(i);
		prulecbox->insertItem(i+1,rulelist.rulename,rulelist.ruleid);

	}

}


void RuleMgrdlg::checkStationRuleUse()
{
	PBNS::StationRuleMgrMsg_Request reqst;
	reqst.set_reqtype(1);

	QString sql;
	if (m_strstacimid == "ȫ����վ")
	{
		QString sqltmp = QString("UPDATE station_rule SET IsUse=1 ;");
		sql = sqltmp;
	}
	else
	{
		QString sqltmp = QString("UPDATE station_rule SET IsUse=1 WHERE StationCim='%1';").arg(m_strstacimid);
		sql = sqltmp;
	}
	reqst.set_mgrsql(sql.toStdString());

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_STATION_RULE_MANAGER,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

	reqStaionRuleList();

	return;

}

void RuleMgrdlg::checkStationRuleNoUse()
{
	PBNS::StationRuleMgrMsg_Request reqst;
	reqst.set_reqtype(1);

	QString sql;
	if (m_strstacimid == "ȫ����վ")
	{
		QString sqltmp = QString("UPDATE station_rule SET IsUse=0 ;");
		sql = sqltmp;
	}
	else
	{
		QString sqltmp = QString("UPDATE station_rule SET IsUse=0 WHERE StationCim='%1';").arg(m_strstacimid);
		sql = sqltmp;
	}
	reqst.set_mgrsql(sql.toStdString());

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_STATION_RULE_MANAGER,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

	reqStaionRuleList();

	return;
}

void RuleMgrdlg::checkRuleUse()
{
	PBNS::StationRuleMgrMsg_Request reqst;
	reqst.set_reqtype(1);

	QString sql;
	if (m_strstacimid == "ȫ������")
	{
		QString sqltmp = QString("UPDATE station_rule SET IsUse=1 ;");
		sql = sqltmp;
	}
	else
	{
		QString sqltmp = QString("UPDATE station_rule SET IsUse=1 WHERE RuleId=%1 ;").arg(m_ruleid);
		sql = sqltmp;
	}
	reqst.set_mgrsql(sql.toStdString());

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_STATION_RULE_MANAGER,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

	reqStaionRuleList();

	return;
}

void RuleMgrdlg::checkRuleNoUse()
{
	PBNS::StationRuleMgrMsg_Request reqst;
	reqst.set_reqtype(1);

	QString sql;
	if (m_strstacimid == "ȫ������")
	{
		QString sqltmp = QString("UPDATE station_rule SET IsUse=0 ;");
		sql = sqltmp;
	}
	else
	{
		QString sqltmp = QString("UPDATE station_rule SET IsUse=0 WHERE RuleId=%1 ;").arg(m_ruleid);
		sql = sqltmp;
	}
	reqst.set_mgrsql(sql.toStdString());

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_STATION_RULE_MANAGER,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

	reqStaionRuleList();

	return;
}

void RuleMgrdlg::checkAllstachbox()
{
	if (pstaallchbox->isChecked())
	{
		pbtnalluse->setEnabled(true);
	}
	else
	{
		pbtnalluse->setEnabled(false);
	}
}

void RuleMgrdlg::allStationRuleUse()
{
	if(QMessageBox::question(this,"����༭��ʾ","ȷ��ȫվ����������",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
	{
		PBNS::StationRuleMgrMsg_Request reqst;
		reqst.set_reqtype(1);
		QString sql = QString("UPDATE station_rule SET IsUse=1 ;");

		reqst.set_mgrsql(sql.toStdString());

		//���䷢������������Ϣ�ź�
		NetClient::instance()->sendData(CMD_STATION_RULE_MANAGER,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

		reqStaionRuleList();

		pstaallchbox->setChecked(false);
		checkAllstachbox();

		return;
	}
	else
	{
		pstaallchbox->setChecked(false);
		checkAllstachbox();
		return;
	}

}

void RuleMgrdlg::reqStaionRuleList()
{
	PBNS::StationRuleListMsg_Request srlreq;

	QString strData;
	strData.append("1");

	srlreq.set_reqdate("1");

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_STATION_RULE_LIST,srlreq.SerializeAsString().c_str(),srlreq.SerializeAsString().length());

	return;
}


void RuleMgrdlg::retStationRuleManager(int msgtype,const char* msg)
{
	switch (msgtype)
	{
	case CMD_STATION_RULE_LIST:
		retStaionRuleList(msg);
		break;
	case CMD_STATION_RULE_MGR:
		{
			PBNS::StationRuleMgrMsg_Response rep;
			rep.ParseFromString(msg);

			if (rep.rescode() > 0)
			{
				//���ɾ����������д�볧վ���������ϵ�����ݳɹ���:���¼��ع������ͺͳ�վ��������б�����
				loadRuleTypeList();

				reqStaionRuleList();
			}
			
		}
		break;
	}
}

void RuleMgrdlg::retStaionRuleList(const char* msg)
{
	m_starulevct.clear();

	PBNS::StationRuleListMsg_Response resp;
	resp.ParseFromString(msg);

	int nrow = resp.starulelist_size();

	//��������
	//ptabwdgsrlist->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::StationRuleBean srbean = resp.starulelist(i);
		QString stcimid = QString::fromStdString(srbean.stationcim());

		QString rid = QString("%1").arg(srbean.ruleid());
		//QString isuse = QString("%1").arg(srbean.isuse());
		/*
		ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(stcimid));											//�����и�ֵ
		ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(QString::fromStdString(srbean.stationname())));
		ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(rid));												//�����и�ֵ
		ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(QString::fromStdString(srbean.rulename())));
		ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(QString::fromStdString(srbean.isuse())));			//�����и�ֵ
		ptabwdgsrlist->setItem(i,5,new QTableWidgetItem(QString::fromStdString(srbean.isusename())));
		*/

		StationRule_S stationrule;
		stationrule.stationCim = stcimid;
		stationrule.stationName = srbean.stationname().c_str();
		stationrule.ruleId = srbean.ruleid();
		stationrule.ruleName = srbean.rulename().c_str();
		stationrule.isUse = srbean.isuse().c_str();
		stationrule.isUseName = srbean.isusename().c_str();

		m_starulevct.push_back(stationrule);

	}

	//���ݰ�ʲô������ʾ���ݸ��º󻹰�ʲô������ʾ
	if (pstachbox->isChecked())
	{
		int idx = pstacbox->currentIndex();
		pstacbox->setCurrentIndex(pstacbox->currentIndex());
		ChangeStaCbox(pstacbox->currentIndex());
	}

	if (prulechbox->isChecked())
	{
		prulecbox->setCurrentIndex(prulecbox->currentIndex());
		ChangeRuleCbox(prulecbox->currentIndex());
	}

}


void RuleMgrdlg::createRuleTypeMgrDlg()
{
	RuleTypeMgrdlg::instance()->setWindowIcon(QIcon(":images/rolemgr.png"));
	RuleTypeMgrdlg::instance()->setWindowTitle("�������");

	RuleTypeMgrdlg::instance()->exec();

	return;
}


void RuleMgrdlg::cellChangeIsUse(int row, int col)
{
	//��δ��ѡ�ύʱ�޸Ĳ���Ч
	if (!pcmitchbox->isChecked())
	{
		return;
	}

	//Ԫ�ر仯��־���仯������
	if (!bcellchageflag)
	{
		return;
	}

	if (col != 5)
	{
		return;
	}
	
	int nrow = ptabwdgsrlist->currentRow();

	StationRule_S strules;

	strules.stationCim = ptabwdgsrlist->item(row,0)->text();
	strules.stationName = ptabwdgsrlist->item(row,1)->text();
	strules.ruleId = ptabwdgsrlist->item(row,2)->text().toInt();
	strules.ruleName = ptabwdgsrlist->item(row,3)->text();

	//��ѡ�б�ʾ���÷����ʾδ����
	if (ptabwdgsrlist->item(row,5)->checkState() == Qt::Checked)
	{
		strules.isUse = QString("%1").arg(1);
	}
	else
	{
		strules.isUse = QString("%1").arg(0);
	}

	//�����������Ƿ����޸ĵĳ�վ�����־
	bool bfind = false;

	//�����������ұ༭�ĳ�վ�����Ƿ��Ѿ��༭���洢��������
	for (int i=0;i<m_strulemdfvct.size();i++)
	{
		//�����������д˱༭�ĳ�վ������Ҫ���³�վ�ṹ�����Ƿ�����ֵֵ
		if (strules.stationCim == m_strulemdfvct.at(i).stationCim && strules.ruleId == m_strulemdfvct.at(i).ruleId && strules.isUse != m_strulemdfvct.at(i).isUse)
		{
			m_strulemdfvct.at(i).isUse = strules.isUse;

			bfind = true;
			break;
		}
	}

	if (!bfind)
	{
		m_strulemdfvct.push_back(strules);
	}

	return;
}


void RuleMgrdlg::checkCommitchbox()
{
	if (pcmitchbox->isChecked())
	{
		pbtncommit->setEnabled(true);

		//���ݰ�ʲô������ʾ���ݸ��º󻹰�ʲô������ʾ
		if (pstachbox->isChecked())
		{
			int idx = pstacbox->currentIndex();
			pstacbox->setCurrentIndex(pstacbox->currentIndex());
			ChangeStaCbox(pstacbox->currentIndex());
		}

		if (prulechbox->isChecked())
		{
			prulecbox->setCurrentIndex(prulecbox->currentIndex());
			ChangeRuleCbox(prulecbox->currentIndex());
		}

		bcellchageflag = true;
	}
	else
	{
		pbtncommit->setEnabled(false);
		bcellchageflag = false;
	}
}

void RuleMgrdlg::commitStationRule()
{
	int ncount = m_strulemdfvct.size();
	if (ncount <= 0 )
	{
		QMessageBox::information(this,tr("��վ�����޸�"),tr("δ�Գ�վ���������Ϣ�����޸�,�����ύ!"));
		pcmitchbox->setChecked(false);
		checkCommitchbox();
		return;
	}

	//����ύ������Ҫ�ύ�ĳ�վ������ϵ�б��е��Ƿ�������
	PBNS::StationRuleMgrMsg_Request reqst;
	reqst.set_reqtype(3);

	for (int i=0;i<m_strulemdfvct.size();i++)
	{
		PBNS::StationRuleBean *strbean = reqst.add_stationrllist();
		strbean->set_stationcim(m_strulemdfvct.at(i).stationCim.toStdString());
		strbean->set_stationname(m_strulemdfvct.at(i).stationName.toStdString());
		strbean->set_ruleid(m_strulemdfvct.at(i).ruleId);
		strbean->set_rulename(m_strulemdfvct.at(i).ruleName.toStdString());
		strbean->set_isuse(m_strulemdfvct.at(i).isUse.toStdString());

	}

	string reqstr;
	reqst.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_STATION_RULE_MANAGER,reqstr.c_str(),reqstr.length());

	//�ύ��ɺ���ձ����޸ĵ�����
	m_strulemdfvct.clear();

	pcmitchbox->setChecked(false);
	checkCommitchbox();

	reqStaionRuleList();

	return;
}


/************************************************************************/
/*                         �������ͱ༭����                             */
/************************************************************************/
RuleTypeMgrdlg* RuleTypeMgrdlg::m_inst = NULL;

RuleTypeMgrdlg* RuleTypeMgrdlg::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new RuleTypeMgrdlg;

	}
	return m_inst;
}

RuleTypeMgrdlg::RuleTypeMgrdlg()
	: QDialog()
{
	//ֻ��ʾ�رհ�ť
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->resize(520,650);

	QGridLayout *pglyt = new QGridLayout;

	QHBoxLayout *phlyt = new QHBoxLayout;
	QHBoxLayout *phlayt = new QHBoxLayout;
	QHBoxLayout *puhlayt = new QHBoxLayout;
	QHBoxLayout *poprhlayt = new QHBoxLayout;
	QVBoxLayout *pvlyt = new QVBoxLayout;
	QVBoxLayout *pmgrvlyt = new QVBoxLayout;
	QVBoxLayout *pmainvlyt = new QVBoxLayout;

	QGroupBox *pulisgbox = new QGroupBox("�����б�");
	ptabwdgrlist = new QTableWidget();
	//���ñ���ɫΪ��ɫ
	QPalette tabplt = ptabwdgrlist->palette();
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	ptabwdgrlist->setPalette(tabplt);
	//��������
	ptabwdgrlist->setColumnCount(5);
	QStringList header;
	header<<"����ID"<<"��������"<<"���յȼ���"<<"���յȼ�"<<"��������";
	ptabwdgrlist->setHorizontalHeaderLabels(header);
	//ʹ����ͷ����Ӧ��ȣ�������ƽ���������հײ���
	ptabwdgrlist->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//�б�����ɫ�仯
	ptabwdgrlist->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	ptabwdgrlist->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	ptabwdgrlist->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	ptabwdgrlist->setSelectionBehavior(QAbstractItemView::SelectRows);
	//ȥ��ÿ�е��к�
	//QHeaderView *headerView = ptabwdgulist->verticalHeader();
	//headerView->setHidden(true);


	QGroupBox *pusgbox = new QGroupBox("������ʾ");
	pridlab = new QLabel("  ����ID");
	pridlab->setFixedWidth(50);
	pridlied = new QLineEdit;
	pridlied->setFixedWidth(145);
	pridlied->setFixedHeight(25);
	pridpbox = new QSpinBox;
	pridpbox->setFixedWidth(145);
	pridpbox->setFixedHeight(25);

	plevlab = new QLabel("���յȼ�");
	plevlab->setFixedWidth(50);
	plevcbx = new QComboBox;
	plevcbx->setFixedWidth(145);
	plevcbx->setFixedHeight(25);

	prnamelab = new QLabel("��������");
	prnamelab->setFixedWidth(50);
	prnamelined = new QLineEdit;
	prnamelined->setFixedWidth(145);
	prnamelined->setFixedHeight(25);

	pdesclab = new QLabel("��������");
	pdesclab->setFixedWidth(50);
	pdescld = new QLineEdit;
	pdescld->setFixedWidth(145);
	pdescld->setFixedHeight(25);


	QGroupBox *poprgbox = new QGroupBox("����");
	pbntadd = new QPushButton("�� ��");
	pbntadd->setFixedHeight(30);

	pbntdel = new QPushButton("ɾ ��");
	pbntdel->setFixedHeight(30);

	pbntmdf = new QPushButton("�� ��");
	pbntmdf->setFixedHeight(30);


	puhlayt->addWidget(ptabwdgrlist);
	pulisgbox->setLayout(puhlayt);

	phlyt->addWidget(pridlab);
	phlyt->addWidget(pridpbox);
	phlyt->addStretch();
	phlyt->addWidget(plevlab);
	phlyt->addWidget(plevcbx);

	phlayt->addWidget(prnamelab);
	phlayt->addWidget(prnamelined);
	phlayt->addStretch();
	phlayt->addWidget(pdesclab);
	phlayt->addWidget(pdescld);

	pmgrvlyt->addLayout(phlyt);
	pmgrvlyt->addSpacing(5);
	pmgrvlyt->addLayout(phlayt);
	pusgbox->setLayout(pmgrvlyt);

	poprhlayt->addWidget(pbntadd);
	poprhlayt->addWidget(pbntdel);
	poprhlayt->addWidget(pbntmdf);
	poprgbox->setLayout(poprhlayt);

	pmainvlyt->addWidget(pulisgbox);
	pmainvlyt->addSpacing(20);
	pmainvlyt->addWidget(pusgbox);
	pmainvlyt->addSpacing(20);
	pmainvlyt->addWidget(poprgbox);
	pmainvlyt->addSpacing(5);

	pmainvlyt->setContentsMargins(8,8,8,20);

	this->setLayout(pmainvlyt);

	//���ع���༭�б�����з��յȼ���
	ptabwdgrlist->setColumnHidden(2,true);

	//Ĭ�����ù���ID,���յȼ�,��������,����������Ϊֻ��
	pridpbox->setDisabled(true);
	plevcbx->setDisabled(true);
	prnamelined->setDisabled(true);
	pdescld->setDisabled(true);

	//����Ĭ�Ϲ�����յȼ�
	plevcbx->clear();
	setRulelevel();

	//Ĭ�����ò�����ť״̬��δѡ���û��б�ʱֻ�������û�,���Ӱ�ť��Ч��ɾ�����޸İ�ť��Ч
	pbntadd->setEnabled(true);
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

	//Ĭ������û���־λfalse
	m_addflg = false;
	m_delflg = false;
	baddflag = false;
	bmdfflag = false;

	//��������б�
	reqRuleList();

	//��������б�ĳ���źŲ۹���
	connect(ptabwdgrlist,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getRuleItem(QTableWidgetItem*)));
	//������ť�źŲ۹���
	connect(pbntadd,SIGNAL(clicked()),this,SLOT(addRule()));
	connect(pbntdel,SIGNAL(clicked()),this,SLOT(delRule()));
	connect(pbntmdf,SIGNAL(clicked()),this,SLOT(mdfRule()));

}

RuleTypeMgrdlg::~RuleTypeMgrdlg()
{
	if (m_inst != NULL)
	{
		delete m_inst;

		m_inst = NULL;
	}

}

void RuleTypeMgrdlg::setRulelevel()
{
	//����Ĭ����ʾ������յȼ�
	plevcbx->insertItem(0,"",-1);
	//���ù�����յȼ�
	plevcbx->insertItem(1,"��",0);
	plevcbx->insertItem(2,"��",1);
	plevcbx->insertItem(3,"��",2);
}

void RuleTypeMgrdlg::recvdata(int msgtype,const char* msg,int msglength)
{

	switch (msgtype)
	{
	case CMD_RULE_ADD:
		{
			PBNS::RuleMgrMsg_Response resp;
			resp.ParseFromString(msg);

			if (resp.rescode() > 0)
			{
				//��ӹ���ɹ�ʱ���¼��ع�������
				ComUtil::instance()->getRuleType();

				if (m_addflg)
				{
					PBNS::StationRuleMgrMsg_Request reqst;
					reqst.set_reqtype(1);

					QString sql = QString("INSERT INTO station_rule(StationCim,RuleId) SELECT st.CimId,rl.ID from stations st,rules rl where rl.ID=%1 ;").arg(m_addrid);

					reqst.set_mgrsql(sql.toStdString());

					//���䷢������������Ϣ�ź�
					NetClient::instance()->sendData(CMD_STATION_RULE_MGR,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

					m_addflg = false;

				}
				
			}
			resp.rescode();
		}
		break;
	case CMD_RULE_DEL:
		{
			PBNS::RuleMgrMsg_Response resp;
			resp.ParseFromString(msg);

			if (resp.rescode() > 0)
			{
				//ɾ������ɹ�ʱ���¼��ع�������
				ComUtil::instance()->getRuleType();

				if (m_delflg)
				{
					PBNS::StationRuleMgrMsg_Request reqst;
					reqst.set_reqtype(1);

					QString sql = QString("delete FROM station_rule where RuleId=%1 ;").arg(m_delrid);

					reqst.set_mgrsql(sql.toStdString());

					//���䷢������������Ϣ�ź�
					NetClient::instance()->sendData(CMD_STATION_RULE_MGR,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

					m_delflg = false;
				}
			}
		}
		break;
	case CMD_RULE_MODIFY:
		{
			PBNS::RuleMgrMsg_Response resp;
			resp.ParseFromString(msg);

			if (resp.rescode() > 0)
			{
				//ɾ������ɹ�ʱ���¼��ع�������
				ComUtil::instance()->getRuleType();

				PBNS::StationRuleMgrMsg_Request reqst;
				reqst.set_reqtype(2);

				QString sql = QString("rule modify").arg(m_delrid);

				reqst.set_mgrsql(sql.toStdString());

				//���䷢������������Ϣ�ź�
				NetClient::instance()->sendData(CMD_STATION_RULE_MGR,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());
			}
		}
		break;
	case CMD_RULE_LIST:
		retRuleList(msg);
		break;
	}

}

void RuleTypeMgrdlg::reqRuleList()
{
	PBNS::RuleListMsg_Request rlreq;

	QString strData;
	strData.append("1");

	rlreq.set_reqdate("1");

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_RULE_LIST,rlreq.SerializeAsString().c_str(),rlreq.SerializeAsString().length());

	return;
}

void RuleTypeMgrdlg::retRuleList(const char* msg)
{
	PBNS::RuleListMsg_Response resp;
	resp.ParseFromString(msg);

	int nrow = resp.rulelist_size();

	//��������
	ptabwdgrlist->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::RuleBean rbean = resp.rulelist(i);
		QString rid = QString("%1").arg(rbean.id());

		QString rname = QString::fromStdString(rbean.name());
		int rlev = QString::fromStdString(rbean.alarmlevel()).toInt();

		ptabwdgrlist->setItem(i,0,new QTableWidgetItem(rid));
		ptabwdgrlist->setItem(i,1,new QTableWidgetItem(QString::fromStdString(rbean.name())));
		ptabwdgrlist->setItem(i,2,new QTableWidgetItem(QString::fromStdString(rbean.alarmlevel())));		//�����и�ֵ
		ptabwdgrlist->setItem(i,3,new QTableWidgetItem(plevcbx->itemText(rlev+1)));
		ptabwdgrlist->setItem(i,4,new QTableWidgetItem(QString::fromStdString(rbean.description())));

	}

	//δѡ�й����б�ʱֻ�����ӹ���,���Ӱ�ť��Ч��ɾ�����޸İ�ť��Ч
	pbntadd->setEnabled(true);
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

}

void RuleTypeMgrdlg::getRuleItem(QTableWidgetItem* item)
{
	int col = item->column();
	int row = item->row();

	int nruleid = ptabwdgrlist->item(row,0)->text().toInt();

	//pridlied->setText(ptabwdgrlist->item(row,0)->text());
	pridpbox->setValue(ptabwdgrlist->item(row,0)->text().toInt());

	prnamelined->setText(ptabwdgrlist->item(row,1)->text());

	plevcbx->setCurrentIndex(ptabwdgrlist->item(row,2)->text().toInt()+1);

	pdescld->setText(ptabwdgrlist->item(row,4)->text());


	//���ò�����ť״̬
	pbntdel->setEnabled(true);
	pbntmdf->setEnabled(true);
}


void RuleTypeMgrdlg::addRule()
{
	if (!baddflag)
	{
		pridpbox->clear();
		prnamelined->clear();

		plevcbx->setCurrentIndex(0);
		pdescld->clear();

		pridpbox->setDisabled(false);
		prnamelined->setDisabled(false);
		plevcbx->setDisabled(false);
		pdescld->setDisabled(false);

		//���ò�����ť״̬
		pbntdel->setEnabled(false);
		pbntmdf->setEnabled(false);

		baddflag = true;
	}
	else
	{
		if (pridpbox->text()=="")
		{
			QMessageBox::information(this,tr("���ӹ�����ʾ:"),tr("����ID����Ϊ��,�������������!"));
			return;
		}

		if (prnamelined->text()=="")
		{
			QMessageBox::information(this,tr("���ӹ�����ʾ:"),tr("�������Ʋ���Ϊ��,�������������!"));
			return;
		}

		if (plevcbx->currentText()=="")
		{
			QMessageBox::information(this,tr("���ӹ�����ʾ:"),tr("����ȼ�����Ϊ��,��ѡ�����ȼ�!"));
			return;
		}

		if (pdescld->text()=="")
		{
			QMessageBox::information(this,tr("���ӹ�����ʾ:"),tr("������������Ϊ��,�������������!"));
			return;
		}

		//insert into rules (ID,Name,AlarmLevel,Description) VALUES (1,'����test',3,'����testtest');
		QString sql = QString("insert into rules (ID,Name,AlarmLevel,Description) VALUES (%1,'%2',%3,'%4');").arg(pridpbox->text()).arg(prnamelined->text()).arg(plevcbx->itemData(plevcbx->currentIndex()).toInt()).arg(pdescld->text());

		PBNS::RuleMgrMsg_Request addrreq;
		addrreq.set_mgrsql(sql.toStdString());

		NetClient::instance()->sendData(CMD_RULE_ADD,addrreq.SerializeAsString().c_str(),addrreq.SerializeAsString().length());

		baddflag = false;
		pridpbox->setDisabled(true);
		prnamelined->setDisabled(true);
		plevcbx->setDisabled(true);
		pdescld->setDisabled(true);

		//������ӹ����־��ID��
		m_addflg = true;
		m_addrid = pridpbox->text().toInt();

		//���ӹ�����ɺ�,��������б�
		reqRuleList();
	}
}

void RuleTypeMgrdlg::delRule()
{
	int ncurrow = ptabwdgrlist->currentRow();
	//ȡ�����б���ѡ���еĵ�һ���û�id��
	int nRuleid = ptabwdgrlist->item(ncurrow,0)->text().toInt();

	QString sql = QString("delete from rules where ID=%1").arg(nRuleid);

	PBNS::RuleMgrMsg_Request delrreq;
	delrreq.set_mgrsql(sql.toStdString());

	NetClient::instance()->sendData(CMD_RULE_DEL,delrreq.SerializeAsString().c_str(),delrreq.SerializeAsString().length());

	//���ò�����ť״̬
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

	//������ӹ����־��ID��
	m_delflg = true;
	m_delrid = nRuleid;

	//ɾ��������ɺ�,�����û��б�
	reqRuleList();
}

void RuleTypeMgrdlg::mdfRule()
{
	if (!bmdfflag)
	{
		prnamelined->setDisabled(false);
		plevcbx->setDisabled(false);
		pdescld->setDisabled(false);

		//���ò�����ť״̬
		pbntadd->setEnabled(false);
		pbntdel->setEnabled(false);

		bmdfflag = true;
	} 
	else
	{
		if (prnamelined->text()=="")
		{
			QMessageBox::information(this,tr("���ӹ�����ʾ:"),tr("�������Ʋ���Ϊ��,�������������!"));
			return;
		}

		if (plevcbx->currentText()=="")
		{
			QMessageBox::information(this,tr("���ӹ�����ʾ:"),tr("����ȼ�����Ϊ��,��ѡ�����ȼ�!"));
			return;
		}

		if (pdescld->text()=="")
		{
			QMessageBox::information(this,tr("���ӹ�����ʾ:"),tr("������������Ϊ��,�������������!"));
			return;
		}

		int ncurrow = ptabwdgrlist->currentRow();
		//ȡ�����б���ѡ���еĵ�һ�й���id��
		int nRuleid = ptabwdgrlist->item(ncurrow,0)->text().toInt();

		//UPDATE rules set Name='����41',AlarmLevel=3,Description='����four' where id=4
		QString sql = QString("UPDATE rules set Name='%1' ,AlarmLevel=%2 ,Description='%3' where ID=%4 ;").arg(prnamelined->text()).arg(plevcbx->itemData(plevcbx->currentIndex()).toInt()).arg(pdescld->text()).arg(nRuleid);

		PBNS::RuleMgrMsg_Request mdfrreq;
		mdfrreq.set_mgrsql(sql.toStdString());

		NetClient::instance()->sendData(CMD_RULE_MODIFY,mdfrreq.SerializeAsString().c_str(),mdfrreq.SerializeAsString().length());

		bmdfflag = false;
		prnamelined->setDisabled(true);
		plevcbx->setDisabled(true);
		pdescld->setDisabled(true);

		//�޸Ĺ�����ɺ�,�����û��б�
		reqRuleList();

	}
}