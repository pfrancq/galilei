PROJECT(kgalileicenter)
INCLUDE_DIRECTORIES(.)

SET(kgalileicenter_frontend_kde_MOC_INCLUDES kdoc.h
                                                                                               kgalileicenter.h
                                                                                               kviewdebug.h
                                                                                               kviewdicts.h
                                                                                               kviewdoc.h
                                                                                               kviewdocs.h
                                                                                               kviewgroup.h
                                                                                               kviewgroups.h
                                                                                               kview.h
                                                                                               kviewhistory.h
                                                                                               kviewmetaengine.h
                                                                                               kviewprg.h
                                                                                               kviewprofile.h
                                                                                               kviewstats.h
                                                                                               kviewstems.h
                                                                                               kviewthgroups.h
                                                                                               kviewusers.h
                                                                                               qmyplugins.h
                                                                                               qsessionprogress.h
)

SET(kgalileicenter_frontend_kde_UI_INCLUDES qchooseparent.ui
                                                                      qhistorydlg.ui
                                                                      qfilldatabase.ui
                                                                      qcreatedatabase.ui
                                                                      qexportmatrixdlg.ui
                                                                      qplugins.ui
                                                                      qsimulationdlg.ui
                                                                       configure.ui
)

SET (KGALILEICENTER_SOURCES   kviewhistory.cpp
                                                                      kviewprg.cpp
                                                                      kgalileicenter_init.cpp
                                                                      kgalileicenter_dlg.cpp
                                                                      kviewstems.cpp
                                                                      qsessionprogress.cpp
                                                                      kviewusers.cpp
                                                                      kviewthgroups.cpp
                                                                      kviewprofile.cpp
                                                                      kviewgroups.cpp
                                                                      kviewgroup.cpp
                                                                      kviewdocs.cpp
                                                                      kviewdoc.cpp
                                                                      kviewstats.cpp
                                                                      kview.cpp
                                                                      kdoc.cpp
                                                                      kviewmetaengine.cpp
                                                                      kgalileicenter.cpp
                                                                      main.cpp
                                                                      kviewdicts.cpp
                                                                      kviewdebug.cpp
          )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(KDE_LIBS  ${QT_AND_KDECORE_LIBS} ${KDE3_LIB_DIR}/libkdeui.so  ${KDE3_LIB_DIR}/libkio.so )

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${kgalileicenter_frontend_kde_MOC_INCLUDES})
    SET(REALFILE ${file})
    KDE3_ADD_MOC_FILES(kgalileicenter_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${kgalileicenter_frontend_kde_MOC_INCLUDES})

FOREACH(file ${kgalileicenter_frontend_kde_UI_INCLUDES})
    SET(REALFILE ${file})
   KDE3_ADD_UI_FILES(kgalileicenter_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${kgalileicenter_frontend_kde_UI_INCLUDES})

#    ADD_LIBRARY(galilei SHARED ${galilei_TARGET_SOURCES})
KDE3_ADD_EXECUTABLE(kgalileicenter ${kgalileicenter_frontend_kde_UIFILE}
                                                                   ${kgalileicenter_frontend_kde_MOCFILE}
                                                                   ${KGALILEICENTER_SOURCES} )
TARGET_LINK_LIBRARIES(kgalileicenter galilei galileikde roptimization rcore rcorekde rmath ${KDE_LIBS})