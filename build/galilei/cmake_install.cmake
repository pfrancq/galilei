# Install script for directory: /home/pfrancq/prj/galilei/galilei

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "DEBUG")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/16x16/actions" TYPE FILE FILES
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/nokonqueror.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_cdata_b.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_comment_b.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_contents_b.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_element_b.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_procinstr_b.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_text_b.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_attribute.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_cdata.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_comment.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_contents.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_element.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_procinstr.png"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/xml_text.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "/usr/lib" TYPE FILE FILES
    "/home/pfrancq/prj/galilei/build/galilei/libgalileikde.la"
    "/home/pfrancq/prj/galilei/build/galilei/libgalileikde.so"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalileikde.so.2.0.0")
      FILE(RPATH_CHECK
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalileikde.so.2.0.0"
           RPATH "")
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalileikde.so.2.0.0")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
      "/home/pfrancq/prj/galilei/build/galilei/libgalileikde.so.2.0.0"
      "/home/pfrancq/prj/galilei/build/galilei/libgalileikde.so.2"
      "/home/pfrancq/prj/galilei/build/galilei/libgalileikde.so"
      )
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalileikde.so.2.0.0")
      FILE(RPATH_REMOVE
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalileikde.so.2.0.0")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalileikde.so.2.0.0")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalileikde.so.2.0.0")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/r/galilei" TYPE FILE FILES
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/qgprofile.h"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/qgdocxml.h"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/qgprofile.h"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/qgdocxml.h"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/qgmeasure2elementsdlg.h"
    "/home/pfrancq/prj/galilei/galilei/frontend/kde/qlistviewitemtype.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "/usr/lib" TYPE FILE FILES
    "/home/pfrancq/prj/galilei/build/galilei/libgalilei.la"
    "/home/pfrancq/prj/galilei/build/galilei/libgalilei.so"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalilei.so.2.0.0")
      FILE(RPATH_CHECK
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalilei.so.2.0.0"
           RPATH "")
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalilei.so.2.0.0")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
      "/home/pfrancq/prj/galilei/build/galilei/libgalilei.so.2.0.0"
      "/home/pfrancq/prj/galilei/build/galilei/libgalilei.so.2"
      "/home/pfrancq/prj/galilei/build/galilei/libgalilei.so"
      )
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalilei.so.2.0.0")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalilei.so.2.0.0")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libgalilei.so.2.0.0")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/r/galilei" TYPE FILE FILES
    "/home/pfrancq/prj/galilei/galilei/docs/gbalancedlinks.h"
    "/home/pfrancq/prj/galilei/galilei/docs/gdocanalyse.h"
    "/home/pfrancq/prj/galilei/galilei/docs/gdocxml.h"
    "/home/pfrancq/prj/galilei/galilei/docs/glinkcalc.h"
    "/home/pfrancq/prj/galilei/galilei/docs/glinks.h"
    "/home/pfrancq/prj/galilei/galilei/docs/gdoc.h"
    "/home/pfrancq/prj/galilei/galilei/docs/gsugs.h"
    "/home/pfrancq/prj/galilei/galilei/docs/glink.h"
    "/home/pfrancq/prj/galilei/galilei/docs/gpostdoc.h"
    "/home/pfrancq/prj/galilei/galilei/docs/gfilter.h"
    "/home/pfrancq/prj/galilei/galilei/engines/genginedoc.h"
    "/home/pfrancq/prj/galilei/galilei/engines/granking.h"
    "/home/pfrancq/prj/galilei/galilei/engines/gengine.h"
    "/home/pfrancq/prj/galilei/galilei/engines/gmetaengine.h"
    "/home/pfrancq/prj/galilei/galilei/engines/gwordoccurs.h"
    "/home/pfrancq/prj/galilei/galilei/engines/gindexer.h"
    "/home/pfrancq/prj/galilei/galilei/groups/gcommunitycalc.h"
    "/home/pfrancq/prj/galilei/galilei/groups/gpostcommunity.h"
    "/home/pfrancq/prj/galilei/galilei/groups/gsubject.h"
    "/home/pfrancq/prj/galilei/galilei/groups/ggroup.h"
    "/home/pfrancq/prj/galilei/galilei/groups/ggroup.hh"
    "/home/pfrancq/prj/galilei/galilei/groups/gcommunity.h"
    "/home/pfrancq/prj/galilei/galilei/groups/gtopic.h"
    "/home/pfrancq/prj/galilei/galilei/groups/ggroupprofiles.h"
    "/home/pfrancq/prj/galilei/galilei/groups/gsubjects.h"
    "/home/pfrancq/prj/galilei/galilei/historic/gcommunityhistory.h"
    "/home/pfrancq/prj/galilei/galilei/historic/gcommunitieshistory.h"
    "/home/pfrancq/prj/galilei/galilei/historic/gweightinfoshistory.h"
    "/home/pfrancq/prj/galilei/galilei/infos/gweightinfos.h"
    "/home/pfrancq/prj/galilei/galilei/infos/gweightinfo.h"
    "/home/pfrancq/prj/galilei/galilei/knowledge/gconcepttype.h"
    "/home/pfrancq/prj/galilei/galilei/knowledge/gconcept.h"
    "/home/pfrancq/prj/galilei/galilei/knowledge/grelation.h"
    "/home/pfrancq/prj/galilei/galilei/knowledge/grelationtype.h"
    "/home/pfrancq/prj/galilei/galilei/knowledge/glang.h"
    "/home/pfrancq/prj/galilei/galilei/knowledge/gxmlindex.h"
    "/home/pfrancq/prj/galilei/galilei/profiles/gprofilecalc.h"
    "/home/pfrancq/prj/galilei/galilei/profiles/gprofile.h"
    "/home/pfrancq/prj/galilei/galilei/profiles/guser.h"
    "/home/pfrancq/prj/galilei/galilei/profiles/gpostprofile.h"
    "/home/pfrancq/prj/galilei/galilei/profiles/gpreprofile.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gsession.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gsessionprg.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/galilei.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gstorage.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gslot.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gslotlog.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gplugin.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gstatscalc.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gsignalhandler.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gmeasure.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/ggalileiapp.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gmeasurefile.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gmeasurefile.hh"
    "/home/pfrancq/prj/galilei/galilei/sessions/gdebugobject.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gpluginmanager.h"
    "/home/pfrancq/prj/galilei/galilei/sessions/gpluginmanager.hh"
    "/home/pfrancq/prj/galilei/galilei/sessions/gplugin.hh"
    "/home/pfrancq/prj/galilei/galilei/tools/gmeasure2elements.h"
    "/home/pfrancq/prj/galilei/galilei/tools/gtool.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

