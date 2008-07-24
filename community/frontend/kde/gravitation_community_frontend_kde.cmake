SET(gravitation_community_frontend_kde_MOC_INCLUDES )

SET(gravitation_community_frontend_kde_UI_INCLUDES dlgconfig_qt.ui)

SET(gravitation_community_frontend_kde_INST_INCLUDES ${gravitation_community_frontend_kde_MOC_INCLUDES}
                                          dlgconfig_qt.ui.h)

SET(gravitation_community_frontend_kde_TARGET_SOURCES gravitation_community_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${gravitation_community_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(gravitation_community_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${gravitation_community_frontend_kde_MOC_INCLUDES})

FOREACH(file ${gravitation_community_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(gravitation_community_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${gravitation_community_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
