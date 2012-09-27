//==============================================================================
// Computer test
//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

namespace llvm {
    class Function;
}   // namespace llvm

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Computer {
    class ComputerEngine;
}   // namespace Computer

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

class Test : public QObject
{
    Q_OBJECT

private:
    OpenCOR::Computer::ComputerEngine *mComputerEngine;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void basicTests();

    void voidFunctionTests();

    void timesOperatorTests();
    void divideOperatorTests();
    void moduloOperatorTests();
    void plusOperatorTests();
    void minusOperatorTests();

    void notOperatorTests();
    void orOperatorTests();
    void xorOperatorTests();
    void andOperatorTests();
    void equalEqualOperatorTests();
    void notEqualOperatorTests();
    void lowerThanOperatorTests();
    void greaterThanOperatorTests();
    void lowerOrEqualThanOperatorTests();
    void greaterOrEqualThanOperatorTests();

    void absFunctionTests();
    void expFunctionTests();
    void logFunctionTests();
    void ceilFunctionTests();
    void floorFunctionTests();
    void factorialFunctionTests();
    void sinFunctionTests();
    void cosFunctionTests();
    void tanFunctionTests();
    void sinhFunctionTests();
    void coshFunctionTests();
    void tanhFunctionTests();
    void asinFunctionTests();
    void acosFunctionTests();
    void atanFunctionTests();
    void asinhFunctionTests();
    void acoshFunctionTests();
    void atanhFunctionTests();

    void arbitraryLogFunctionTests();
    void powFunctionTests();

    void gcdFunctionTests();
    void lcmFunctionTests();
    void maxFunctionTests();
    void minFunctionTests();

    void defIntFunctionTests();
};

//==============================================================================
// End of file
//==============================================================================
