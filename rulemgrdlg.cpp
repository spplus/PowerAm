#include "rulemgrdlg.h"


RuleMgrdlg::RuleMgrdlg(QWidget *parent)
	: QDialog(parent)
{
	//只显示关闭按钮
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

	QGroupBox *pstalisgbox = new QGroupBox("按厂站策略");
	pstachbox = new QCheckBox;
	pstacbox = new QComboBox;
	pstacbox->setFixedWidth(250);
	pstacbox->setFixedHeight(25);
	pbtnstause = new QPushButton("按厂站启用");
	pbtnstause->setFixedHeight(30);
	pbtnstanouse = new QPushButton("按厂站禁用");
	pbtnstanouse->setFixedHeight(30);

	QGroupBox *prulelisgbox = new QGroupBox("按规则策略");
	prulechbox = new QCheckBox;
	prulecbox = new QComboBox;
	prulecbox->setFixedWidth(250);
	prulecbox->setFixedHeight(25);
	pbtnruleuse = new QPushButton("按规则启用");
	pbtnruleuse->setFixedHeight(30);
	pbtnrulenouse = new QPushButton("按规则禁用");
	pbtnrulenouse->setFixedHeight(30);

	QGroupBox *poprgbox = new QGroupBox("操作");
	pstaallchbox = new QCheckBox;
	pbtnalluse = new QPushButton("全站规则启用");
	pbtnalluse->setFixedHeight(30);

	pcmitchbox = new QCheckBox;
	pbtncommit = new QPushButton("提  交");
	pbtncommit->setFixedHeight(30);


	QGroupBox *psrlisgbox = new QGroupBox("厂站规则关联关系列表");
	ptabwdgsrlist = new QTableWidget();
	//设置背景色为白色
	QPalette tabplt = ptabwdgsrlist->palette();
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	ptabwdgsrlist->setPalette(tabplt);
	//设置列数
	ptabwdgsrlist->setColumnCount(6);
	QStringList header;
	header<<"厂站CIMID"<<"厂站名称"<<"规则ID"<<"规则名称"<<"是否启用值"<<"是否启用";
	ptabwdgsrlist->setHorizontalHeaderLabels(header);
	//使行列头自适应宽度，所有列平均分来填充空白部分
	ptabwdgsrlist->horizontalHeader()->setResizeMode(QHeaderView::Stretch); //QHeaderView::ResizeToContents QHeaderView::Stretch
	//行背景颜色变化
	ptabwdgsrlist->setAlternatingRowColors(true);
	//设置每行内容不可编辑
	ptabwdgsrlist->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置只能选择一行，不能选择多行
	ptabwdgsrlist->setSelectionMode(QAbstractItemView::SingleSelection);
	//设置单击选择一行
	ptabwdgsrlist->setSelectionBehavior(QAbstractItemView::SelectRows);

	//去掉每行的行号
	//QHeaderView *headerView = ptabwdgsrlist->verticalHeader();
	//headerView->setHidden(true);

	//隐藏站点规则关联关系列表第一、三、五列
	ptabwdgsrlist->setColumnHidden(0,true);
	ptabwdgsrlist->setColumnHidden(2,true);
	ptabwdgsrlist->setColumnHidden(4,true);

	pbtnruletype = new QPushButton("规则类型");
	pbtnruletype->setFixedHeight(30);

	

	//按厂站策略
	phlyt->addWidget(pstachbox);
	phlyt->addWidget(pstacbox);
	phlyt->addSpacing(100);
	phlyt->addWidget(pbtnstause);
	phlyt->addWidget(pbtnstanouse);
	pstalisgbox->setLayout(phlyt);

	//按规则策略
	phlayt->addWidget(prulechbox);
	phlayt->addWidget(prulecbox);
	phlayt->addSpacing(100);
	phlayt->addWidget(pbtnruleuse);
	phlayt->addWidget(pbtnrulenouse);
	prulelisgbox->setLayout(phlayt);

	//操作
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

	//厂站规则关联关系列表
	psrhlayt->addWidget(ptabwdgsrlist);
	psrlisgbox->setLayout(psrhlayt);

	pmainvlyt->addWidget(pstalisgbox);
	pmainvlyt->addWidget(prulelisgbox);
	pmainvlyt->addWidget(poprgbox);
	pmainvlyt->addWidget(psrlisgbox);

	pmainvlyt->addSpacing(5);

	pmainvlyt->setContentsMargins(20,10,20,20);

	this->setLayout(pmainvlyt);

	//加载厂站列表和规则类型列表
	loadSationList();
	loadRuleTypeList();

	//操作按钮信号槽关联
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
	//全站启用
	connect(pbtnalluse,SIGNAL(clicked()),this,SLOT(allStationRuleUse()));
	connect(pbtncommit,SIGNAL(clicked()),this,SLOT(commitStationRule()));

	connect(pbtnruletype,SIGNAL(clicked()),this,SLOT(createRuleTypeMgrDlg()));
	connect(ptabwdgsrlist,SIGNAL(cellChanged(int,int)),this,SLOT(cellChangeIsUse(int,int)));
	

	//默认选择按厂站显示
	pstachbox->setChecked(true);
	checkStachbox();

	pstacbox->setCurrentIndex(0);

	pbtnalluse->setEnabled(false);
	pbtncommit->setEnabled(false);
	//初始清空容器
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

		//按当前已设置的厂站显示列表
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

		//按当前设置的规则显示列表
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

		//按当前设置的规则显示列表
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

		//按当前已设置的厂站显示列表
		ChangeStaCbox(pstacbox->currentIndex());
	}

}

