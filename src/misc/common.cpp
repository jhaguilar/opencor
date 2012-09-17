//==============================================================================
// Some common methods between the command line and GUI version of OpenCOR
//==============================================================================

#include "common.h"
#include "utils.h"

//==============================================================================

#include <iostream>

//==============================================================================

#include <QCoreApplication>
#include <QFileInfo>

//==============================================================================

#include <QxtCommandOptions>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void usage(QCoreApplication *pApp)
{
    std::cout << "Usage: " << qPrintable(pApp->applicationName())
              << " [OPTION]... [FILE]..." << std::endl;
    std::cout << "Start " << qPrintable(pApp->applicationName())
              << " and open the FILE(s) passed as argument(s), if any." << std::endl;
    std::cout << std::endl;
    std::cout << " -a, --about     Display OpenCOR about information"
              << std::endl;
    std::cout << " -h, --help      Display this help information" << std::endl;
    std::cout << " -v, --version   Display OpenCOR version information"
              << std::endl;
}

//==============================================================================

void version(QCoreApplication *pApp)
{
    std::cout << qPrintable(getAppVersion(pApp)) << std::endl;
}

//==============================================================================

void about(QCoreApplication *pApp)
{
    version(pApp);

    std::cout << qPrintable(getOsName()) << std::endl;
    std::cout << qPrintable(getAppCopyright(false)) << std::endl;
    std::cout << std::endl;
    std::cout << qPrintable(pApp->applicationName())
              << " is a cross-platform CellML-based modelling environment"
              << " which can be" << std::endl;
    std::cout << "used to organise, edit, simulate and analyse CellML files."
              << std::endl;
}

//==============================================================================

void error(QCoreApplication *pApp, const QString &pMsg)
{
    version(pApp);

    std::cout << std::endl;
    std::cout << "Error: " << qPrintable(pMsg) << std::endl;
}

//==============================================================================

void initApplication(QCoreApplication *pApp)
{
    // Set the name of the application

    pApp->setApplicationName(QFileInfo(pApp->applicationFilePath()).baseName());

    // Retrieve and set the version of the application

    QFile versionFile(":version");

    versionFile.open(QIODevice::ReadOnly);

    QString version = QString(versionFile.readLine()).trimmed();

    if (version.endsWith(".0"))
        // There is no actual patch information, so trim it

        version.truncate(version.length()-2);

    versionFile.close();

    pApp->setApplicationVersion(version);
}

//==============================================================================

bool consoleApplication(QCoreApplication *pApp, int *pRes)
{
    *pRes = 0;   // By default, everything is fine

    // Specify the type of command line options that are allowed
    // Note #1: we don't rely on the QxtCommandOptions::showUsage() method
    // Note #2: we don't distinguish between Windows and Linux / OS X when it
    //          comes to the formatting of the command line options

    QxtCommandOptions cmdLineOptions;

    cmdLineOptions.setFlagStyle(QxtCommandOptions::DoubleDash);
    cmdLineOptions.setParamStyle(QxtCommandOptions::SpaceAndEquals);

    cmdLineOptions.add("help");
    cmdLineOptions.alias("help", "h");

    cmdLineOptions.add("about");
    cmdLineOptions.alias("about", "a");

    cmdLineOptions.add("version");
    cmdLineOptions.alias("version", "v");

    // Parse the command line options

    cmdLineOptions.parse(pApp->arguments());

    // See what needs doing with the command line options, if anything

    if (cmdLineOptions.count("help")) {
        // The user wants to know how to use OpenCOR from the console, so...

        usage(pApp);

        return true;
    }
    else if (cmdLineOptions.count("about")) {
        // The user wants to know how to use OpenCOR from the console, so...

        about(pApp);

        return true;
    } else if (cmdLineOptions.count("version")) {
        // The user wants to know the version of OpenCOR this is, so...

        version(pApp);

        return true;
    } else if (cmdLineOptions.getUnrecognizedWarning().count()) {
        // The user provided OpenCOR with wrong command line options, so...

        usage(pApp);

        *pRes = -1;

        return true;
    } else {
        // The user didn't provide any command line options that requires
        // running OpenCOR as a console application, so...

        return false;
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
