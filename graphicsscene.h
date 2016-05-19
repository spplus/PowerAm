/************************************************************************/
/* 
			DESC:		�Զ���scene.
			DATE:		2016-04-03
			AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef		__GRAPHICSSCENE_H__
#define		__GRAPHICSSCENE_H__

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include "buff/msgbody.pb.h"
#include "textitem.h"
#include "svgparser.h"
#include "svgrenderer.h"

class GraphicsScene		:public QGraphicsScene
{
	Q_OBJECT
public:
	GraphicsScene(QObject* parant,QMenu* cntmenu);
	~GraphicsScene();

	// ����SVG�ļ�
	SvgGraph* parserSvg(QString filename);

	// ��ͼ���ļ�
	void			openSvgFile(QString filename);

	// ��ʾ�豸״̬��Ϣ
	void			showDevState(const char* msg,int length);

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

	// �ѽ�����Ķ�������SVGͼԪ
	void			drawSvgGraph(SvgGraph* graph);

protected:

	// �޸�dom�п���״̬
	void			setBreakState(SvgGraph* graph,BaseDevice* pdev,eBreakerState state);

	// �޸��豸״̬�б��е��豸״̬
	void			setDevState(PBNS::DevStateMsg_Response &res,SvgGraph* graph,BaseDevice* pdev);

	// ���ر�λ
	void					switchChange(int state);
	QString			getNewSymbolId(QString oldid,int state);
	void					mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
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