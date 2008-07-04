SET(gda_frontend_kde_MOC_INCLUDES )

SET(gda_frontend_kde_UI_INCLUDES dlgconfig_qt.ui)

SET(gda_frontend_kde_INST_INCLUDES ${galilei_frontend_kde_MOC_INCLUDES}
                                          dlgconfig_qt.ui.h)

SET(gca_frontend_kde_TARGET_SOURCES gaconfig_kde.cpp ggdagrouping_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${gda_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(gda_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${gda_frontend_kde_MOC_INCLUDES})

FOREACH(file ${gda_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(gda_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${gda_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
