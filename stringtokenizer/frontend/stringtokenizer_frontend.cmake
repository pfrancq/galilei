IF(QT4_FOUND)
    SET(stringtokenizer_AVAILABLE_FRONTENDS qt)
    SET(stringtokenizer_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(stringtokenizer_frontend stringtokenizer qt stringtokenizer_INCLUDES)
ENDIF(QT4_FOUND)

