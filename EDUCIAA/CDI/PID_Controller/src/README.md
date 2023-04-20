## Se realiza la implementacion de un controlador PID y una interfaz grafica en tiempo real:

* El sistema se constituye de un controlador en EDUCIAA usando las librerias PID de cmsis arm
* Se cuenta con un pequeno programa en python para observar la respuesta en tiempo real del controlador setpoint y planta
* Se realiza calculo de ziegle-nichols usando los datos observados por el osciloscopio de la planta ante una respuesta escalon
* Se realiza simulacion en Python de la respuesta PID
* 
* 


La estructura basica del firmware de control PID se encuentra dentro de un RTOS con 4 tareas paralelas, donde la tarea a realiza el DAC, la tarea b realiza el calculo pid, la tarea c realiza el adc de planta, y la tarea d realiza las comunicaciones, los tiempos de ejecucion de la tarea a b y c son de 1 ms, y el de comunicacion se establecio a 50ms como el mas rapido soportado por las pruebas, probablemente limitado por la maxima velocidad de baudios disponible 115200.

Se realizaron los calculos de ziegle-nichols siendo los resultados iniciales de entonacion, kp ki kd los siguientes

Se realizo un modelo ideal en python para comprobar contra el modelo y respuesta real via osciloscopio 


![Diagrama ISO](https://user-images.githubusercontent.com/87079881/187349380-d29026ac-fa5c-4450-a85b-d4d870658a51.png)



Referencias Externas:

https://www.keil.com/pack/doc/CMSIS/DSP/html/group__PID.html