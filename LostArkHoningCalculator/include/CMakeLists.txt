#Project-header
cmake_minimum_required (VERSION 3.10.0)
set (HeaderFiles ${HeaderFiles}
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/ChaosExchange.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/Config.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/ConfigStructs.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/GearScoreSelection.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/HoningCalculation.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/HoningChain.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/HoningInput.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/HoningParameter.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/HoningResult.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/utils/Functions.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/utils/Http.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/utils/StringManipulation.h
	${CMAKE_CURRENT_SOURCE_DIR}/LostArkHoningCalculator/Utils.h
	PARENT_SCOPE
)