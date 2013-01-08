//==============================================================================
// FileBrowser plugin
//==============================================================================

#ifndef FILEBROWSERPLUGIN_H
#define FILEBROWSERPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace FileBrowser {

//==============================================================================

PLUGININFO_FUNC FileBrowserPluginInfo();

//==============================================================================

class FileBrowserWindow;

//==============================================================================

class FileBrowserPlugin : public QObject, public CoreInterface,
                          public GuiInterface, public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.FileBrowserPlugin" FILE "filebrowserplugin.json")

    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual void initialize();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

private:
    QAction *mFileBrowserAction;

    FileBrowserWindow *mFileBrowserWindow;
};

//==============================================================================

}   // namespace FileBrowser
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
