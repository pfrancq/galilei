IF(QT4_FOUND)
    SET(sgga_profiles_AVAILABLE_FRONTENDS qt)
    SET(sgga_profiles_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(sgga_profiles_frontend sgga_profiles qt sgga_profiles_INCLUDES)
ENDIF(QT4_FOUND)
