/************************************************************************/
/* 
	DESC:		�豸�����.
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

protected:
	QString		m_svgId;
	QString		m_metaId;
	QString		m_measId;
	QString		m_metaName;

	// ͼԪģ��ID
	QString		m_symbolId;
	eDeviceType	m_devType;
};

#endif