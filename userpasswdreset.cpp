#include "userpasswdreset.h"

UserPasswdReset::UserPasswdReset(QWidget *parent)
	: QDialog(parent)
{
	//只显示关闭按钮
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

	this->resize(300,200);

	QHBoxLayout *poldhlayt = new QHBoxLayout;
	QHBoxLayout *pnewhlayt = new QHBoxLayout;
	QHBoxLayout *pnewaghlayt = new QHBoxLayout;
	QHBoxLayout *pbnthlayt = new QHBoxLayout;
	QVBoxLayout *pmainvlyt = new QVBoxLayout;

	QLabel *poldlab = new QLabel("    原密码:");
	poldpwd = new QLineEdit;
	poldpwd->setEchoMode(QLineEdit::Password);
	poldpwd->setFixedSize(150,25);

	QLabel *pnewlab = new QLabel("    新密码:");
	pnewpwd = new QLineEdit;
	pnewpwd->setEchoMode(QLineEdit::Password);
	pnewpwd->setFixedSize(150,25);

	QLabel *pnewaglab = new QLabel("确认新密码:");
	pnewagpwd = new QLineEdit;
	pnewagpwd->setEchoMode(QLineEdit::Password);
	pnewagpwd->setFixedSize(150,25);

	pbntpwdok = new QPushButton("确  认");
	pbntpwdok->setFixedSize(120,25);

	poldhlayt->addWidget(poldlab);
	poldhlayt->addWidget(poldpwd);

	pnewhlayt->addWidget(pnewlab);
	pnewhlayt->addWidget(pnewpwd);

	pnewaghlayt->addWidget(pnewaglab);
	pnewaghlayt->addWidget(pnewagpwd);

	pbnthlayt->addStretch();
	pbnthlayt->addWidget(pbntpwdok);
	pbnthlayt->addStretch();

	pmainvlyt->addLayout(poldhlayt);
	pmainvlyt->addLayout(pnewhlayt);
	pmainvlyt->addLayout(pnewaghlayt);
	pmainvlyt->addSpacing(15);
	pmainvlyt->addLayout(pbnthlayt);

	pmainvlyt->setContentsMargins(40,20,45,20);

	this->setLayout(pmainvlyt);

	connect(pbntpwdok,SIGNAL(pressed()),this,SLOT(commitPwd()));

	qstrpwdmd5 = UserLogindlg::instance()->getLoginPasswd();

}

UserPasswdReset::~UserPasswdReset()
{

}


void UserPasswdReset::commitPwd()
{
	QString strpwd = poldpwd->text();
	QString strnpwd = pnewpwd->text();
	QString strngpwd = pnewagpwd->text();

	if (strpwd == "")
	{
		QMessageBox::information(this,tr("密码重置提示"),tr("原密码输入为空,请输入原密码!"));
		return;
	}

	if (strnpwd == "")
	{
		QMessageBox::information(this,tr("密码重置提示"),tr("新密码输入为空,请输入新密码!"));
		return;
	}

	if (strngpwd == "")
	{
		QMessageBox::information(this,tr("密码重置提示"),tr("确认新密码输入为空,请输入确认新密码!"));
		return;
	}

	if (strnpwd != strngpwd)
	{
		QMessageBox::information(this,tr("密码重置提示"),tr("新密码两次输入不一致,请重新输入!"));
		return;
	}

	//对输入密码值进行Md5加密
	QByteArray byte_array;
	byte_array.append(strpwd);
	QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
	QString strpwdmd5 = hash_byte_array.toHex();

	if (strpwdmd5 != UserLogindlg::instance()->getLoginPasswd())
	{
		QMessageBox::information(this,tr("密码重置提示"),tr("原密码验证失败,请重新输入!"));
		return;
	}


	//对输入密码值进行Md5加密
	QByteArray byte_array1;
	byte_array1.append(strnpwd);
	QByteArray hash_byte_array1 = QCryptographicHash::hash(byte_array1, QCryptographicHash::Md5);
	QString strnpwdmd5 = hash_byte_array1.toHex();

	int nuserid = UserLogindlg::instance()->getLoginUserId();

	UserLogindlg::instance()->getLoginUser();

	QString sql = QString("UPDATE users set Password='%1' WHERE ID=%2 ").arg(strnpwdmd5).arg(nuserid);
	PBNS::UserMgrMsg_Request req;
	req.set_mgrsql(sql.toStdString());

	NetClient::instance()->sendData(CMD_PWD_MODIFY,req.SerializeAsString().c_str(),sql.size());

	qstrpwdmd5 = strnpwdmd5;
}


void UserPasswdReset::retpasswdMgr(int msgtype,const char* msg)
{
	PBNS::UserMgrMsg_Response pwdrep;
	pwdrep.ParseFromString(msg);
	pwdrep.rescode();
	if (pwdrep.rescode() == 0)
	{
		QMessageBox::information(this,tr("密码重置提示"),tr("密码重置成功!"));
		//更新新密码
		UserLogindlg::instance()->setLoginPasswd(qstrpwdmd5);
		this->close();
	} 
	else
	{
		QMessageBox::information(this,tr("密码重置提示"),tr("密码重置失败!"));
	}

}





