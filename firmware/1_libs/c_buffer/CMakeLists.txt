# === НАЗВАНИЕ МОДУЛЯ === #
    set(MODULE_NAME c_buffer)

# === НАСТРОЙКА МОДУЛЯ === #
    include(${CMAKE_SOURCE_DIR}/ConfigModule.cmake)
    config_module(${MODULE_NAME})

# === НАСТРОЙКА ЗАВИСИМОСТЕЙ === #
  target_link_libraries(${MODULE_NAME} PUBLIC 
    c_errors
  )

# === НАСТРОЙКА UNIT ТЕСТА === #
  include(${CMAKE_SOURCE_DIR}/ConfigUnitTest.cmake)
  config_unit_test(${MODULE_NAME})