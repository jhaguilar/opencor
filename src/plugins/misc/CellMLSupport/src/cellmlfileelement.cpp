//==============================================================================
// CellML file element
//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlfilemanager.h"

//==============================================================================

#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Model *pModel) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pModel),
    mCmetaId(QString::fromStdWString(pModel->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::CellMLImport *pCellmlImport) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pCellmlImport),
    mCmetaId(QString::fromStdWString(pCellmlImport->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::ImportUnits *pImportUnits) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pImportUnits),
    mCmetaId(QString::fromStdWString(pImportUnits->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::ImportComponent *pImportComponent) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pImportComponent),
    mCmetaId(QString::fromStdWString(pImportComponent->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Units *pUnits) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pUnits),
    mCmetaId(QString::fromStdWString(pUnits->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Unit *pUnit) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pUnit),
    mCmetaId(QString::fromStdWString(pUnit->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::CellMLComponent *pComponent) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pComponent),
    mCmetaId(QString::fromStdWString(pComponent->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::CellMLVariable *pVariable) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pVariable),
    mCmetaId(QString::fromStdWString(pVariable->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Group *pGroup) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pGroup),
    mCmetaId(QString::fromStdWString(pGroup->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::RelationshipRef *pRelationshipRef) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pRelationshipRef),
    mCmetaId(QString::fromStdWString(pRelationshipRef->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::ComponentRef *pComponentRef) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pComponentRef),
    mCmetaId(QString::fromStdWString(pComponentRef->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Connection *pConnection) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pConnection),
    mCmetaId(QString::fromStdWString(pConnection->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::MapComponents *pMapComponents) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pMapComponents),
    mCmetaId(QString::fromStdWString(pMapComponents->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::MapVariables *pMapVariables) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pMapVariables),
    mCmetaId(QString::fromStdWString(pMapVariables->cmetaId()))
{
}

//==============================================================================

CellmlFileElement::~CellmlFileElement()
{
    // We took ownership of the CellML API element (i.e. mCellmlElement), so we
    // must call its release_ref() method

    mCellmlElement->release_ref();
}

//==============================================================================

CellmlFileRdfTriples CellmlFileElement::rdfTriples() const
{
    // Return all the RDF triples associated with the element

    if (mCmetaId.isEmpty())
        // The element doesn't have a 'proper' cmeta:id, so...

        return CellmlFileRdfTriples(mCellmlFile);
    else
        // The element has a 'proper' cmeta:id, so we can retrieve and return
        // the RDF triples associated with it

        return mCellmlFile->rdfTriples(mCmetaId);
}

//==============================================================================

QString CellmlFileElement::rdfTripleSubject()
{
    // Make sure that we have a 'proper' cmeta:id or generate one, if needed

    if (mCmetaId.isEmpty()) {
        // We don't have a 'proper' cmeta:id, so we need to generate one and in
        // order to do so, we need to know what cmeta:ids are currently in use
        // in the CellML file, meaning we first need to retrieve all the RDF
        // triples in the CellML file

        CellmlFileRdfTriples *rdfTriples = mCellmlFile->rdfTriples();

        // Next, we need to retrieve all the cmeta:ids from our different RDF
        // triples

        QStringList cmetaIds = QStringList();

        foreach (CellmlFileRdfTriple *rdfTriple, *rdfTriples) {
            QString cmetaId = rdfTriple->metadataId();

            if (!cmetaIds.contains(cmetaId))
                cmetaIds << cmetaId;
        }

        // Now, we try different cmeta:id values until we find one which is not
        // present in our list

        int counter = 0;

        forever {
            mCmetaId = QString("id_%1").arg(++counter, 5, 10, QChar('0'));

            if (!cmetaIds.contains(mCmetaId)) {
                // We have found a unique cmeta:id, so update our CellML element
                // and leave

                mCellmlElement->cmetaId(mCmetaId.toStdWString());

                mCellmlFile->setModified(true);

                break;
            }
        }
    }

    // Return the subject which should be used for an RDF triple

    return mCellmlFile->uriBase()+"#"+mCmetaId;
}

//==============================================================================

bool CellmlFileElement::hasMetadata(const QString &pQualifier,
                                    const QString &pResource,
                                    const QString &pId)
{
    // Return whether the given metadata is associated with the CellML element

    if (mCmetaId.isEmpty())
        // The CellML element doesn't have a 'proper' cmeta:id, so...

        return false;

    // Go through the RDF triples associated with the CellML element and check
    // whether one of them corresponds to the given metadata

    CellmlFileRdfTriples rdfTriples = mCellmlFile->rdfTriples(mCmetaId);

    foreach (CellmlFileRdfTriple *rdfTriple, rdfTriples)
        if (   !pQualifier.compare(rdfTriple->qualifierAsString())
            && !pResource.compare(rdfTriple->resource())
            && !pId.compare(rdfTriple->id()))
            // This is the metadata we are after, so...

            return true;

    // We couldn't find the metadata, so...

    return false;
}

//==============================================================================

bool CellmlFileElement::hasMetadata(const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                    const QString &pResource, const QString &pId)
{
    // Call our generic hasMetadata() function

    return hasMetadata(CellMLSupport::CellmlFileRdfTriple::modelQualifierAsString(pModelQualifier),
                       pResource, pId);
}

//==============================================================================

bool CellmlFileElement::hasMetadata(const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                    const QString &pResource, const QString &pId)
{
    // Call our generic hasMetadata() function

    return hasMetadata(CellMLSupport::CellmlFileRdfTriple::bioQualifierAsString(pBioQualifier),
                       pResource, pId);
}

//==============================================================================

CellMLSupport::CellmlFileRdfTriple * CellmlFileElement::addMetadata(const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                                                    const QString &pResource,
                                                                    const QString &pId)
{
    // Add our metadata to our CellML file, this as an RDF triple

    return mCellmlFile->rdfTriples()->add(new CellMLSupport::CellmlFileRdfTriple(mCellmlFile, rdfTripleSubject(),
                                                                                 pModelQualifier, pResource, pId));
}

//==============================================================================

CellMLSupport::CellmlFileRdfTriple * CellmlFileElement::addMetadata(const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                                                    const QString &pResource,
                                                                    const QString &pId)
{
    // Add our metadata to our CellML file, this as an RDF triple

    return mCellmlFile->rdfTriples()->add(new CellMLSupport::CellmlFileRdfTriple(mCellmlFile, rdfTripleSubject(),
                                                                                 pBioQualifier, pResource, pId));
}

//==============================================================================

void CellmlFileElement::removeAllMetadata()
{
    // Remove, from our CellML file, all the metadata associated with the CellML
    // element's cmeta:id, but only if the CellML element has a 'proper'
    // cmeta:id

    if (!mCmetaId.isEmpty())
        mCellmlFile->rdfTriples()->remove(mCmetaId);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
