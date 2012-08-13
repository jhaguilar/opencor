//==============================================================================
// CellML file import
//==============================================================================

#ifndef CELLMLFILEIMPORT_H
#define CELLMLFILEIMPORT_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlfileimportunit.h"
#include "cellmlfileimportcomponent.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImport : public CellmlFileElement
{
public:
    explicit CellmlFileImport(CellmlFile *pCellmlFile,
                              iface::cellml_api::CellMLImport *pCellmlApiImport);
    ~CellmlFileImport();

    QString xlinkHref() const;

    CellmlFileImportUnits units() const;
    CellmlFileImportComponents components() const;

private:
    QString mXlinkHref;

    CellmlFileImportUnits mUnits;
    CellmlFileImportComponents mComponents;
};

//==============================================================================

typedef QList<CellmlFileImport *> CellmlFileImports;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
