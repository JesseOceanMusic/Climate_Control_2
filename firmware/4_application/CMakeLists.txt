set(APP_NAME climate_control_2)

set(APP_SOURCES
    src/${APP_NAME}.c
)

# Создаем основной исполняемый файл.
add_executable(${APP_NAME} ${APP_SOURCES})

# Добавляем публичные заголовки приложения.
target_include_directories(${APP_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Связываем приложение со всеми зависимостями.
# Важно: здесь мы перечисляем те библиотеки, которые непосредственно используются приложением.
# CMake сам разрешит транзитивные зависимости, если они правильно указаны с PUBLIC/PRIVATE.
# Однако, явное указание часто более понятно.
target_link_libraries(${APP_NAME} PRIVATE
    # Модули, которые используются в 4_application
    c_errors_writer
    c_buffer
    c_errors
    c_time_wrapper
)

# === Настройка интеграционных тестов ===
add_subdirectory(test)