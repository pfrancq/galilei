SET(langnl_frontend_kde_MOC_INCLUDES )

SET(langnl_frontend_kde_UI_INCLUDES )

SET(langnl_frontend_kde_INST_INCLUDES ${langnl_frontend_kde_MOC_INCLUDES})

SET(langnl_frontend_kde_TARGET_SOURCES langnl_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${langnl_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(langnl_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${langnl_frontend_kde_MOC_INCLUDES})

FOREACH(file ${langnl_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(langnl_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${langnl_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
