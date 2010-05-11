PROJECT(kgalileicenter)
INCLUDE_DIRECTORIES(.)

SET (KGALILEICENTER_SOURCES
main.cpp
kgalileicenter.cpp
qpluginslist.cpp
qsessionprogress.cpp
configure.cpp
kviewdoc.cpp
kviewmetaengine.cpp
kviewprg.cpp
qfilldatabase.cpp
qcreatedatabase.cpp
kviewstats.cpp
kviewgroup.cpp
kviewusers.cpp
kviewgroups.cpp
kviewidealgroups.cpp
kviewprofile.cpp
kviewdicts.cpp
kprgconsole.cpp
kviewclasses.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

KDE4_HANDLE_AUTOMOC(${KGALILEICENTER_SOURCES})

KDE4_ADD_UI_FILES(KGALILEICENTER_SOURCES
	mainconfigure.ui
	qpluginslist.ui
	configure.ui
	kviewdoc.ui
	addfdbkdlg.ui
	kviewmetaengine.ui
	kviewprg.ui
	qfilldatabase.ui
	kviewstats.ui
	kviewgroup.ui
	kviewusers.ui
	kviewgroups.ui
	kviewidealgroups.ui
	kviewprofile.ui
	qcreatedatabase.ui
	kviewdicts.ui
	kprgconsole.ui
	qchooselist.ui
	kviewclasses.ui
)


KDE4_ADD_EXECUTABLE(kgalileicenter ${KGALILEICENTER_SOURCES})
TARGET_LINK_LIBRARIES(kgalileicenter galilei galileikde roptimization rcore rcorekde rmath ${KDE4_KDEUI_LIBS} ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS})


########### install files ###############
install(TARGETS kgalileicenter ${INSTALL_TARGETS_DEFAULT_ARGS})
install( FILES kgalileicenter.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install(FILES kgalileicenterui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kgalileicenter )
install(FILES kgalileicenterui.rc  DESTINATION  /usr/share/kde4/apps/kgalileicenter )
kde4_install_icons(${ICON_INSTALL_DIR})