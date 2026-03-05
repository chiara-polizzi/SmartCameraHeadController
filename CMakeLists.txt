cmake_minimum_required(VERSION 3.16)

# Nome del nostro progetto C++
project(SmartCameraHeadController VERSION 1.0 LANGUAGES CXX)

# Diciamo di usare lo standard C++17 (necessario per Qt6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Queste sono le 3 righe magiche che attivano i superpoteri di Qt (Signals/Slots)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Chiediamo a CMake di trovare la libreria Qt6 che abbiamo appena installato
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Elenchiamo i nostri file sorgente
set(SOURCES
    src/main.cpp
    src/encoder_reader.cpp
    src/motor_driver.cpp
    src/pathfinder.cpp
)

# Creiamo l'eseguibile finale
add_executable(${PROJECT_NAME} ${SOURCES})

# Diciamo a CMake dove trovare i nostri file .h
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Colleghiamo le librerie grafiche di Qt al nostro programma
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# Copia la cartella data nella directory di build per renderla accessibile all'eseguibile
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})