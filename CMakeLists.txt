cmake_minimum_required(VERSION 3.5)
###### Author: Alexis Devillard
###### Date:   27/01/2020
###### Brief: This CmakeLists.txt file enable to build generic project with the following structure:
######                                +---------+
######                                | project |
######                                +----+----+
######                                     |
######            +----------+------------++-------------+-----------+
######            |          |            |              |           |
######         +--+--+  +----+----+ +-----+------+ +-----+------+ +--+--+
######         | src |  | include | | CMakelists | | CMakefunct | | lib |
######         +--+--+  +----+----+ |    .txt    | |  ions.txt  | +--+--+
######            |          |      +------------+ +------------+    |
######      +-----+------+   |                                       |
######      |            |   +-------+            +--------------+---+
###### +----+-----+      |           |            |              |
###### | main.cpp | +----+----+ +----+----+ +-----+------+ +-----+------+
###### +----------+ | lib.cpp | | lib.hpp | | subproject | | subproject |
######              +---------+ +---------+ |     1      | |     2      |
######                                      +-----+------+ +------------+
######                                            |
######                        +-----------+-------+------+-----------+
######                        |           |              |           |
######                     +--+---+ +-----+------+ +-----+------+ +--+--+
######                     | .git | | CMakelists | | CMakeFunct | | ... |
######                     +------+ |    .txt    | |  ions.txt  | +-----+
######                              +------------+ +------------+
###### 
######

###### PROJECT NAME AND VERSION ######
set(EXEC_NAME stewart_platform)
set(PROJECT_VERSION 6.6)
project(${EXEC_NAME} VERSION ${PROJECT_VERSION})
message("\n## ${EXEC_NAME} (v${PROJECT_VERSION}) ##")


###### LIBRARY NAME ######
set(LIB_NAME ${EXEC_NAME}.${PROJECT_VERSION})

###### C++ standard ######
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

###### INTERN libraries ######
set(EXE_INTERN_LIBS joystick)
set(LIB_INTERN_LIBS lxm32 )

###### EXTERN libraries ######
set(EXE_EXTERN_LIBS Qt5)
set(LIB_EXTERN_LIBS gazebo)

## LIBRARIES SETTINGS ##
# set(name_V 1.55)
# set(name_REQUIRED "REQUIRED")
# set(name_COMPONENTS_VAR "name_LIBRARIES")
# set(name_INCLUDE_DIRS_VAR "name_INCLUDE_DIRS")
# set(name_DEPENDANT_SRC "name_file")


set(gazebo_COMPONENTS_VAR "GAZEBO_LIBRARIES")
set(gazebo_INCLUDE_DIRS_VAR "GAZEBO_INCLUDE_DIRS")
set(gazebo_DEPENDANT_SRC "gazebo_sim")

set(Qt5_REQUIRED "REQUIRED")
set(Qt5_COMPONENTS_VAR "Qt5Widgets_LIBRARIES")
set(Qt5_COMPONENTS "Widgets")
set(Qt5_INCLUDE_DIRS_VAR "Qt5Widgets_INCLUDE_DIRS")




