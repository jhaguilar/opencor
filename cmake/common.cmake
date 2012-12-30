MACRO(INITIALISE_PROJECT)
#    SET(CMAKE_VERBOSE_MAKEFILE ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)

    # Some settings which depend on whether we want a debug or release version
    # of OpenCOR

    IF(WIN32)
        STRING(REPLACE "/W3" "/W3 /WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        # Note: MSVC has a /Wall flag, but it results in MSVC being very
        #       pedantic, so instead we use what MSVC recommends for production
        #       code which is /W3 and which is also what CMake uses by
        #       default...

        SET(LINK_FLAGS_PROPERTIES "/STACK:10000000 /MACHINE:X86")
    ELSE()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
        SET(LINK_FLAGS_PROPERTIES)
    ENDIF()

    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        MESSAGE("Building a debug version...")

        SET(DEBUG_MODE ON)

        # Default compiler settings

        IF(WIN32)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
            SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} /DEBUG")
        ELSE()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
        ENDIF()
    ELSE()
        SET(CMAKE_BUILD_TYPE "Release")

        MESSAGE("Building a release version...")

        SET(DEBUG_MODE OFF)

        # Default compiler and linker settings

        IF(WIN32)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNDEBUG /MD /O2 /Ob2")
            SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES}")
        ELSE()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ffast-math")
        ENDIF()

        IF(NOT WIN32 AND NOT APPLE)
            SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-s")
            # Note #1: -Wl,-s strips all the symbols, thus reducing the final
            #          size of OpenCOR or one its shared libraries...
            # Note #2: the above linking option has become obsolete on OS X,
            #          so...
        ENDIF()
    ENDIF()

    # Ask MSVC not to treat wchat_t as a built-in type

    IF(WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-")
    ENDIF()

    # Ask MSVC to ignore the LNK4099 warning since it has nothing to do with
    # OpenCOR itself

    IF(WIN32)
        SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} /IGNORE:4099")
    ENDIF()

    # Make sure that we can use install_name_tool without any prolem

    IF(APPLE)
        SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -headerpad_max_install_names")
    ENDIF()

    # Required packages

    IF(APPLE)
        # Note: when calling CMake from Qt Creator, on OS X, our PATH is not
        #       passed to CMake, meaning that CMake cannot find Qt. So, we have
        #       no choice but to set QT_QMAKE_EXECUTABLE, so that CMake can find
        #       Qt and we do this assuming that Qt's path is set in /etc/profile
        #       (as recommended in our developer's documentation)...

        EXECUTE_PROCESS(
            COMMAND sh -c ". /etc/profile && which qmake"
            OUTPUT_STRIP_TRAILING_WHITESPACE
            OUTPUT_VARIABLE QT_QMAKE_EXECUTABLE
        )
    ENDIF()

    FIND_PACKAGE(Qt4 4.8.1 REQUIRED)

    # Whether we are building for 32-bit or 64-bit

    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(64BIT_MODE ON)
    ELSE()
        SET(64BIT_MODE OFF)
    ENDIF()

    # Default location of third-party libraries
    # Note: this is only required so that we can quickly test third-party
    #       libraries without first having to package everything...

    IF(NOT WIN32)
        SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
        # Note: MSVC doesn't care about this location, so...
    ENDIF()

    # Location of our plugins so that we don't have to deploy OpenCOR on
    # Windows and Linux before being able to test it

    IF(APPLE)
        SET(DEST_PLUGINS_DIR ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME})
    ELSE()
        SET(DEST_PLUGINS_DIR ${CMAKE_BINARY_DIR}/plugins/${MAIN_PROJECT_NAME})
    ENDIF()

    # Default location of external dependencies

    IF(WIN32)
        SET(DISTRIB_DIR windows/x86)
    ELSEIF(APPLE)
        SET(DISTRIB_DIR osx)
    ELSE()
        IF(64BIT_MODE)
            SET(DISTRIB_DIR linux/x64)
        ELSE()
            SET(DISTRIB_DIR linux/x86)
        ENDIF()
    ENDIF()

    IF(WIN32)
        IF(DEBUG_MODE)
            SET(DISTRIB_BINARY_DIR ${DISTRIB_DIR}/debug)
        ELSE()
            SET(DISTRIB_BINARY_DIR ${DISTRIB_DIR}/release)
        ENDIF()
    ELSE()
        SET(DISTRIB_BINARY_DIR ${DISTRIB_DIR})
    ENDIF()

    # Set the RPATH information on Linux
    # Note: this prevent us from having to use the uncool LD_LIBRARY_PATH...

    IF(NOT WIN32 AND NOT APPLE)
        SET(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/../plugins/${PROJECT_NAME}")
    ENDIF()
ENDMACRO()

MACRO(UPDATE_LANGUAGE_FILES TARGET_NAME)
    # Update the translation (.ts) files (if they exist) and generate the
    # language (.qm) files which will later on be embedded in the project
    # itself
    # Note: this requires SOURCES, HEADERS_MOC and UIS to be defined for the
    #       current CMake project, even if that means that these variables are
    #       to be empty (the case with some plugins for example). Indeed, since
    #       otherwise the value of these variables, as defined in a previous
    #       project, may be used, so...

    SET(LANGUAGE_FILES
        ${TARGET_NAME}_fr
    )

    FOREACH(LANGUAGE_FILE ${LANGUAGE_FILES})
        SET(TS_FILE i18n/${LANGUAGE_FILE}.ts)

        IF(EXISTS "${PROJECT_SOURCE_DIR}/${TS_FILE}")
            EXECUTE_PROCESS(COMMAND ${QT_LUPDATE_EXECUTABLE} -no-obsolete
                                                             ${SOURCES} ${HEADERS_MOC} ${UIS}
                                                         -ts ${TS_FILE}
                            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
            EXECUTE_PROCESS(COMMAND ${QT_LRELEASE_EXECUTABLE} ${PROJECT_SOURCE_DIR}/${TS_FILE}
                                                          -qm ${CMAKE_BINARY_DIR}/${LANGUAGE_FILE}.qm)
        ENDIF()
    ENDFOREACH()
ENDMACRO()

MACRO(INCLUDE_THIRD_PARTY_LIBRARY MAIN_PROJECT_SOURCE_DIR THIRD_PARTY_LIBRARY)
    SET(MAIN_PROJECT_SOURCE_DIR ${MAIN_PROJECT_SOURCE_DIR})

    INCLUDE(${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/${THIRD_PARTY_LIBRARY}/${THIRD_PARTY_LIBRARY}.cmake)
ENDMACRO()

MACRO(ADD_PLUGIN PLUGIN_NAME)
    # Various initialisations

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(SOURCES)
    SET(HEADERS_MOC)
    SET(UIS)
    SET(INCLUDE_DIRS)
    SET(DEFINITIONS)
    SET(OPENCOR_DEPENDENCIES)
    SET(OPENCOR_BINARY_DEPENDENCIES)
    SET(QT_DEPENDENCIES)
    SET(EXTERNAL_DEPENDENCIES_DIR)
    SET(EXTERNAL_DEPENDENCIES)
    SET(TESTS)

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "THIRD_PARTY")
            # We are dealing with a third-party plugin, so disable all warnings
            # since it may generate some and this is not something we can or
            # should have control over
            # Note: for some reasons, MSVC eventually uses /W1, so we can't
            #       replace /W3 /WX with /w since this would conflict with
            #       /W1 and generate a warning, so...

            IF(WIN32)
                STRING(REPLACE "/W3 /WX" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            ELSE()
                STRING(REPLACE "-Wall -Werror" "-w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            ENDIF()

            # Add a definition in case of compilation from within Qt Creator
            # using MSVC since JOM overrides some of our settings, so...

            IF(WIN32)
                ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
            ENDIF()
        ELSEIF(${PARAMETER} STREQUAL "SOURCES")
            SET(TYPE_OF_PARAMETER 1)
        ELSEIF(${PARAMETER} STREQUAL "HEADERS_MOC")
            SET(TYPE_OF_PARAMETER 2)
        ELSEIF(${PARAMETER} STREQUAL "UIS")
            SET(TYPE_OF_PARAMETER 3)
        ELSEIF(${PARAMETER} STREQUAL "INCLUDE_DIRS")
            SET(TYPE_OF_PARAMETER 4)
        ELSEIF(${PARAMETER} STREQUAL "DEFINITIONS")
            SET(TYPE_OF_PARAMETER 5)
        ELSEIF(${PARAMETER} STREQUAL "OPENCOR_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 6)
        ELSEIF(${PARAMETER} STREQUAL "OPENCOR_BINARY_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 7)
        ELSEIF(${PARAMETER} STREQUAL "QT_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 8)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_DEPENDENCIES_DIR")
            SET(TYPE_OF_PARAMETER 9)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 10)
        ELSEIF(${PARAMETER} STREQUAL "TESTS")
            SET(TYPE_OF_PARAMETER 11)
        ELSE()
            # Not one of the headers, so add the parameter to the corresponding
            # set

            IF(${TYPE_OF_PARAMETER} EQUAL 1)
                LIST(APPEND SOURCES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 2)
                LIST(APPEND HEADERS_MOC ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 3)
                LIST(APPEND UIS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 4)
                LIST(APPEND INCLUDE_DIRS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 5)
                LIST(APPEND DEFINITIONS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 6)
                LIST(APPEND OPENCOR_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 7)
                LIST(APPEND OPENCOR_BINARY_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 8)
                LIST(APPEND QT_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 9)
                SET(EXTERNAL_DEPENDENCIES_DIR ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 10)
                LIST(APPEND EXTERNAL_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 11)
                LIST(APPEND TESTS ${PARAMETER})
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Various include directories

    SET(PLUGIN_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

    INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

    # Resource file, if any

    SET(QRC_FILE res/${PLUGIN_NAME}.qrc)

    IF(EXISTS "${PROJECT_SOURCE_DIR}/${QRC_FILE}")
        SET(RESOURCES ${QRC_FILE})
    ELSE()
        SET(RESOURCES)
    ENDIF()

    # Update the translation (.ts) files and generate the language (.qm) files
    # which will later on be embedded in the plugin itself

    IF(NOT "${RESOURCES}" STREQUAL "")
        UPDATE_LANGUAGE_FILES(${PLUGIN_NAME})
    ENDIF()

    # Definition to make sure that the plugin can be used by other plugins

    ADD_DEFINITIONS(-D${PLUGIN_NAME}_PLUGIN)

    # Some plugin-specific definitions

    FOREACH(DEFINITION ${DEFINITIONS})
        ADD_DEFINITIONS(-D${DEFINITION})
    ENDFOREACH()

    # Generate and add the different files needed by the plugin

    IF("${HEADERS_MOC}" STREQUAL "")
        SET(SOURCES_MOC)
    ELSE()
        QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
    ENDIF()

    IF("${UIS}" STREQUAL "")
        SET(SOURCES_UIS)
    ELSE()
        QT4_WRAP_UI(SOURCES_UIS ${UIS})
    ENDIF()

    IF("${RESOURCES}" STREQUAL "")
        SET(SOURCES_RCS)
    ELSE()
        QT4_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})
    ENDIF()

    ADD_LIBRARY(${PROJECT_NAME} SHARED
        ${SOURCES}
        ${SOURCES_MOC}
        ${SOURCES_UIS}
        ${SOURCES_RCS}
    )

    # OpenCOR dependencies

    FOREACH(OPENCOR_DEPENDENCY ${OPENCOR_DEPENDENCIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${OPENCOR_DEPENDENCY}Plugin
        )
    ENDFOREACH()

    # OpenCOR binary dependencies

    FOREACH(OPENCOR_BINARY_DEPENDENCY ${OPENCOR_BINARY_DEPENDENCIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${OPENCOR_BINARY_DEPENDENCY}
        )
    ENDFOREACH()

    # Qt dependencies

    FOREACH(QT_DEPENDENCY ${QT_DEPENDENCIES})
        IF(WIN32)
            IF(DEBUG_MODE)
                SET(QT_DEPENDENCY_VERSION d)
            ELSE()
                SET(QT_DEPENDENCY_VERSION)
            ENDIF()

            SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${QT_DEPENDENCY}${QT_DEPENDENCY_VERSION}${QT_VERSION_MAJOR}${CMAKE_STATIC_LIBRARY_SUFFIX})
        ELSEIF(APPLE)
            SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${QT_DEPENDENCY}.framework)
        ELSE()
            SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ENDIF()

        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${QT_LIBRARY_PATH}
        )
    ENDFOREACH()

    # Linker settings

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PLUGIN_NAME}
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    # External dependencies

    IF(NOT ${EXTERNAL_DEPENDENCIES_DIR} STREQUAL "")
        FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
            TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                ${EXTERNAL_DEPENDENCIES_DIR}/${EXTERNAL_DEPENDENCY}
            )
        ENDFOREACH()
    ENDIF()

    # Location of our plugins

    IF(WIN32)
        STRING(REPLACE "${${MAIN_PROJECT_NAME}_SOURCE_DIR}" "" PLUGIN_BUILD_DIR ${PROJECT_SOURCE_DIR})
        SET(PLUGIN_BUILD_DIR "${CMAKE_BINARY_DIR}${PLUGIN_BUILD_DIR}")
        # Note: MSVC generate things in a different place to GCC, so...
    ELSE()
        SET(PLUGIN_BUILD_DIR ${LIBRARY_OUTPUT_PATH})
    ENDIF()

    # Copy the plugin to our plugins directory
    # Note: this is done so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package OpenCOR...

    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                                                        ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # On Windows, make a copy of the plugin to our main build directory, since
    # this is where it will be on Linux and OS X and where any test which
    # requires the plugin will expect it to be, but this is not where MSVC
    # generates the plugin, so...

    IF(WIN32)
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                                                            ${CMAKE_BINARY_DIR}/${PLUGIN_FILENAME})
    ENDIF()

    # A few OS X specific things

    IF(APPLE)
        # Clean up our plugin's id

        SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND install_name_tool -id ${PLUGIN_FILENAME}
                                                         ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})

        # Make sure that the plugin refers to our embedded version of the other
        # plugins on which it depends

        FOREACH(OPENCOR_DEPENDENCY ${OPENCOR_DEPENDENCIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${PLUGIN_BUILD_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${OPENCOR_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 @executable_path/../PlugIns/${MAIN_PROJECT_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${OPENCOR_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()

        # Make sure that the plugin refers to our embedded version of the
        # binary plugins on which it depends

        FOREACH(OPENCOR_BINARY_DEPENDENCY ${OPENCOR_BINARY_DEPENDENCIES})
            STRING(REPLACE "${PLUGIN_BUILD_DIR}/" "" OPENCOR_BINARY_DEPENDENCY "${OPENCOR_BINARY_DEPENDENCY}")

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${OPENCOR_BINARY_DEPENDENCY}
                                                                 @executable_path/../PlugIns/${MAIN_PROJECT_NAME}/${OPENCOR_BINARY_DEPENDENCY}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()

        # Make sure that the plugin refers to our embedded version of the Qt
        # libraries on which it depends

        FOREACH(QT_DEPENDENCY ${QT_DEPENDENCIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${QT_DEPENDENCY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_DEPENDENCY}
                                                                 @executable_path/../Frameworks/${QT_DEPENDENCY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_DEPENDENCY}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()

        # Make sure that the plugin refers to our embedded version of the
        # external dependencies on which it depends
        # Note #1: we do it in two different ways, since some external libraries
        #          we use refer to the library itself (e.g. CellML) while others
        #          refer to some @executable_path information (e.g. LLVM), so...
        # Note #2: we must do it for both the deployed and 'test' versions of
        #          the plugin...

        FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
            # First, for the deployed version of the plugin

            SET(EXTERNAL_DEPENDENCY_FILENAME ${EXTERNAL_DEPENDENCY})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${EXTERNAL_DEPENDENCY_FILENAME}
                                                                 @executable_path/../Frameworks/${EXTERNAL_DEPENDENCY_FILENAME}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change @executable_path/../lib/${EXTERNAL_DEPENDENCY_FILENAME}
                                                                 @executable_path/../Frameworks/${EXTERNAL_DEPENDENCY_FILENAME}
                                                                 ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${MAIN_PROJECT_NAME}/${PLUGIN_FILENAME})
        ENDFOREACH()
    ENDIF()

    # Package the plugin itself, but only if we are not on OS X since it will
    # have already been copied

    IF(NOT APPLE)
        INSTALL(FILES ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME} DESTINATION plugins/${MAIN_PROJECT_NAME})
    ENDIF()

    # Create some tests, if any and if required

    IF(ENABLE_TESTING)
        FOREACH(TEST ${TESTS})
            SET(TEST_NAME ${PLUGIN_NAME}_${TEST})

            SET(TEST_SOURCE_FILE test/${TEST}.cpp)
            SET(TEST_HEADER_MOC_FILE test/${TEST}.h)

            IF(    EXISTS ${PROJECT_SOURCE_DIR}/${TEST_SOURCE_FILE}
               AND EXISTS ${PROJECT_SOURCE_DIR}/${TEST_HEADER_MOC_FILE})
                # The test exists, so build it

                # On Linux and OS X, we need to refer to some bits from the Core
                # plugin even if we don't use them, so...

                IF(WIN32)
                    SET(CORE_SOURCES_MOC)
                    SET(CORE_SOURCES)
                ELSE()
                    SET(CORE_SOURCES_MOC
                        ../../misc/Core/src/dockwidget.h
                    )

                    SET(CORE_SOURCES
                        ../../misc/Core/src/commonwidget.cpp
                        ../../misc/Core/src/dockwidget.cpp
                    )
                ENDIF()

                # Rules to build the test

                QT4_WRAP_CPP(TEST_SOURCES_MOC
                    ../../plugin.h
                    ../../pluginmanager.h

                    ${CORE_SOURCES_MOC}

                    ${HEADERS_MOC}
                    ${TEST_HEADER_MOC_FILE}
                )

                ADD_EXECUTABLE(${TEST_NAME}
                    ../../../../test/testutils.cpp

                    ../../coreinterface.cpp
                    ../../interface.cpp
                    ../../plugin.cpp
                    ../../plugininfo.cpp
                    ../../pluginmanager.cpp

                    ${CORE_SOURCES}

                    ${TEST_SOURCE_FILE}
                    ${TEST_SOURCES_MOC}
                )

                # OpenCOR dependencies

                FOREACH(OPENCOR_DEPENDENCY ${OPENCOR_DEPENDENCIES} ${PLUGIN_NAME})
                    TARGET_LINK_LIBRARIES(${TEST_NAME}
                        ${OPENCOR_DEPENDENCY}Plugin
                    )
                ENDFOREACH()

                # Qt dependencies

                FOREACH(QT_DEPENDENCY ${QT_DEPENDENCIES} QtTest)
                    IF(WIN32)
                        IF(DEBUG_MODE)
                            SET(QT_DEPENDENCY_VERSION d)
                        ELSE()
                            SET(QT_DEPENDENCY_VERSION)
                        ENDIF()

                        SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${QT_DEPENDENCY}${QT_DEPENDENCY_VERSION}${QT_VERSION_MAJOR}${CMAKE_STATIC_LIBRARY_SUFFIX})
                    ELSEIF(APPLE)
                        SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${QT_DEPENDENCY}.framework)
                    ELSE()
                        SET(QT_LIBRARY_PATH ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_DEPENDENCY}${CMAKE_SHARED_LIBRARY_SUFFIX})
                    ENDIF()

                    TARGET_LINK_LIBRARIES(${TEST_NAME}
                        ${QT_LIBRARY_PATH}
                    )
                ENDFOREACH()

                # External dependencies

                IF(NOT ${EXTERNAL_DEPENDENCIES_DIR} STREQUAL "")
                    FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
                        TARGET_LINK_LIBRARIES(${TEST_NAME}
                            ${EXTERNAL_DEPENDENCIES_DIR}/${EXTERNAL_DEPENDENCY}
                        )
                    ENDFOREACH()
                ENDIF()

                # Copy the test to our tests directory
                # Note: DEST_TESTS_DIR is defined in our main CMake file...

                SET(TEST_FILENAME ${TEST_NAME}${CMAKE_EXECUTABLE_SUFFIX})

                ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                   COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${TEST_FILENAME}
                                                                    ${DEST_TESTS_DIR}/${TEST_FILENAME})

                # Make sure that, on OS X, the test refers to our test version
                # of the external libraries on which it depends

                IF(APPLE)
                    FOREACH(EXTERNAL_DEPENDENCY ${EXTERNAL_DEPENDENCIES})
                        SET(EXTERNAL_DEPENDENCY_FILENAME ${EXTERNAL_DEPENDENCY})


                        ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                           COMMAND install_name_tool -change ${EXTERNAL_DEPENDENCY_FILENAME}
                                                                             ${CMAKE_BINARY_DIR}/${EXTERNAL_DEPENDENCY_FILENAME}
                                                                             ${DEST_TESTS_DIR}/${TEST_FILENAME})

                        ADD_CUSTOM_COMMAND(TARGET ${TEST_NAME} POST_BUILD
                                           COMMAND install_name_tool -change @executable_path/../lib/${EXTERNAL_DEPENDENCY_FILENAME}
                                                                             ${CMAKE_BINARY_DIR}/${EXTERNAL_DEPENDENCY_FILENAME}
                                                                             ${DEST_TESTS_DIR}/${TEST_FILENAME})
                    ENDFOREACH()
                ENDIF()
            ELSE()
                MESSAGE(AUTHOR_WARNING "The '${TEST}' test for the '${PLUGIN_NAME}' plugin doesn't exist")
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()

MACRO(ADD_PLUGIN_BINARY PLUGIN_NAME)
    # Various initialisations

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(INCLUDE_DIRS)
    SET(QT_DEPENDENCIES)

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "INCLUDE_DIRS")
            SET(TYPE_OF_PARAMETER 1)
        ELSEIF(${PARAMETER} STREQUAL "QT_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 2)
        ELSE()
            # Not one of the headers, so add the parameter to the corresponding
            # set

            IF(${TYPE_OF_PARAMETER} EQUAL 1)
                SET(INCLUDE_DIRS ${INCLUDE_DIRS} ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 2)
                SET(QT_DEPENDENCIES ${QT_DEPENDENCIES} ${PARAMETER})
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Various include directories

    SET(PLUGIN_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

    INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

    # Location of our plugins

    SET(PLUGIN_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR})

    # Copy the plugin to our plugins directory
    # Note: this is done so that we can, on Windows and Linux, test the use of
    #       plugins in OpenCOR without first having to package and deploy it...

    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    ADD_CUSTOM_TARGET(${PLUGIN_NAME}_COPY_PLUGIN_TO_PLUGINS_DIRECTORY ALL
                      COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BINARY_DIR}/${PLUGIN_FILENAME}
                                                       ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # Make a copy of the plugin to our main build directory

    ADD_CUSTOM_TARGET(${PLUGIN_NAME}_COPY_PLUGIN_TO_BUILD_DIRECTORY ALL
                      COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BINARY_DIR}/${PLUGIN_FILENAME}
                                                       ${CMAKE_BINARY_DIR}/${PLUGIN_FILENAME})

    # A few OS X specific things

    IF(APPLE)
        # Make sure that the copy of our plugin in our main build directory
        # refers to the system version of the Qt libraries on which it depends
        # Note: indeed, right now, it refers to our embedded version of the Qt
        #       libraries while, if we want the tests to work, it should refer
        #       to the system version of the Qt libraries, so...

        FOREACH(QT_DEPENDENCY ${QT_DEPENDENCIES})
            ADD_CUSTOM_TARGET(${PLUGIN_NAME}_UPDATE_MAC_OS_X_QT_REFERENCE ALL
                               COMMAND install_name_tool -change @executable_path/../Frameworks/${QT_DEPENDENCY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_DEPENDENCY}
                                                                 ${QT_LIBRARY_DIR}/${QT_DEPENDENCY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_DEPENDENCY}
                                                                 ${LIBRARY_OUTPUT_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ENDFOREACH()
    ENDIF()

    # Package the plugin itself, but only if we are not on OS X since it will
    # have already been copied

    IF(NOT APPLE)
        INSTALL(FILES ${PLUGIN_BINARY_DIR}/${PLUGIN_FILENAME} DESTINATION plugins/${MAIN_PROJECT_NAME})
    ENDIF()
ENDMACRO()

MACRO(DEPLOY_MAC_OS_X_LIBRARY LIBRARY_NAME)
    # Various initialisations

    SET(TYPE)
    SET(DIR)
    SET(FRAMEWORKS)
    SET(LIBRARIES)

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "TYPE")
            SET(TYPE_OF_PARAMETER 1)
        ELSEIF(${PARAMETER} STREQUAL "DIR")
            SET(TYPE_OF_PARAMETER 2)
        ELSEIF(${PARAMETER} STREQUAL "FRAMEWORKS")
            SET(TYPE_OF_PARAMETER 3)
        ELSEIF(${PARAMETER} STREQUAL "LIBRARIES")
            SET(TYPE_OF_PARAMETER 4)
        ELSE()
            # Not one of the headers, so add the parameter to the corresponding
            # set

            IF(${TYPE_OF_PARAMETER} EQUAL 1)
                SET(TYPE ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 2)
                SET(DIR ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 3)
                LIST(APPEND FRAMEWORKS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 4)
                LIST(APPEND LIBRARIES ${PARAMETER})
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Deploy the library itself

    IF("${TYPE}" STREQUAL "Library")
        SET(LIBRARY_LIB_DIR ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/Frameworks)

        IF("${DIR}" STREQUAL "")
            # We must deploy a Qt library

            SET(LIBRARY_RELATIVE_FILEPATH ${LIBRARY_NAME}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX})
            SET(LIBRARY_FILEPATH ${LIBRARY_LIB_DIR}/${LIBRARY_RELATIVE_FILEPATH})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${QT_LIBRARY_DIR}/${LIBRARY_RELATIVE_FILEPATH}
                                                                ${LIBRARY_FILEPATH})
        ELSE()
            # We must deploy a third-party library

            SET(LIBRARY_FILEPATH ${LIBRARY_LIB_DIR}/${LIBRARY_NAME})

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${DIR}/${LIBRARY_NAME}
                                                                ${LIBRARY_FILEPATH})

            # In the case of a third-party library, we must also copy the
            # library to the build directory, so that we can test any plugin
            # that has a dependency on it
            # Note: we don't care about stripping the library from anything that
            #       is not essential, cleaning up its id, making sure that it
            #       refers to our embedded version of the Qt libraries on which
            #       it depends, etc. since it's never going to be deployed...

            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${DIR}/${LIBRARY_NAME}
                                                                ${CMAKE_BINARY_DIR}/${LIBRARY_NAME})
        ENDIF()
    ELSE()
        # We must deploy a library which is bundled in a Qt framework

        SET(LIBRARY_RELATIVE_FILEPATH ${LIBRARY_NAME}.framework/Versions/${QT_VERSION_MAJOR}/${LIBRARY_NAME})
        SET(LIBRARY_FILEPATH ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/Frameworks/${LIBRARY_RELATIVE_FILEPATH})

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${QT_LIBRARY_DIR}/${LIBRARY_RELATIVE_FILEPATH}
                                                            ${LIBRARY_FILEPATH})
    ENDIF()

    # Strip the library from anything that is not essential

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND strip -S -x ${LIBRARY_FILEPATH})

    # Do things that are only related to Qt libraries or non-Qt libraries

    IF("${DIR}" STREQUAL "")
        # Clean up the library's id

        IF("${TYPE}" STREQUAL "Library")
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -id ${LIBRARY_NAME}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                             ${LIBRARY_FILEPATH})
        ELSE()
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -id ${LIBRARY_NAME}
                                                             ${LIBRARY_FILEPATH})
        ENDIF()

        # Make sure that the library refers to our embedded version of the Qt
        # libraries on which it depends

        FOREACH(LIBRARY ${LIBRARIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${LIBRARY}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 @executable_path/../Frameworks/${LIBRARY}.${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}
                                                                 ${LIBRARY_FILEPATH})
        ENDFOREACH()

        # Make sure that the library refers to our embedded version of the Qt
        # frameworks on which it depends

        FOREACH(FRAMEWORK ${FRAMEWORKS})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${FRAMEWORK}.framework/Versions/${QT_VERSION_MAJOR}/${FRAMEWORK}
                                                                 @executable_path/../Frameworks/${FRAMEWORK}.framework/Versions/${QT_VERSION_MAJOR}/${FRAMEWORK}
                                                                 ${LIBRARY_FILEPATH})
        ENDFOREACH()
    ELSE()
        # Make sure that the third-party library refers to our embedded version
        # of the libraries on which it depends

        FOREACH(LIBRARY ${LIBRARIES})
            ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                               COMMAND install_name_tool -change ${LIBRARY}
                                                                 @executable_path/../Frameworks/${LIBRARY}
                                                                 ${LIBRARY_FILEPATH})
        ENDFOREACH()
    ENDIF()
