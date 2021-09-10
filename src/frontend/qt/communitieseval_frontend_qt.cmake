SET(communitieseval_frontend_kde_TARGET_SOURCES communitieseval_kde.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(communitieseval_frontend_qt_Qt_INCLUDES )

SET(communitieseval_frontend_qt_UI_INCLUDES)

SET(communitieseval_frontend_qt_TARGET_SOURCES communitieseval_qt.cpp)

FOREACH(file ${communitieseval_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(communitieseval_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${communitieseval_frontend_qt_UI_INCLUDES})

FOREACH(file ${communitieseval_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(communitieseval_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${communitieseval_frontend_qt_Qt_INCLUDES})
