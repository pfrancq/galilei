SET(galilei_frontend_qt_INST_INCLUDES
	qgmatrixmeasuredlg.h
	qgdescription.h
	qgdocstruct.h
	qgobjectslist.h
	qgobjectvars.h
)


SET(galilei_frontend_qt_Qt_INCLUDES
	qgmatrixmeasuredlg.h
	qgdescription.h
	qgobjectslist.h
)

SET(galilei_frontend_qt_TARGET_SOURCES
	qgmatrixmeasuredlg.cpp
	qgdescription.cpp
	qgdocstruct.cpp
	qgobjectslist.cpp
	qgobjectvars.cpp
)

SET(galilei_frontend_qt_UI_INCLUDES
	qgmatrixmeasuredlg.ui
	qgobjectslist.ui
	qgobjectvars.ui
	qgdocstruct.ui
	qgdescription.ui
)

FOREACH(file ${galilei_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(galilei_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${galilei_frontend_qt_UI_INCLUDES})

FOREACH(file ${galilei_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(galilei_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${galilei_frontend_qt_Qt_INCLUDES})

