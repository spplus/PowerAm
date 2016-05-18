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

	// 设置编码
	QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

	// 读取客户端配置信息
	if (!ComUtil::instance()->initConfig())
	{
		QMessageBox::warning(NULL,"系统提示","加载客户端配置失败，请检查配置.");
		return -1;
	}

	QFile qss(ComUtil::instance()->getAppPath()+"/"+STYLE);
	qss.open(QFile::ReadOnly);
	a.setStyleSheet(qss.readAll());
	qss.close();


	// 加载颜色配置
	if(!ComUtil::instance()->loadColorRule())
	{
		QMessageBox::warning(NULL,"系统提示","加载颜色规则失败，请检查配置.");
		return -1;
	}

	//初始化网络连接
	if(!NetClient::instance()->init())
	{
		QMessageBox::warning(NULL,"系统提示","连接服务器失败");
		return -1;
	}


	// 加载分类列表
	ComUtil::instance()->getStationType();
	
	
	HomeWindow::instance()->setUserName("超级管理员");
	HomeWindow::instance()->show();
	
	//MainWindow::instance()->show();

	a.setStyle(new QPlastiqueStyle);

    return a.exec();
}
