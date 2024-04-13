# Fok_Gyem flipdot clock

Проект по воссозданию блока управления часов на основе перекидных (flip_dot, блинкер) модулей венгерской фирмы Fok_GYEM. Внешний вид часов до разборки:

![image](extras/images/old_clock.jpg)

Каждый из цифровых блоков выглядит так:

<img src="extras/images/digit1.jpg" alt="front side" width="300"/> <img src="extras/images/digit2.jpg" alt="rear side" width="300"/>

Размеры примерно 200х115мм, вес каждого блока около 1 кг.

Для формирования изображения используются точки-блинкеры. Каждый блинкер представляет из себя пластиковую шторку, управляемую электромагнитной катушкой. Одна сторона шторки черная, другая покрашена светоотражающей краской. При подаче напряжения на катушку шторка открывается или закрывается, в зависимости от полярности. После снятия напряжения шторка сохраняет свое положение месяцы и даже годы.
На каждом модуле 35 блинкеров, формирующих изображение 5х7 пикселей. Схема коммутации блинкеров показана ниже (для упрощения на рисунке только 4 катушки из 35).

<img src="extras/images/digit_module_schematic.png" alt="module schematic" />

Один контакт всех катушек на модуле обьединен на общий провод Jmain, что исключает применение матричной схемы управления. Реле K1 блокирует управление всеми блинкерами модуля, что позволяет управлять несколькими модулями последовательно по одному кабелю.

Чтобы не придумывать сложную мостовыую схему на каждую катушку для изменения полярности, я решил использовать переменный ток. Идея управления передается схемой ниже. 
