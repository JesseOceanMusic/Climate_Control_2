# === НАЗВАНИЕ МОДУЛЯ === #
    set(MODULE_NAME c_errors)

# === НАСТРОЙКА МОДУЛЯ === #
    include(${CMAKE_SOURCE_DIR}/ConfigModule.cmake)
    config_module(${MODULE_NAME})

# === НАСТРОЙКА ЗАВИСИМОСТЕЙ === #


# === НАСТРОЙКА UNIT ТЕСТА === #
  include(${CMAKE_SOURCE_DIR}/ConfigUnitTest.cmake)
  config_unit_test(${MODULE_NAME})