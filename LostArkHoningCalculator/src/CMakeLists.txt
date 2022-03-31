#Project-source
cmake_minimum_required (VERSION 3.10.0)
set (SourceFiles ${SourceFiles}
	${CMAKE_CURRENT_SOURCE_DIR}/Config.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/HoningCalculation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/utils/Functions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/utils/Http.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/utils/StringManipulation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp
	PARENT_SCOPE
)