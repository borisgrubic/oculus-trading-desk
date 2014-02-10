
MACRO(GENERATE_GMOCK_CLASS SOURCE_FILE SOURCE_DIR MockHeader ClassName)
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp
        COMMAND echo "\"" "#pragma" "once" "\"" > ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp
        COMMAND echo "\"" "#include" "<${SOURCE_DIR}/${MockHeader}.hpp>" "\"" >>  ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp
        COMMAND ${GMOCK_SCRIPTS_DIR}/generator/gmock_gen.py ${PROJECT_SOURCE_DIR}/src/${SOURCE_DIR}/${MockHeader}.hpp ${ClassName} >>  ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp
        MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/src/${SOURCE_DIR}/${MockHeader}.hpp
)
set_property(SOURCE ${SOURCE_FILE} APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp)
ENDMACRO(GENERATE_GMOCK_CLASS)


MACRO(GENERATE_GMOCK SOURCE_FILE SOURCE_DIR MockHeader)
	GENERATE_GMOCK_CLASS (${SOURCE_FILE} ${SOURCE_DIR} ${MockHeader} "")
ENDMACRO(GENERATE_GMOCK) 


## same as GENERATE_GMOCK, but working with .h file
MACRO(GENERATE_GMOCK_H SOURCE_FILE SOURCE_DIR MockHeader)
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp
        COMMAND echo "\"" "#pragma" "once" "\"" > ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp
        COMMAND echo "\"" "#include" "<${SOURCE_DIR}/${MockHeader}.h>" "\"" >>  ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp
        COMMAND ${GMOCK_SCRIPTS_DIR}/generator/gmock_gen.py ${PROJECT_SOURCE_DIR}/src/${SOURCE_DIR}/${MockHeader}.h >>  ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp
        MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/src/${SOURCE_DIR}/${MockHeader}.h
)
set_property(SOURCE ${SOURCE_FILE} APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Mock${MockHeader}.hpp)
ENDMACRO(GENERATE_GMOCK_H) 



