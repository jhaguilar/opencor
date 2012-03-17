//==============================================================================
// Core DAE solver class
//==============================================================================

#ifndef COREDAESOLVER_H
#define COREDAESOLVER_H

//==============================================================================

#include "coresolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

class CORESOLVER_EXPORT CoreDaeSolver : public CoreSolver
{
public:
    typedef void (*ComputeResidualsFunction)(double, double *, double *, double *, double *, double *, double *, double *, double *);
    typedef void (*ComputeEssentialVariablesFunction)(double, double *, double *, double *, double *, double *, double *, double *);
    typedef void (*ComputeRootInformationFunction)(double, double *, double *, double *, double *, double *, double *, double *);

    explicit CoreDaeSolver();

    virtual void initialize(const double &/* pVoiStart */,
                            const int &pStatesCount, const int &pCondVarCount,
                            double *pConstants, double *pRates, double *pStates,
                            double *pAlgebraic, double *pCondVar,
                            ComputeResidualsFunction /* pComputeResiduals */,
                            ComputeEssentialVariablesFunction /* pComputeEssentialVariables */,
                            ComputeRootInformationFunction /* pComputeRootInformation */);

    virtual void solve(double &pVoi, const double &pVoiEnd) const = 0;

protected:
    int mStatesCount;
    int mCondVarCount;

    double *mConstants;
    double *mRates;
    double *mStates;
    double *mAlgebraic;
    double *mCondVar;
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================