void RuleMgrdlg::ChangeStaCbox(int idx)
{
	QString stacimid = pstacbox->itemData(idx).toString();
	m_strstacimid = stacimid;
	vector<StationRule_S> strulevect;

	if (stacimid == "全部厂站")
	{
		ptabwdgsrlist->clearContents();
		int nrow = m_starulevct.size();
		ptabwdgsrlist->setRowCount(nrow);

		for (int i=0;i<m_starulevct.size();i++)
		{
			StationRule_S strule = m_starulevct.at(i);
			int nIsUse = strule.isUse.toInt();
			ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(strule.stationCim));										//隐藏列赋值
			ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(strule.stationName));
			ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(QString("%1").arg(strule.ruleId)));							//隐藏列赋值
			ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(strule.ruleName));
			ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(strule.isUse));												//隐藏列赋值
			
			//在第6列种嵌入checkbox并设置状态值
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
			cboxUse->insertItem(0,"否",0);
			cboxUse->insertItem(1,"是",1);
			cboxUse->insertItem(2,"未知",2);

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
	
	if (stacimid != "全部厂站")
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
			ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(strl.stationCim));											//隐藏列赋值
			ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(strl.stationName));
			ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(QString("%1").arg(strl.ruleId)));							//隐藏列赋值
			ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(strl.ruleName));
			ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(strl.isUse));												//隐藏列赋值

			//在第6列种嵌入checkbox并设置状态值
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

	//按厂站保存厂站规则关系列表容器
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
			ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(strule.stationCim));										//隐藏列赋值
			ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(strule.stationName));
			ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(QString("%1").arg(strule.ruleId)));							//隐藏列赋值
			ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(strule.ruleName));
			ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(strule.isUse));												//隐藏列赋值
			
			//在第6列种嵌入checkbox并设置状态值
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
			ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(strl.stationCim));											//隐藏列赋值
			ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(strl.stationName));
			ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(QString("%1").arg(strl.ruleId)));							//隐藏列赋值
			ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(strl.ruleName));
			ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(strl.isUse));												//隐藏列赋值
			
			//在第6列种嵌入checkbox并设置状态值
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
	
	//按规则保存厂站规则关系列表容器
	m_rulestvct = strulevect;
}

