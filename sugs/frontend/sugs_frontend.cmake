IF(QT4_FOUND)
    SET(sugs_AVAILABLE_FRONTENDS qt)
    SET(sugs_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(sugs_frontend sugs qt sugs_INCLUDES)
ENDIF(QT4_FOUND)
