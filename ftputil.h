/************************************************************************/
/* 
	DESC:		FTP����SVG�ļ�.
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
	// ����ȫ���ļ�
	void		getAll();

	// ����ָ���ļ�
	void		getFile(QString fname);

	// ��ȡ�ļ��б�
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

	// ����������
	int			m_downCount;
	// ftp
	QFtp*		m_ftp;	

	// ftp��ַ
	QString		m_host;

	QLabel*		m_statusLabel;
	QLabel*		m_msgLabel;
	QProgressBar*	m_progressBar;
	
	// ��ǰ·��
	QString		m_currentPath;

	// �ļ��б�
	QList<QString>		m_flist;

	// �ļ��б�
	QHash<QString, bool> m_isDirectory;
};

#endif