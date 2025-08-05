# ПРАВИЛА ФОРМАТИРОВАНИЯ
	## Общее
		табуляция   - 2  символа (VS code -> settings -> Folding Strategy -> indenation (включает сворачивание по отступпам) )
		комментарии - 80 символ

	## Названия 

	### Пространства имён (snake_case)
	- `namespace network_utils { ... }`																							

	### Классы, структуры (PascalCase)
	- `class MyClass {};`
	- `struct UserData {};`  
	
	### Перечисления (enum: PascalCase, значения — SCREAMING_SNAKE_CASE)
	- `enum class Color { RED, GREEN, BLUE };`  

	### Приватные члены класса и static(_ в конце) 
	- `int count_;`  
	- `static int total_сount_;`

	### Объекты (camelCase)
	- `UserData userDataOne;`

	### Глобальные переменные (g_)
	- `int g_totalConnections = 0;`

	### Константы и макросы (SCREAMING_SNAKE_CASE)
	- `const int MAX_BUFFER_SIZE = 1024;`
	- `#define MIN(a, b) ((a) < (b) ? (a) : (b))`

	### Переменные, функции, методы (snake_case)
	- `float average_temp;`
	- `void calculate_sum(); ` 

	### Шаблонные параметры (T, U или PascalCase)
	- `template <typename T, typename ValueType>`  

	### Указатели/ссылки (суффикс _ptr _ref)**: 
	- `int* value_ptr;`  
	- `std::string& name_ref;`
				
	### Массивы (суффикс _arr)**: 
	- `char buffer_arr[256];`

	### Исключения ( PascalCase + EXCEPTION/ERROR )
	- `class InvalidInputERROR {};`  