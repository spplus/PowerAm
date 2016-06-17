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

	// 解析SVG文件
	SvgGraph* parserSvg(QString filename);

	// 打开图形文件
	void			openSvgFile(QString filename);

	// 显示设备状态信息
	void			showDevState(const char* msg,int length);

public slots:

	// 前进
	void			goNext();

	// 后退
	void			goPrev();

	// 置分
	void			setOpen();		

	// 置合
	void			setClose();

	// 设置电源点
	void			setPower();

	// 设置进出线
	void			setLine();

	// 挂牌
	void			tagOn();

	// 摘牌
	void			tagOff();
	
	// 打开存档
	void			readSaving();

	// 保存存档
	void			writeSaving();

	// 测试动画
	void			startAnimation();

	// 把解析后的对象生成SVG图元
	void			drawSvgGraph(SvgGraph* graph);

protected:

	// 修改dom中开关状态
	void			setBreakState(SvgGraph* graph,BaseDevice* pdev,eBreakerState state);

	// 修改dom中的style属性
	void			setSvgStyle(SvgGraph* graph,QString svgid,QString style);

	// 修改设备状态列表中的设备状态
	void			setDevState(PBNS::DevStateMsg_Response &res,SvgGraph* graph,BaseDevice* pdev);

	// 设置关联设备的颜色，开关设备除外，因为开关设备有着色过程
	void				setConnectedDevColor(SvgGraph* pgraph,SvgItem* item);

	// 根据svgid查找svgItem
	SvgItem*		findSvgItemById(QString id);

	// 根据cimid查找设备对象
	bool				findUnitBeanByCimId(QString cimid,PBNS::StateBean & bean);

	// 发送挂牌操作请求
	void				sendTagReq(eTagState type);

	// 开关变位
	void					switchChange(int state);
	QString				getNewSymbolId(QString oldid,int state);
	void					mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void			contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	// 当前打开的存档ID
	int					m_saveId;

	// 当前打开的图形序号
	int					m_curIndex;

	// 当前选中的item
	SvgItem*			m_curItem;
	QMenu*				m_cntMenu;
	SvgParser			m_svgParser;
	SvgRenderer*		m_svgRender;
	QList<SvgGraph*>	m_graphList;
	QPropertyAnimation	m_animation;

	PBNS::DevStateMsg_Response m_devList;
};

#endif