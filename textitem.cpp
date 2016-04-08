#include <QMatrix>
#include "textitem.h"

TextItem::TextItem( TextSvg& tg)
{
	m_tg = tg;
	m_width = tg.m_fontWidth;
	m_height = tg.m_fontHeight;
	if (tg.m_transform.length()>0)
	{
		parserMatrix(tg);
	}
}

void TextItem::parserMatrix(TextSvg& tg)
{
	QString mtx = tg.m_transform;
	int idx1 = mtx.indexOf("(");
	int idx2 = mtx.indexOf(")");
	QString mp = mtx.mid(idx1+1,idx2-idx1-1);
	QStringList mplist = mp.split(" ");
	if (mplist.size() == 6)
	{
		QMatrix qmtx;
		qmtx.setMatrix(mplist.at(0).toDouble(),mplist.at(1).toDouble(),mplist.at(2).toDouble(),mplist.at(3).toDouble(),mplist.at(4).toDouble(),mplist.at(5).toDouble());
		//this->setMatrix(qmtx);

		this->scale(mplist.at(0).toDouble(),mplist.at(3).toDouble());
		this->shear(mplist.at(1).toDouble(),mplist.at(2).toDouble());
		tg.m_x = mplist.at(4).toDouble();
		tg.m_y = mplist.at(5).toDouble();
	}

	// ½âÎötranslate
	idx1= mtx.lastIndexOf("(");
	idx2 = mtx.lastIndexOf(")");

	QString tl = mtx.mid(idx1+1,idx2-idx1-1);
	QStringList tlist = tl.split(",");

	if (tlist.size() == 2)
	{
		this->translate(tlist.at(0).toInt(),tlist.at(1).toInt());
		//tg.m_x += tlist.at(0).toDouble();
		//tg.m_y += tlist.at(1).toDouble();
	}
}

QRectF TextItem::boundingRect()const
{
	return QRectF(0, 0, m_width, m_height);;
}

void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	Q_UNUSED(widget);

	if(m_tg.m_stroke.isValid())
	{
		m_pen.setColor(m_tg.m_stroke);
	}
	else if (m_tg.m_fill.isValid())
	{
		m_pen.setColor(m_tg.m_fill);
	}
	
	QFont fnt;

	if (m_tg.m_fontFamily.length()>0)
	{
		fnt.setFamily(m_tg.m_fontFamily);
	}
	
	fnt.setPixelSize(m_tg.m_fontSize);

	painter->setFont(fnt);

	QBrush bs;
	bs.setColor(m_tg.m_fill);
	painter->setBackground(bs);
	painter->setBackgroundMode(Qt::OpaqueMode);
	painter->setPen(m_pen);

	painter->drawText(0,0,m_tg.m_text);

}