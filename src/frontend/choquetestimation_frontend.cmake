IF(QT4_FOUND)
    SET(choquetestimation_AVAILABLE_FRONTENDS qt)
    SET(choquetestimation_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(choquetestimation_frontend choquetestimation qt choquetestimation_INCLUDES)
ENDIF(QT4_FOUND)
