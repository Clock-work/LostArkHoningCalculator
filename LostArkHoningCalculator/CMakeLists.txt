#ChatClient-root cmake file 
cmake_minimum_required (VERSION 3.10.0)
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0054 NEW)
project (LostArkHoningCalculator)#project definition 

#for every project
message("${PROJECT_NAME}: you can also set CMAKE_BUILD_TYPE to debug, or release, or use multi-config generators")
message("${PROJECT_NAME}: the project will have the following preprocessor definitions depending on the build: ${PROJECT_NAME}_DEBUG, ${PROJECT_NAME}_RELEASE, ${PROJECT_NAME}_STATIC, ${PROJECT_NAME}_DYNAMIC, ${PROJECT_NAME}_64, ${PROJECT_NAME}_32")
math(EXPR BitCount "8*${CMAKE_SIZEOF_VOID_P}")
option(Link_Static "Link static to static libs and runtime, or dynamically" ON)#option that can be set to determine static, or dynamic linking
if(Link_Static)
set(StaticLinking TRUE)
else()
set(StaticLinking FALSE)
endif()
message("${PROJECT_NAME}: for some special libraries you also have to set the Is_Debug variable to On/Off if you're using multi config generators. Otherwise setting the CMAKE_BUILD_TYPE to Debug is also ok")
option(Is_Debug "Debug mode, or not" OFF)#option that can be set to determine static, or dynamic linking. Dont ste 
if(Is_Debug OR CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "debug")
set(DebugMode TRUE)#DebugMode can be checked to see if current build is debug, or not | not 100% safe
else()
set(DebugMode FALSE)
endif()

set(TargetName ${PROJECT_NAME})
set(DllPath ${CMAKE_SOURCE_DIR}/dlls)
set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
if(StaticLinking)
  set(TargetName ${PROJECT_NAME}-static)
  foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  string(REPLACE "/MDd" "/MTd" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach()
else()
  set(TargetName ${PROJECT_NAME}-shared)
  foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MT" "/MD" ${CompilerFlag} "${${CompilerFlag}}")
  string(REPLACE "/MTd" "/MDd" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach()
endif()
if(WIN32)
  set(DllPath ${DllPath}/Windows)
else()
  set(DllPath ${DllPath}/Unix)
endif()
set(TargetName ${TargetName}-${BitCount})
set(DllPath ${DllPath}-${BitCount})

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)#compiler output to find header files for autocomplete in #include
get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  $<1:${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin>)

add_subdirectory (include)#recursive adding headers
add_subdirectory (src) #recursive adding sources
message("${PROJECT_NAME}: target configuration: "${TargetName})
#for every project

#executable specific
add_executable(${PROJECT_NAME} ${SourceFiles} ${HeaderFiles}) #link sources and headers to executeable file
#executable specific

#for every project
target_compile_definitions(${PROJECT_NAME} PUBLIC "_CRT_SECURE_NO_WARNINGS")
if(StaticLinking)
target_compile_definitions(${PROJECT_NAME} PUBLIC "${PROJECT_NAME}_STATIC")
else()
target_compile_definitions(${PROJECT_NAME} PUBLIC "${PROJECT_NAME}_DYNAMIC")
endif()
target_compile_definitions(${PROJECT_NAME} PUBLIC "${PROJECT_NAME}_${BitCount}")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:${PROJECT_NAME}_DEBUG>")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<NOT:$<CONFIG:Debug>>:${PROJECT_NAME}_RELEASE>")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME_DEBUG ${TargetName}-debug)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME_RELEASE ${TargetName}-release)
set(InstallPath ${PROJECT_NAME})#installpath variable
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${InstallPath}/include>)#include directory added for own and other projects
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${CMAKE_DL_LIBS})#important fix for the undefined reference to dlopen in cmake projects 
#for every project

#helper tasks MODIFIED TO ONLY COPY CONFIG FILE!!!!!!!!!!!!!!!!!!
add_custom_target(copy_res_dir_${PROJECT_NAME} ALL COMMAND cmake -E copy ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/LostArkCalculatorConfig.txt ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin/LostArkCalculatorConfig.txt)#copy config file
add_dependencies(${PROJECT_NAME} copy_res_dir_${PROJECT_NAME})#you can also remove the depends and call it manually if the res folder gets too big for performance

if(StaticLinking)
else()
add_custom_target(copy_dlls_${TargetName} ALL COMMAND cmake -E copy_directory ${DllPath} ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin)#copy dlls in the dll folder next to the binary. you have to put both your debug and release dlls in the root folder/dlls/configuration 
add_dependencies(${PROJECT_NAME} copy_dlls_${TargetName})#you can also remove the depends and call it manually if the res folder gets too big for performance
endif()
#helper tasks

#libraries:

#start zlib 
set(ZLIB_ROOT "${CMAKE_INSTALL_PREFIX}/zlib")
target_include_directories(${PROJECT_NAME} PUBLIC ${ZLIB_ROOT}/include)
if(UNIX)
if(StaticLinking)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC debug ${ZLIB_ROOT}/lib/zlibstaticd.a)#automatic find_package(zlib) not working, so this is the workaround
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC optimized ${ZLIB_ROOT}/lib/zlibstatic.a)
else()
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC debug ${ZLIB_ROOT}/lib/zlibd.so)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC optimized ${ZLIB_ROOT}/lib/zlib.so)
endif()
else()
if(StaticLinking)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC debug ${ZLIB_ROOT}/lib/zlibstaticd.lib)#automatic find_package(zlib) not working, so this is the workaround
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC optimized ${ZLIB_ROOT}/lib/zlibstatic.lib)
else()
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC debug ${ZLIB_ROOT}/lib/zlibd.lib)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC optimized ${ZLIB_ROOT}/lib/zlib.lib)
endif()
endif()
#end zlib

#Start curl
find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CURL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIRS})
#End curl