void RuleMgrdlg::loadSationList()
{
	m_stationvct.clear();
	pstacbox->clear();

	m_stationvct = ComUtil::instance()->getStationMgrList();

	int nrow = m_stationvct.size();

	pstacbox->insertItem(0,QString("全部"),QString("全部厂站"));

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
	prulecbox->insertItem(0,QString("全部"),QString("全部规则"));

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
	if (m_strstacimid == "全部厂站")
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

	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_STATION_RULE_MANAGER,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

	reqStaionRuleList();

	return;

}

void RuleMgrdlg::checkStationRuleNoUse()
{
	PBNS::StationRuleMgrMsg_Request reqst;
	reqst.set_reqtype(1);

	QString sql;
	if (m_strstacimid == "全部厂站")
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

	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_STATION_RULE_MANAGER,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

	reqStaionRuleList();

	return;
}

void RuleMgrdlg::checkRuleUse()
{
	PBNS::StationRuleMgrMsg_Request reqst;
	reqst.set_reqtype(1);

	QString sql;
	if (m_strstacimid == "全部规则")
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

	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_STATION_RULE_MANAGER,reqst.SerializeAsString().c_str(),reqst.SerializeAsString().length());

	reqStaionRuleList();

	return;
}

void RuleMgrdlg::checkRuleNoUse()
{
	PBNS::StationRuleMgrMsg_Request reqst;
	reqst.set_reqtype(1);

	QString sql;
	if (m_strstacimid == "全部规则")
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

	//发射发送数据请求消息信号
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
	if(QMessageBox::question(this,"规则编辑提示","确定全站规则启用吗？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
	{
		PBNS::StationRuleMgrMsg_Request reqst;
		reqst.set_reqtype(1);
		QString sql = QString("UPDATE station_rule SET IsUse=1 ;");

		reqst.set_mgrsql(sql.toStdString());

		//发射发送数据请求消息信号
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

	//发射发送数据请求消息信号
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
				//添加删除规则后关联写入厂站规则关联关系表数据成功后:重新加载规则类型和厂站规则关联列表数据
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

	//设置行数
	//ptabwdgsrlist->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::StationRuleBean srbean = resp.starulelist(i);
		QString stcimid = QString::fromStdString(srbean.stationcim());

		QString rid = QString("%1").arg(srbean.ruleid());
		//QString isuse = QString("%1").arg(srbean.isuse());
		/*
		ptabwdgsrlist->setItem(i,0,new QTableWidgetItem(stcimid));											//隐藏列赋值
		ptabwdgsrlist->setItem(i,1,new QTableWidgetItem(QString::fromStdString(srbean.stationname())));
		ptabwdgsrlist->setItem(i,2,new QTableWidgetItem(rid));												//隐藏列赋值
		ptabwdgsrlist->setItem(i,3,new QTableWidgetItem(QString::fromStdString(srbean.rulename())));
		ptabwdgsrlist->setItem(i,4,new QTableWidgetItem(QString::fromStdString(srbean.isuse())));			//隐藏列赋值
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

	//根据按什么策略显示数据更新后还按什么策略显示
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
	RuleTypeMgrdlg::instance()->setWindowTitle("规则管理");

	RuleTypeMgrdlg::instance()->exec();

	return;
}


void RuleMgrdlg::cellChangeIsUse(int row, int col)
{
	//在未勾选提交时修改不生效
	if (!pcmitchbox->isChecked())
	{
		return;
	}

	//元素变化标志不变化不处理
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

	//当选中表示启用否则表示未启用
	if (ptabwdgsrlist->item(row,5)->checkState() == Qt::Checked)
	{
		strules.isUse = QString("%1").arg(1);
	}
	else
	{
		strules.isUse = QString("%1").arg(0);
	}

	//设置容器中是否有修改的厂站规则标志
	bool bfind = false;

	//遍历容器查找编辑的厂站规则是否已经编辑过存储在容器中
	for (int i=0;i<m_strulemdfvct.size();i++)
	{
		//若容器中已有此编辑的厂站规则，需要更新厂站结构体中是否启用值值
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

		//根据按什么策略显示数据更新后还按什么策略显示
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
		QMessageBox::information(this,tr("厂站规则修改"),tr("未对厂站规则关联信息进行修改,不需提交!"));
		pcmitchbox->setChecked(false);
		checkCommitchbox();
		return;
	}

	//组合提交具体需要提交的厂站关联关系列表中的是否启用项
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

	//提交完成后清空保存修改的容器
	m_strulemdfvct.clear();

	pcmitchbox->setChecked(false);
	checkCommitchbox();

	reqStaionRuleList();

	return;
}


/************************************************************************/
/*                         规则类型编辑管理                             */
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
	//只显示关闭按钮
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

	QGroupBox *pulisgbox = new QGroupBox("规则列表");
	ptabwdgrlist = new QTableWidget();
	//设置背景色为白色
	QPalette tabplt = ptabwdgrlist->palette();
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	ptabwdgrlist->setPalette(tabplt);
	//设置列数
	ptabwdgrlist->setColumnCount(5);
	QStringList header;
	header<<"规则ID"<<"规则名称"<<"风险等级号"<<"风险等级"<<"规则描述";
	ptabwdgrlist->setHorizontalHeaderLabels(header);
	//使行列头自适应宽度，所有列平均分来填充空白部分
	ptabwdgrlist->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//行背景颜色变化
	ptabwdgrlist->setAlternatingRowColors(true);
	//设置每行内容不可编辑
	ptabwdgrlist->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置只能选择一行，不能选择多行
	ptabwdgrlist->setSelectionMode(QAbstractItemView::SingleSelection);
	//设置单击选择一行
	ptabwdgrlist->setSelectionBehavior(QAbstractItemView::SelectRows);
	//去掉每行的行号
	//QHeaderView *headerView = ptabwdgulist->verticalHeader();
	//headerView->setHidden(true);


	QGroupBox *pusgbox = new QGroupBox("规则显示");
	pridlab = new QLabel("  规则ID");
	pridlab->setFixedWidth(50);
	pridlied = new QLineEdit;
	pridlied->setFixedWidth(145);
	pridlied->setFixedHeight(25);
	pridpbox = new QSpinBox;
	pridpbox->setFixedWidth(145);
	pridpbox->setFixedHeight(25);

	plevlab = new QLabel("风险等级");
	plevlab->setFixedWidth(50);
	plevcbx = new QComboBox;
	plevcbx->setFixedWidth(145);
	plevcbx->setFixedHeight(25);

	prnamelab = new QLabel("规则名称");
	prnamelab->setFixedWidth(50);
	prnamelined = new QLineEdit;
	prnamelined->setFixedWidth(145);
	prnamelined->setFixedHeight(25);

	pdesclab = new QLabel("规则描述");
	pdesclab->setFixedWidth(50);
	pdescld = new QLineEdit;
	pdescld->setFixedWidth(145);
	pdescld->setFixedHeight(25);


	QGroupBox *poprgbox = new QGroupBox("操作");
	pbntadd = new QPushButton("增 加");
	pbntadd->setFixedHeight(30);

	pbntdel = new QPushButton("删 除");
	pbntdel->setFixedHeight(30);

	pbntmdf = new QPushButton("修 改");
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

	//隐藏规则编辑列表第三列风险等级号
	ptabwdgrlist->setColumnHidden(2,true);

	//默认设置规则ID,风险等级,规则名称,规则描述框为只读
	pridpbox->setDisabled(true);
	plevcbx->setDisabled(true);
	prnamelined->setDisabled(true);
	pdescld->setDisabled(true);

	//设置默认规则风险等级
	plevcbx->clear();
	setRulelevel();

	//默认设置操作按钮状态，未选中用户列表时只可增加用户,增加按钮有效，删除和修改按钮无效
	pbntadd->setEnabled(true);
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

	//默认添加用户标志位false
	m_addflg = false;
	m_delflg = false;
	baddflag = false;
	bmdfflag = false;

	//请求规则列表
	reqRuleList();

	//点击规则列表某行信号槽关联
	connect(ptabwdgrlist,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getRuleItem(QTableWidgetItem*)));
	//操作按钮信号槽关联
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
	//设置默认显示规则风险等级
	plevcbx->insertItem(0,"",-1);
	//设置规则风险等级
	plevcbx->insertItem(1,"低",0);
	plevcbx->insertItem(2,"中",1);
	plevcbx->insertItem(3,"高",2);
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
				//添加规则成功时重新加载规则类型
				ComUtil::instance()->getRuleType();

				if (m_addflg)
				{
					PBNS::StationRuleMgrMsg_Request reqst;
					reqst.set_reqtype(1);

					QString sql = QString("INSERT INTO station_rule(StationCim,RuleId) SELECT st.CimId,rl.ID from stations st,rules rl where rl.ID=%1 ;").arg(m_addrid);

					reqst.set_mgrsql(sql.toStdString());

					//发射发送数据请求消息信号
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
				//删除规则成功时重新加载规则类型
				ComUtil::instance()->getRuleType();

				if (m_delflg)
				{
					PBNS::StationRuleMgrMsg_Request reqst;
					reqst.set_reqtype(1);

					QString sql = QString("delete FROM station_rule where RuleId=%1 ;").arg(m_delrid);

					reqst.set_mgrsql(sql.toStdString());

					//发射发送数据请求消息信号
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
				//删除规则成功时重新加载规则类型
				ComUtil::instance()->getRuleType();

				PBNS::StationRuleMgrMsg_Request reqst;
				reqst.set_reqtype(2);

				QString sql = QString("rule modify").arg(m_delrid);

				reqst.set_mgrsql(sql.toStdString());

				//发射发送数据请求消息信号
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

	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_RULE_LIST,rlreq.SerializeAsString().c_str(),rlreq.SerializeAsString().length());

	return;
}

void RuleTypeMgrdlg::retRuleList(const char* msg)
{
	PBNS::RuleListMsg_Response resp;
	resp.ParseFromString(msg);

	int nrow = resp.rulelist_size();

	//设置行数
	ptabwdgrlist->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::RuleBean rbean = resp.rulelist(i);
		QString rid = QString("%1").arg(rbean.id());

		QString rname = QString::fromStdString(rbean.name());
		int rlev = QString::fromStdString(rbean.alarmlevel()).toInt();

		ptabwdgrlist->setItem(i,0,new QTableWidgetItem(rid));
		ptabwdgrlist->setItem(i,1,new QTableWidgetItem(QString::fromStdString(rbean.name())));
		ptabwdgrlist->setItem(i,2,new QTableWidgetItem(QString::fromStdString(rbean.alarmlevel())));		//隐藏列赋值
		ptabwdgrlist->setItem(i,3,new QTableWidgetItem(plevcbx->itemText(rlev+1)));
		ptabwdgrlist->setItem(i,4,new QTableWidgetItem(QString::fromStdString(rbean.description())));

	}

	//未选中规则列表时只可增加规则,增加按钮有效，删除和修改按钮无效
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


	//设置操作按钮状态
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

		//设置操作按钮状态
		pbntdel->setEnabled(false);
		pbntmdf->setEnabled(false);

		baddflag = true;
	}
	else
	{
		if (pridpbox->text()=="")
		{
			QMessageBox::information(this,tr("增加规则提示:"),tr("规则ID不可为空,请输入规则名称!"));
			return;
		}

		if (prnamelined->text()=="")
		{
			QMessageBox::information(this,tr("增加规则提示:"),tr("规则名称不可为空,请输入规则名称!"));
			return;
		}

		if (plevcbx->currentText()=="")
		{
			QMessageBox::information(this,tr("增加规则提示:"),tr("规则等级不可为空,请选择规则等级!"));
			return;
		}

		if (pdescld->text()=="")
		{
			QMessageBox::information(this,tr("增加规则提示:"),tr("规则描述不可为空,请输入规则描述!"));
			return;
		}

		//insert into rules (ID,Name,AlarmLevel,Description) VALUES (1,'规则test',3,'规则testtest');
		QString sql = QString("insert into rules (ID,Name,AlarmLevel,Description) VALUES (%1,'%2',%3,'%4');").arg(pridpbox->text()).arg(prnamelined->text()).arg(plevcbx->itemData(plevcbx->currentIndex()).toInt()).arg(pdescld->text());

		PBNS::RuleMgrMsg_Request addrreq;
		addrreq.set_mgrsql(sql.toStdString());

		NetClient::instance()->sendData(CMD_RULE_ADD,addrreq.SerializeAsString().c_str(),addrreq.SerializeAsString().length());

		baddflag = false;
		pridpbox->setDisabled(true);
		prnamelined->setDisabled(true);
		plevcbx->setDisabled(true);
		pdescld->setDisabled(true);

		//设置添加规则标志和ID号
		m_addflg = true;
		m_addrid = pridpbox->text().toInt();

		//增加规则完成后,请求规则列表
		reqRuleList();
	}
}

void RuleTypeMgrdlg::delRule()
{
	int ncurrow = ptabwdgrlist->currentRow();
	//取规则列表中选中行的第一列用户id号
	int nRuleid = ptabwdgrlist->item(ncurrow,0)->text().toInt();

	QString sql = QString("delete from rules where ID=%1").arg(nRuleid);

	PBNS::RuleMgrMsg_Request delrreq;
	delrreq.set_mgrsql(sql.toStdString());

	NetClient::instance()->sendData(CMD_RULE_DEL,delrreq.SerializeAsString().c_str(),delrreq.SerializeAsString().length());

	//设置操作按钮状态
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

	//设置添加规则标志和ID号
	m_delflg = true;
	m_delrid = nRuleid;

	//删除规则完成后,请求用户列表
	reqRuleList();
}

void RuleTypeMgrdlg::mdfRule()
{
	if (!bmdfflag)
	{
		prnamelined->setDisabled(false);
		plevcbx->setDisabled(false);
		pdescld->setDisabled(false);

		//设置操作按钮状态
		pbntadd->setEnabled(false);
		pbntdel->setEnabled(false);

		bmdfflag = true;
	} 
	else
	{
		if (prnamelined->text()=="")
		{
			QMessageBox::information(this,tr("增加规则提示:"),tr("规则名称不可为空,请输入规则名称!"));
			return;
		}

		if (plevcbx->currentText()=="")
		{
			QMessageBox::information(this,tr("增加规则提示:"),tr("规则等级不可为空,请选择规则等级!"));
			return;
		}

		if (pdescld->text()=="")
		{
			QMessageBox::information(this,tr("增加规则提示:"),tr("规则描述不可为空,请输入规则描述!"));
			return;
		}

		int ncurrow = ptabwdgrlist->currentRow();
		//取规则列表中选中行的第一列规则id号
		int nRuleid = ptabwdgrlist->item(ncurrow,0)->text().toInt();

		//UPDATE rules set Name='规则41',AlarmLevel=3,Description='规则four' where id=4
		QString sql = QString("UPDATE rules set Name='%1' ,AlarmLevel=%2 ,Description='%3' where ID=%4 ;").arg(prnamelined->text()).arg(plevcbx->itemData(plevcbx->currentIndex()).toInt()).arg(pdescld->text()).arg(nRuleid);

		PBNS::RuleMgrMsg_Request mdfrreq;
		mdfrreq.set_mgrsql(sql.toStdString());

		NetClient::instance()->sendData(CMD_RULE_MODIFY,mdfrreq.SerializeAsString().c_str(),mdfrreq.SerializeAsString().length());

		bmdfflag = false;
		prnamelined->setDisabled(true);
		plevcbx->setDisabled(true);
		pdescld->setDisabled(true);

		//修改规则完成后,请求用户列表
		reqRuleList();

	}
}