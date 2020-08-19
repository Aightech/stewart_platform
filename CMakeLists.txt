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
set(PROJECT_VERSION 1.0)
project(${EXEC_NAME} VERSION ${PROJECT_VERSION})

###### LIBRARY NAME ######
set(LIB_NAME ${EXEC_NAME}.${PROJECT_VERSION})

###### C++ standard ######
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


message("\n> EXTERN LIB:")
###### Special libraries ######
set(REMOVED_SRC "none")
set(REMOVED_HEADER "none")


if(0)
  set(EXTRA_LIBS "pthread" "util")
endif()

find_package(Boost 1.56 REQUIRED COMPONENTS system filesystem iostreams QUIET)
if (Boost_FOUND)
  message("-- Boost \t[OK]")
  list(APPEND EXTRA_LIBS ${Boost_IOSTREAMS_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
else ()
  message("-- Boost \t[NO]")
endif (Boost_FOUND)

find_package(gazebo QUIET)
if (gazebo_FOUND)
  message("-- Gazebo \t[OK]")
  list(APPEND EXTRA_LIBS ${GAZEBO_LIBRARIES})
  list(APPEND EXTRA_INCLUDE ${GAZEBO_INCLUDE_DIRS})
else ()
  message("-- Gazebo \t[NO]")
  list(APPEND REMOVED_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/gazebo_sim.hpp")
  list(APPEND REMOVED_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/gazebo_sim.cpp")
endif (gazebo_FOUND)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
if (Qt5_FOUND)
  message("-- Qt5    \t[OK]")
  list(APPEND EXTRA_LIBS ${Qt5Widgets_LIBRARIES})
  list(APPEND EXTRA_INCLUDE ${Qt5Widgets_INCLUDE_DIRS})
  
else ()
  message("-- Qt5    \t[NO]")
endif (Qt5_FOUND)




###### Get the sources and headers ######
###### Exec main src file ######
set(MAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
###### Exec-only header file ######
set(EXTRA_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/gnuplot-iostream.h")

###### Library src and header files (everything else) ######
file(GLOB LIB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" )
list(REMOVE_ITEM LIB_SRCS ${MAIN_FILE})
list(REMOVE_ITEM LIB_SRCS ${REMOVED_SRC})
file(GLOB LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp" )

message("${LIB_SRCS}")
list(REMOVE_ITEM LIB_SRCS ${REMOVED_SRC})
list(REMOVE_ITEM LIB_HEADERS ${REMOVED_HEADER})


   



###### include helper functions ######
include(CMakeFunctions.txt)


###### Search for Subdirectories/Libraries ######
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib") 
  message("\n> INTERN LIB:")
  subdirlist(LIBS "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
  foreach(subdir ${LIBS})
    #git submodules init
    execute_process(COMMAND git submodule update --remote --merge --init -- "lib/${subdir}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory("lib/${subdir}")
    #get the version
    subproject_version("${subdir}" subdir_version)
    message("-- ${EXEC_NAME}: Added library ${subdir} version: ${subdir_version}")
    list(APPEND EXTRA_LIBS "${subdir}.${subdir_version}")
  endforeach()
endif()


message("\n>  BUILD:")

####### Create the library ######
#add_library(${LIB_NAME} ${LIB_SRCS})
#target_link_libraries (${LIB_NAME}
#  PUBLIC ${EXTRA_LIBS})
#target_include_directories(${LIB_NAME}
#  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
#  PUBLIC "${EXTRA_INCLUDE}"
#  )



###### Create the executable ######

set(CMAKE_INCLUDE_CURRENT_DIR ON)
qt5_wrap_cpp(EXEC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/include/mainwindow.h)
qt5_wrap_ui(EXEC_UI ${CMAKE_CURRENT_SOURCE_DIR}/forms/mainwindow.ui)
set(LIB_SRCS ${LIB_SRCS}  ${EXEC_SRC})


add_executable(${EXEC_NAME} ${MAIN_FILE} ${LIB_SRCS})
target_link_libraries (${EXEC_NAME} ${EXTRA_LIBS})
target_include_directories(${EXEC_NAME}
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PUBLIC "${EXTRA_INCLUDE}"
  PRIVATE "${EXEC_UI}")




 
###### Instalation rule ######
install(TARGETS ${EXEC_NAME} DESTINATION bin)
#install(TARGETS ${LIB_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)


# enable_testing()

# # define a function to simplify adding tests
# function(do_test target arg result)
#   add_test(NAME Comp${arg} COMMAND ${target} ${arg})
#   set_tests_properties(Comp${arg}
#     PROPERTIES PASS_REGULAR_EXPRESSION ${result}
#     )
# endfunction(do_test)

# # do a bunch of result based tests
# do_test(canopen 1 1000 0 "")
# do_test(canopen 2 607A 0 4 d 1000 "")