########### AUTO ###########
###### include helper functions ######
include(CMakeFunctions.txt)
###### Search for Subdirectories/Libraries ######
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib")
  if(NOT AVAILABLE_INTERN_LIBS)
    define_property(GLOBAL PROPERTY AVAILABLE_INTERN_LIBS
      BRIEF_DOCS "PX4 module paths"
      FULL_DOCS "List of paths to all PX4 modules"
      )
  endif()
  message("\n> AVAILABLE INTERNAL LIBS:")
  subdirlist(LIBS "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
  foreach(subdir ${LIBS})
    #git submodules init
    execute_process(COMMAND git submodule update --remote --merge --init -- "lib/${subdir}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory("lib/${subdir}")
    #get the version
    subproject_version("${subdir}" subdir_version)
    message("-- ${EXEC_NAME}: Added library ${subdir} version: ${subdir_version}")
    set_property(GLOBAL APPEND PROPERTY AVAILABLE_INTERN_LIBS "${subdir}.${subdir_version}")
    #list(APPEND AVAILABLE_INTERN_LIBS "${subdir}.${subdir_version}")
  endforeach()
endif()



########### EXEC SOURCES ###########
###### Get the sources and headers ######
###### Exec main src file ######
set(MAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp")

###### Library src and header files (everything else) ######
file(GLOB LIB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" )
list(REMOVE_ITEM LIB_SRCS ${MAIN_FILE})
file(GLOB LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp" )





########### LIBRARY CREATION ###########
message("\n LIBRARY : ${EXEC_NAME}.${PROJECT_VERSION}")
set(LIB_REMOVED_SRC "none")
set(LIB_REMOVED_HEADER "none")
set(LIB_LIBS "")
set(LIB_INCLUDE_DIRS "")
## SEARCH FOR EXTERNAL LIBRARIES ##
message("> EXTERN LIB:")
foreach(LIB IN LISTS LIB_EXTERN_LIBS)
  if("${${LIB}_COMPONENTS_VAR}" STREQUAL "")
    message("-- [OK] ${LIB}")
    list(APPEND LIB_LIBS ${LIB})
  else()
    message(" ${${LIB}_COMPONENTS}")
    find_package(${LIB} ${${LIB}_V} QUIET ${${LIB}_REQUIRED} COMPONENTS ${${LIB}_COMPONENTS}) #search the library
    if (${LIB}_FOUND) #If found
      message("-- [OK] ${LIB}")
      list(APPEND LIB_LIBS ${${${LIB}_COMPONENTS_VAR}})
      list(APPEND LIB_INCLUDE_DIRS ${${${LIB}_INCLUDE_DIRS_VAR}})
    else ()
      message("-- [NO] ${LIB}")
      foreach(DEP_SRC IN LISTS ${${LIB}_DEPENDANT_SRC})
	list(APPEND LIB_REMOVED_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/${DEP_SRC}.hpp")
	list(APPEND LIB_REMOVED_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/${DEP_SRC}.cpp")
      endforeach()
    endif (${LIB}_FOUND)
  endif()
endforeach()
list(REMOVE_ITEM LIB_SRCS ${LIB_REMOVED_SRC})
list(REMOVE_ITEM LIB_HEADERS ${LIB_REMOVED_HEADER})
## SEARCH FOR INTERNAL LIBRARIES ##
foreach(LIB IN LISTS LIB_INTERN_LIBS)
  get_property(available_libs GLOBAL PROPERTY AVAILABLE_INTERN_LIBS)
  string(REGEX MATCH "${LIB}.[0-9]+.[0-9]+" VAL ${available_libs})
  if(VAL)
    message("-- [OK] ${VAL}")
    list(APPEND LIB_LIBS "${VAL}")
  else()
    message("-- [NO] ${LIB} not found")
  endif()
endforeach()

###### Create the library ######
add_library(${LIB_NAME} ${LIB_SRCS})
target_link_libraries (${LIB_NAME}
  PUBLIC ${LIB_LIBS})
target_include_directories(${LIB_NAME}
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PUBLIC ${LIB_INCLUDE_DIRS})
#######################################








########### EXECUTABLE CREATION ##########
message("\n EXECUTABLE : ${EXEC_NAME}")
set(EXE_REMOVED_SRC "none")
set(EXE_REMOVED_HEADER "none")
set(EXE_LIBS "")
set(EXE_INCLUDE_DIRS "")
## SEARCH FOR EXTERNAL LIBRARIES ##
message("> EXTERN LIB:")
foreach(LIB IN LISTS EXE_EXTERN_LIBS)
  if("${${LIB}_COMPONENTS_VAR}" STREQUAL "")
    message("-- [OK] ${LIB}")
    list(APPEND EXE_LIBS ${LIB})
  else()
    find_package(${LIB} ${${LIB}_V} QUIET ${${LIB}_REQUIRED} COMPONENTS ${${LIB}_COMPONENTS}) #search the library
    if (${LIB}_FOUND) #If found
      message("-- [OK] ${LIB}")
      list(APPEND EXE_LIBS ${${${LIB}_COMPONENTS_VAR}})
      list(APPEND EXE_INCLUDE_DIRS ${${${LIB}_INCLUDE_DIRS_VAR}})
    else ()
      message("-- [NO] ${LIB}")
      foreach(DEP_SRC IN LISTS ${${LIB}_DEPENDANT_SRC})
	list(APPEND EXE_REMOVED_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/${DEP_SRC}.hpp")
	list(APPEND EXE_REMOVED_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/${DEP_SRC}.cpp")
      endforeach()
    endif (${LIB}_FOUND)
  endif()
endforeach()
## SEARCH FOR INTERNAL LIBRARIES ##
foreach(LIB IN LISTS EXE_INTERN_LIBS)
  get_property(available_libs GLOBAL PROPERTY AVAILABLE_INTERN_LIBS)
  string(REGEX MATCH "${LIB}.[0-9]+.[0-9]+" VAL ${available_libs})
  if(VAL)
    list(APPEND EXE_LIBS "${VAL}")
    message("-- [OK] ${VAL}")
  else()
    message("-- [NO] ${LIB} not found")
  endif()
endforeach()

message("\n>  BUILD:")

###### QT SETTINGS ######
set(CMAKE_INCLUDE_CURRENT_DIR ON)
qt5_wrap_cpp(QT_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/include/mainwindow.h)
qt5_wrap_ui(QT_UI ${CMAKE_CURRENT_SOURCE_DIR}/forms/mainwindow.ui)


###### Create the executable ######
add_executable(${EXEC_NAME} ${MAIN_FILE} ${QT_SRCS} ${QT_UI})
target_link_libraries (${EXEC_NAME}
  PUBLIC ${LIB_NAME}
  ${EXE_LIBS})
target_include_directories(${EXEC_NAME}
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PUBLIC "${EXE_INCLUDE}"
  PUBLIC "${QT_UI}")

##############################



###### Instalation rule ######
install(TARGETS ${EXEC_NAME} DESTINATION bin)
#install(TARGETS ${LIB_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)



find_program(DOT_EXE "dot")
if(DOT_EXE)
  message(STATUS "dot found: ${DOT_EXE}")
  set(DOT_OUTPUT_TYPE "png")
    add_custom_target(dependency-graph_${EXEC_NAME}
      COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} --graphviz=${CMAKE_BINARY_DIR}/graphviz/${PROJECT_NAME}.dot
        COMMAND ${DOT_EXE} -T${DOT_OUTPUT_TYPE} ${CMAKE_BINARY_DIR}/graphviz/${PROJECT_NAME}.dot -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.${DOT_OUTPUT_TYPE}
	)

      add_custom_command(
        TARGET dependency-graph_${EXEC_NAME} PRE_BUILD
        COMMAND echo "set\\\(GRAPHVIZ_EXTERNAL_LIBS NO\\\)" > CMakeGraphVizOptions.cmake
	# COMMAND echo "set\\\(GRAPHVIZ_IGNORE_TARGETS lxm32\\\)" >> CMakeGraphVizOptions.cmake
	)

    add_custom_command(
        TARGET dependency-graph_${EXEC_NAME} POST_BUILD
        COMMAND ;
        COMMENT
        "Dependency graph generated and located at ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.${DOT_OUTPUT_TYPE}"
	)
      else()
    message(STATUS "dot not found!")
endif()
