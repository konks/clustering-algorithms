# Алгоритмы кластеризации в машинном обучении
Данная программа является реализацией известных алгоритмов кластеризации точек на плоскости. Она написана на языке С++ с использованием методов объектно-ориентированного программирования. 

Основные принципы работы:
1. Программа считывает команды из файла instruction.txt построчно.
2. При наличии недоступной команды выводится ошибка в консоль.
3. Список доступных команд и их краткое описание можно найти в файле helpfile.txt.
4. Корректным результатом работы программы являются координаты точек кластеров, которые в последствии могут быть использованы для отрисовки в gnuplot.
5. При запуске программы классы controller и interface выводят в файл логи, примеры которых можно увидеть в файлах log_controller.txt и log_interface.txt.
6. Код программы содержится в файлах Main.cpp, Second.cpp, Second.h.
7. Для алгоритмов FOREL и IERA предусмотрена пошаговая распречатка для создания анимации. 
8. В презентации Тесты.pptx можно увидеть примеры работы программы с соответствующей инструкцией для запуска и необходимыми командами для статической отрисовки в gnuplot.

!!! В коде программы явно прописан путь к файлам для считывания и печати. При запуске на стороннем устройстве названия необходимо изменить.


Механико-математический факультет МГУ. 2 курс. 2021 \\
Автор: Кондаурова Ксения\\
Преподаватель: Кумсков М.И.
