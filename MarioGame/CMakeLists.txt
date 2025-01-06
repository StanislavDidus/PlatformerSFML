# Мінімальна версія CMake
cmake_minimum_required(VERSION 3.15)

# Назва проекту
project(MarioGame)

# Вказуємо стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Додаємо основний код проекту
add_executable(MarioGame 
    src/main.cpp
    src/Game.cpp
)

# Додаємо директорію з заголовками
target_include_directories(MarioGame PRIVATE ${CMAKE_SOURCE_DIR})

# Якщо використовуємо бібліотеку zlib:
add_subdirectory(External/ZLIB)
target_link_libraries(MarioGame PRIVATE ZLIB)