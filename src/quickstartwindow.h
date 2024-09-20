#ifndef QUICKSTARTWINDOW_H
#define QUICKSTARTWINDOW_H
#include <QWidget>
#include "ui_quickstartwindow.h"
#include "startwindow.h"

namespace QuickStart
{
	bool isShowOnStartup();
	void setShowOnStartup();
	void resetShowOnStartup();
}

class Step
{
public:
	Step(QString txt, QPixmap pxmp, unsigned int idx) :
		text(txt),
		pixmap(pxmp),
		step_index(idx) {};

	QString getText() const { return text; };
	QPixmap getPixmap() const { return pixmap; };
	unsigned int getStepIndex() const { return step_index; };
private:
	QString text;
	QPixmap pixmap;
	unsigned int step_index;
};

class QuickStartWindow : public QDialog {
	Q_OBJECT
public:
	QuickStartWindow(const StartWindow *startWnd, QDialog *parent = 0);
	~QuickStartWindow();
private:
	void initializeShowOnStartupFlag();
	Ui::QuickStartWindowUi *m_ui;

	QList<Step> stepsList;
	unsigned int current_step = 0;
	unsigned int step_num = 0;
	void populateStepsList();
	void showCurrentStep();
public slots:
	void showNextStep();
	void showPrevStep();
	void closeWindow();
	void changeShowOnStartupSetting(int);
	void showWindow();
};

#endif