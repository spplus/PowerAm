#include <QUrl>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ftputil.h"
#include "define.h"
#include "comutil.h"

FtpUtil* FtpUtil::m_inst = NULL;

FtpUtil* FtpUtil::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new FtpUtil;
	}
	return m_inst;
}

FtpUtil::FtpUtil(QWidget *parent)
	:QDialog(parent)
{
	m_downCount = 0;

	initUi();
	connectToFtp();
}

FtpUtil::~FtpUtil()
{
	if (m_ftp != NULL)
	{
		m_ftp->close();
		delete m_ftp;
		m_ftp = NULL;
	}
}

QSize FtpUtil::sizeHint() const
{
	return QSize(300, 100);
}

QList<QString> FtpUtil::getFileList()
{
	m_flist.clear();

	QHash<QString,bool>::iterator iter = m_isDirectory.begin();
	for (;iter!=m_isDirectory.end();iter++)
	{
		// 跳过目录
		if (iter.value())
		{
			continue;
		}

		m_flist.push_back(_FromSpecialEncoding(iter.key()));
	}

	return m_flist;
}

void FtpUtil::getAll()
{
	m_downCount = 0;
	// 遍历文件列表，下载全部文件
	QHash<QString,bool>::iterator iter = m_isDirectory.begin();
	for (;iter!=m_isDirectory.end();iter++)
	{
		// 跳过目录
		if (iter.value())
		{
			continue;
		}
		
		getFile(_FromSpecialEncoding(iter.key()));
	}
}

void FtpUtil::getFile(QString fname)
{
	
	QString fpath = ComUtil::instance()->getSvgRoot()+"/"+fname;
	QFile *file = new QFile(fpath);
	
	// 保存文件名称，不包含路径
	file->setProperty("fname",fname);

	if (!file->open(QIODevice::WriteOnly)) 
	{
		m_statusLabel->setText(tr("Unable to save the file %1: %2.")
			.arg(fname).arg(file->errorString()));
		delete file;
		return;
	}

	// 通过FTP下载
	m_ftp->get(_ToSpecialEncoding(fname), file);

	m_msgLabel->setText(tr("正在下载%1...").arg(fname));
}

