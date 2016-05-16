#include "waitwidget.h"
WaitWidget::WaitWidget(QWidget*parent /* = NULL */)
	:QWidget(parent)
{
	QHBoxLayout* hbox = new QHBoxLayout;
	QLabel* giflabel = new QLabel;
	
	setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);

	this->setAutoFillBackground(true);
	this->setContentsMargins(0,0,0,0);
	giflabel->setContentsMargins(0,0,0,0);
	QMovie*movie=new QMovie(":images/loading.gif");

	giflabel->setMovie(movie);
	movie->start();
	hbox->addWidget(giflabel);
	hbox->setMargin(0);
	
	setFixedSize(48,48);
	setLayout(hbox);

	QDesktopWidget *deskdop = QApplication::desktop();
	move((deskdop->width() - this->width())/2, (deskdop->height() - this->height())/2);

}