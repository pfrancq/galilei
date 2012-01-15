SET(stringtokenizer_frontend_kde_UI_INCLUDES config.ui)

SET(stringtokenizer_frontend_kde_TARGET_SOURCES gstringtokenizer_kde.cpp)

FOREACH(file ${stringtokenizer_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(stringtokenizer_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${stringtokenizer_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})