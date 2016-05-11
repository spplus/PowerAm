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

	// 设置编码
	QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

	QFile qss("style.qss");
	qss.open(QFile::ReadOnly);
	a.setStyleSheet(qss.readAll());
	qss.close();

	// 初始化网络连接
	if(!NetClient::instance()->init())
	{
		QMessageBox::warning(NULL,"系统提示","连接服务器失败");
		return -1;
	}
	// 加载分类列表
	ComUtil::instance()->getStationType();
	

	HomeWindow::instance()->show();

	a.setStyle(new QPlastiqueStyle);

   // w.show();

    return a.exec();
}
