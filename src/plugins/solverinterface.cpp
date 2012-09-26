//==============================================================================
// Solver interface
//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

QString SolverInterface::typeAsString() const
{
    // Return the type of the solver as a string

    switch (type()) {
    case Solver::Ode:
        return QObject::tr("ODE");
    case Solver::Dae:
        return QObject::tr("DAE");
    case Solver::NonLinearAlgebraic:
        return QObject::tr("Non-linear algebraic");
    default:
        return "???";
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
