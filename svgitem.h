/************************************************************************/
/* 
	DESC:		自定义ITEM，继承至QGraphicsSvgItem.
	DATE:		2016-04-01
	AUTHOR:		YUANLS	
*/
/************************************************************************/

#ifndef __SVGITEM_H__
#define __SVGITEM_H__

#include <QGraphicsSvgItem>
#include "define.h"

class SvgItem	:public QGraphicsSvgItem
{
public:
	void		setType(eDeviceType tp);
	eDeviceType	getType();
	QString		getLayerId();
	void		setLayerId(QString lid);

private:
	QString			m_layerId;
	eDeviceType		m_type;
};

#endif

