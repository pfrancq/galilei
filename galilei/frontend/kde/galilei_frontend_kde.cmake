SET(galilei_frontend_kde_INST_INCLUDES
qgmatrixmeasuredlg.h
qgweightinfos.h
qgdocstruct.h
qgobjectslist.h
qgobjectvars.h
)

SET(galilei_frontend_kde_TARGET_SOURCES
qgmatrixmeasuredlg.cpp
qgweightinfos.cpp
qgdocstruct.cpp
qgobjectslist.cpp
qgobjectvars.cpp                                        
)

SET(galilei_frontend_kde_UI_INCLUDES qgmatrixmeasuredlg.ui qgobjectslist.ui qgobjectvars.ui qgdocstruct.ui qgweightinfos.ui)

FOREACH(file ${galilei_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(galilei_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${galilei_frontend_kde_UI_INCLUDES})

#KDE4_ADD_KCFG_FILES(roptimization_frontend_kde_TARGET_SOURCES )
