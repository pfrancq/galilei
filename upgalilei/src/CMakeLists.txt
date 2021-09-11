PROJECT(upgalilei)

INCLUDE_DIRECTORIES(.)

SET(upgalilei_SOURCES
	main.cpp
	rungalileiprogram.cpp
	cgithread.cpp
	html.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(upgalilei ${upgalilei_SOURCES})

TARGET_LINK_LIBRARIES(upgalilei galilei roptimization rcore rmath rprg fcgi)
