SET(kmeans_profiles_frontend_qt_Qt_INCLUDES)

SET(kmeans_profiles_frontend_qt_UI_INCLUDES config.ui)

SET(kmeans_profiles_frontend_qt_TARGET_SOURCES kmeansprofiles_qt.cpp)

FOREACH(file ${kmeans_profiles_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(kmeans_profiles_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${kmeans_profiles_frontend_qt_UI_INCLUDES})

FOREACH(file ${kmeans_profiles_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(kmeans_profiles_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${kmeans_profiles_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})