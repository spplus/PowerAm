#include <string>
#include "usermgrdlg.h"

using namespace std;
UserMgrdlg::UserMgrdlg(QWidget *parent)
	: QDialog(parent)
{
	//只显示关闭按钮
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

	QGroupBox *pulisgbox = new QGroupBox("用户列表");
	ptabwdgulist = new QTableWidget();
	//设置背景色为白色
	QPalette tabplt = ptabwdgulist->palette();
	//tabplt.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));//设置透明
	tabplt.setBrush(QPalette::Background,QBrush(QColor(255,255,255,255)));
	ptabwdgulist->setPalette(tabplt);
	//设置列数
	ptabwdgulist->setColumnCount(6);
	QStringList header;
	header<<"用户ID"<<"用户名"<<"密码"<<"角色ID"<<"角色名称"<<"真实名称";
	ptabwdgulist->setHorizontalHeaderLabels(header);
	//使行列头自适应宽度，所有列平均分来填充空白部分
	ptabwdgulist->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//行背景颜色变化
	ptabwdgulist->setAlternatingRowColors(true);
	//设置每行内容不可编辑
	ptabwdgulist->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置只能选择一行，不能选择多行
	ptabwdgulist->setSelectionMode(QAbstractItemView::SingleSelection);
	//设置单击选择一行
	ptabwdgulist->setSelectionBehavior(QAbstractItemView::SelectRows);
	//去掉每行的行号
	//QHeaderView *headerView = ptabwdgulist->verticalHeader();
	//headerView->setHidden(true);


	QGroupBox *pusgbox = new QGroupBox("用户显示");
	pnamelab = new QLabel("用户名");
	pnamelab->setFixedWidth(50);
	pnamelied = new QLineEdit;
	pnamelied->setFixedWidth(135);
	pnamelied->setFixedHeight(25);

	prolelab = new QLabel("  角  色");
	prolelab->setFixedWidth(60);
	prolecbx = new QComboBox;
	prolecbx->setFixedWidth(135);
	prolecbx->setFixedHeight(25);

	ppwdlab = new QLabel("密  码");
	ppwdlab->setFixedWidth(50);
	ppwdlined = new QLineEdit;
	ppwdlined->setEchoMode(QLineEdit::Password);
	ppwdlined->setFixedWidth(135);
	ppwdlined->setFixedHeight(25);

	prealnamelab = new QLabel("真实姓名");
	prealnamelab->setFixedWidth(60);
	prealnameld = new QLineEdit;
	prealnameld->setFixedWidth(135);
	prealnameld->setFixedHeight(25);


	QGroupBox *poprgbox = new QGroupBox("操作");
	pbntadd = new QPushButton("增 加");
	pbntadd->setFixedHeight(30);

	pbntdel = new QPushButton("删 除");
	pbntdel->setFixedHeight(30);

	pbntmdf = new QPushButton("修 改");
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

	//隐藏用户列表第三列用户密码,第四列角色ID
	ptabwdgulist->setColumnHidden(2,true);
	ptabwdgulist->setColumnHidden(3,true);

	//默认设置用户名,密码,角色,真实姓名框为只读
	pnamelied->setDisabled(true);
	ppwdlined->setDisabled(true);
	prolecbx->setDisabled(true);
	prealnameld->setDisabled(true);

	//默认设置操作按钮状态，未选中用户列表时只可增加用户,增加按钮有效，删除和修改按钮无效
	pbntadd->setEnabled(true);
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

	//默认添加用户标志位false
	baddflag = false;
	bmdfflag = false;
	bfst = false;

	//点击用户列表某行信号槽关联
	connect(ptabwdgulist,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(getUserItem(QTableWidgetItem*)));
	//操作按钮信号槽关联
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

	//发射请求用户角色列表
	NetClient::instance()->sendData(CMD_USER_ROLE,sdata.c_str(),sdata.length());

	return;
}

void UserMgrdlg::retUserRoleList(const char* msg)
{
	PBNS::UserRoleListMsg_Response resp;
	resp.ParseFromString(msg);

	int idx = prolecbx->currentIndex();

	prolecbx->clear();

	//在默认显示的一行添加显示值为空
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
	//先请求用户角色列表
	reqUserRoleList();
	
	PBNS::UserListMsg_Request ulreq;

	QString strData;
	strData.append("1");

	ulreq.set_reqdate("1");

	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_USER_MANAGER,ulreq.SerializeAsString().c_str(),ulreq.SerializeAsString().length());

	return;
}

