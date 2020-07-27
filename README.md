# ObjectDetector

Приложение для детектирования объектов. Программе на вход подается случайное изобьражение. 
На нем при помощи нейросети детектируются объекты. 
Затем, это же изображение выводится на экран, и пользователю предлагается выделить один из случайно выбранных задетектированных объектов.
Пользователь выделяет объект прямоугольником, и если он это делает правильно (выделенный задетектированный прямоугольник практически совпадают), то пользователю засчитывается очко.
Так происходит до тех пор, пока пользователь не наберет определенное количество очков или не ошибется.

Для детектирования объектов используется модель `retinanet-tf`, которую можно найти в репозитории [open_model_zoo](https://github.com/openvinotoolkit/open_model_zoo)

## Пример работы программы

![img1](/data/Plane1.png)

![img2](/data/Plane2.png)

## Возможные улучшения

* Уровни сложности - в зависимотси от уровня сложности изменяется taget score и порог Intersection over union, по которому определяется, правильно ли пользователь выделил объект
* Отображать bounding box для выбранного объекта, чтобы пользователь видел, насколько он правильно выделил объект или ошибся
* Улучшение интерфейса - добавление меню, окна, где отображаются все показатели и т.д.
* Поддержка видеороликов
