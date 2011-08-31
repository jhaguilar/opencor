/* This file is automatically generated from xpcom.idl
 * DO NOT EDIT DIRECTLY OR CHANGES WILL BE LOST!
 */
#ifndef _GUARD_IFACE_xpcom
#define _GUARD_IFACE_xpcom
#include "cda_compiler_support.h"
#ifdef MODULE_CONTAINS_xpcom
#define PUBLIC_xpcom_PRE CDA_EXPORT_PRE
#define PUBLIC_xpcom_POST CDA_EXPORT_POST
#else
#define PUBLIC_xpcom_PRE CDA_IMPORT_PRE
#define PUBLIC_xpcom_POST CDA_IMPORT_POST
#endif
namespace iface
{
  namespace XPCOM
  {
    typedef char* utf8string;
    typedef wchar_t* utf8wstring;
    PUBLIC_xpcom_PRE 
    class  PUBLIC_xpcom_POST IObject
    {
    public:
      virtual ~IObject() {}
      virtual void add_ref() throw(std::exception&) = 0;
      virtual void release_ref() throw(std::exception&) = 0;
      virtual void* query_interface(const char* id) throw(std::exception&) WARN_IF_RETURN_UNUSED = 0;
      virtual char* objid() throw(std::exception&)  WARN_IF_RETURN_UNUSED = 0;
    };
  };
};
#undef PUBLIC_xpcom_PRE
#undef PUBLIC_xpcom_POST
#endif // guard
