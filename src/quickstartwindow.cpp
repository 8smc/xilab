/* Dear curious user, this file implements Quick Start window functionality.
 *
 * If your restless mind wishes to add or change texts and/or images of the Quick Start steps, you must know some details:
 *     * Images (image.png) and texts (message.html) reside in ./Resources/quickstartwindow/ directory in "step-i" folders,
 *       where "i" is your desired step number.
 *     * File names are important! To succeed, use "image.png" for images and "message.html" for texts.
 *     * Image should not exceed 625x210px
 *     * If you want to add new step (tested on Windows):
 *         1. Create ./Resources/quickstartwindow/step-i folder, where "i" is your desired step number
 *         2. Fill the folder with message.html. Optionally: add image.png
 *         3. Add "<file>quickstartwindow/step-i/message.html</file>" line into ./Resources/XILab.qrc file into <qresource> section.
 *            The section already contains some "<file>quickstartwindow/step-i/message.html</file>" lines, so you won't get confused.
 *            Optionally: add "<file>quickstartwindow/step-i/image.png</file>" line.
 *         4. In ./Resources directory run:
 *                rcc XILab.qrc -o ..\GeneratedFiles\qrc_XILab.cpp
 *            to update/create the resource file.
 */
#include <QFile>
#include <QTextStream>
#include <QDirIterator>
#include "quickstartwindow.h"
#include "xsettings.h"
#include "functions.h"

static bool show_on_startup = true;

bool QuickStart::isShowOnStartup() { return show_on_startup; }
void QuickStart::setShowOnStartup() { show_on_startup = true; }
void QuickStart::resetShowOnStartup() { show_on_startup = false; }

QuickStartWindow::QuickStartWindow(const StartWindow *startWnd, QDialog *parent) : QDialog(parent), m_ui(new Ui::QuickStartWindowUi)
{
	m_ui->setupUi(this);

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint );
	
	populateStepsList();
	showCurrentStep();

	m_ui->text_browser->setOpenExternalLinks(true);

	connect(m_ui->next_step_button, SIGNAL(clicked()), this, SLOT(showNextStep()));
	connect(m_ui->prev_step_button, SIGNAL(clicked()), this, SLOT(showPrevStep()));
	connect(m_ui->close_button, SIGNAL(clicked()), this, SLOT(closeWindow()));
	connect(m_ui->dontShowOnStartupCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeShowOnStartupSetting(int)));
	connect(startWnd, SIGNAL(showQuickStartWindow()), this, SLOT(showWindow()));
	
	initializeShowOnStartupFlag();
	m_ui->dontShowOnStartupCheckBox->setChecked(!show_on_startup);

	if (show_on_startup)
		showWindow();
}


static bool compareStepIndecies(const Step &step_a, const Step &step_b)
{
	return step_a.getStepIndex() < step_b.getStepIndex();
}

void QuickStartWindow::populateStepsList()
{
	QDirIterator step_dirs_iterator(":/quickstartwindow/");
	while (step_dirs_iterator.hasNext())
	{
		const QString curr_dir = step_dirs_iterator.next();
		const unsigned int step_index = curr_dir.split("-")[1].toInt();

		QFile message_file(curr_dir + "/message.html");
		QString message;
		if (message_file.open(QFile::ReadOnly | QFile::Text))
		{
			QTextStream stream(&message_file);
			message = stream.readAll();
		}
		else
			message = "";

		QPixmap pixmap = QFile::exists(curr_dir + "/image.png") ? QPixmap(curr_dir + "/image.png") : QPixmap();
		stepsList.append(Step(message, pixmap, step_index));

		step_num++;
		message_file.close();
	}
	qSort(stepsList.begin(), stepsList.end(), compareStepIndecies);
}

QuickStartWindow::~QuickStartWindow()
{
	QString config_filename = MakeProgramConfigFilename();
	if (!QFile::exists(config_filename))
		return;

	XSettings settings(config_filename, QSettings::IniFormat, QIODevice::ReadWrite);
	settings.beginGroup("Start_window");
	settings.setValue("show_quickstart_on_startup", QVariant(show_on_startup));
	settings.endGroup();
	delete m_ui;
	stepsList.clear();
}

void QuickStartWindow::showCurrentStep()
{
	if (current_step == 0)
		this->setWindowTitle(QString("Quick Start: Welcome page"));
	else
		this->setWindowTitle(QString("Quick Start: Step ") + QString::number(current_step));
	Step step = stepsList.at(current_step);
	m_ui->text_browser->setHtml(step.getText());
	m_ui->image_label->setPixmap(step.getPixmap());

	m_ui->prev_step_button->setEnabled(current_step == 0 ? false : true);
	m_ui->next_step_button->setEnabled(current_step >= step_num - 1 ? false : true);
}

void QuickStartWindow::showNextStep()
{
	current_step < step_num - 1 ? current_step++ : current_step = step_num - 1;
	showCurrentStep();
}

void QuickStartWindow::showPrevStep()
{
	current_step > 0 ? current_step-- : current_step = 0;
	showCurrentStep();
}

void QuickStartWindow::closeWindow()
{
	close();
}

void QuickStartWindow::changeShowOnStartupSetting(int dont_show_on_startup)
{
	show_on_startup = !dont_show_on_startup;
} 

void QuickStartWindow::initializeShowOnStartupFlag()
{
	QString config_filename = MakeProgramConfigFilename();
	if (!QFile::exists(config_filename))
	{
		show_on_startup = true;
		return;
	}
	XSettings settings(config_filename, QSettings::IniFormat, QIODevice::ReadWrite);
	settings.beginGroup("Start_window");
	show_on_startup = settings.value("show_quickstart_on_startup", true).value<bool>();
	settings.endGroup();
}

void QuickStartWindow::showWindow()
{
	this->show();
	this->activateWindow();
}