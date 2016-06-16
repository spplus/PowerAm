#include "savewidget.h"

SaveWidget::SaveWidget(QWidget* parent/* =NULL */)
	:QDialog(parent)
{
	initUi();
}

void SaveWidget::initUi()
{
	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox1 = new QHBoxLayout;
	QHBoxLayout* hbox2 = new QHBoxLayout;
	QLabel* pName = new QLabel;
	pName->setText(tr("�浵����"));
	m_saveName = new QLineEdit;

	hbox1->addStretch();
	hbox1->addWidget(pName);
	hbox1->addWidget(m_saveName);

	QPushButton* okbtn = new QPushButton(tr("ȷ��"));
	QPushButton* cancelbtn = new QPushButton(tr("ȡ��"));
	connect(okbtn,SIGNAL(pressed()),this,SLOT(accept()));
	connect(cancelbtn,SIGNAL(pressed()),this,SLOT(reject()));
	hbox2->addStretch();
	hbox2->addWidget(okbtn);
	hbox2->addWidget(cancelbtn);

	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2);
	setLayout(vbox);

	setFixedSize(400,300);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowTitle(tr("����浵"));

}

QString SaveWidget::getSaveName()
{
	return m_saveName->text();
}