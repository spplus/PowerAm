﻿
#include "radiowidget.h"

RadioWidget::RadioWidget(QWidget* parent/* = NULL */)
{
	initUi();
	initSlots();
}

void RadioWidget::initUi()
{
	QHBoxLayout* hbox = new QHBoxLayout;
	
	//char* pname = "ID";
	m_realtimeBtn = new QRadioButton(tr("实时态"));

	m_analogBtn = new QRadioButton(tr("模拟态"));
	m_analogBtn->setChecked(true);

	m_ticketBtn = new QRadioButton(tr("拟票态"));

	m_realtimeBtn->setIcon(QIcon(ICON_REALTIME));
	m_analogBtn->setIcon(QIcon(ICON_ANALOG));
	m_ticketBtn->setIcon(QIcon(ICON_TICKET));
	hbox->addWidget(m_realtimeBtn);
	hbox->addWidget(m_analogBtn);
	hbox->addWidget(m_ticketBtn);

	this->setLayout(hbox);
}

void RadioWidget::initSlots()
{
	connect(m_realtimeBtn,SIGNAL(pressed()),this,SIGNAL(realtime()));
	connect(m_analogBtn,SIGNAL(pressed()),this,SIGNAL(analog()));
	connect(m_ticketBtn,SIGNAL(pressed()),this,SIGNAL(ticket()));
}