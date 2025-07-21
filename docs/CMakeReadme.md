# Continuous (Непрерывные таргеты)**
  **Используются в CI/CD для автоматического мониторинга изменений в репозитории и запуска сборки/тестов**
    - Continuous	Запускает полный цикл: Configure → Build → Test → Coverage → Submit (если настроено)
    - ContinuousBuild	Только сборка (Build)
    - ContinuousConfigure	Только конфигурация (Configure)
    - ContinuousCoverage	Запуск тестов с замером покрытия кода
    - ContinuousMemCheck	Проверка на утечки памяти (через Valgrind или аналоги)
    - ContinuousStart	Начинает мониторинг изменений (например, через git pull)
    - ContinuousSubmit	Отправляет результаты на сервер (например, CDash)
    - ContinuousTest	Только запуск тестов (Test)
    - ContinuousUpdate	Обновляет исходники (например, делает git pull)

# Experimental (Экспериментальные таргеты)
  **Аналогичны Continuous, но запускаются вручную (не автоматически)**
  *Пример: ExperimentalTest — запуск тестов без ожидания изменений в репозитории*
# Nightly (Ночные таргеты)
  **Запускаются по расписанию (например, каждую ночь). Используются для долгих тестов (stress-тесты, полное покрытие и т. д.).**


# === Для большинства кейсов можно выставить в левой менюшке (CMake Tools) ===
  Configure: Debug
  Buid (target): ExperimentalBuild

  **Debug корректно запускается ТОЛЬКО из CMakeTools!**
  **Run Test (Testing UI) и Test (CMake Tools) работают оба корректно!**





  lcov --capture --directory . --output-file coverage.info