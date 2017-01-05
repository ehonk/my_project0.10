cmake_minimum_required(VERSION 3.0)
project(my_Project_0_10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp GPIOHandle.cpp GPIOHandle.h LEDHandle.h LEDHandle.cpp SPI_MCP3008.cpp SPI_MCP3008.h SPI_MCP23S17.cpp SPI_MCP23S17.h I2C_Dummy.cpp I2C_Dummy.h i2c8Bit.cpp i2c8Bit.h)
add_executable(my_Project_0_10 ${SOURCE_FILES} SPI_MCP23S17.cpp SPI_MCP23S17.h I2C_Dummy.cpp I2C_Dummy.h i2c8Bit.cpp i2c8Bit.h)