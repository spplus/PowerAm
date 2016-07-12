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
	SvgGraph*	parserSvg(QString filename);

	// ��ͼ���ļ�
	void			openSvgFile(QString filename);

	// ��ʾ�豸״̬��Ϣ
	void			showDevState(const char* msg,int length);

	// ��ʾ�浵�б�
	void			showSavingList(const char* msg,int msglength);

	// ���ص�ǰ��saveid
	int			getSaveId(){return m_saveId;}

	// ���������豸״̬����
	void			reqUnitState(QString stationCim);

	// ��ʾ�����Ĺ����б�
	void			showRuleList(const char* msg,int length);

	// ���ر�λ��̨�������
	void			recvBreakerOpRes(const char* msg,int length);

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
	bool			setSvgStyle(SvgGraph* graph,QString svgid,QString style);

	// �޸��豸״̬�б��е��豸״̬
	void			setDevState(QList<PBNS::StateBean>devlist,SvgGraph* graph,BaseDevice* pdev);

	// ���ù����豸����ɫ�������豸���⣬��Ϊ�����豸����ɫ����
	void			setConnectedDevColor(SvgGraph* pgraph,SvgItem* item);

	// ����svgid����svgItem
	SvgItem*		findSvgItemById(QString id);

	// ����cimid�����豸����
	bool				findUnitBeanByCimId(QString cimid,PBNS::StateBean & bean);

	// ���͹��Ʋ�������
	void				sendTagReq(eTagState type);

	// ���Ϳ��ر�λ����
	void				sendBreakOpReq(eBreakerState state,bool ischeck);

	// ���ر�λ
	void					switchChange(int state);
	QString				getNewSymbolId(QString oldid,int state);
	void					mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void			contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	// buff תlist
	QList<PBNS::StateBean>	getStateBeanList(PBNS::DevStateMsg_Response& res);
	QList<PBNS::StateBean>	getStateBeanList(PBNS::OprationMsg_Response& res);

	// �豸��ɫ����λ
	void						drawDev(QList<PBNS::StateBean> & devList);

	// ������ɫ
	void						colorDev(SvgGraph* graph,BaseDevice* pdev,PBNS::StateBean &bean,QString color);
private:

	// ��ǰ�򿪵Ĵ浵ID
	int					m_saveId;

	// ��ǰ�򿪵�վ��ID
	QString				m_stationCim;

	// ��ǰ�򿪵�ͼ�����
	int					m_curIndex;

	// ��ǰ�浵�ı�λ��¼
	QList<PBNS::StateBean>	m_opDevList;

	// ��ǰѡ�е�item
	SvgItem*				m_curItem;
	QMenu*				m_cntMenu;
	SvgParser			m_svgParser;
	SvgRenderer*			m_svgRender;
	QList<SvgGraph*>		m_graphList;
	QPropertyAnimation	m_animation;

	PBNS::DevStateMsg_Response m_devList;
};

#endif