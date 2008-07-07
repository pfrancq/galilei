SET(pdf_frontend_kde_MOC_INCLUDES )

SET(pdf_frontend_kde_UI_INCLUDES )

SET(pdf_frontend_kde_INST_INCLUDES ${pdf_frontend_kde_MOC_INCLUDES})

SET(pdf_frontend_kde_TARGET_SOURCES pdf_kde.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${pdf_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(pdf_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${pdf_frontend_kde_MOC_INCLUDES})

FOREACH(file ${pdf_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(pdf_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${pdf_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
