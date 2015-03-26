PROJECT(gqalilei)
INCLUDE_DIRECTORIES(.)

SET(qgalilei_SOURCES
	main.cpp
	qgalilei.cpp
	qgalileiwin.cpp
	qsessionprogress.cpp
	qpluginslist.cpp
	configure.cpp
	preferences.cpp
	kviewdicts.cpp
	kviewdoc.cpp
	kviewdocs.cpp
	kviewmetaengine.cpp
	kviewprg.cpp
	kprgconsole.cpp
	kviewstats.cpp
	qfilldatabase.cpp
	qcreatedatabase.cpp
	kviewgroup.cpp
	kviewgroups.cpp
	kviewclasses.cpp
	kviewclass.cpp
	kviewprofile.cpp
	kviewusers.cpp
	kviewidealgroups.cpp
)

SET(qgalilei_HEADERS
	qgalileiwin.h
	qsessionprogress.h
	qpluginslist.h
	configure.h
	preferences.h
	kviewdicts.h
	kviewdoc.h
	kviewdocs.h
	kviewmetaengine.h
	kviewprg.h
	kprgconsole.h
	kviewstats.h
	qfilldatabase.h
	kviewgroup.h
	kviewgroups.h
	kviewclasses.h
	kviewclass.h
	kviewprofile.h
	kviewusers.h
	kviewidealgroups.h
)


SET(qgalilei_FORMS
	qgalileiwin.ui
	qpluginslist.ui
	configure.ui
	preferences.ui
	kviewdicts.ui
	kviewdoc.ui
	kviewdocs.ui
	addfdbkdlg.ui
	kviewmetaengine.ui
	kviewprg.ui
	kprgconsole.ui
	kviewstats.ui
	qfilldatabase.ui
	qcreatedatabase.ui
	qchooselist.ui
	kviewgroup.ui
	kviewgroups.ui
	kviewclasses.ui
	kviewclass.ui
	kviewprofile.ui
	kviewusers.ui
	kviewidealgroups.ui
)


SET(qgalilei_RESOURCES
	qgalilei.qrc
)


QT4_WRAP_CPP(qgalilei_HEADERS_MOC ${qgalilei_HEADERS})
QT4_WRAP_UI(qgalilei_FORMS_HEADERS ${qgalilei_FORMS})
QT4_ADD_RESOURCES(qgalilei_RESOURCES_RCC ${qgalilei_RESOURCES})

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(qgalilei
	${qgalilei_SOURCES}
	${qgalilei_HEADERS_MOC}
	${qgalilei_FORMS_HEADERS}
	${qgalilei_RESOURCES_RCC})

IF(WIN32)
	TARGET_LINK_LIBRARIES(qgalilei
						"C:/Users/Pascal/prj/r/build/rcore/librcore.a"
						"C:/Users/Pascal/prj/r/build/rcore/librcoreqt.a"
						${QT_LIBRARIES} pthread iconv
						"C:/Program Files/MySQL/MySQL Connector C 6.1/lib/libmysql.lib"
						"C:/Program Files/MySQL/MySQL Connector C 6.1/lib/vs11/mysqlclient.lib"
						C:/curl/lib/libcurl.a C:/curl/lib/libcurldll.a)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(qgalilei rcore rcoreqt rmath galilei galileiqt ${QT_LIBRARIES})
ENDIF(WIN32)

#KDE4_ADD_EXECUTABLE(qgalilei ${QGALILEI_SOURCES})
#TARGET_LINK_LIBRARIES(qgalilei galilei galileiqt roptimization rcore rcoreqt rmath ${KDE4_KDEUI_LIBS} ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS})


########### install files ###############
# install(TARGETS qgalilei ${INSTALL_TARGETS_DEFAULT_ARGS})
# install( FILES qgalilei.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
# install(FILES qgalileiui.rc  DESTINATION  ${DATA_INSTALL_DIR}/qgalilei )
# install(FILES qgalileiui.rc  DESTINATION  /usr/share/kde4/apps/qgalilei )
# kde4_install_icons(${ICON_INSTALL_DIR})