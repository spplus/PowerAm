#include <string>
#include "userlogindlg.h"
#include "comutil.h"
UserLogindlg* UserLogindlg::m_inst = NULL;
using namespace std;
UserLogindlg* UserLogindlg::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new UserLogindlg;
	}
	return m_inst;
}

UserLogindlg::UserLogindlg()
	: QDialog()
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground);

	//初始化为未按下鼠标左键
	mouse_press = false;

	loginwidget = new QWidget(this);
	loginwidget->resize(586,379);
	loginwidget->setAutoFillBackground(true);
	QPalette lgnpalette;
	lgnpalette.setBrush(QPalette::Background,QBrush(QPixmap(":loginmainbk")));
	loginwidget->setPalette(lgnpalette);

	QHBoxLayout *plgnhlyt = new QHBoxLayout;
	QVBoxLayout *plgnvlyt = new QVBoxLayout;
	QVBoxLayout *ploginvlyt = new QVBoxLayout;

	QVBoxLayout *pvlyt = new QVBoxLayout;

	//创建标题widget
	plgntitel_widget = new QWidget(this);
	plgntitel_widget->setFixedWidth(544);
	plgntitel_widget->setFixedHeight(137);
	//button_widget->setFixedHeight(30);
	//设置widget圆角和背景图片border:1px groove gray;border-radius:5px;padding:2px 4px;
	plgntitel_widget->setStyleSheet("background-image:url(:stgirdicon);");

	plgntitel_widget->setAutoFillBackground(true);

	close_button = new PushButton(plgntitel_widget);
	//设置按钮背景图片
	close_button->setPicName(QString(":close"));

	//按钮摆放当前widget的右上角位置，默认在左上角把移动到左上角
	int width = plgntitel_widget->width();
	close_button->move(width-close_button->width(), 0);


	//登录框用户名
	//pusercobx = new QComboBox;
	//pusercobx->setEditable(true);
	//pusercobx->lineEdit()->setPlaceholderText("用户名");
	//pusercobx->setFixedSize(180,32);

	//登录框用户名
	puserlined = new QLineEdit;
	puserlined->setPlaceholderText("用户名");
	puserlined->setFixedSize(180,32);

	//登录框密码
	ppwdlined = new QLineEdit;
	ppwdlined->setPlaceholderText("密  码");
	ppwdlined->setEchoMode(QLineEdit::Password);
	ppwdlined->setFixedSize(180,32);
	//登录按钮
	ploginbtn = new QPushButton();
	//ploginbtn->setIcon(QPixmap(QString(":login")));
	ploginbtn->setStyleSheet("border-radius:5px;background-image:url(:login);");
	ploginbtn->setFixedSize(180,32);

	//布局登录用户名和密码框
	plgnvlyt->addWidget(puserlined);
	plgnvlyt->addWidget(ppwdlined);
	plgnvlyt->setSpacing(0);

	//布局登录框和按钮
	ploginvlyt->addLayout(plgnvlyt);
	ploginvlyt->addSpacing(32);
	ploginvlyt->addWidget(ploginbtn);

	//布局登录输入框及按钮
	plgnhlyt->addSpacing(265);
	plgnhlyt->addLayout(ploginvlyt);

	//总体布局
	pvlyt->addWidget(plgntitel_widget);
	pvlyt->addSpacing(45);
	pvlyt->addLayout(plgnhlyt);

	pvlyt->setContentsMargins(21,21,21,50);

	loginwidget->setLayout(pvlyt);
	//设置按钮焦点
	ploginbtn->setFocus(Qt::ActiveWindowFocusReason);
	//点击关闭按钮关闭登录界面
	connect(close_button, SIGNAL(clicked()), this, SLOT(closedlg()));
	//登录响应
	connect(ploginbtn,SIGNAL(clicked()),this,SLOT(login()));
	
	bcloselgn = false;
	bnetflag  = false;
	
}

UserLogindlg::~UserLogindlg()
{

}

