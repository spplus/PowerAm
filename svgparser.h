/************************************************************************/
/* 
	DESC:	����SVG�ļ�������ͼ�ζ���.
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
	void			parserACLine(SvgGraph*grahp,QDomNode &node);
	void			parserText(SvgGraph*grahp,QDomNode &node);
	void			parserDisconnector(SvgGraph*grahp,QDomNode &node);
	void			parserGroundDisconnector(SvgGraph*grahp,QDomNode &node);
	void			parserTransformer(SvgGraph*grahp,QDomNode &node);
	void			parserComDev(SvgGraph*grahp,QDomNode &node);
	void			parserMeasure(SvgGraph* graph,QDomNode& node);

	// ��ȡ����
	QString			getAttribute(const QDomNode &node,QString attrname);

	// ����metadata
	void			parserMetaData(const QDomNode& node,BaseDevice* pdev);

	// �����豸ģ��
	void			parserUse(const QDomNode& node,BaseDevice* pdev);

	// �����豸�б�
	void			parserSvgLayer(SvgGraph* graph,const QDomNode & node,eDeviceType type);

	// ���ļ�
	bool			openSvg(QString filename,QDomDocument* doc);

	// ����CSS
	void			parserStyle(QDomNode& node,QMap<QString,QString>& sMap);

	// ����rgb 
	QColor		rgb2Qcolor(QString rgb);

	// �����ı�ͼԪ
	TextSvg*	parserSvgText(QDomNode& node);

	// ����no useͼԪ
	BaseDevice*		parserOriginal(const QDomNode& node);

	// ����use ͼԪ
	BaseDevice*		parserTemplate(const QDomNode& node);

	// ������ѹ��
	BaseDevice*		parserTransformer(const QDomNode& node);

	// ����Ƿ���ID���ԣ����û�����Զ�����һ��Ĭ�ϵ�ID
	void			checkIdAttr(const QDomNode& node);

	// ת��svg��ʽ
	void			convertSvg(SvgGraph* pgraph);
};

#endif
