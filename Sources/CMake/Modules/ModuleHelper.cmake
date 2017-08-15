include ( CMakeParseArguments  )

macro ( module_options OPTIONS )
    foreach( VALUE ${OPTIONS}   )
        list (FIND ARG_COMPONENTS ${VALUE} _index)
        if ( ${_index} GREATER -1 )
            set( ${VALUE} true )
        endif()
    endforeach()
endmacro ()

macro( find_dava_module NAME  )
    cmake_parse_arguments ( ARG ""  "" "OPTIONS" ${ARGN} )
    foreach( FOLDER ${CMAKE_MODULE_PATH} ${DAVA_MODULES_DIR}  )
        get_filename_component( FOLDER ${FOLDER} ABSOLUTE )
        set( MODULE_DIR ${FOLDER}/${NAME} )
        if( EXISTS  ${MODULE_DIR})
            add_module_subdirectory( ${NAME}  "${MODULE_DIR}" OPTIONS ${ARG_OPTIONS} )
        endif()
    endforeach()
endmacro()

macro ( add_module_subdirectory NAME SOURCE_DIR )
    cmake_parse_arguments ( ARG ""  "" "COMPONENTS;OPTIONS" ${ARGN} )

    list(APPEND ARG_COMPONENTS ${ARG_OPTIONS})
    list(LENGTH ARG_COMPONENTS ARG_COMPONENTS_LENGTH ) 

    if( ARG_COMPONENTS_LENGTH )
        set( MODULE_COMPONENTS_VALUE_NAME ${NAME} )
        list( APPEND ARG_COMPONENTS ${NAME} )
    else()
        set( MODULE_COMPONENTS_VALUE_NAME ) 
    endif()

    set_property( GLOBAL PROPERTY COMPONENTS_${MODULE_COMPONENTS_VALUE_NAME} ${ARG_COMPONENTS} )

    foreach( VALUE ${MAIN_MODULE_VALUES} )
        set( ${VALUE}_HASH ${${VALUE}} )
        set( ${VALUE})
    endforeach()
    
    add_subdirectory ( ${SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${NAME} )

    foreach( VALUE ${MAIN_MODULE_VALUES} )
        set(  ${VALUE} ${${VALUE}_HASH} )
    endforeach()

endmacro()    

macro ( add_plugin NAME SOURCE_DIR )
    set_property( GLOBAL PROPERTY COMPONENTS_${MODULE_COMPONENTS_VALUE_NAME}  )

    foreach( VALUE ${GLOBAL_PROPERTY_VALUES} )
        set( ${VALUE}_HASH ${${VALUE}} )
        get_property( ${VALUE}_HASH_PROPERTY GLOBAL PROPERTY ${VALUE} )
        set( ${VALUE} )
        set_property( GLOBAL PROPERTY ${VALUE} )
    endforeach()

    add_subdirectory ( ${SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${NAME} )

    foreach( VALUE ${GLOBAL_PROPERTY_VALUES} )
        set(  ${VALUE} ${${VALUE}_HASH} )
        set_property( GLOBAL PROPERTY ${VALUE} ${${VALUE}_HASH_PROPERTY} )
    endforeach()
    
endmacro()
