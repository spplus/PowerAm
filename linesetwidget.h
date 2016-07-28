/************************************************************************/
/* 
	DESC:	进出线设置窗口.
	DATE:	2016-06-15
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __LINESETWIDGET_H__
#define __LINESETWIDGET_H__

#include <QtGui>

class LineSetWidget	:public QDialog
{
	Q_OBJECT
public:
	LineSetWidget(QWidget *parent = 0);

	QString		getOneCim();
	QString		getOhrCim();

private slots:
	void		onOk();
	void		onCancel();
	void		oneItemChanged(int index);
	void		ohrItemChanged(int index);
private:
	void		initUi();
	void		initConnections();
	void		initStationList(); 
private:
	QString			m_oneCim;
	QString			m_ohrCim;

	QPushButton*		m_okBtn;
	QPushButton*		m_cancelBtn;
	QComboBox*		m_stationOneBox;
	QComboBox*		m_stationOhrBox;
};

#endif