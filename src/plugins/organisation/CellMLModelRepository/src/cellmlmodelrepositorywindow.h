#ifndef CELLMLMODELREPOSITORYWINDOW_H
#define CELLMLMODELREPOSITORYWINDOW_H

#include "dockwidget.h"

namespace Ui {
    class CellmlModelRepositoryWindow;
}

namespace OpenCOR {
namespace CellMLModelRepository {

class CellmlModelRepositoryWidget;

class CellmlModelRepositoryWindow : public Core::DockWidget
{
    Q_OBJECT

public:
    explicit CellmlModelRepositoryWindow(QWidget *pParent = 0);
    ~CellmlModelRepositoryWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::CellmlModelRepositoryWindow *mUi;

    CellmlModelRepositoryWidget *mCellmlModelRepositoryWidget;
};

} }

#endif