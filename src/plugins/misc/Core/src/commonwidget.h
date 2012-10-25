//==============================================================================
// Common widget
//==============================================================================

#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

//==============================================================================

#include "coreglobal.h"
#include "plugin.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

#include <QColor>
#include <QString>

//==============================================================================

class QSettings;
class QSize;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

// The default font family and size below were taken from Qt Creator

#ifdef Q_WS_WIN
    static const QString DefaultFontFamily = "Courier";
    static const int DefaultFontSize = 10;
#elif defined(Q_WS_MAC)
    static const QString DefaultFontFamily = "Monaco";
    static const int DefaultFontSize = 12;
#else
    static const QString DefaultFontFamily = "Monospace";
    static const int DefaultFontSize = 9;
#endif

//==============================================================================

class CORE_EXPORT CommonWidget
{
public:
    explicit CommonWidget(QWidget *pParent);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void loadingOfSettingsDone(const Plugins &pLoadedPlugins);

protected:
    QSize defaultSize(const double &pRatio) const;

    void initBorderColor();
    void drawBorderIfDocked(const bool &pForceDrawing = false,
                            const bool &pTop = true, const bool &pLeft = true,
                            const bool &pBottom = true, const bool &pRight = true);

private:
    QWidget *mParent;

    QColor mBorderColor;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
