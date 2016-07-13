

#include "svgitem.h"

SvgItem::SvgItem()
{
	m_isColor = false;
	m_cimId = "";
	m_svgId = "";
	m_layerId = "";
	m_color = "";
	m_type = eDEFAULT;
}

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

QString SvgItem::getSvgId()
{
	return m_svgId;
}

void SvgItem::setSvgId(QString svgid)
{
	m_svgId = svgid;
}

void SvgItem::setIsColor(bool iscolor)
{
	m_isColor = iscolor;
}

bool SvgItem::getIsColor()
{
	return m_isColor;
}

void SvgItem::setColor(QString color)
{
	m_color = color;
}

QString SvgItem::getColor()
{
	return m_color;
}