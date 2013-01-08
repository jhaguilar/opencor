//==============================================================================
// CoreSolver plugin
//==============================================================================

#ifndef CORESOLVERPLUGIN_H
#define CORESOLVERPLUGIN_H

//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

PLUGININFO_FUNC CoreSolverPluginInfo();

//==============================================================================

class CoreSolverPlugin : public QObject
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CoreSolverPlugin" FILE "coresolverplugin.json")

public:
    ~CoreSolverPlugin();
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