void UserMgrdlg::retUserList(const char* msg)
{
	PBNS::UserListMsg_Response resp;
	resp.ParseFromString(msg);

	int nrow = resp.userlist_size();

	//设置行数
	ptabwdgulist->setRowCount(nrow);

	for (int i = 0; i < nrow; i++)
	{
		PBNS::UserBean ubean = resp.userlist(i);
		QString uid = QString::fromStdString(ubean.userid());

		QString uname = QString::fromStdString(ubean.username());
		QString urole = QString::fromStdString(ubean.userrole());

		ptabwdgulist->setItem(i,0,new QTableWidgetItem(uid));
		ptabwdgulist->setItem(i,1,new QTableWidgetItem(QString::fromStdString(ubean.username())));
		ptabwdgulist->setItem(i,2,new QTableWidgetItem(QString::fromStdString(ubean.userpwd())));	//隐藏列赋值
		ptabwdgulist->setItem(i,3,new QTableWidgetItem(QString::fromStdString(ubean.userrole())));	//隐藏列赋值
		ptabwdgulist->setItem(i,4,new QTableWidgetItem(QString::fromStdString(ubean.rolename())));
		ptabwdgulist->setItem(i,5,new QTableWidgetItem(QString::fromStdString(ubean.realname())));

	}

	//未选中用户列表时只可增加用户,增加按钮有效，删除和修改按钮无效
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
	

	//设置操作按钮状态
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

		//设置操作按钮状态
		pbntdel->setEnabled(false);
		pbntmdf->setEnabled(false);

		baddflag = true;
	}
	else
	{
		if (pnamelied->text()=="")
		{
			QMessageBox::information(this,tr("增加用户提示:"),tr("用户名不可为空,请输入用户名!"));
			return;
		}

		if (ppwdlined->text()=="")
		{
			QMessageBox::information(this,tr("增加用户提示:"),tr("密码不可为空,请输入密码!"));
			return;
		}

		if (prolecbx->currentText()=="")
		{
			QMessageBox::information(this,tr("增加用户提示:"),tr("角色不可为空,请选择用户角色!"));
			return;
		}

		if (prealnameld->text()=="")
		{
			QMessageBox::information(this,tr("增加用户提示:"),tr("真实姓名不可为空,请输入真实姓名!"));
			return;
		}

		QString strpwd = ppwdlined->text();
		//对输入密码值进行Md5加密
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

		//增加用户完成后,请求用户列表
		reqUserList();
	}

}

void UserMgrdlg::delUser()
{
	int ncurrow = ptabwdgulist->currentRow();
	//取用户列表中选中行的第一列用户id号
	int nUserid = ptabwdgulist->item(ncurrow,0)->text().toInt();

	QString sql = QString("delete from users where id=%1").arg(nUserid);

	PBNS::UserMgrMsg_Request delureq;
	delureq.set_mgrsql(sql.toStdString());

	string sdata = delureq.SerializeAsString();

	NetClient::instance()->sendData(CMD_USER_DEL,sdata.c_str(),sdata.length());

	//设置操作按钮状态
	pbntdel->setEnabled(false);
	pbntmdf->setEnabled(false);

	//删除用户完成后,请求用户列表
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

		//设置操作按钮状态
		pbntadd->setEnabled(false);
		pbntdel->setEnabled(false);

		bmdfflag = true;
	} 
	else
	{
		if (pnamelied->text()=="")
		{
			QMessageBox::information(this,tr("增加用户提示:"),tr("用户名不可为空,请输入用户名!"));
			return;
		}

		if (ppwdlined->text()=="")
		{
			QMessageBox::information(this,tr("增加用户提示:"),tr("密码不可为空,请输入密码!"));
			return;
		}

		if (prolecbx->currentText()=="")
		{
			QMessageBox::information(this,tr("增加用户提示:"),tr("角色不可为空,请选择用户角色!"));
			return;
		}

		if (prealnameld->text()=="")
		{
			QMessageBox::information(this,tr("增加用户提示:"),tr("真实姓名不可为空,请输入真实姓名!"));
			return;
		}

		int ncurrow = ptabwdgulist->currentRow();
		//取用户列表中选中行的第一列用户id号
		int nUserid = ptabwdgulist->item(ncurrow,0)->text().toInt();
		//取用户列表中选中行的第三列用户密码
		QString strpwditem = ptabwdgulist->item(ncurrow,2)->text();
		//取用户列表中选中行的第四列用户id号
		int nRoleid = ptabwdgulist->item(ncurrow,3)->text().toInt();

		//获得当前密码输入框中的密码值(默认为:123456)
		QString strpwd = ppwdlined->text();

		QString strpwdmd5;

		//当密码为默认设置的密码值时，说明密码未做修改直接取此用户ID对应的密码更新数据,否则就是输入了新的密码重新对新密码进行加密
		if (strpwd == "123456")
		{
			strpwdmd5 = strpwditem;
		}
		else
		{
			//对输入密码值进行Md5加密
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

		//修改用户完成后,请求用户列表
		reqUserList();

	}

}
