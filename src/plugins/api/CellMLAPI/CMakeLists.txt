PROJECT(CellMLAPIPlugin)

MACRO(RETRIEVE_CELLML_API_SETTINGS)
    # Retrieve CellML API settings

    IF(WIN32)
        SET(CELLML_API_EXTERNAL_DEPENDENCIES
            ${CMAKE_IMPORT_LIBRARY_PREFIX}annotools${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}ccgs${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}celeds${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}celedsexporter${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}cellml${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}cevas${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}cis${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}cuses${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}malaes${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}spros${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}srus${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}telicems${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}vacss${CMAKE_IMPORT_LIBRARY_SUFFIX}
            ${CMAKE_IMPORT_LIBRARY_PREFIX}xpath${CMAKE_IMPORT_LIBRARY_SUFFIX}
        )

        SET(CELLML_API_EXTERNAL_DEPENDENCIES_FOR_DEPLOYMENT
            annotools ccgs celeds celedsexporter cellml cevas cis cuses malaes spros srus telicems vacss xpath)
    ELSEIF(APPLE)
        SET(CELLML_API_EXTERNAL_DEPENDENCIES
            ${CMAKE_SHARED_LIBRARY_PREFIX}annotools.2${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}ccgs.2${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}celeds.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}celedsexporter.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}cellml.2${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}cevas.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}cis.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}cuses.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}malaes.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}spros.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}srus.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}telicems.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}vacss.1${CMAKE_SHARED_LIBRARY_SUFFIX}
            ${CMAKE_SHARED_LIBRARY_PREFIX}xpath.1${CMAKE_SHARED_LIBRARY_SUFFIX}
        )
    ELSE()
        SET(CELLML_API_EXTERNAL_DEPENDENCIES
            ${CMAKE_SHARED_LIBRARY_PREFIX}annotools${CMAKE_SHARED_LIBRARY_SUFFIX}.2
            ${CMAKE_SHARED_LIBRARY_PREFIX}ccgs${CMAKE_SHARED_LIBRARY_SUFFIX}.2
            ${CMAKE_SHARED_LIBRARY_PREFIX}celeds${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}celedsexporter${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}cellml${CMAKE_SHARED_LIBRARY_SUFFIX}.2
            ${CMAKE_SHARED_LIBRARY_PREFIX}cevas${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}cis${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}cuses${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}malaes${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}spros${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}srus${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}telicems${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}vacss${CMAKE_SHARED_LIBRARY_SUFFIX}.1
            ${CMAKE_SHARED_LIBRARY_PREFIX}xpath${CMAKE_SHARED_LIBRARY_SUFFIX}.1
        )
    ENDIF()

    # Ignore some warnings on Windows

    IF(WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4091 /wd4290")
        # Note: on Windows, some warnings get generated. Yet, we have nothing to
        #       do with them, so we ignore them...
    ENDIF()
ENDMACRO()

# CellML API libraries

SET(CELLML_API_EXTERNAL_DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR})

RETRIEVE_CELLML_API_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLAPI
    SOURCES
        ../../plugininfo.cpp

        src/cellmlapiplugin.cpp
    HEADERS_MOC
        src/cellmlapiplugin.h
    INCLUDE_DIRS
        include
        src
    QT_MODULES
        Core
    QT_DEPENDENCIES
        QtCore
    EXTERNAL_DEPENDENCIES_DIR
        ${CELLML_API_EXTERNAL_DEPENDENCIES_DIR}
    EXTERNAL_DEPENDENCIES
        ${CELLML_API_EXTERNAL_DEPENDENCIES}
)

# Deploy the plugin's external dependencies
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external dependency may have dependencies of its own

IF(WIN32)
    # The CellML API libraries

    FOREACH(CELLML_API_EXTERNAL_DEPENDENCY_FOR_DEPLOYMENT ${CELLML_API_EXTERNAL_DEPENDENCIES_FOR_DEPLOYMENT})
        WINDOWS_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR}
                               ${CMAKE_SHARED_LIBRARY_PREFIX}${CELLML_API_EXTERNAL_DEPENDENCY_FOR_DEPLOYMENT}${CMAKE_SHARED_LIBRARY_SUFFIX})
    ENDFOREACH()
ELSEIF(APPLE)
    # Deploy the CellML API libraries together with their respective CellML API
    # dependencies, if any

    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} annotools.2
                        cellml.2)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} ccgs.2
                        annotools.2 cellml.2 cevas.1 cuses.1 malaes.1)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} celeds.1
                        cellml.2 malaes.1)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} celedsexporter.1
                        annotools.2 ccgs.2 celeds.1 cellml.2 cevas.1 cuses.1 malaes.1)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} cellml.2)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} cevas.1
                        cellml.2)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} cis.1
                        annotools.2 ccgs.2 cellml.2 cevas.1 cuses.1 malaes.1)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} cuses.1
                        annotools.2 cellml.2)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} malaes.1
                        cellml.2)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} spros.1
                        cellml.2)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} srus.1
                        annotools.2 ccgs.2 cellml.2 cevas.1 cis.1 cuses.1 malaes.1 xpath.1)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} telicems.1
                        cellml.2)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} vacss.1
                        annotools.2 cellml.2 cuses.1)
    OS_X_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR} xpath.1
                        cellml.2)
ELSE()
    # The CellML API binaries

    FOREACH(CELLML_API_EXTERNAL_DEPENDENCY ${CELLML_API_EXTERNAL_DEPENDENCIES})
        LINUX_DEPLOY_LIBRARY(${CELLML_API_EXTERNAL_DEPENDENCIES_DIR}
                             ${CELLML_API_EXTERNAL_DEPENDENCY})
    ENDFOREACH()
ENDIF()
