//==============================================================================
// Tool bar widget
//==============================================================================

#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QToolBar>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT ToolBarWidget : public QToolBar
{
    Q_OBJECT

public:
    explicit ToolBarWidget(QWidget *pParent = 0);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
