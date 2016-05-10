/************************************************************************/
/* 
		DESC:	公共方法类.
		DATE:	2016-05-10
		AUTHOR:	YUANLS	
*/
/************************************************************************/
#ifndef __COMUTIL_H__
#define __COMUTIL_H__

#include <QString>

class ComUtil
{

public:
	
	// 
	static ComUtil*		instance();

	//  获取系统名称
	QString				getSysName();

private:
	ComUtil();

private:
	static	ComUtil*	m_inst;
};

#endif