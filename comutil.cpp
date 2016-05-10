#include <QTextCodec>
#include <QCoreApplication>
#include <QSettings>
#include "comutil.h"

ComUtil* ComUtil::m_inst = NULL;

ComUtil::ComUtil()
{

}

ComUtil* ComUtil::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new ComUtil;
	}
	return m_inst;
}

QString ComUtil::getSysName()
{
	QSettings syncini(QCoreApplication::applicationDirPath()+"/client.conf",QSettings::IniFormat);
	syncini.setIniCodec("UTF8");
	syncini.setIniCodec(QTextCodec::codecForName("GB2312")); 

	return syncini.value("AM/SysName").toString();
}

