######################### General Settings ######################
cmake_minimum_required(VERSION 3.8.2)
######################## End Of General Settings ################

######################### Output Dir Settings ######################
##### An example output redirection setting

#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#    set (OUTPUT_DIR ../output/debug) # could be assigned by parrent CMakeLists.txt
#else()
#    set (OUTPUT_DIR ../output/release) # could be assigned by parrent CMakeLists.txt
#endif()

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

######################### End of Output Dir Settings ######################


################## Qt AUTOMOC Settings ##########################
#!!!!Caution: Should Be defined at top of CMakeLists.txt 
#QT Library Definition/Inclusion 
#First install QT and set bin path to PATH list
#Be shure insalled sudo apt-get install mesa-common-dev
#The build and install cmake from source ./bootstrap --qt-gui, make, make install
#QT_DIR  - root directory (bin,libs,include) where qt is installed. 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_DIR}})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
################# End of Qt AUTOMOC SETTINGS #######################

############################ Librarry Project Settings #################### 
# Main Project Definition LDAF Source Code Inclusion
project(ldaf)
set (SOURCES  ${SOURCES} 
    ldafmain.cpp 
    ldafbase.cpp 
    ldafbrowser.cpp 
    ldafmediator.cpp)

set (RESOURCES ldaf.qrc)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${RESOURCES})
############################ End of Main Executable Project Settings ############# 

######################### Dependency And Link Settings ##########################
# Qt Link
find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Qml Qt5::Quick)
######################### End of Dependency And Link Settings ##########################

