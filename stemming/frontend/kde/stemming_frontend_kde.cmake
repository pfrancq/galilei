SET(stemming_frontend_kde_TARGET_SOURCES gstemming_kde.cpp)

#KDE4_HANDLE_AUTOMOC(textanalyze_frontend_kde_MOCFILE frontend/kde/gtextanalyze_kde.cpp)

FOREACH(file ${stemming_frontend_kde_UI_INCLUDES})
   SET(REALFILEstemming/kde/${file})
   KDE4_ADD_UI_FILES(stemming_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${stemming_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
