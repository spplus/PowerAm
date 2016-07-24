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

	//��ʼ��Ϊδ����������
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

	//��������widget
	plgntitel_widget = new QWidget(this);
	plgntitel_widget->setFixedWidth(544);
	plgntitel_widget->setFixedHeight(137);
	//button_widget->setFixedHeight(30);
	//����widgetԲ�Ǻͱ���ͼƬborder:1px groove gray;border-radius:5px;padding:2px 4px;
	plgntitel_widget->setStyleSheet("background-image:url(:stgirdicon);");

	plgntitel_widget->setAutoFillBackground(true);

	close_button = new PushButton(plgntitel_widget);
	//���ð�ť����ͼƬ
	close_button->setPicName(QString(":close"));

	//��ť�ڷŵ�ǰwidget�����Ͻ�λ�ã�Ĭ�������Ͻǰ��ƶ������Ͻ�
	int width = plgntitel_widget->width();
	close_button->move(width-close_button->width(), 0);


	//��¼���û���
	//pusercobx = new QComboBox;
	//pusercobx->setEditable(true);
	//pusercobx->lineEdit()->setPlaceholderText("�û���");
	//pusercobx->setFixedSize(180,32);

	//��¼���û���
	puserlined = new QLineEdit;
	puserlined->setPlaceholderText("�û���");
	puserlined->setFixedSize(180,32);

	//��¼������
	ppwdlined = new QLineEdit;
	ppwdlined->setPlaceholderText("��  ��");
	ppwdlined->setEchoMode(QLineEdit::Password);
	ppwdlined->setFixedSize(180,32);
	//��¼��ť
	ploginbtn = new QPushButton();
	//ploginbtn->setIcon(QPixmap(QString(":login")));
	ploginbtn->setStyleSheet("border-radius:5px;background-image:url(:login);");
	ploginbtn->setFixedSize(180,32);

	//���ֵ�¼�û����������
	plgnvlyt->addWidget(puserlined);
	plgnvlyt->addWidget(ppwdlined);
	plgnvlyt->setSpacing(0);

	//���ֵ�¼��Ͱ�ť
	ploginvlyt->addLayout(plgnvlyt);
	ploginvlyt->addSpacing(32);
	ploginvlyt->addWidget(ploginbtn);

	//���ֵ�¼����򼰰�ť
	plgnhlyt->addSpacing(265);
	plgnhlyt->addLayout(ploginvlyt);

	//���岼��
	pvlyt->addWidget(plgntitel_widget);
	pvlyt->addSpacing(45);
	pvlyt->addLayout(plgnhlyt);

	pvlyt->setContentsMargins(21,21,21,50);

	loginwidget->setLayout(pvlyt);
	//���ð�ť����
	ploginbtn->setFocus(Qt::ActiveWindowFocusReason);
	//����رհ�ť�رյ�¼����
	connect(close_button, SIGNAL(clicked()), this, SLOT(closedlg()));
	//��¼��Ӧ
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
		QMessageBox::information(this,tr("�û���¼��ʾ:"),tr("��¼�û���Ϊ��,�������û���!"));
		return;
	}

	if (strpwd == "")
	{
		QMessageBox::information(this,tr("�û���¼��ʾ:"),tr("��¼����Ϊ��,����������!"));
		return;
	}

	//����������ֵ����Md5����
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
	//���������ź�
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

			//�õ���Ӧ�û���������û���Ϣ,��½�ɹ����رյ�½����
			if (userep.userlist_size() > 0)
			{
				for (int i=0;i<userep.userlist_size();i++)
				{
					ubean = userep.userlist(i);
				}

				//�����û������Ϣ
				userid  = atoi(ubean.userid().c_str());
				uname   = QString::fromStdString(ubean.username());
				passwd	= QString::fromStdString(ubean.userpwd());
				roleid  = atoi(ubean.userrole().c_str());
				
				// ���浱ǰ��¼�û��Ľ�ɫ
				ComUtil::instance()->setCurUserRole(roleid);
				ComUtil::instance()->setCurUserId(userid);

				realname = QString::fromStdString(ubean.realname());
				switch (roleid)
				{
				case eManager:
					rolename = tr("����Ա");
					break;
				case eDispatcher:
					rolename = tr("����");
					break;
				case eAutomater:
					rolename = tr("�Զ���");
					break;
				case eMaintainers:
					rolename = tr("��ά");
					break;
				default:
					rolename = tr("δ֪");
					break;
				}

				this->close();

				// �ж�cim�ļ��Ƿ��и���
				if (userep.rescode() == 2)
				{
					QMessageBox::information(this,tr("CIM�ļ�����"),tr("CIM�ļ��и��£�������·ͼ���� ��ˢ�¡���ť���и���CIM!"));
				}
			}
			else
			{
				QMessageBox::information(this,tr("�û���¼��ʾ:"),tr("�û����������������!"));
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
	//ֻ�����������ƶ��͸ı��С
	if(event->button() == Qt::LeftButton) 
	{
		mouse_press = true;
	}

	//�����ƶ�����
	move_point = event->globalPos() - pos(); 

}

void UserLogindlg::mouseReleaseEvent(QMouseEvent *event)
{
	mouse_press = false;
}

void UserLogindlg::mouseMoveEvent(QMouseEvent *event)
{
	//�ƶ�����
	if(mouse_press)   
	{
		QPoint move_pos = event->globalPos();
		move(move_pos - move_point);
	}
}
