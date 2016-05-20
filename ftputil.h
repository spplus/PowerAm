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
	FtpUtil(QWidget *parent = 0);
	~FtpUtil();
	QSize sizeHint() const;

signals:
	void		downloaded(QString fname,int stationId = 0,bool needRoot=true);

public slots:
	// 下载全部文件
	void		getAll();

	// 下载指定文件
	void		getFile(QString fname);

	// 获取文件列表
	QList<QString>	getFileList();

private:
	void		initUi();
	void		showListInfo();
	QString		_FromSpecialEncoding(const QString &InputStr);
	QString		_ToSpecialEncoding(const QString &InputStr);

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
	QList<QString>		m_flist;

	// 文件列表
	QHash<QString, bool> m_isDirectory;
};

#endif