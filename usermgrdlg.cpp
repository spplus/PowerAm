#include <string>
#include "usermgrdlg.h"

using namespace std;
UserMgrdlg::UserMgrdlg(QWidget *parent)
	: QDialog(parent)
{
	//ֻ��ʾ�رհ�ť
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->resize(600,650);

	QGridLayout *pglyt = new QGridLayout;

	QHBoxLayout *phlyt = new QHBoxLayout;
	QHBoxLayout *phlayt = new QHBoxLayout;
	QHBoxLayout *puhlayt = new QHBoxLayout;
	QHBoxLayout *poprhlayt = new QHBoxLayout;
	QVBoxLayout *pvlyt = new QVBoxLayout;
	QVBoxLayout *pmgrvlyt = new QVBoxLayout;
	QVBoxLayout *pmainvlyt = new QVBoxLayout;

	QGroupBox *pulisgbox = new QGroupBox("�û��б�");
	ptabwdgulist = new QTableWidget();
	//���ñ���ɫΪ��ɫ
	QPalette tabplt = ptabwdgulist->palette();
	//tabplt.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));//����͸��
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	ptabwdgulist->setPalette(tabplt);
	//��������
	ptabwdgulist->setColumnCount(6);
	QStringList header;
	header<<"�û�ID"<<"�û���"<<"����"<<"��ɫID"<<"��ɫ����"<<"��ʵ����";
	ptabwdgulist->setHorizontalHeaderLabels(header);
	//ʹ����ͷ����Ӧ��ȣ�������ƽ���������հײ���
	ptabwdgulist->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//�б�����ɫ�仯
	ptabwdgulist->setAlternatingRowColors(true);
	//����ÿ�����ݲ��ɱ༭
	ptabwdgulist->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ֻ��ѡ��һ�У�����ѡ�����
	ptabwdgulist->setSelectionMode(QAbstractItemView::SingleSelection);
	//���õ���ѡ��һ��
	ptabwdgulist->setSelectionBehavior(QAbstractItemView::SelectRows);
	//ȥ��ÿ�е��к�
	//QHeaderView *headerView = ptabwdgulist->verticalHeader();
	//headerView->setHidden(true);


	QGroupBox *pusgbox = new QGroupBox("�û���ʾ");
	pnamelab = new QLabel("�û���");
	pnamelab->setFixedWidth(50);
	pnamelied = new QLineEdit;
	pnamelied->setFixedWidth(135);
	pnamelied->setFixedHeight(25);

	prolelab = new QLabel("  ��  ɫ");
	prolelab->setFixedWidth(60);
	prolecbx = new QComboBox;
	prolecbx->setFixedWidth(135);
	prolecbx->setFixedHeight(25);

	ppwdlab = new QLabel("��  ��");
	ppwdlab->setFixedWidth(50);
	ppwdlined = new QLineEdit;
	ppwdlined->setEchoMode(QLineEdit::Password);
	ppwdlined->setFixedWidth(135);
	ppwdlined->setFixedHeight(25);

	prealnamelab = new QLabel("��ʵ����");
	prealnamelab->setFixedWidth(60);
	prealnameld = new QLineEdit;
	prealnameld->setFixedWidth(135);
	prealnameld->setFixedHeight(25);


	QGroupBox *poprgbox = new QGroupBox("����");
	pbntadd = new QPushButton("�� ��");
	pbntadd->setFixedHeight(30);

	pbntdel = new QPushButton("ɾ ��");
	pbntdel->setFixedHeight(30);

	pbntmdf = new QPushButton("�� ��");
	pbntmdf->setFixedHeight(30);


	puhlayt->addWidget(ptabwdgulist);
	pulisgbox->setLayout(puhlayt);

	phlyt->addWidget(pnamelab);
	phlyt->addWidget(pnamelied);
	phlyt->addStretch();
	phlyt->addWidget(prolelab);
	phlyt->addWidget(prolecbx);

	phlayt->addWidget(ppwdlab);
	phlayt->addWidget(ppwdlined);
	phlayt->addStretch();
	phlayt->addWidget(prealnamelab);
	phlayt->addWidget(prealnameld);

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

	//�����û��б�������û�����,�����н�ɫID
	ptabwdgulist->setColumnHidden(2,true);
	ptabwdgulist->setColumnHidden(3,true);

	//Ĭ�������û���,����,��ɫ,��ʵ������Ϊֻ��
	pnamelied->setDisabled(true);
	ppwdlined->setDisabled(true);
	prolecbx->setDisabled(true);
	prealnameld->setDisabled(true);

	//Ĭ�����ò�����ť״̬��δѡ���û��б�ʱֻ�������û�,���Ӱ�ť��Ч��ɾ�����޸İ�ť��Ч
	pbntadd->setEnabled(true);
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

	//Ĭ������û���־λfalse
	baddflag = false;
	bmdfflag = false;
	bfst = false;

	//����û��б�ĳ���źŲ۹���
	connect(ptabwdgulist,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getUserItem(QTableWidgetItem*)));
	//������ť�źŲ۹���
	connect(pbntadd,SIGNAL(clicked()),this,SLOT(addUser()));
	connect(pbntdel,SIGNAL(clicked()),this,SLOT(delUser()));
	connect(pbntmdf,SIGNAL(clicked()),this,SLOT(mdfUser()));

}

