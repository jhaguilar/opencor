//==============================================================================
// Thread
//==============================================================================

#ifndef THREAD_H
#define THREAD_H

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QThread>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT Thread : public QThread
{
public:
    static void sleep(const unsigned long &pDuration);
    static void msleep(const unsigned long &pDuration);
    static void usleep(const unsigned long &pDuration);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
