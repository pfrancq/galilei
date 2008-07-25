SET(docsagree_frontend_kde_MOC_INCLUDES )

SET(docsagree_frontend_kde_UI_INCLUDES)

SET(docsagree_frontend_kde_INST_INCLUDES ${docsagree_frontend_kde_MOC_INCLUDES})

SET(docsagree_frontend_kde_TARGET_SOURCES docsagree_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${docsagree_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(docsagree_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${docsagree_frontend_kde_MOC_INCLUDES})

FOREACH(file ${docsagree_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(docsagree_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${docsagree_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