UserMgrdlg::~UserMgrdlg()
{

}

void UserMgrdlg::reqUserRoleList()
{
	PBNS::UserRoleListMsg_Request urlreq;
	QString strData;
	strData.append("1");

	urlreq.set_reqdate("1");

	string sdata = urlreq.SerializeAsString();

	//���������û���ɫ�б�
	NetClient::instance()->sendData(CMD_USER_ROLE,sdata.c_str(),sdata.length());

	return;
}

void UserMgrdlg::retUserRoleList(const char* msg)
{
	PBNS::UserRoleListMsg_Response resp;
	resp.ParseFromString(msg);

	int idx = prolecbx->currentIndex();

	prolecbx->clear();

	//��Ĭ����ʾ��һ�������ʾֵΪ��
	prolecbx->insertItem(0,"");

	for (int i = 0; i < resp.userrolelist_size();i++)
	{
		PBNS::UserRoleBean urBean = resp.userrolelist(i);
		QString roleid = QString::fromStdString(urBean.userroleid());
		QString rolename = QString::fromStdString(urBean.userrolename());

		prolecbx->insertItem(roleid.toInt(),rolename);
	}

	if (resp.userrolelist_size() >= idx)
	{
		prolecbx->setCurrentIndex(idx);
	}
	else
	{
		prolecbx->setCurrentIndex(0);
	}
	
	
}

void UserMgrdlg::reqUserList()
{
	//�������û���ɫ�б�
	reqUserRoleList();
	
	PBNS::UserListMsg_Request ulreq;

	QString strData;
	strData.append("1");

	ulreq.set_reqdate("1");

	//���䷢������������Ϣ�ź�
	NetClient::instance()->sendData(CMD_USER_MANAGER,ulreq.SerializeAsString().c_str(),ulreq.SerializeAsString().length());

	return;
}

void UserMgrdlg::retUserList(const char* msg)
{
	PBNS::UserListMsg_Response resp;
	resp.ParseFromString(msg);

	int nrow = resp.userlist_size();

	//��������
	ptabwdgulist->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::UserBean ubean = resp.userlist(i);
		QString uid = QString::fromStdString(ubean.userid());

		QString uname = QString::fromStdString(ubean.username());
		QString urole = QString::fromStdString(ubean.userrole());

		ptabwdgulist->setItem(i,0,new QTableWidgetItem(uid));
		ptabwdgulist->setItem(i,1,new QTableWidgetItem(QString::fromStdString(ubean.username())));
		ptabwdgulist->setItem(i,2,new QTableWidgetItem(QString::fromStdString(ubean.userpwd())));	//�����и�ֵ
		ptabwdgulist->setItem(i,3,new QTableWidgetItem(QString::fromStdString(ubean.userrole())));	//�����и�ֵ
		ptabwdgulist->setItem(i,4,new QTableWidgetItem(QString::fromStdString(ubean.rolename())));
		ptabwdgulist->setItem(i,5,new QTableWidgetItem(QString::fromStdString(ubean.realname())));

	}

	//δѡ���û��б�ʱֻ�������û�,���Ӱ�ť��Ч��ɾ�����޸İ�ť��Ч
	pbntadd->setEnabled(true);
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

}


