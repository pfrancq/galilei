SET(langes_frontend_kde_MOC_INCLUDES )

SET(langes_frontend_kde_UI_INCLUDES )

SET(langes_frontend_kde_INST_INCLUDES ${langes_frontend_kde_MOC_INCLUDES})

SET(langes_frontend_kde_TARGET_SOURCES langes_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${langes_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(langes_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${langes_frontend_kde_MOC_INCLUDES})

FOREACH(file ${langes_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(langes_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${langes_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
