#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QPlastiqueStyle>
#include <qdir.h>
#include <QObject>
#include <QTranslator>
#include "netclient.h"
#include "ftputil.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	// 设置编码
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

    MainWindow w;

	// 建立业务层和网络层连接
	QObject::connect(NetClient::instance(),SIGNAL(connected()),&w,SLOT(connected()));
	QObject::connect(NetClient::instance(),SIGNAL(disconnected()),&w,SLOT(disconnected()));
	QObject::connect(NetClient::instance(),SIGNAL(recvdata(int,const char*,int)),&w,SLOT(recvdata(int,const char*,int)));

	a.setStyle(new QPlastiqueStyle);

    w.show();

    return a.exec();
}
