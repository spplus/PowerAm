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

	// ÉèÖÃ±àÂë
	QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

	QFile qss("style.qss");
	qss.open(QFile::ReadOnly);
	a.setStyleSheet(qss.readAll());
	qss.close();

	//MainWindow w;
	HomeWindow::instance()->show();

	a.setStyle(new QPlastiqueStyle);

   // w.show();

    return a.exec();
}
