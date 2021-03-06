//==============================================================================
// KINSOLSolver plugin
//==============================================================================

#ifndef KINSOLSOLVERPLUGIN_H
#define KINSOLSOLVERPLUGIN_H

//==============================================================================

#include "plugininfo.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {
namespace KINSOLSolver {

//==============================================================================

PLUGININFO_FUNC KINSOLSolverPluginInfo();

//==============================================================================

class KINSOLSolverPlugin : public QObject, public SolverInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.KINSOLSolverPlugin" FILE "kinsolsolverplugin.json")

    Q_INTERFACES(OpenCOR::SolverInterface)

public:
    virtual Solver::Type type() const;
    virtual QString name() const;
    virtual Solver::Properties properties() const;

    virtual void * instance() const;
};

//==============================================================================

}   // namespace KINSOLSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
