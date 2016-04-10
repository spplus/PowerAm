/************************************************************************/
/* 
	DESC:		设备类基类.
	DATE:		2016-03-30
	AUTHOR:		YUANLS
*/
/************************************************************************/
#ifndef __BASEDEV_H__
#define __BASEDEV_H__
#include <QString>
#include "define.h"

class BaseDevice 
{
public:
	BaseDevice()
	{
		m_svgId = "";
		m_symbolId = "";
		m_transform = "";
		m_x = 0;
		m_y = 0;
		m_w = 0;
		m_h = 0;
	}
	QString		getSvgId(){return m_svgId;}
	void		setSvgId(QString svgId){m_svgId = svgId;}

	QString		getMetaId(){return m_metaId;}
	void		setMetaId(QString metaId){m_metaId = metaId;}

	void		setMetaName(QString metaName){m_metaName = metaName;}
	QString		getMetaName(){return m_metaName;}

	void		setDevType(eDeviceType dtype){m_devType = dtype;}
	eDeviceType	getDevType(){return m_devType;}

	void		setMeasId(QString mid){m_metaId = mid;}
	QString		getMeasId(){return m_measId;}

	void				setSymbolId(QString symbolId){m_symbolId = symbolId;}
	QString				getSymbolId(){return m_symbolId;}

	void				setX(qreal x){m_x = x;}
	qreal			getX(){return m_x;}

	void				setY(qreal y){m_y = y;}
	qreal			getY(){return m_y;}

	void				setTransform(QString trsf){m_transform = trsf;}
	QString		getTransform(){return m_transform;}

public:
	qreal			m_w;
	qreal			m_h;
	qreal			m_x;
	qreal			m_y;

protected:
	
	QString		m_transform;
	QString		m_svgId;
	QString		m_metaId;
	QString		m_measId;
	QString		m_metaName;

	// 图元模板ID
	QString		m_symbolId;
	eDeviceType	m_devType;
};

#endif