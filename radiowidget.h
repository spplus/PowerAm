/************************************************************************/
/* 
		DESC:	������ʵʱ̬��ģ��̬����Ʊ̬��ť.
		DATE:	2016-07-20
		AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __RADIOWIDGET_H__
#define __RADIOWIDGET_H__

#include <QtGui>
#include "comutil.h"

class RadioWidget	:public QWidget
{
	Q_OBJECT
public:
	RadioWidget(QWidget* parent= NULL);

signals:
	void		realtime();
	void		analog();
	void		ticket();

private:
	void		initUi();
	void		initSlots();
private:
	QRadioButton	*		m_analogBtn;
	QRadioButton *		m_realtimeBtn;
	QRadioButton *		m_ticketBtn;
};


#endif