void FtpUtil::initUi()
{
	m_msgLabel = new QLabel;
	m_statusLabel = new QLabel;
	m_progressBar = new QProgressBar;
	m_progressBar->setTextVisible(false);
	QHBoxLayout* hbox2 = new QHBoxLayout;
	hbox2->addWidget(m_progressBar);
	
	QHBoxLayout* hbox3 = new QHBoxLayout;
	hbox3->addWidget(m_statusLabel);
	

	//QPushButton* downBtn = new QPushButton(tr("下载"));
	//connect(downBtn,SIGNAL(pressed()),this,SLOT(getAll()));

	QHBoxLayout* hbox = new QHBoxLayout;
	
	hbox->addWidget(m_msgLabel);
	//hbox->addStretch();
	//hbox->addWidget(downBtn);

	QVBoxLayout* vbox = new QVBoxLayout(this);

	vbox->addStretch();
	vbox->addLayout(hbox);
	vbox->addLayout(hbox2);
	vbox->addLayout(hbox3);
	vbox->addStretch();

	setLayout(vbox);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

void FtpUtil::connectToFtp()
{
	m_ftp = new QFtp(this);
	
	connect(m_ftp, SIGNAL(commandFinished(int,bool)),
		this, SLOT(ftpCommandFinished(int,bool)));
	connect(m_ftp, SIGNAL(listInfo(QUrlInfo)),
		this, SLOT(addToList(QUrlInfo)));
	connect(m_ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
		this, SLOT(updateDataTransferProgress(qint64,qint64)));

	
	m_currentPath.clear();
	m_isDirectory.clear();

	FtpConfig ftpconf = ComUtil::instance()->getFtpConfig();
	m_host = ftpconf.m_ftpAddr;
	QUrl url(m_host);
	url.setUserName(ftpconf.m_ftpUser);
	url.setPort(ftpconf.m_ftpPort);
	url.setPassword(ftpconf.m_ftpPwd);

	if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp")) 
	{
		m_ftp->connectToHost(m_host, ftpconf.m_ftpPort);
		m_ftp->login();
	} 
	else 
	{
		m_ftp->connectToHost(url.host(), url.port());

		if (!url.userName().isEmpty())
			m_ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
		else
			m_ftp->login();
		if (!url.path().isEmpty())
			m_ftp->cd(url.path());
	}

}

void FtpUtil::updateDataTransferProgress(qint64 readBytes,
	qint64 totalBytes)
{
	m_progressBar->setMaximum(totalBytes);
	m_progressBar->setValue(readBytes);
}

void FtpUtil::cdToParent()
{
#ifndef QT_NO_CURSOR
	setCursor(Qt::WaitCursor);
#endif
	
	m_isDirectory.clear();
	m_currentPath = m_currentPath.left(m_currentPath.lastIndexOf('/'));
	if (m_currentPath.isEmpty()) {
		m_ftp->cd("/");
	} else {
		m_ftp->cd(m_currentPath);
	}
	m_ftp->list();
}

void FtpUtil::ftpCommandFinished(int, bool error)
{
#ifndef QT_NO_CURSOR
	setCursor(Qt::ArrowCursor);
#endif

	if (m_ftp->currentCommand() == QFtp::ConnectToHost) 
	{
		if (error) 
		{
			QString msg = QString("Unable to connect to the FTP server "
				"at %1. Please check that the host "
				"name is correct.")
				.arg(m_host);

			m_statusLabel->setText(msg);
			
			if (m_ftp) 
			{
				m_ftp->abort();
				m_ftp->deleteLater();
				m_ftp = NULL;
			}
			
			return;
		}
		else
		{
			m_statusLabel->setText("连接FTP成功:"+m_host);
		}
		return;
	}
	
	if (m_ftp->currentCommand() == QFtp::Login)
	{
		QString ftpdir = ComUtil::instance()->getFtpConfig().m_ftpDir;
		if (ftpdir.length()<=0)
		{
			QMessageBox::warning(this,"系统提示","未设置文件在FTP服务器上的目录");
			accept();
		}
		m_ftp->cd(ftpdir);
		m_ftp->list();
	}
	if (m_ftp->currentCommand() == QFtp::Get) 
	{
		QFile* file = (QFile*)m_ftp->currentDevice();
		if (error) 
		{
			QString msg = QString("下载文件失败 %1,%2.").arg(file->fileName()).arg(m_ftp->errorString());
			m_statusLabel->setText(msg);
			file->close();
			file->remove();
			QMessageBox::warning(this,"系统提示",msg);
			accept();
		} 
		else 
		{
			m_downCount++;

			QString fname = file->fileName();
			
			QString msg = QString("下载 %1 完成.").arg(fname);
			m_statusLabel->setText(msg);
			file->close();

			// 获取文件名称
			fname = file->property("fname").toString();

			emit downloaded(fname);

			// 关闭窗口
			this->accept();
		}
		
		delete file;
	} 
	else if (m_ftp->currentCommand() == QFtp::List) 
	{
		// 文件列表
		showListInfo();
	}
}


void FtpUtil::addToList(const QUrlInfo &urlInfo)
{
	QString fname = urlInfo.name();
	m_isDirectory[urlInfo.name()] = urlInfo.isDir();
}

void FtpUtil::showListInfo()
{
	m_msgLabel->setText(tr("共有发现文件%1个").arg(m_isDirectory.count()));
}

QString FtpUtil::_FromSpecialEncoding(const QString &InputStr)
{
#ifdef Q_OS_WIN
	return  QString::fromLocal8Bit(InputStr.toLatin1());
#else
	QTextCodec *codec = QTextCodec::codecForName("gbk");
	if (codec)
	{
		return codec->toUnicode(InputStr.toLatin1());
	}
	else
	{
		return QString("");
	}
#endif
}


QString FtpUtil::_ToSpecialEncoding(const QString &InputStr)
{
#ifdef Q_OS_WIN
	return QString::fromLatin1(InputStr.toLocal8Bit());
#else
	QTextCodec *codec= QTextCodec::codecForName("gbk");
	if (codec)
	{
		return QString::fromLatin1(codec->fromUnicode(InputStr));
	}
	else
	{
		return QString("");
	}
#endif
}