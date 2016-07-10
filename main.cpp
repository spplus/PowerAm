#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QPlastiqueStyle>
#include "netclient.h"
#include "homewindow.h"
#include "comutil.h"
#include "userlogindlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QApplication::addLibraryPath("./plugins");

	// ���ñ���
	QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

	// ��ȡ�ͻ���������Ϣ
	if (!ComUtil::instance()->initConfig())
	{
		QMessageBox::warning(NULL,"ϵͳ��ʾ","���ؿͻ�������ʧ�ܣ���������.");
		return -1;
	}

	QFile qss(ComUtil::instance()->getAppPath()+"/"+STYLE);
	qss.open(QFile::ReadOnly);
	a.setStyleSheet(qss.readAll());
	qss.close();

	// ������ɫ����
	if(!ComUtil::instance()->loadColorRule())
	{
		QMessageBox::warning(NULL,"ϵͳ��ʾ","������ɫ����ʧ�ܣ���������.");
		return -1;
	}

	//��ʼ����������
	if(!NetClient::instance()->init())
	{
		QMessageBox::warning(NULL,"ϵͳ��ʾ","���ӷ�����ʧ��");
		return -1;
	}

	//���ص�¼����
	UserLogindlg::instance()->exec();

	//��¼�û��Ƿ�ر�
	if (UserLogindlg::instance()->getCloselgn())
	{
		return -1;
	}

	// ���ط����б�
	ComUtil::instance()->getStationType();

	//���س�վ
	ComUtil::instance()->getStation();

	//���ع��������б�
	ComUtil::instance()->getRuleType();

	//�����û��б�
	ComUtil::instance()->getAllUserList();

	//���õ�¼�û���
	HomeWindow::instance()->setUserName(UserLogindlg::instance()->getLoginUser());
	HomeWindow::instance()->show();
	
	//MainWindow::instance()->show();

	a.setStyle(new QPlastiqueStyle);

    return a.exec();
}
