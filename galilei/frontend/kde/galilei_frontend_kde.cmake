SET(galilei_frontend_kde_MOC_INCLUDES qggroupshistory.h
					qgprofile.h
					qgsubprofiles.h
					qgdocxml.h)

SET(galilei_frontend_kde_UI_INCLUDES dlgmeasure2elements.ui)

SET(galilei_frontend_kde_INST_INCLUDES ${galilei_frontend_kde_MOC_INCLUDES}
                                        qgprofile.h
                                        qgdocxml.h
                                        qgsubprofiles.h
                                        qgmeasure2elementsdlg.h
                                        qlistviewitemtype.h
                                        qggroupshistory.h)

SET(galilei_frontend_kde_TARGET_SOURCES qgprofile.cpp
                                        qgdocxml.cpp
                                        qgsubprofiles.cpp
                                        qlistviewitemtype.cpp
                                        qggroupshistory.cpp
                                        qgmeasure2elementsdlg.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${galilei_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(galilei_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${galilei_frontend_kde_MOC_INCLUDES})

FOREACH(file ${galilei_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(galilei_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${galilei_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
