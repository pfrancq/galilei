SET(stringtokenizer_frontend_kde_TARGET_SOURCES gstringtokenizer_kde.cpp)

#KDE4_HANDLE_AUTOMOC(textanalyze_frontend_kde_MOCFILE frontend/kde/gtextanalyze_kde.cpp)

FOREACH(file ${stringtokenizer_frontend_kde_UI_INCLUDES})
   SET(REALFILEstringtokenizer/kde/${file})
   KDE4_ADD_UI_FILES(stringtokenizer_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${stringtokenizer_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
