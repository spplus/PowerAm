#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <qdir.h>
#include <QObject>
#include <QTranslator>
#include "netclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	// ���ñ���
	QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));


    MainWindow w;

	// ����ҵ�������������
	QObject::connect(NetClient::instance(),SIGNAL(connected()),&w,SLOT(connected()));
	QObject::connect(NetClient::instance(),SIGNAL(disconnected()),&w,SLOT(disconnected()));
	QObject::connect(NetClient::instance(),SIGNAL(recvdata(int,const char*,int)),&w,SLOT(recvdata(int,const char*,int)));

    w.show();

    return a.exec();
}