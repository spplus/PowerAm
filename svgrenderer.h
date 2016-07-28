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

class GraphicsScene;
class SvgRenderer
{
public:

	SvgRenderer(GraphicsScene* scene);
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

	SvgItem*			addItem(QString id,eDeviceType tp = eWINDING);
	SvgItem*			addItem(BaseDevice* pdev);

	// 初始化svg
	QString				initSvgRenderer();
	
	// 判断是否需要坐标反转 ，如果有旋转-180，则X,Y需要反向补偿
	// 如果旋转-90，则仅Y需要补偿
	int					isReverseCoordination(BaseDevice* pdev);

private:
	SvgGraph*		m_graph;
	QSvgRenderer*	m_renderer;
	GraphicsScene*	m_scene;
};


#endif