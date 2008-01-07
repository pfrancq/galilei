#copy this file to your project and include it in your cmakelists file
#this file just include the rmacro.cmake file in your project
#also set some vars, usefull to include some sub project related file in sub sub project

#this macro load a dependency file, used to find R, Galilei, or some other R based libraries
MACRO(R_LOAD_DEPENDENCY _depName _depEnvName _dep_paramName)
    SET(CLIBPATH "$ENV{${_depEnvName}}") #get Current LIBrary PATH from env variables
    
    IF(${_dep_paramName}-libs) #get the command line option value if any and overwrite the env pat
        SET(CLIBPATH ${${_dep_paramName}-libs})
    ENDIF(${_dep_paramName}-libs)
    
    IF(CLIBPATH)
        SET(CMACROPATH "${CLIBPATH}/${_dep_paramName}macro.cmake")
    ELSE(CLIBPATH)
        SET(STDPATH "/usr;/usr/local;/opt")
        FIND_PATH(CMACROPATH share/r/${_dep_paramName}macro.cmake ${STDPATH})
        IF(CMACROPATH)
            SET(CLIBPATH ${CMACROPATH})
            SET(CMACROPATH ${CMACROPATH}/share/r/${_dep_paramName}macro.cmake)
        ELSE(CMACROPATH)
            MESSAGE(FATAL_ERROR "Can't find ${_dep_paramName}macro.cmake file, please set ${_depEnvName} environment variable or use -D${_dep_paramName}-libs to set the ${_dep_paramName} library path")
        ENDIF(CMACROPATH)
    ENDIF(CLIBPATH)
    
    MESSAGE(STATUS "Will use ${_depName} in ${CLIBPATH} and macro file in ${CMACROPATH}")
    
    INCLUDE(${CMACROPATH}) #this macro file must add the name of file to search
    
    CHECK_IF_THIS_SPECIFIC_R_LIB_IS_INSTALLED(${_depName} ${_dep_paramName} ${CLIBPATH})
    ADD_R_INCLUDES(${_depName} ${_dep_paramName} ${CLIBPATH}) #these call the macro for CMACROPATH named DO_${_depName}_R_LIB_INCLUDE
        
ENDMACRO(R_LOAD_DEPENDENCY _depEnvName _dep_paramName)