void UserMgrdlg::retUserManager(int msgtype,const char* msg)
{
	
	switch (msgtype)
	{
	case CMD_USER_MANAGER:
		retUserList(msg);
		break;
	case CMD_USER_ROLE:
		retUserRoleList(msg);
		break;
	}
	
}


void UserMgrdlg::getUserItem(QTableWidgetItem* item)
{
	int col = item->column();
	int row = item->row();

	int nuserid = ptabwdgulist->item(row,0)->text().toInt();

	pnamelied->setText(ptabwdgulist->item(row,1)->text());

	ppwdlined->setText("123456");

	prolecbx->setCurrentIndex(ptabwdgulist->item(row,3)->text().toInt());

	prealnameld->setText(ptabwdgulist->item(row,5)->text());
	

	//���ò�����ť״̬
	pbntdel->setEnabled(true);
	pbntmdf->setEnabled(true);
}

void UserMgrdlg::addUser()
{
	if (!baddflag)
	{
		pnamelied->clear();
		ppwdlined->clear();

		prolecbx->setCurrentIndex(0);
		prealnameld->clear();

		pnamelied->setDisabled(false);
		ppwdlined->setDisabled(false);
		prolecbx->setDisabled(false);
		prealnameld->setDisabled(false);

		//���ò�����ť״̬
		pbntdel->setEnabled(false);
		pbntmdf->setEnabled(false);

		baddflag = true;
	}
	else
	{
		if (pnamelied->text()=="")
		{
			QMessageBox::information(this,tr("�����û���ʾ:"),tr("�û�������Ϊ��,�������û���!"));
			return;
		}

		if (ppwdlined->text()=="")
		{
			QMessageBox::information(this,tr("�����û���ʾ:"),tr("���벻��Ϊ��,����������!"));
			return;
		}

		if (prolecbx->currentText()=="")
		{
			QMessageBox::information(this,tr("�����û���ʾ:"),tr("��ɫ����Ϊ��,��ѡ���û���ɫ!"));
			return;
		}

		if (prealnameld->text()=="")
		{
			QMessageBox::information(this,tr("�����û���ʾ:"),tr("��ʵ��������Ϊ��,��������ʵ����!"));
			return;
		}

		QString strpwd = ppwdlined->text();
		//����������ֵ����Md5����
		QByteArray byte_array;
		byte_array.append(strpwd);
		QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
		QString strpwdmd5 = hash_byte_array.toHex();

		//insert into users (name,password,roleid,realname) VALUES ('test','123','3','testtest');
		QString sql = QString("insert into users (name,password,roleid,realname) VALUES ('%1','%2','%3','%4');").arg(pnamelied->text()).arg(strpwdmd5).arg(prolecbx->currentIndex()).arg(prealnameld->text());

		PBNS::UserMgrMsg_Request addureq;
		addureq.set_mgrsql(sql.toStdString());

		string sdata = addureq.SerializeAsString();

		NetClient::instance()->sendData(CMD_USER_ADD,sdata.c_str(),sdata.length());

		baddflag = false;
		pnamelied->setDisabled(true);
		ppwdlined->setDisabled(true);
		prolecbx->setDisabled(true);
		prealnameld->setDisabled(true);

		//�����û���ɺ�,�����û��б�
		reqUserList();
	}

}

