/************************************************************************/
/* 
	DESC:		���忪�����ģ�ͣ��������ɿ��أ���բ���ӵص�բ.
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
	
	// ����״̬
	eBreakerState		m_state;

};

#endif
