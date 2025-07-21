cmake_minimum_required(VERSION 3.10)
project(climate_control_2 C) # Указываем язык(и) проекта. Пока только C.
add_compile_options(-fdiagnostics-color=never) # !!! Без этого парсер ошибок не работает !!!

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -g")

if(BUILD_TESTING)
  include(CTest)
  enable_testing()
  add_compile_definitions(ENABLE_UNIT_TESTING_API)
endif()

# Кастомная команды, которая вызываются (прописано в settings.json) при запуске Run Tests With Coverage из Расширения Testing UI
  add_custom_target(CustomPreGenerateCoverageInfo
    COMMAND rm -f ${CMAKE_BINARY_DIR}/coverage.info # Удаляем предыдущий отчет, если он есть (-rm remove, -f force (означает, что rm не будет запрашивать подтверждение и не выдаст ошибку, если файл не существует. Это делает команду безопасной для использования в скриптах.))
    COMMAND lcov -z --directory ${CMAKE_BINARY_DIR} # Обнуляем счетчики (удаляет .gcda файлы и этого похоже хватает)
  )

  add_custom_target(CustomPostGenerateCoverageInfo
    COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} --output-file ${CMAKE_BINARY_DIR}/coverage.info --rc lcov_branch_coverage=1
  )

# Добавляем корневую директорию для исходников firmware.
  set(FIRMWARE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/firmware")

# Порядок здесь важен из-за правил зависимостей (снизу вверх)
  add_subdirectory(${FIRMWARE_SOURCE_DIR}/0_core)
  add_subdirectory(${FIRMWARE_SOURCE_DIR}/1_libs)
  add_subdirectory(${FIRMWARE_SOURCE_DIR}/2_services)
  # add_subdirectory(${FIRMWARE_SOURCE_DIR}/3_managers) # Добавим, когда будут модули
  add_subdirectory(${FIRMWARE_SOURCE_DIR}/4_application)