IF(QT4_FOUND)
    SET(stopwords_AVAILABLE_FRONTENDS qt)
    SET(stopwords_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(stopwords_frontend stopwords qt stopwords_INCLUDES)
ENDIF(QT4_FOUND)

