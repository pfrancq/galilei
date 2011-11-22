SET(stopwords_frontend_kde_UI_INCLUDES config.ui)

SET(stopwords_frontend_kde_TARGET_SOURCES gstopwords_kde.cpp)

FOREACH(file ${stopwords_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(stopwords_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${stopwords_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})