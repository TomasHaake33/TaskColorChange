# TaskColorChange

Класс, который считывает изображение и меняет все его зеленые пиксели на красные  
Под зеленым понимается диапазон в HSV от ~(30, 30, 30) до ~(80, 255, 255)  
Изменение на красный означает замену компоненты H на число 0  
Например, (70, 200, 200) -> (0, 200, 200)


На вход принимает папку с .jpg изображениями. Также, можно указать путь к папке с выходами  
Делает это при помощи парсинга командной строки

![Image alt](https://github.com/TomasHaake33/TaskColorChange/raw/main/example/test.png)
![Image alt](https://github.com/TomasHaake33/TaskColorChange/raw/main/example/out1.png)
