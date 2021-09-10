SET(nngga_profiles_frontend_qt_Qt_INCLUDES nnggaconfig_qt.h)

SET(nngga_profiles_frontend_qt_UI_INCLUDES config.ui)

SET(nngga_profiles_frontend_qt_TARGET_SOURCES nnggagroupprofiles_qt.cpp nnggaconfig_qt.cpp)

FOREACH(file ${nngga_profiles_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(nngga_profiles_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${nngga_profiles_frontend_qt_UI_INCLUDES})

FOREACH(file ${nngga_profiles_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(nngga_profiles_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${nngga_profiles_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


