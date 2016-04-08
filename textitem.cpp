#include "textitem.h"

TextItem::TextItem(const TextSvg& tg)
{
	m_tg = tg;
	m_width = tg.m_fontWidth;
	m_height = tg.m_fontHeight;
}

QRectF TextItem::boundingRect()const
{
	return QRectF(0, 0, m_width, m_height);;
}

void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	Q_UNUSED(widget);

	m_pen.setColor(m_tg.m_stroke);
	QFont fnt;
	fnt.setFamily(m_tg.m_fontFamily);
	fnt.setPixelSize(m_tg.m_fontSize);

	painter->setFont(fnt);

	QBrush bs;
	bs.setColor(m_tg.m_fill);
	painter->setBackground(bs);
	painter->setBackgroundMode(Qt::OpaqueMode);
	painter->setPen(m_pen);

	painter->drawText(0,0,m_tg.m_text);

}