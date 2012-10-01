//==============================================================================
// Core DAE solver class
//==============================================================================

#include "coredaesolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreDaeSolver::CoreDaeSolver() :
    CoreSolver(),
    mStatesCount(0),
    mCondVarCount(0),
    mConstants(0),
    mRates(0),
    mStates(0),
    mAlgebraic(0),
    mCondVar(0)
{
}

//==============================================================================

void CoreDaeSolver::initialize(const double &pVoiStart, const double &pVoiEnd,
                               const int &pStatesCount, const int &pCondVarCount,
                               double *pConstants, double *pRates,
                               double *pStates, double *pAlgebraic,
                               double *pCondVar,
                               ComputeEssentialVariablesFunction pComputeEssentialVariables,
                               ComputeResidualsFunction pComputeResiduals,
                               ComputeRootInformationFunction pComputeRootInformation,
                               ComputeStateInformationFunction pComputeStateInformation)
{
    Q_UNUSED(pVoiStart);
    Q_UNUSED(pVoiEnd);
    Q_UNUSED(pComputeEssentialVariables);
    Q_UNUSED(pComputeResiduals);
    Q_UNUSED(pComputeRootInformation);
    Q_UNUSED(pComputeStateInformation);

    // Initialise the DAE solver

    mStatesCount  = pStatesCount;
    mCondVarCount = pCondVarCount;

    mConstants = pConstants;
    mRates     = pRates;
    mStates    = pStates;
    mAlgebraic = pAlgebraic;
    mCondVar   = pCondVar;
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
