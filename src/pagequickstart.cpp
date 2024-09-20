#include "quickstartwindow.h"
#include "pagequickstart.h"

PageQuickStart::PageQuickStart(QWidget *parent) : QWidget(parent), m_ui(new Ui::PageQuickWindowClass)
{
	m_ui->setupUi(this);
	m_ui->showOnStartupCheckBox->setCheckState(QuickStart::isShowOnStartup() ? Qt::Checked : Qt::Unchecked);
}

PageQuickStart::~PageQuickStart()
{
	delete m_ui;
}

void PageQuickStart::updateShowOnStartupSetting()
{
	if (m_ui->showOnStartupCheckBox->checkState() != Qt::Unchecked)
		QuickStart::setShowOnStartup();
	else
		QuickStart::resetShowOnStartup();
}