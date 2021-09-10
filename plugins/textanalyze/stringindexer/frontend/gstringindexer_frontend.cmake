IF(QT4_FOUND)
    SET(gstringindexer_AVAILABLE_FRONTENDS qt)
    SET(gstringindexer_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(gstringindexer_frontend gstringindexer qt gstringindexer_INCLUDES)
ENDIF(QT4_FOUND)

