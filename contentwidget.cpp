#include "contentwidget.h"

ContentWidget::ContentWidget(QWidget* parent/* = NULL */)
	:QScrollArea(parent)
{
	initUi();
}

ContentWidget::~ContentWidget()
{

}

void ContentWidget::initUi()
{
	QWidget* rightBottomWidget = new QWidget; 
	m_gbox = new QGridLayout;
	rightBottomWidget->setLayout(m_gbox);
	
	//QScrollArea * scrollArea = new QScrollArea;
	this->setWidget(rightBottomWidget);
	this->setFrameShape(QScrollArea::NoFrame);

}

void ContentWidget::loadData()
{
	// º”‘ÿ∞¥≈•
	for (int i = 0;i<10;i++)
	{
		for (int j = 0;j<5;j++)
		{
			QPushButton* btn = new QPushButton("’æµ„");
			m_gbox->addWidget(btn,i,j,1,1);
		}
	}

}