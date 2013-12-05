cmake_minimum_required(VERSION 2.6)
		
#Declaration du projet
project(ManageMyMobile)

#Declaration des paths
set(EXECUTABLE_OUTPUT_PATH bin/)

#Boost asio
if (WIN32)
	add_definitions(-D_WIN32_WINNT=0x0501)
endif ()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREAD ON)

#Boost
find_package(
	Boost
	1.52.0
	COMPONENTS thread system filesystem random
)
	
link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

#Thread
find_package(Threads)

#SOCI
find_package(
	Soci
	3.2.0)
	
link_directories(${SOCI_LIBRARY_DIR})
include_directories(${SOCI_INCLUDE_DIRS})

#OpenSSL
find_package(OpenSSL)
link_directories(${OPENSSL_LIBRARY_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})

#include dans les src
include_directories(${CMAKE_SOURCE_DIR}/src)

#Declaration de la liste des fichiers sources
file(
	GLOB_RECURSE
	source_files
	src/*.cpp
)

#Declaration de la liste des fichiers sources
file(
	GLOB_RECURSE
	header_files
	src/*.h
	src/*.inl
)

#Déclaration de l'exécutable
add_executable(
	manage_server
	${source_files}
	${header_files}
)

#Configuration de l'etion de liens
target_link_libraries(
	manage_server
	${OPENSSL_LIBRARIES}
	${Boost_LIBRARIES}
	${SOCI_LIBRARY}
	${CMAKE_THREAD_LIBS_INIT}
)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
