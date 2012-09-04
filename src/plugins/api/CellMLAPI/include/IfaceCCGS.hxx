/* This file is automatically generated from CCGS.idl
 * DO NOT EDIT DIRECTLY OR CHANGES WILL BE LOST!
 */
#ifndef _GUARD_IFACE_CCGS
#define _GUARD_IFACE_CCGS
#include "cda_compiler_support.h"
#ifdef MODULE_CONTAINS_CCGS
#define PUBLIC_CCGS_PRE CDA_EXPORT_PRE
#define PUBLIC_CCGS_POST CDA_EXPORT_POST
#else
#define PUBLIC_CCGS_PRE CDA_IMPORT_PRE
#define PUBLIC_CCGS_POST CDA_IMPORT_POST
#endif
#include "Ifacexpcom.hxx"
#include "IfaceDOM_APISPEC.hxx"
#include "IfaceMathML_content_APISPEC.hxx"
#include "IfaceCellML_APISPEC.hxx"
#include "IfaceCUSES.hxx"
#include "IfaceAnnoTools.hxx"
#include "IfaceCeVAS.hxx"
#include "IfaceMaLaES.hxx"
namespace iface
{
  namespace cellml_services
  {
    class CustomGenerator;
    typedef enum _enum_VariableEvaluationType
    {
      VARIABLE_OF_INTEGRATION = 0,
      CONSTANT = 1,
      STATE_VARIABLE = 2,
      ALGEBRAIC = 3,
      FLOATING = 4,
      LOCALLY_BOUND = 5,
      PSEUDOSTATE_VARIABLE = 6
    } VariableEvaluationType;
    typedef enum _enum_ModelConstraintLevel
    {
      UNDERCONSTRAINED = 0,
      UNSUITABLY_CONSTRAINED = 1,
      OVERCONSTRAINED = 2,
      CORRECTLY_CONSTRAINED = 3
    } ModelConstraintLevel;
    PUBLIC_CCGS_PRE 
    class  PUBLIC_CCGS_POST ComputationTarget
     : public virtual iface::XPCOM::IObject
    {
    public:
      static const char* INTERFACE_NAME() { return "cellml_services::ComputationTarget"; }
      virtual ~ComputationTarget() {}
      virtual already_AddRefd<iface::cellml_api::CellMLVariable>  variable() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual uint32_t degree() throw(std::exception&)  = 0;
      virtual iface::cellml_services::VariableEvaluationType type() throw(std::exception&)  = 0;
      virtual std::wstring name() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual uint32_t assignedIndex() throw(std::exception&)  = 0;
    };
    PUBLIC_CCGS_PRE 
    class  PUBLIC_CCGS_POST ComputationTargetIterator
     : public virtual iface::XPCOM::IObject
    {
    public:
      static const char* INTERFACE_NAME() { return "cellml_services::ComputationTargetIterator"; }
      virtual ~ComputationTargetIterator() {}
      virtual already_AddRefd<iface::cellml_services::ComputationTarget>  nextComputationTarget() throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
    };
    PUBLIC_CCGS_PRE 
    class  PUBLIC_CCGS_POST CodeInformation
     : public virtual iface::XPCOM::IObject
    {
    public:
      static const char* INTERFACE_NAME() { return "cellml_services::CodeInformation"; }
      virtual ~CodeInformation() {}
      virtual std::wstring errorMessage() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual iface::cellml_services::ModelConstraintLevel constraintLevel() throw(std::exception&)  = 0;
      virtual uint32_t algebraicIndexCount() throw(std::exception&)  = 0;
      virtual uint32_t rateIndexCount() throw(std::exception&)  = 0;
      virtual uint32_t constantIndexCount() throw(std::exception&)  = 0;
      virtual std::wstring initConstsString() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual std::wstring ratesString() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual std::wstring variablesString() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual std::wstring functionsString() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual already_AddRefd<iface::cellml_services::ComputationTargetIterator>  iterateTargets() throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
      virtual already_AddRefd<iface::mathml_dom::MathMLNodeList>  flaggedEquations() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual already_AddRefd<iface::cellml_services::ComputationTarget>  missingInitial() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
    };
    PUBLIC_CCGS_PRE 
    class  PUBLIC_CCGS_POST CodeGenerator
     : public virtual iface::XPCOM::IObject
    {
    public:
      static const char* INTERFACE_NAME() { return "cellml_services::CodeGenerator"; }
      virtual ~CodeGenerator() {}
      virtual std::wstring constantPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void constantPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring stateVariableNamePattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void stateVariableNamePattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring algebraicVariableNamePattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void algebraicVariableNamePattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring rateNamePattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void rateNamePattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring voiPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void voiPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring sampleDensityFunctionPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void sampleDensityFunctionPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring sampleRealisationsPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void sampleRealisationsPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring boundVariableName() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void boundVariableName(const std::wstring& attr) throw(std::exception&) = 0;
      virtual uint32_t arrayOffset() throw(std::exception&)  = 0;
      virtual void arrayOffset(uint32_t attr) throw(std::exception&) = 0;
      virtual std::wstring assignPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void assignPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring solvePattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void solvePattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring solveNLSystemPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void solveNLSystemPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring temporaryVariablePattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void temporaryVariablePattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring declareTemporaryPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void declareTemporaryPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring conditionalAssignmentPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void conditionalAssignmentPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual already_AddRefd<iface::cellml_services::MaLaESTransform>  transform() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void transform(iface::cellml_services::MaLaESTransform* attr) throw(std::exception&) = 0;
      virtual already_AddRefd<iface::cellml_services::CeVAS>  useCeVAS() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void useCeVAS(iface::cellml_services::CeVAS* attr) throw(std::exception&) = 0;
      virtual already_AddRefd<iface::cellml_services::CUSES>  useCUSES() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void useCUSES(iface::cellml_services::CUSES* attr) throw(std::exception&) = 0;
      virtual already_AddRefd<iface::cellml_services::AnnotationSet>  useAnnoSet() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void useAnnoSet(iface::cellml_services::AnnotationSet* attr) throw(std::exception&) = 0;
      virtual already_AddRefd<iface::cellml_services::CodeInformation>  generateCode(iface::cellml_api::Model* sourceModel) throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
      virtual already_AddRefd<iface::cellml_services::CustomGenerator>  createCustomGenerator(iface::cellml_api::Model* sourceModel) throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
      virtual bool allowPassthrough() throw(std::exception&)  = 0;
      virtual void allowPassthrough(bool attr) throw(std::exception&) = 0;
    };
    PUBLIC_CCGS_PRE 
    class  PUBLIC_CCGS_POST CustomCodeInformation
     : public virtual iface::XPCOM::IObject
    {
    public:
      static const char* INTERFACE_NAME() { return "cellml_services::CustomCodeInformation"; }
      virtual ~CustomCodeInformation() {}
      virtual iface::cellml_services::ModelConstraintLevel constraintLevel() throw(std::exception&)  = 0;
      virtual uint32_t indexCount() throw(std::exception&)  = 0;
      virtual already_AddRefd<iface::cellml_services::ComputationTargetIterator>  iterateTargets() throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
      virtual std::wstring generatedCode() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual std::wstring functionsString() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
    };
    PUBLIC_CCGS_PRE 
    class  PUBLIC_CCGS_POST CustomGenerator
     : public virtual iface::XPCOM::IObject
    {
    public:
      static const char* INTERFACE_NAME() { return "cellml_services::CustomGenerator"; }
      virtual ~CustomGenerator() {}
      virtual already_AddRefd<iface::cellml_services::ComputationTargetIterator>  iterateTargets() throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
      virtual void requestComputation(iface::cellml_services::ComputationTarget* wanted) throw(std::exception&) = 0;
      virtual void markAsKnown(iface::cellml_services::ComputationTarget* known) throw(std::exception&) = 0;
      virtual void markAsUnwanted(iface::cellml_services::ComputationTarget* unwanted) throw(std::exception&) = 0;
      virtual already_AddRefd<iface::cellml_services::CustomCodeInformation>  generateCode() throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
    };
    PUBLIC_CCGS_PRE 
    class  PUBLIC_CCGS_POST IDACodeInformation
     : public virtual iface::cellml_services::CodeInformation
    {
    public:
      static const char* INTERFACE_NAME() { return "cellml_services::IDACodeInformation"; }
      virtual ~IDACodeInformation() {}
      virtual std::wstring essentialVariablesString() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual std::wstring stateInformationString() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual uint32_t conditionVariableCount() throw(std::exception&)  = 0;
      virtual std::wstring rootInformationString() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
    };
    PUBLIC_CCGS_PRE 
    class  PUBLIC_CCGS_POST IDACodeGenerator
     : public virtual iface::cellml_services::CodeGenerator
    {
    public:
      static const char* INTERFACE_NAME() { return "cellml_services::IDACodeGenerator"; }
      virtual ~IDACodeGenerator() {}
      virtual already_AddRefd<iface::cellml_services::IDACodeInformation>  generateIDACode(iface::cellml_api::Model* sourceModel) throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
      virtual std::wstring residualPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void residualPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring constrainedRateStateInfoPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void constrainedRateStateInfoPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring unconstrainedRateStateInfoPattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void unconstrainedRateStateInfoPattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring infDelayedRatePattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void infDelayedRatePattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual std::wstring infDelayedStatePattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void infDelayedStatePattern(const std::wstring& attr) throw(std::exception&) = 0;
      virtual bool trackPiecewiseConditions() throw(std::exception&)  = 0;
      virtual void trackPiecewiseConditions(bool attr) throw(std::exception&) = 0;
      virtual std::wstring conditionVariablePattern() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
      virtual void conditionVariablePattern(const std::wstring& attr) throw(std::exception&) = 0;
    };
    PUBLIC_CCGS_PRE 
    class  PUBLIC_CCGS_POST CodeGeneratorBootstrap
     : public virtual iface::XPCOM::IObject
    {
    public:
      static const char* INTERFACE_NAME() { return "cellml_services::CodeGeneratorBootstrap"; }
      virtual ~CodeGeneratorBootstrap() {}
      virtual already_AddRefd<iface::cellml_services::CodeGenerator>  createCodeGenerator() throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
      virtual already_AddRefd<iface::cellml_services::IDACodeGenerator>  createIDACodeGenerator() throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
    };
  };
};
#undef PUBLIC_CCGS_PRE
#undef PUBLIC_CCGS_POST
#endif // guard
