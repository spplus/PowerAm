/************************************************************************/
/* 
		DESC:		ÎÄ×ÖÍ¼Ôª.
		DATE:		2016-04-01
		AUTHOR:	YUANLS
*/
/************************************************************************/
#ifndef __TEXTSVG_H__
#define __TEXTSVG_H__
#include <QColor>
#include <QString>

class TextSvg
{
public:
	qreal			m_x;
	qreal			m_y;
	int			m_fontSize;
	int			m_fontWidth;
	int			m_fontHeight;
	QString	m_fontFamily;
	QColor		m_fill;
	QColor		m_stroke;
	QString	m_text;
	QString	m_transform;
};

#endif