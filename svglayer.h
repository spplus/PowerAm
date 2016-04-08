/************************************************************************/
/* 
	DESC:		定义SVG图层.
	DATE:		2016-03-31
	AUTHOR:		YUANLS
*/
/************************************************************************/

#ifndef __SVGLAYER_H__
#define __SVGLAYER_H__

#include "textsvg.h"
#include "basedev.h"

class SvgLayer	
{
public:
	SvgLayer()
	{
		m_id = "";
		m_x = 0;
		m_y = 0;
	}
	~SvgLayer()
	{
		for (int i = 0;i<m_devList.size();i++)
		{
			BaseDevice* pdev = m_devList.at(i);
			delete pdev;
			pdev = NULL;
		}
	}

	QString				getId(){return m_id;}
	void				setId(QString id){m_id = id;}

	QString				getFill(){return m_fill;}
	void				setFill(QString fill){m_fill = fill;}

	int					getHeight(){return m_height;}
	void				setHeight(int h){m_height = h;}

	int					getWidth(){return m_width;}
	void				setWidth(int w){m_width = w;}

	int					getX(){return m_x;}
	void				setX(int x){m_x = x;}

	int					getY(){return m_y;}
	void				setY(int y){m_y = y;}

	// 获取设备列表的引用
	QList<BaseDevice*> & getDevList(){return m_devList;}

	QList<TextSvg*>&			getTextList(){return m_textList;}

private:
	// 图层ID
	QString				m_id;

	// x
	int					m_x;

	// y
	int					m_y;

	// 宽
	int					m_width;

	// 高
	int					m_height;

	// 背景
	QString				m_fill;

	// 设备列表
	QList<BaseDevice*>	m_devList;

	// 文本图元列表
	QList<TextSvg*>	m_textList;
};


#endif