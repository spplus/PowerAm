/************************************************************************/
/* 

	DESC:	定义图形相关数据结构. 
	DATE:	2016-03-24
	AUTHOR:	YUANLS
	
*/
/************************************************************************/

#ifndef __SVGSTRUCT_H__
#define __SVGSTRUCT_H__
#include <QString>
#include <QList>
#include <QMap>
#include <QDomDocument>

typedef struct sEleDevice{

	QString			sid;
	int				sxPos;
	int				syPos;
	QString			sclass;
	QString			sdevId;
	QString			sdevName;
	QString			smeasId;
	QString			shrefId;
	QString			stransform;

} svgDevice;

typedef struct sGraphLayer {

	QString				sid;
	int					height;
	int					width;
	QString				fill;
	QList<svgDevice*>	sdevList;
	sGraphLayer()
	{
		sid = "";
		height = 0;
		width = 0;
		fill = "";
	}
	~sGraphLayer()
	{
		for (int i=0;i<sdevList.size();i++)
		{
			svgDevice* pdev = sdevList.at(i);
			if (pdev != NULL)
			{
				delete pdev;
				pdev = NULL;
			}
		}
	}

}svgLayer;

typedef struct sGraph{

	QString			sid;
	QString			sfilePath;
	QDomDocument*	sdom;
	QList<svgLayer*>slayerList;
	QMap<QString,QString> sStyleMap;

	sGraph()
	{
		sid = "";
		sfilePath = "";
		sdom = new QDomDocument();
	}
	~sGraph()
	{
		if (sdom != NULL)
		{
			delete sdom;
		}
		for (int i=0;i<slayerList.size();i++)
		{
			svgLayer *player = slayerList.at(i);
			if (player != NULL)
			{
				delete player;
				player = NULL;
			}
		}
	}

}svgGraph;

#endif