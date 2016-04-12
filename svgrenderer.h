/************************************************************************/
/* 
	DESC:	把SVG图元渲染成QGraphicsSvgItem对象
			渲染整张SVG图形到QGrahicsSencen中
	DATE:	2016-03-24
	AUTHOR:	YUANLS
*/
/************************************************************************/
#ifndef __SVGRENDER_H__
#define __SVGRENDER_H__




#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>

#include "svggraph.h"
#include "svgitem.h"
//#include "structs.h"

class SvgRenderer
{
public:

	SvgRenderer(QGraphicsScene* scene);
	~SvgRenderer();

	// 把SVG中的图元，生成qgraphicsitem，并插入到scene中
	void		drawGraph(SvgGraph* graph);

	// 根据ID创建svgItem
	SvgItem*	renderById(SvgGraph* graph,BaseDevice* pdev);

private:
	SvgItem*		makeSvgItem(QString id);
	void			setItemPos(SvgItem* item,BaseDevice* pdev);
	void			drawHeadLayer();
	void			drawTextLayer();

	SvgLayer*			findLayer(QString lid);

	SvgItem*			addItem(QString id,eDeviceType tp = eDEFAULT);
	SvgItem*			addItem(BaseDevice* pdev);

	// 初始化svg
	QString				initSvgRenderer();
	
	// 判断是否需要坐标反转
	bool				isReverseCoordination(BaseDevice* pdev);

private:
	SvgGraph*		m_graph;
	QSvgRenderer*	m_renderer;
	QGraphicsScene*	m_scene;
};


#endif