ENDMACRO()

MACRO(CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT PLUGIN_DIRNAME PLUGIN_NAME)
    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND install_name_tool -id ${PLUGIN_FILENAME}
                                                     ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${PLUGIN_DIRNAME}/${PLUGIN_FILENAME})
ENDMACRO()

MACRO(COPY_FILE_TO_BUILD_DIR DEST_DIRNAME DIRNAME FILENAME)
    IF(EXISTS ${CMAKE_BINARY_DIR}/../cmake)
        # A cmake directory exists at the same level as the binary directory,
        # so we are dealing with the main project

        SET(REAL_DEST_DIRNAME ${CMAKE_BINARY_DIR}/${DEST_DIRNAME})
    ELSE()
        # No cmake directory exists at the same level as the binary directory,
        # so we are dealing with a non-main project

        SET(REAL_DEST_DIRNAME ${CMAKE_BINARY_DIR}/../../build/${DEST_DIRNAME})
    ENDIF()

    IF("${ARGN}" STREQUAL "")
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${DIRNAME}/${FILENAME}
                                                            ${REAL_DEST_DIRNAME}/${FILENAME})
    ELSE()
        # An argument was passed so use it to rename the file which is to be
        # copied

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${DIRNAME}/${FILENAME}
                                                            ${REAL_DEST_DIRNAME}/${ARGN})
    ENDIF()
