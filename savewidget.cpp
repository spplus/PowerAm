#include "savewidget.h"
#include "define.h"

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
	hbox1->addStretch();

	QPushButton* okbtn = new QPushButton(tr("ȷ��"));
	QPushButton* cancelbtn = new QPushButton(tr("ȡ��"));
	connect(okbtn,SIGNAL(pressed()),this,SLOT(onOk()));
	connect(cancelbtn,SIGNAL(pressed()),this,SLOT(reject()));
	hbox2->addStretch();
	hbox2->addWidget(okbtn);
	hbox2->addWidget(cancelbtn);
	hbox2->addStretch();

	vbox->addStretch();
	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2);
	vbox->addStretch();

	setLayout(vbox);

	setFixedSize(300,150);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowTitle(tr("����浵"));

}

QString SaveWidget::getSaveName()
{
	return m_saveName->text();
}

void SaveWidget::onOk()
{
	if (m_saveName->text().length()<=0)
	{
		QMessageBox::warning(this,MSG_TITLE,"�浵���Ʋ�����Ϊ��");
		return;
	}
	else
	{
		accept();
	}
}