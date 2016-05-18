#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QPlastiqueStyle>
#include "netclient.h"
#include "homewindow.h"
#include "comutil.h"

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


	// ���ط����б�
	ComUtil::instance()->getStationType();
	
	
	HomeWindow::instance()->setUserName("��������Ա");
	HomeWindow::instance()->show();
	
	//MainWindow::instance()->show();

	a.setStyle(new QPlastiqueStyle);

    return a.exec();
}
