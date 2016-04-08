/************************************************************************/
/* 
	DESC:		��ѹ��.
	DATE:		2016-03-31
	AUTHOR:		YUANLS	
*/
/************************************************************************/

#ifndef __TRANSFORMER_H__
#define __TRANSFORMER_H__
#include <QList>

#include "basedev.h"

class Transformer	:public BaseDevice
{
public:
	~Transformer()
	{
		for (int i = 0;i<m_windingList.size();i++)
		{
			BaseDevice* pdev = m_windingList.at(i);
			delete pdev;
			pdev = NULL;
		}
	}
	eTransType			getTransType(){return m_transType;}
	void				setTransType(eTransType ttype){m_transType = ttype;}
	QList<BaseDevice*>&	getWindList(){return m_windingList;}
private:
	// ��ѹ������
	eTransType			m_transType;

	// �����б�
	QList<BaseDevice*>	m_windingList;
};

#endif