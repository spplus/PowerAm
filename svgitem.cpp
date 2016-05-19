

#include "svgitem.h"


void SvgItem::setType(eDeviceType tp)
{
	m_type = tp;
}

eDeviceType SvgItem::getType()
{
	return m_type;
}

QString SvgItem::getLayerId()
{
	return m_layerId;
}

void SvgItem::setLayerId(QString lid)
{
	m_layerId = lid;
}

void SvgItem::setCimId(QString cimId)
{
	m_cimId = cimId;
}

QString SvgItem::getCimId()
{
	return m_cimId;
}