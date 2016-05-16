/************************************************************************/
/* 
	DESC:	SVG文件解析线程.
	DATE:	2016-05-16
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __OPENTHREAD_H__
#define __OPENTHREAD_H__

#include <QThread>
#include "graphicsscene.h"

class OpenThread	:public QThread	
{
	Q_OBJECT

public:
	OpenThread(GraphicsScene* scene);

	void	open(QString fname);

private:
	void	run();
		
private:
	QString			m_fname;
	GraphicsScene*	m_pscene;
};

#endif