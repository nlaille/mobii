cmake_minimum_required(VERSION 2.6)
		
#Declaration du projet
project(Mobii_test)

#Declaration des paths
set(EXECUTABLE_OUTPUT_PATH bin/)
add_definitions(-DMOBII_TEST_MACRO)

#Boost asio
if (WIN32)
	add_definitions(-D_WIN32_WINNT=0x0501)
	add_definitions(-DNODEFAULTLIB:library)
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

#gmock
include_directories(${GMOCK_INCLUDE_DIR})

#gtest
include_directories(${GTEST_INCLUDE_DIR})

#include dans les src
include_directories(../src)

enable_testing()

#Declaration de la liste des fichiers sources
file(
	GLOB_RECURSE
	source_files
	../src/*.cpp
	tests/*.cpp
)

#Declaration de la liste des fichiers sources
file(
	GLOB_RECURSE
	header_files
	../src/*.h
	../src/*.inl
	tests/*.h
	tests/*.inl
)

#Déclaration de l'exécutable
add_executable(
	mobii_test
	${source_files}
	${header_files}
)

#Configuration de l'etion de liens
target_link_libraries(
	mobii_test
	${OPENSSL_LIBRARIES}
	${Boost_LIBRARIES}
	${SOCI_LIBRARY}
	${CMAKE_THREAD_LIBS_INIT}
	${GTEST_LIBRARY}
	${GTEST_MAIN_LIBRARY}
	${GMOCK_LIBRARY}
	${GMOCK_MAIN_LIBRARY}
)

add_test(test mobii_test)
