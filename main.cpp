#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QPlastiqueStyle>
#include <qdir.h>
#include <QObject>
#include <QTranslator>
#include "netclient.h"
#include "ftputil.h"
#include "homewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	// 设置编码
	QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

	QFile qss("style.qss");
	qss.open(QFile::ReadOnly);
	a.setStyleSheet(qss.readAll());
	qss.close();

	HomeWindow hw;
	hw.show();

    //MainWindow w;

	// 建立业务层和网络层连接
	/*QObject::connect(NetClient::instance(),SIGNAL(connected()),&w,SLOT(connected()));
	QObject::connect(NetClient::instance(),SIGNAL(disconnected()),&w,SLOT(disconnected()));
	QObject::connect(NetClient::instance(),SIGNAL(recvdata(int,const char*,int)),&w,SLOT(recvdata(int,const char*,int)));
*/
	a.setStyle(new QPlastiqueStyle);

   // w.show();

    return a.exec();
}
