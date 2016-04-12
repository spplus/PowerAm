/************************************************************************/
/* 
		DESC:		定义SVG图形.
		DATE:		2016-03-31
		AUTHOR:		YUANLS
*/
/************************************************************************/
#ifndef __SVGGRAPH_H__
#define __SVGGRAPH_H__


#include <QDomDocument>
#include <QMap>

#include "svglayer.h"

class SvgGraph
{
public:

	SvgGraph();

	~SvgGraph();
	void				setId(QString id);
	QString				getId();

	void				setFilePath(QString path);
	QString				getFilePath();

	QDomDocument*		getDom();

	QMap<QString,QString>& getStyleMap();

	// 获取图层列表的引用
	QList<SvgLayer*>&	getLayerList();

	// 设置DOM节点属性
	bool			setAttribute(QString nodeid,QString attr,QString val);

	// 获取DOM节点属性
	QString			getAttribute(QString nodeid,QString attr);

	// 根据svgId获取设备对象
	BaseDevice*		getDevById(QString svgid);

private:

	QDomNode	getElementById(QString nodeid);

	// 图ID
	QString			m_id;

	// 图文件路径
	QString			m_filePath;

	// 文件dom对象
	QDomDocument*	m_dom;

	// CSS
	QMap<QString,QString> m_styleMap;

	// 图层列表
	QList<SvgLayer*>	m_layerList;
};

#endif