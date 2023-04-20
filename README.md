## Se realiza la implementacion de un controlador PID y una interfaz grafica en tiempo real:

* El sistema se constituye de un controlador en EDUCIAA usando las librerias PID de cmsis arm
* Se cuenta con un pequeno programa en python para observar la respuesta en tiempo real del controlador setpoint y planta
* Se realiza calculo de ziegle-nichols usando los datos observados por el osciloscopio de la planta ante una respuesta escalon
* Se realiza simulacion en Python de la respuesta PID
* 
* 


La estructura basica del firmware de control PID se encuentra dentro de un RTOS con 4 tareas paralelas, donde la tarea a realiza el DAC, la tarea b realiza el calculo pid, la tarea c realiza el adc de planta, y la tarea d realiza las comunicaciones, los tiempos de ejecucion de la tarea a b y c son de 1 ms, y el de comunicacion se establecio a 50ms como el mas rapido soportado por las pruebas, probablemente limitado por la maxima velocidad de baudios disponible 115200.

A continuacion se muestra el diagrama general de control


![Diagrama PID](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/DiagramaFuncional.png)



Se realizaron mediciones de la planta con osciloscopio tomando nota del tiempo de respuesta al 10% 90% del spam maximo

![Osciloscopio 10_90](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/osc_10_90.jpg)

Se realizaron los calculos de ziegle-nichols siendo los resultados iniciales de entonacion, kp ki kd los siguientes

* kp
* ki
* kd




Se realizo un modelo ideal en python para comprobar contra el modelo y respuesta real via osciloscopio 





Referencias Externas:

https://www.keil.com/pack/doc/CMSIS/DSP/html/group__PID.html