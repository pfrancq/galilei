SET(email_frontend_kde_MOC_INCLUDES )

SET(email_frontend_kde_UI_INCLUDES dlgconfig_qt.ui)

SET(email_frontend_kde_INST_INCLUDES ${email_frontend_kde_MOC_INCLUDES}
                                          dlgconfig_qt.ui.h)

SET(email_frontend_kde_TARGET_SOURCES email_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${email_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(email_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${email_frontend_kde_MOC_INCLUDES})

FOREACH(file ${email_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(email_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${email_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
