/************************************************************************/
/* 
	DESC:	解析SVG文件，生成图形对象.
	DATE:	2016-03-24
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __SVGPARSER_H__
#define __SVGPARSER_H__

#include <QMap>
#include <QDomDocument>
#include "structs.h"
#include "define.h"
#include "svggraph.h"


class SvgParser
{
public:

	SvgParser();
	~SvgParser();
	SvgGraph*	parserSvg(QString filename);

private:
	void			parserHeader(SvgGraph*grahp,QDomNode &node);
	void			parserBus(SvgGraph*grahp,QDomNode &node);
	void			parserBreaker(SvgGraph*grahp,QDomNode &node);
	void			parserLink(SvgGraph*grahp,QDomNode &node);
	void			parserText(SvgGraph*grahp,QDomNode &node);
	void			parserDisconnector(SvgGraph*grahp,QDomNode &node);
	void			parserGroundDisconnector(SvgGraph*grahp,QDomNode &node);
	void			parserTransformer(SvgGraph*grahp,QDomNode &node);
	void			parserComDev(SvgGraph*grahp,QDomNode &node);
	void			parserMeasure(SvgGraph* graph,QDomNode& node);

	// 获取属性
	QString			getAttribute(const QDomNode &node,QString attrname);

	// 解析metadata
	void			parserMetaData(const QDomNode& node,BaseDevice* pdev);

	// 解析设备模型
	void			parserUse(const QDomNode& node,BaseDevice* pdev);

	// 解析设备列表
	void			parserSvgLayer(SvgGraph* graph,const QDomNode & node,eDeviceType type);

	// 打开文件
	bool			openSvg(QString filename,QDomDocument* doc);

	// 解析CSS
	void			parserStyle(QDomNode& node,QMap<QString,QString>& sMap);

	// 解析rgb 
	QColor		rgb2Qcolor(QString rgb);

	// 解析文本图元
	TextSvg*	parserSvgText(QDomNode& node);

	// 解析no use图元
	BaseDevice*		parserOriginal(const QDomNode& node);

	// 解析use 图元
	BaseDevice*		parserTemplate(const QDomNode& node);

	// 解析变压器
	BaseDevice*		parserTransformer(const QDomNode& node);
};

#endif
