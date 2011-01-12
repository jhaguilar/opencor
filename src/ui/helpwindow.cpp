#include "helpwindow.h"
#include "helpwidget.h"

#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QHelpEngine *helpEngine, const QUrl& homepage,
                       QDockWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);

    helpWidget = new HelpWidget(helpEngine);

    setWidget(helpWidget);
    setHomepage(homepage);
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

void HelpWindow::setHomepage(const QUrl& homepage)
{
    helpWidget->load(homepage);
}

void HelpWindow::setHelpWidgetTextSizeMultiplier(const double& value)
{
    helpWidget->setTextSizeMultiplier(value);
}

double HelpWindow::helpWidgetTextSizeMultiplier()
{
    return helpWidget->textSizeMultiplier();
}
