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

	// ���ñ���
	QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

	QFile qss("style.qss");
	qss.open(QFile::ReadOnly);
	a.setStyleSheet(qss.readAll());
	qss.close();

	// ��ʼ����������
	if(!NetClient::instance()->init())
	{
		QMessageBox::warning(NULL,"ϵͳ��ʾ","���ӷ�����ʧ��");
		return -1;
	}
	// ���ط����б�
	ComUtil::instance()->getStationType();
	

	HomeWindow::instance()->show();

	a.setStyle(new QPlastiqueStyle);

   // w.show();

    return a.exec();
}
