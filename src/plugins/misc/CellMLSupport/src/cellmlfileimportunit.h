//==============================================================================
// CellML file import unit
//==============================================================================

#ifndef CELLMLFILEIMPORTUNIT_H
#define CELLMLFILEIMPORTUNIT_H

//==============================================================================

#include "cellmlfileunit.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImportUnit : public CellmlFileUnit
{
public:
    explicit CellmlFileImportUnit(CellmlFile *pCellmlFile,
                                  iface::cellml_api::ImportUnits *pCellmlApiImportUnits);

    QString unitReference() const;

private:
    QString mUnitReference;
};

//==============================================================================

typedef QList<CellmlFileImportUnit *> CellmlFileImportUnits;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