ENDMACRO()

MACRO(DEPLOY_WINDOWS_EXTERNAL_DEPENDENCY DIRNAME FILENAME)
    # Copy the library file to both the build and build/bin folders, so we can
    # test things without first having to deploy OpenCOR

    COPY_FILE_TO_BUILD_DIR(. ${DIRNAME} ${FILENAME})
    COPY_FILE_TO_BUILD_DIR(bin ${DIRNAME} ${FILENAME})

    # Install the library file

    INSTALL(FILES ${DIRNAME}/${FILENAME} DESTINATION bin)
ENDMACRO()

MACRO(DEPLOY_LINUX_EXTERNAL_DEPENDENCY DIRNAME FILENAME)
    # Copy the library file to the build folder, so we can test things without
    # first having to deploy OpenCOR

    COPY_FILE_TO_BUILD_DIR(. ${DIRNAME} ${FILENAME})

    # Install the library file

    INSTALL(FILES ${DIRNAME}/${FILENAME} DESTINATION lib)
ENDMACRO()

MACRO(DEPLOY_LINUX_FILE DEST_DIRNAME DIRNAME FILENAME)
    # Copy the Linux file to its destination, so we can test things without
    # first having to deploy OpenCOR

    COPY_FILE_TO_BUILD_DIR(${DEST_DIRNAME} ${DIRNAME} ${FILENAME})

    # Install the Linux file

    INSTALL(FILES ${DIRNAME}/${FILENAME} DESTINATION ${DEST_DIRNAME})
ENDMACRO()
