SET(docsdisagree_frontend_kde_MOC_INCLUDES )

SET(docsdisagree_frontend_kde_UI_INCLUDES)

SET(docsdisagree_frontend_kde_INST_INCLUDES ${docsdisagree_frontend_kde_MOC_INCLUDES})

SET(docsdisagree_frontend_kde_TARGET_SOURCES docsdisagree_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${docsdisagree_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(docsdisagree_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${docsdisagree_frontend_kde_MOC_INCLUDES})

FOREACH(file ${docsdisagree_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(docsdisagree_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${docsdisagree_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
