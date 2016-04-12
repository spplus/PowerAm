/************************************************************************/
/* 
		DESC:		����SVGͼ��.
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

	// ��ȡͼ���б������
	QList<SvgLayer*>&	getLayerList();

	// ����DOM�ڵ�����
	bool			setAttribute(QString nodeid,QString attr,QString val);

	// ��ȡDOM�ڵ�����
	QString			getAttribute(QString nodeid,QString attr);

	// ����svgId��ȡ�豸����
	BaseDevice*		getDevById(QString svgid);

private:

	QDomNode	getElementById(QString nodeid);

	// ͼID
	QString			m_id;

	// ͼ�ļ�·��
	QString			m_filePath;

	// �ļ�dom����
	QDomDocument*	m_dom;

	// CSS
	QMap<QString,QString> m_styleMap;

	// ͼ���б�
	QList<SvgLayer*>	m_layerList;
};

#endif