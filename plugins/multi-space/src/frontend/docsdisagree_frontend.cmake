IF(QT4_FOUND)
    SET(docsdisagree_AVAILABLE_FRONTENDS qt)
    SET(docsdisagree_qt_DEPENDENCY rcoreqt galileiqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(docsdisagree_frontend docsdisagree qt docsdisagree_INCLUDES)
ENDIF(QT4_FOUND)