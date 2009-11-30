PROJECT(upgalilei)
INCLUDE_DIRECTORIES(.)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(upgalilei main.cpp command.cpp rungalileiprogram.cpp)
TARGET_LINK_LIBRARIES(upgalilei galilei roptimization rcore rmath)
