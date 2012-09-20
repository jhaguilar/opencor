//==============================================================================
// LLVM plugin
//==============================================================================

#include "llvmplugin.h"

//==============================================================================

namespace OpenCOR {
namespace LLVM {

//==============================================================================

PLUGININFO_FUNC LLVMPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to get access to <a href=\"http://www.llvm.org/\">LLVM</a> (including <a href=\"http://clang.llvm.org/\">Clang</a>)");
    descriptions.insert("fr", "Une extension pour avoir acc�s � <a href=\"http://www.llvm.org/\">LLVM</a> (y compris <a href=\"http://clang.llvm.org/\">Clang</a>)");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::General,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(LLVM, LLVMPlugin)

//==============================================================================

}   // namespace LLVM
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
