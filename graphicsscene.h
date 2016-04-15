/************************************************************************/
/* 
			DESC:		�Զ���scene.
			DATE:		2016-04-03
			AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef		__GRAPHICSSCENE_H__
#define		__GRAPHICSSCENE_H__

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

	// ��ͼ���ļ�
	void			openSvgFile(QString filename);

public slots:

	// ǰ��
	void			goNext();

	// ����
	void			goPrev();

	// �÷�
	void			setOpen();		

	// �ú�
	void			setClose();
	
	// ���Զ���
	void			startAnimation();
protected:
	void			switchChange(int state);

	QString				getNewSymbolId(QString oldid,int state);
	void				mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void		contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	// ��ǰ�򿪵�ͼ�����
	int					m_curIndex;

	// ��ǰѡ�е�item
	SvgItem*			m_curItem;
	QMenu*				m_cntMenu;
	SvgParser			m_svgParser;
	SvgRenderer*		m_svgRender;
	QList<SvgGraph*>	m_graphList;
	QPropertyAnimation	m_animation;
};

#endif