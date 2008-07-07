SET(feedback_frontend_kde_MOC_INCLUDES )

SET(feedback_frontend_kde_UI_INCLUDES dlgconfig_qt.ui)

SET(feedback_frontend_kde_INST_INCLUDES ${feedback_frontend_kde_MOC_INCLUDES}
                                          dlgconfig_qt.ui.h)

SET(feedback_frontend_kde_TARGET_SOURCES feedback_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${feedback_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(feedback_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${feedback_frontend_kde_MOC_INCLUDES})

FOREACH(file ${feedback_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(feedback_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${feedback_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
