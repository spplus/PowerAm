/************************************************************************/
/* 
			DESC:		自定义scene.
			DATE:		2016-04-03
			AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef		__GRAPHICSSCENE_H__
#define		__GRAPHICSSCENE_H__

#include <QMutex>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include "buff/msgbody.pb.h"
#include "textitem.h"
#include "svgparser.h"
#include "svgrenderer.h"
#include "ticketmgr.h"

class GraphicsScene		:public QGraphicsScene
{
	Q_OBJECT
public:
	GraphicsScene(QObject* parant,QMenu* cntmenu);
	~GraphicsScene();

	// 解析SVG文件
	SvgGraph*	parserSvg(QString filename);

	// 打开图形文件
	void			openSvgFile(QString filename);

	// 显示设备状态信息
	void			showDevState(const char* msg,int length);

	// 显示存档列表
	void			showSavingList(const char* msg,int msglength);

	// 返回当前的saveid
	int			getSaveId(){return m_saveId;}

	// 发送请求设备状态请求
	void			reqUnitState(QString stationCim);

	// 显示触发的规则列表
	void			showRuleList(const char* msg,int length);

	// 开关变位后台结果返回
	void			recvBreakerOpRes(const char* msg,int length);

	void			putItem(SvgItem* item);
	void			clearItem();
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

	// 设置系统状态
	void			setRealState();
	void			setAnalogState();
	void			setTicketState();
protected:

	// 修改dom中开关状态
	void			setBreakState(SvgGraph* graph,BaseDevice* pdev,eBreakerState state);
	void			setBreakStateEx(SvgGraph* graph,QString svgid,QString cimid,eBreakerState state);

	// 修改dom中的style属性
	bool			setSvgStyle(SvgGraph* graph,QString svgid,QString style);

	// 修改设备状态列表中的设备状态
	void			setDevState(QList<PBNS::StateBean>devlist,SvgGraph* graph,BaseDevice* pdev);
	void			setDevStateEx(QList<PBNS::StateBean>devlist,SvgGraph* graph,BaseDevice* pdev);

	// 设置关联设备的颜色，开关设备除外，因为开关设备有着色过程
	void			setConnectedDevColor(SvgGraph* pgraph,SvgItem* item);

	// 根据svgid查找svgItem
	SvgItem*		findSvgItemById(QString id);
	SvgItem*		findSvgItemByCim(QString cimid);

	// 根据cimid查找设备对象
	bool				findUnitBeanByCimId(QString cimid,PBNS::StateBean & bean);

	// 发送挂牌操作请求
	void				sendTagReq(eTagState type);

	// 发送开关变位请求
	void				sendBreakOpReq(eBreakerState state,bool ischeck);

	// 开关变位
	void					switchChange(int state);
	QString				getNewSymbolId(QString oldid,int state);
	void					mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void			contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	// buff 转list
	QList<PBNS::StateBean>	getStateBeanList(PBNS::DevStateMsg_Response& res);
	QList<PBNS::StateBean>	getStateBeanList(PBNS::OprationMsg_Response& res);

	// 设备着色，变位
	void						drawDev(QList<PBNS::StateBean>  devList);

	// 关联着色
	void						colorDev(SvgGraph* graph,BaseDevice* pdev,PBNS::StateBean &bean,QString color);
	void						colorDevEx(SvgGraph* graph,SvgItem* item,PBNS::StateBean &bean,QString color);

	// 根据CIM查找已操作的设备
	int						findDevByCim(QString cim,PBNS::StateBean & sbean);

	// 加入本地操作的设备到列表
	void						putDev2OpList(QString cim,int state);

private:

	// 当前的系统状态
	eSysState				m_sysState;

	QMutex				m_mutex;
	QList<SvgItem*>		m_itemList;

	// 当前打开的存档ID
	int					m_saveId;

	// 当前打开的站点ID
	QString				m_stationCim;

	// 当前打开的图形序号
	int					m_curIndex;

	// 当前存档的变位记录
	QList<PBNS::StateBean>	m_opDevList;

	// 当前选中的item
	SvgItem*				m_curItem;
	QMenu*				m_cntMenu;
	SvgParser			m_svgParser;
	SvgRenderer*			m_svgRender;
	QList<SvgGraph*>		m_graphList;
	QPropertyAnimation	m_animation;

	PBNS::DevStateMsg_Response m_devList;
};

#endif