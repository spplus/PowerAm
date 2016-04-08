/************************************************************************/
/* 
	DESC:		定义开关相关模型，包括负荷开关，刀闸，接地刀闸.
	DATE:		2016-03-30
	AUTHOR:		YUANLS
*/
/************************************************************************/

#ifndef __BREAKER_H__
#define __BREAKER_H__

#include "define.h"
#include "basedev.h"


class Breaker	:public BaseDevice
{
public:

	eBreakerState		getState(){return m_state;}
	void				setState(eBreakerState state){m_state = state;}

private:
	
	// 开关状态
	eBreakerState		m_state;

};

#endif
