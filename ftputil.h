/************************************************************************/
/* 
	DESC:		FTP下载SVG文件.
	DATE:		2016-04-19
	AUTHOR:		YUANLS
*/
/************************************************************************/
 
#ifndef __FTPUTIL_H__
#define __FTPUTIL_H__

#include <QFile>
#include <QFtp>
#include <QDialog>
#include <QHash>
#include <QLabel>
#include <QProgressBar>
#include <QMutex>

class FtpUtil	:public QDialog
{
	Q_OBJECT
public:
	FtpUtil(QWidget *parent = 0,QString host = "192.168.0.3");
	~FtpUtil();
	QSize sizeHint() const;
signals:
	void		downloaded(QString fname);

public slots:
	// 下载全部文件
	void		getAll();

	// 下载指定文件
	void		getFile(QString fname);

private:
	void		initUi();
	void		showListInfo();
private slots:
	void	connectToFtp();
	void	ftpCommandFinished(int commandId, bool error);
	void	addToList(const QUrlInfo &urlInfo);
	void	cdToParent();
	void	updateDataTransferProgress(qint64 readBytes,qint64 totalBytes);

private:

	// 已下载数量
	int			m_downCount;
	// ftp
	QFtp*		m_ftp;	

	// ftp地址
	QString		m_host;

	QLabel*		m_statusLabel;
	QLabel*		m_msgLabel;
	QProgressBar*	m_progressBar;
	// 当前路径
	QString		m_currentPath;

	// 文件列表
	QHash<QString, bool> m_isDirectory;
};

#endif