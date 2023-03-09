cmake_minimum_required(VERSION 3.15)
project(supagrep VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# find_package(Qt6 REQUIRED COMPONENTS SvgWidgets)
qt_standard_project_setup()

add_compile_definitions( VERSION="${PROJECT_VERSION}" )

add_library(cpputilsio STATIC 
	cpputils/io/ColoredOutput.h
	cpputils/io/ColoredOutput.cc
	cpputils/io/debug.cc
	cpputils/io/debug.h
	cpputils/io/DetectLocale.h
	cpputils/io/DetectLocale.cc
	cpputils/io/OutDebug.h
	cpputils/io/OutDebug.cc
	cpputils/io/read_file.h
	cpputils/io/read_file.cc 
)

target_include_directories(cpputilsio PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/cpputilsshared/cpputilsformat/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/cpputilsshared/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/io/>
)
	
add_library(cpputilsformat STATIC 
	cpputils/cpputilsshared/cpputilsformat/format.h
	cpputils/cpputilsshared/cpputilsformat/format1.cc
	cpputils/cpputilsshared/cpputilsformat/format2.cc
	cpputils/cpputilsshared/cpputilsformat/format2.h
	cpputils/cpputilsshared/cpputilsformat/format1.h
	cpputils/cpputilsshared/cpputilsformat/cformat.h
	cpputils/cpputilsshared/cpputilsformat/wformat2.cc
	cpputils/cpputilsshared/cpputilsformat/wformat2.h
	cpputils/cpputilsshared/cpputilsformat/cwformat.h
	cpputils/cpputilsshared/cpputilsformat/cwformat.cc 
)	

target_include_directories(cpputilsformat PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/cpputilsshared/cpputilsformat/>
)
	
add_library(cpputilsshared STATIC	
		cpputils/cpputilsshared/arg.h
		cpputils/cpputilsshared/arg.cc
		cpputils/cpputilsshared/backtrace.h
		cpputils/cpputilsshared/backtrace.cc
		cpputils/cpputilsshared/cppdir.h
		cpputils/cpputilsshared/cppdir.cc
		cpputils/cpputilsshared/file_option.h
		cpputils/cpputilsshared/file_option.cc
		cpputils/cpputilsshared/iterator.h
		cpputils/cpputilsshared/leoini.h
		cpputils/cpputilsshared/leoini.cc
		cpputils/cpputilsshared/map_utils.h
		cpputils/cpputilsshared/pairs.h
		cpputils/cpputilsshared/pairs.cc
		cpputils/cpputilsshared/range.h
		cpputils/cpputilsshared/ref.h
		cpputils/cpputilsshared/stderr_exception.h
		cpputils/cpputilsshared/string_utils.h
		cpputils/cpputilsshared/string_utils.cc
		cpputils/cpputilsshared/utf8.h
		cpputils/cpputilsshared/utf8/core.h
		cpputils/cpputilsshared/utf8/checked.h
		cpputils/cpputilsshared/utf8/unchecked.h
		cpputils/cpputilsshared/utf8/cpp11.h
		cpputils/cpputilsshared/utf8/cpp17.h
		cpputils/cpputilsshared/utf8_util.h
		cpputils/cpputilsshared/utf8_util.cc
		cpputils/cpputilsshared/xml.h
		cpputils/cpputilsshared/xml.cc
)
		
target_include_directories(cpputilsshared PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/cpputilsshared/cpputilsformat/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/cpputilsshared/>
)		

add_library(common STATIC
	common/lang.h
	common/lang.cc 
	common/setup.h
	common/setup.cc
	common/search.h
	common/search.cc
)

target_include_directories(common PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/cpputilsshared/cpputilsformat/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/cpputilsshared/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/io/>
)
		
add_executable(supagrep
	srcqt/mainqt.h
	srcqt/mainqt.cc
	srcqt/searchwinqt.h
	srcqt/searchwinqt.cc 
	srcqt/separatorqt.h
	srcqt/separatorqt.cc
	srcqt/desccomboqt.h
	srcqt/desccomboqt.cc
)

target_include_directories(supagrep PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/cpputilsshared/cpputilsformat/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/cpputilsshared/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpputils/io/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
)

target_link_libraries(supagrep PRIVATE Qt6::Core)
target_link_libraries(supagrep PRIVATE Qt6::Widgets)
# target_link_libraries(supagrep PRIVATE Qt6::SvgWidgets)

target_link_libraries( supagrep PRIVATE common )
target_link_libraries( supagrep PRIVATE cpputilsio )
target_link_libraries( supagrep PRIVATE cpputilsshared )
target_link_libraries( supagrep PRIVATE cpputilsformat )

if (WIN32)
target_link_libraries( supagrep PRIVATE iconv )
set(CMAKE_EXE_LINKER_FLAGS  "-mwindows")
endif (WIN32)


