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

	// ���õ�Դ��
	void			setPower();

	// ���ý�����
	void			setLine();

	// ����
	void			tagOn();

	// ժ��
	void			tagOff();
	
	// �򿪴浵
	void			readSaving();

	// ����浵
	void			writeSaving();

	// ���Զ���
	void			startAnimation();

	// �ѽ�����Ķ�������SVGͼԪ
	void			drawSvgGraph(SvgGraph* graph);

protected:

	// �޸�dom�п���״̬
	void			setBreakState(SvgGraph* graph,BaseDevice* pdev,eBreakerState state);

	// �޸�dom�е�style����
	void			setSvgStyle(SvgGraph* graph,QString svgid,QString style);

	// �޸��豸״̬�б��е��豸״̬
	void			setDevState(PBNS::DevStateMsg_Response &res,SvgGraph* graph,BaseDevice* pdev);

	// ���ù����豸����ɫ�������豸���⣬��Ϊ�����豸����ɫ����
	void				setConnectedDevColor(SvgGraph* pgraph,SvgItem* item);

	// ����svgid����svgItem
	SvgItem*		findSvgItemById(QString id);

	// ����cimid�����豸����
	bool				findUnitBeanByCimId(QString cimid,PBNS::StateBean & bean);

	// ���͹��Ʋ�������
	void				sendTagReq(eTagState type);

	// ���ر�λ
	void					switchChange(int state);
	QString				getNewSymbolId(QString oldid,int state);
	void					mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void			contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	// ��ǰ�򿪵Ĵ浵ID
	int					m_saveId;

	// ��ǰ�򿪵�ͼ�����
	int					m_curIndex;

	// ��ǰѡ�е�item
	SvgItem*			m_curItem;
	QMenu*				m_cntMenu;
	SvgParser			m_svgParser;
	SvgRenderer*		m_svgRender;
	QList<SvgGraph*>	m_graphList;
	QPropertyAnimation	m_animation;

	PBNS::DevStateMsg_Response m_devList;
};

#endif