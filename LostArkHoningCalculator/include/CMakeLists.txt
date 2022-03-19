#Project-header
cmake_minimum_required (VERSION 3.10.0)
set (HeaderFiles ${HeaderFiles}
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/Config.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/ConfigStructs.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/HoningChain.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/HoningInput.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/HoningParameter.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/HoningResult.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/LostArkHoning.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/Utils.h
	PARENT_SCOPE
)