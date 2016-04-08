/************************************************************************/
/* 
			DESC:		×Ô¶¨Òåview.
			DATE:		2016-04-03
			AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __GRAPHICSVIEW_H__
#define __GRAPHICSVIEW_H__

#include <QGraphicsView>
#include "graphicsscene.h"

class GraphicsView		:public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsView(GraphicsScene* scene);
	~GraphicsView();

public slots:
	void		zoomOut();
	void		zoomIn();
	void		zoomHome();
private:
	
	qreal			m_interal;
	qreal			m_scale;



};

#endif