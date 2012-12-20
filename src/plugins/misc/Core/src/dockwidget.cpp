//==============================================================================
// Dock widget
//==============================================================================

#include "dockwidget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

DockWidget::DockWidget(QWidget *pParent) :
    QDockWidget(pParent),
    CommonWidget(pParent)
{
#ifdef Q_OS_MAC
    // Remove the padding for the float and close buttons on OS X (indeed it
    // makes the title bar of a docked (i.e. non-floating) window insanely big,
    // so...)

    setStyleSheet("QDockWidget::float-button, QDockWidget::close-button {"
                  "    padding: 0px;"
                  "}");
#endif
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
