//==============================================================================
// Help plugin
//==============================================================================

#ifndef HELPPLUGIN_H
#define HELPPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace Help {

//==============================================================================

PLUGININFO_FUNC HelpPluginInfo();

//==============================================================================

class HelpWindow;

//==============================================================================

class HelpPlugin : public QObject, public CoreInterface, public GuiInterface,
                   public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.HelpPlugin" FILE "helpplugin.json")

    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual void initialize();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

private:
    QAction *mHelpAction;

    HelpWindow *mHelpWindow;
};

//==============================================================================

}   // namespace Help
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
