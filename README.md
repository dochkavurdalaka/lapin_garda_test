# Calculator Server

Данный проект реализован в качестве решения тестового задания на конкурс в собеседование в компанию **"Гарда"**.

Проект реализует первые 10 пунктов технического задания и разработан на **C++**, с использованием дополнительных библиотек.

---

## Структура проекта

В корневой структуре проекта находятся следующие файлы и папки:

- **`Client.cpp`** — код для CLI-клиента **Calculator Server**.
- **`Server.cpp`** — код серверной части **Calculator Server**.
- **`CMakeLists.txt`** — CMake-скрипт для сборки проекта.
- **`.clang-format`** — файл с настройками кодестайла, которому соответствует проект.
- **`Dockerfile`** — файл конфигурации Docker для запуска проекта.
- **`include/`** — папка с подключаемыми библиотеками.
- **`Client`** — бинарный файл CLI-клиента. Можно запустить в докере.
- **`Server`** — бинарный файл серверной части. Можно запустить в докере.
---

## Используемые библиотеки

1. **`cpp-httplib`**  
   Библиотека для написания клиент-серверных приложений.  
   Ссылка: [cpp-httplib](https://github.com/yhirose/cpp-httplib)  
   Файл: `include/httplib.h`

2. **`nlohmann::json`**  
   Библиотека для парсинга и передачи JSON-данных.  
   Ссылка: [nlohmann/json](https://github.com/nlohmann/json)  
   Файл: `include/json.hpp`

3. **`check_correct.h`**  
   Библиотека, написанная мной, содержит функцию для проверки численного выражения на синтаксическую корректность.
   Файл: `include/check_correct.h`

4. **`eval.h` и `calculator.h`**  
   Библиотеки, разработанные мной в ходе прохождения курса по **C++** от **Школы Анализа Данных Яндекс** , у которой совместная магистерская программа с **ННГУ имени Лобачевского**, на которой я обучаюсь.  
   Эти библиотеки не используют обратную польскую запись, а преобразуют исходное выражение в **AST** ([Abstract Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree)) на основе математического аппарата формальной грамматики.  
   Упрощенный вид грамматики приведен ниже:

   ![Грамматика](https://github.com/user-attachments/assets/0d143d84-cb44-499f-84cd-0378147d503b)

---
## Запуск проекта
Данный проект собирался и компилировался в системе Ubuntu 24.04 с использованием компилятора gcc-12.
Эти условия выставлены в `Dockerfile`.

### Установка проекта через Docker:
1. Клонируйте репозиторий:
   ```bash
   git clone https://github.com/dochkavurdalaka/lapin_garda_test.git
   cd lapin_garda_test
2. Соберите образ докера и запустите контейнер:
   ```bash
   docker build -t lapin_garda_test .
   docker run -p 8080:8080 -it --name lapin_garda_test lapin_garda_test

Обратите внимание, что указаны порты 8080:8080 — их будет прослушивать сервер, и на них будет посылать сообщения клиент. Порты пробрасываются для возможности подключиться к серверу извне контейнера.

2. Перейдите в директорию с исходниками в контейнере:
   ```bash
   cd lapin_garda_test
   ```


Здесь вы можете собрать проект различными способами, можете в консоли собрать запустить cmake, можете подключиться к контейнеру через VSCode, открыть в нем папку с проектом, и IDE автоматически предоставит возможности по сборке проекта.
Так же присуствует возможность простого запуска бинарников:

   ```bash
   ./Server
   ./Client
   ```


---

## Использование программы
В этом разделе нужно описывать, как правильно взаимодействовать с программой.
После запуска сервера должно появиться следующее сообщение:

   ![image](https://github.com/user-attachments/assets/1f9ccd82-d592-4c6a-b9e6-faa9d6d53fbc)
)
1. Команда **`echo`**:
   ```bash
   ./Client -c "echo"
   ```

   ![image](https://github.com/user-attachments/assets/4f610f74-9ed1-480c-ba7d-d8ef5607fd9e)

2. Команда вычисления простого целочисленного выражения:
   ```bash
   ./Client -e "-5*6 + (-7)*2 + 1"
   ```
   ![image](https://github.com/user-attachments/assets/717db81c-4626-4d87-a106-f86c437f463a)

3. Команда вычисления выражения с плавающей запятой:
   ```bash
   ./Client -e "-5*6 + (-7)*2 + 1.0"
   ```
   ![image](https://github.com/user-attachments/assets/80974e6d-69ec-48c4-80d1-2fe352c5930d)

4. Попытка ввести некорректное выражение:
   ```bash
   ./Client -e "*6 + (-7)*2 + 1.0"
   ```
   ![image](https://github.com/user-attachments/assets/7425b7f5-8e56-4eef-aa04-207fef3f4b7e)
   
5. Попытка поделить на ноль:
   ```bash
   ./Client -e "-2 + 9.0 * 2/0"
   ```
   ![image](https://github.com/user-attachments/assets/aa7cabed-c4b3-4c4d-8b66-d700780a0c67)


6. Поддержка переменных:
   ```bash
   ./Client -e "pi = 3.14"
   ./Client -e "pi"
   ```
   ![image](https://github.com/user-attachments/assets/b00ed333-aed8-4999-9d1e-eff076d872b5)


7. Поддержка многострочного ввода:
   ```bash
   ./Client -e "s = 2.5;\
   s = s * s + 1;\
   s"
   ```
   ![image](https://github.com/user-attachments/assets/2db8dc04-87c9-4a1c-83b6-64dc84579ce3)

8. Поддержка API для сброса состояния:
      ```bash
   ./Client -e "s = 2.5 + 1"
   ./Client -e "s"
   ./Client -c clean
   ./Client -e "s"
   ```
   ![image](https://github.com/user-attachments/assets/e83aaceb-3af1-48c0-a3e0-459415fd1202)

---
## Подключение к серверу извне контейнера
Одним из заявленных в техническом задании требований была возможность подключения к серверу извне контейнера. Это было вполне успешно мной реализовано:

   ```bash
   /home/user/lapin_garda_test/build/Client -e "si = 2;\
   > 5 * 3;\
   > si * 2.5"
   ```
   ![image](https://github.com/user-attachments/assets/b4c0f17d-80e0-4de7-ac9e-825f8d74cffa)


   ![image](https://github.com/user-attachments/assets/cface994-c61b-4bb4-bbd7-8047882d75dd)

---

## Контакты

Если у вас возникнут вопросы, вы можете связаться со мной:

- **Telegram**: [@dochkavurdalaka](https://t.me/dochkavurdalaka)
- **Email**: [dmitriylapin2001@gmail.com](mailto:dmitriylapin2001@gmail.com)
