/************************************************************************/
/* 
			DESC:		自定义scene.
			DATE:		2016-04-03
			AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef		__GRAPHICSSCENE_H__
#define		__GRAPHICSSCENE_H__

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPropertyAnimation>

#include "textitem.h"
#include "svgparser.h"
#include "svgrenderer.h"

class GraphicsScene		:public QGraphicsScene
{
	Q_OBJECT
public:
	GraphicsScene(QObject* parant,QMenu* cntmenu);
	~GraphicsScene();

	// 打开图形文件
	void			openSvgFile(QString filename);

public slots:

	// 前进
	void			goNext();

	// 后退
	void			goPrev();

	// 置分
	void			setOpen();		

	// 置合
	void			setClose();
	
	// 测试动画
	void			startAnimation();
protected:
	void			switchChange(int state);

	QString				getNewSymbolId(QString oldid,int state);
	void				mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void		contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	// 当前打开的图形序号
	int					m_curIndex;

	// 默认背景层
	QGraphicsPixmapItem*	m_bkItem;

	// 当前选中的item
	SvgItem*			m_curItem;
	QMenu*				m_cntMenu;
	SvgParser			m_svgParser;
	SvgRenderer*		m_svgRender;
	QList<SvgGraph*>	m_graphList;
	QPropertyAnimation	m_animation;
};

#endif