void UserLogindlg::closedlg()
{
	bcloselgn = this->close();
}

void UserLogindlg::login()
{
	QString strusername = puserlined->text();
	QString strpwd		= ppwdlined->text();

	if (strusername == "")
	{
		QMessageBox::information(this,tr("用户登录提示:"),tr("登录用户名为空,请输入用户名!"));
		return;
	}

	if (strpwd == "")
	{
		QMessageBox::information(this,tr("用户登录提示:"),tr("登录密码为空,请输入密码!"));
		return;
	}

	//对输入密码值进行Md5加密
	QByteArray byte_array;
	byte_array.append(strpwd);
	QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
	QString strpwdmd5 = hash_byte_array.toHex();

	QString strData;
	strData.append(strusername).append(strpwdmd5);

	PBNS::UserLoginMsg_Request loginreq;
	loginreq.set_username(strusername.toStdString());
	loginreq.set_userpwd(strpwdmd5.toStdString());

	string data = loginreq.SerializeAsString();
	//发送数据信号
	NetClient::instance()->sendData(CMD_USER_LONGIN,data.c_str(),data.length());

	return;
}

void UserLogindlg::recvdata(int msgtype,const char* msg,int msglength)
{
	switch (msgtype)
	{
	case CMD_USER_LONGIN:
		{
			PBNS::UserLoginMsg_Response userep;
			userep.ParseFromString(msg);
			PBNS::UserBean ubean;

			//得到对应用户名密码的用户信息,登陆成功，关闭登陆界面
			if (userep.userlist_size() > 0)
			{
				for (int i=0;i<userep.userlist_size();i++)
				{
					ubean = userep.userlist(i);
				}

				//设置用户相关信息
				userid  = atoi(ubean.userid().c_str());
				uname   = QString::fromStdString(ubean.username());
				passwd	= QString::fromStdString(ubean.userpwd());
				roleid  = atoi(ubean.userrole().c_str());
				
				// 保存当前登录用户的角色
				ComUtil::instance()->setCurUserRole(roleid);
				ComUtil::instance()->setCurUserId(userid);

				realname = QString::fromStdString(ubean.realname());
				switch (roleid)
				{
				case eManager:
					rolename = tr("管理员");
					break;
				case eDispatcher:
					rolename = tr("调度");
					break;
				case eAutomater:
					rolename = tr("自动化");
					break;
				case eMaintainers:
					rolename = tr("运维");
					break;
				default:
					rolename = tr("未知");
					break;
				}

				this->close();

				// 判断cim文件是否有更新
				if (userep.rescode() == 2)
				{
					QMessageBox::information(this,tr("CIM文件更新"),tr("CIM文件有更新，请点击电路图界面 ‘刷新’按钮进行更新CIM!"));
				}
			}
			else
			{
				QMessageBox::information(this,tr("用户登录提示:"),tr("用户名或密码输入错误!"));
				return;
			}
		}
		break;
	}

	return;

}

QString UserLogindlg::getLoginUser()
{
	return puserlined->text();
}

int UserLogindlg::getLoginUserId()
{
	return userid;
}

int UserLogindlg::getLoginRoleId()
{
	return roleid;
}

QString UserLogindlg::getLoginRoleName()
{
	return rolename;
}

QString UserLogindlg::getLoginPasswd()
{
	return passwd;
}

QString UserLogindlg::getLoginRealName()
{
	return realname;
}

void UserLogindlg::mousePressEvent(QMouseEvent *event)
{
	//只能是鼠标左键移动和改变大小
	if(event->button() == Qt::LeftButton) 
	{
		mouse_press = true;
	}

	//窗口移动距离
	move_point = event->globalPos() - pos(); 

}

void UserLogindlg::mouseReleaseEvent(QMouseEvent *event)
{
	mouse_press = false;
}

void UserLogindlg::mouseMoveEvent(QMouseEvent *event)
{
	//移动窗口
	if(mouse_press)   
	{
		QPoint move_pos = event->globalPos();
		move(move_pos - move_point);
	}
}
