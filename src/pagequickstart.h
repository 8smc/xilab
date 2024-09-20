#ifndef PAGEQUICKSTART_H
#define PAGEQUICKSTART_H
#include <QtGui/QWidget>
#include "ui_pagequickstart.h"

class PageQuickStart : public QWidget
{
	Q_OBJECT
private:
	Q_DISABLE_COPY(PageQuickStart)

public:
	PageQuickStart(QWidget *parent);
	~PageQuickStart();
	void updateShowOnStartupSetting();
private:
	Ui::PageQuickWindowClass *m_ui;
};


#endif // PAGEQUICKSTART_H