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

#ifdef WIN32
	// 设置编码
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
#else
	// 设置编码
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif
	

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
	/*if(!ComUtil::instance()->loadColorRule())
	{
		QMessageBox::warning(NULL,"系统提示","加载颜色规则失败，请检查配置.");
		return -1;
	}*/

	//初始化网络连接
	if(!NetClient::instance()->init())
	{
		QMessageBox::warning(NULL,"系统提示","连接服务器失败");
		return -1;
	}

	//加载登录界面
	UserLogindlg::instance()->exec();

	//登录用户是否关闭
	if (UserLogindlg::instance()->getCloselgn())
	{
		return -1;
	}

	// 加载分类列表
	ComUtil::instance()->getStationType();

	//加载厂站
	ComUtil::instance()->getStation();

	//加载规则类型列表
	ComUtil::instance()->getRuleType();

	//加载用户列表
	ComUtil::instance()->getAllUserList();

	//设置登录用户名
	/*HomeWindow::instance()->setUserName(UserLogindlg::instance()->getLoginUser());
	HomeWindow::instance()->show();*/
	
	//MainWindow::instance()->show();

	a.setStyle(new QPlastiqueStyle);

    return a.exec();
}
