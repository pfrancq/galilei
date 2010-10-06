SET(featureseval_frontend_kde_UI_INCLUDES config.ui)

SET(featureseval_frontend_kde_TARGET_SOURCES featureseval_kde.cpp)

FOREACH(file ${featureseval_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(featureseval_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${featureseval_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
