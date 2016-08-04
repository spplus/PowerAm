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
	SvgItem();
	void		setType(eDeviceType tp);
	eDeviceType	getType();
	QString		getLayerId();
	void		setLayerId(QString lid);
	void		setCimId(QString cimId);
	QString		getCimId();
	QString		getSvgId();
	void				setSvgId(QString svgid);
	void				setIsColor(bool iscolor);
	bool				getIsColor();
	void				setColor(QString color);
	QString			getColor();
	void		setSymbolId(QString symid);
	QString		getSymbolId();

private:

	// 是否设置过颜色
	bool				m_isColor;
	QString			m_svgId;
	QString			m_cimId;
	QString			m_layerId;
	QString			m_color;
	QString			m_symbolId;
	eDeviceType		m_type;

};

#endif