void UserMgrdlg::delUser()
{
	int ncurrow = ptabwdgulist->currentRow();
	//ȡ�û��б���ѡ���еĵ�һ���û�id��
	int nUserid = ptabwdgulist->item(ncurrow,0)->text().toInt();

	QString sql = QString("delete from users where id=%1").arg(nUserid);

	PBNS::UserMgrMsg_Request delureq;
	delureq.set_mgrsql(sql.toStdString());

	string sdata = delureq.SerializeAsString();

	NetClient::instance()->sendData(CMD_USER_DEL,sdata.c_str(),sdata.length());

	//���ò�����ť״̬
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

	//ɾ���û���ɺ�,�����û��б�
	reqUserList();
}

void UserMgrdlg::mdfUser()
{
	if (!bmdfflag)
	{
		pnamelied->setDisabled(false);
		ppwdlined->setDisabled(false);
		prolecbx->setDisabled(false);
		prealnameld->setDisabled(false);

		//���ò�����ť״̬
		pbntadd->setEnabled(false);
		pbntdel->setEnabled(false);

		bmdfflag = true;
	} 
	else
	{
		if (pnamelied->text()=="")
		{
			QMessageBox::information(this,tr("�����û���ʾ:"),tr("�û�������Ϊ��,�������û���!"));
			return;
		}

		if (ppwdlined->text()=="")
		{
			QMessageBox::information(this,tr("�����û���ʾ:"),tr("���벻��Ϊ��,����������!"));
			return;
		}

		if (prolecbx->currentText()=="")
		{
			QMessageBox::information(this,tr("�����û���ʾ:"),tr("��ɫ����Ϊ��,��ѡ���û���ɫ!"));
			return;
		}

		if (prealnameld->text()=="")
		{
			QMessageBox::information(this,tr("�����û���ʾ:"),tr("��ʵ��������Ϊ��,��������ʵ����!"));
			return;
		}

		int ncurrow = ptabwdgulist->currentRow();
		//ȡ�û��б���ѡ���еĵ�һ���û�id��
		int nUserid = ptabwdgulist->item(ncurrow,0)->text().toInt();
		//ȡ�û��б���ѡ���еĵ������û�����
		QString strpwditem = ptabwdgulist->item(ncurrow,2)->text();
		//ȡ�û��б���ѡ���еĵ������û�id��
		int nRoleid = ptabwdgulist->item(ncurrow,3)->text().toInt();

		//��õ�ǰ����������е�����ֵ(Ĭ��Ϊ:123456)
		QString strpwd = ppwdlined->text();

		QString strpwdmd5;

		//������ΪĬ�����õ�����ֵʱ��˵������δ���޸�ֱ��ȡ���û�ID��Ӧ�������������,��������������µ��������¶���������м���
		if (strpwd == "123456")
		{
			strpwdmd5 = strpwditem;
		}
		else
		{
			//����������ֵ����Md5����
			QByteArray byte_array;
			byte_array.append(strpwd);
			QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
			strpwdmd5 = hash_byte_array.toHex();
		}

		//update users set Name='aa' , Password='202cb962ac59075b964b07152d234b70' ,RoleId=7 ,RealName='ss' where ID=7
		QString sql = QString("update users set Name='%1' , Password='%2' ,RoleId=%3 ,RealName='%4' where ID=%5 ;").arg(pnamelied->text()).arg(strpwdmd5).arg(nRoleid).arg(prealnameld->text()).arg(nUserid);

		PBNS::UserMgrMsg_Request delureq;
		delureq.set_mgrsql(sql.toStdString());

		string sdata = delureq.SerializeAsString();
		NetClient::instance()->sendData(CMD_USER_MODIFY,sdata.c_str(),sdata.length());

		bmdfflag = false;
		pnamelied->setDisabled(true);
		ppwdlined->setDisabled(true);
		prolecbx->setDisabled(true);
		prealnameld->setDisabled(true);

		//�޸��û���ɺ�,�����û��б�
		reqUserList();

	}

}
