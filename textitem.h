/************************************************************************/
/* 
	DESC:		�Զ����ı�item.
	DATE:		2016-04-01
	AUTHOR:		YUANLS
*/
/************************************************************************/

#ifndef __TEXTITEM_H__
#define __TEXTITEM_H__

#include <QPen>
#include <QPainter>
#include <QFont>
#include <QBrush>
#include <QGraphicsItem>
#include "svgitem.h"
#include "textsvg.h"

class TextItem	:public QGraphicsItem
{
public:
	TextItem(const TextSvg& tg);
	//����QGraphicsItem�ǳ�����࣬��������Ҫʵ���������麯��boundingrect,paint

	QRectF	boundingRect() const;
	void		paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

private:

	QPen		m_pen;
	qreal		m_width;
	qreal		m_height;
	TextSvg	m_tg;
};

#endif