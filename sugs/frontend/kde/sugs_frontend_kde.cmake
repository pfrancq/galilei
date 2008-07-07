SET(sugs_frontend_kde_MOC_INCLUDES )

SET(sugs_frontend_kde_UI_INCLUDES dlgconfig_qt.ui)

SET(sugs_frontend_kde_INST_INCLUDES ${sugs_frontend_kde_MOC_INCLUDES}
                                          dlgconfig_qt.ui.h)

SET(sugs_frontend_kde_TARGET_SOURCES sugs_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${sugs_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(sugs_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${sugs_frontend_kde_MOC_INCLUDES})

FOREACH(file ${sugs_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(sugs_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${sugs_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
