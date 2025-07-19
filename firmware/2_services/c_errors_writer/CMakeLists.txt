set(MODULE_NAME c_errors_writer)

set(MODULE_SOURCES
    src/${MODULE_NAME}.c
)

set(MODULE_HEADERS
  include/${MODULE_NAME}_test.h
  include/${MODULE_NAME}.h
)

# Создаем статическую библиотеку из исходников модуля
add_library(${MODULE_NAME} STATIC ${MODULE_SOURCES})

# Добавляем директории для инклудов этого модуля
# PUBLIC означает, что эти инклуды будут доступны тем, кто линкуется с MODULE_NAME
target_include_directories(${MODULE_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Зависимости с другими модулями
target_link_libraries(${MODULE_NAME} PUBLIC 
  c_errors
  c_buffer
)

# === Настройка модульных тестов===

if(BUILD_TESTING)
  set(TEST_SOURCES test/${MODULE_NAME}_test.c)

  # Создаем исполняемый файл для модульного теста
  add_executable(${MODULE_NAME}_test ${TEST_SOURCES})

  # Связываем тестовый исполняемый файл с самой библиотекой MODULE_NAME
  # И с другими зависимостями, если тест их использует
  target_link_libraries(${MODULE_NAME}_test PRIVATE ${MODULE_NAME})

  # Добавляем тест в CTest
    add_test(
      NAME ${MODULE_NAME}_unit_test
      COMMAND ${MODULE_NAME}_test
    )
endif()