set(MODULE_NAME c_buffer)

set(MODULE_SOURCES
    src/${MODULE_NAME}.c
)

set(MODULE_HEADERS
  include/${MODULE_NAME}.h
  include/${MODULE_NAME}_list.def
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
  )


  
# ================================== #
# === НАСТРОЙКА МОДУЛЬНЫХ ТЕСТОВ === #
# ================================== #

if(BUILD_TESTING)
    set(TEST_SOURCES test/${MODULE_NAME}_test.c)

    # Создаем исполняемый файл для модульного теста
    add_executable(${MODULE_NAME}_test ${TEST_SOURCES})

    # Флаги компиляции для покрытия: применяем к исходникам модуля и тестов.
    # Это гарантирует, что .gcno файлы будут созданы для всех необходимых .o файлов.
    # Используем target_compile_options, чтобы применить их только к соответствующим целям.
    # --coverage это сокращение для -fprofile-arcs -ftest-coverage
    target_compile_options(${MODULE_NAME} PRIVATE --coverage)
    #target_compile_options(${MODULE_NAME}_test PRIVATE --coverage)

    # Связываем тестовый исполняемый файл с самой библиотекой MODULE_NAME
    # И с другими зависимостями, если тест их использует
    target_link_libraries(${MODULE_NAME}_test PRIVATE ${MODULE_NAME})

    # Флаги линковки для покрытия: это критически важно для создания .gcda файлов
    # --coverage также включает -lgcov
    target_link_options(${MODULE_NAME}_test PRIVATE --coverage)

    # Добавляем тест в CTest
    add_test(
        NAME ${MODULE_NAME}_unit_test
        COMMAND ${MODULE_NAME}_test
    )
endif()