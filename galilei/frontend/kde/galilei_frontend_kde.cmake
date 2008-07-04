SET(galilei_frontend_kde_MOC_INCLUDES   qgprofile.h
					qgdocxml.h qgcommunitieshistory.h)

SET(galilei_frontend_kde_UI_INCLUDES dlgmeasure2elements.ui)

SET(galilei_frontend_kde_INST_INCLUDES ${galilei_frontend_kde_MOC_INCLUDES}
                                        qgprofile.h
                                        qgdocxml.h
                                        qgmeasure2elementsdlg.h
                                        qlistviewitemtype.h
                                        qgcommunitieshistory.h)

SET(galilei_frontend_kde_TARGET_SOURCES qgprofile.cpp
                                        qgdocxml.cpp
                                        qlistviewitemtype.cpp
                                        qgmeasure2elementsdlg.cpp
                                        qgcommunitieshistory.cpp)

###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${galilei_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(galilei_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${galilei_frontend_kde_MOC_INCLUDES})

FOREACH(file ${galilei_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_UI_FILES(galilei_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${galilei_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

INSTALL(FILES frontend/kde/nokonqueror.png
		frontend/kde/xml_cdata_b.png
		frontend/kde/xml_comment_b.png
		frontend/kde/xml_contents_b.png
		frontend/kde/xml_element_b.png
		frontend/kde/xml_procinstr_b.png
		frontend/kde/xml_text_b.png
		frontend/kde/xml_attribute.png
		frontend/kde/xml_cdata.png
		frontend/kde/xml_comment.png
		frontend/kde/xml_contents.png
		frontend/kde/xml_element.png
		frontend/kde/xml_procinstr.png
		frontend/kde/xml_text.png

		DESTINATION share/icons/hicolor/16x16/actions/)
