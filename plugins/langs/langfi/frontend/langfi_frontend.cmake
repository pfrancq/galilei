IF(QT4_FOUND)
    SET(langfi_AVAILABLE_FRONTENDS qt)
    SET(langfi_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(langfi_frontend langfi qt langfi_INCLUDES)
ENDIF(QT4_FOUND)