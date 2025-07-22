function(config_module MODULE_NAME)
  set(MODULE_SOURCES
      src/${MODULE_NAME}.c
  )

  # подключаем заголовки из папки /include
  file(GLOB MODULE_HEADERS 
      "include/*.h"
      "include/*.def"
  )

  # Создаем статическую библиотеку из исходников модуля
  add_library(${MODULE_NAME} STATIC ${MODULE_SOURCES})

  # Добавляем директории для инклудов этого модуля
  # PUBLIC означает, что эти инклуды будут доступны тем, кто линкуется с MODULE_NAME
  target_include_directories(${MODULE_NAME} PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )
endfunction()