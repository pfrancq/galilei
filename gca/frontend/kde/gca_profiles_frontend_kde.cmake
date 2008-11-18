SET(gca_profiles_frontend_kde_UI_INCLUDES config.ui)

SET(gca_profiles_frontend_kde_TARGET_SOURCES gaconfig_kde.cpp gcagroupprofiles_kde.cpp)

FOREACH(file ${gca_profiles_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(gca_profiles_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${gca_profiles_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
