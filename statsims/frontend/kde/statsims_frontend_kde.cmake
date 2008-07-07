SET(statsims_frontend_kde_MOC_INCLUDES )

SET(statsims_frontend_kde_UI_INCLUDES dlgconfig_qt.ui)

SET(statsims_frontend_kde_INST_INCLUDES ${statsims_frontend_kde_MOC_INCLUDES}
                                          dlgconfig_qt.ui.h)

SET(statsims_frontend_kde_TARGET_SOURCES statsims_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${statsims_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(statsims_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${statsims_frontend_kde_MOC_INCLUDES})

FOREACH(file ${statsims_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(statsims_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${statsims_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
