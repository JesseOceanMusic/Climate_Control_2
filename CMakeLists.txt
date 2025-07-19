# Минимальная требуемая версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(climate_control_2 C) # Указываем язык(и) проекта. Пока только C.

# Включаем CTest для поддержки тестирования.
if(BUILD_TESTING)
  include(CTest)
  enable_testing()
  add_compile_definitions(ENABLE_UNIT_TESTING_API)
endif()


# Устанавливаем выходные директории для исполняемых файлов и библиотек.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")

# CMAKE_C_FLAGS для C компилятора
# CMAKE_CXX_FLAGS для C++ компилятора (добавим на будущее)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -fdiagnostics-color=always -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fdiagnostics-color=always -g")

# Добавляем корневую директорию для исходников firmware.
set(FIRMWARE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/firmware")

# Включаем поддиректории для каждого уровня.
# Порядок здесь важен из-за правил зависимостей (снизу вверх)
add_subdirectory(${FIRMWARE_SOURCE_DIR}/0_core)
add_subdirectory(${FIRMWARE_SOURCE_DIR}/1_libs)
add_subdirectory(${FIRMWARE_SOURCE_DIR}/2_services)
# add_subdirectory(${FIRMWARE_SOURCE_DIR}/3_managers) # Добавим, когда будут модули
add_subdirectory(${FIRMWARE_SOURCE_DIR}/4_application)