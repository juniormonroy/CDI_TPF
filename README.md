## Se realiza la implementacion de un controlador PID y una interfaz grafica en tiempo real:

* El sistema se constituye de un controlador en EDUCIAA usando las librerias PID de cmsis arm
* Se cuenta con un pequeno programa en python para observar la respuesta en tiempo real del controlador setpoint y planta
* Se realiza calculo de ziegle-nichols usando los datos observados por el osciloscopio de la planta ante una respuesta escalon
* Se realiza simulacion en Python de la respuesta PID
* Se tomaron medida de los valores reales de los componentes de la planta para asi ser lo mas preciso posible, siendo R1=9.970Kohm, R2=974ohm C1=10.29uf C2=9.48uf
* 


La estructura basica del firmware de control PID se encuentra dentro de un RTOS con 4 tareas paralelas, donde la tarea a realiza el DAC, la tarea b realiza el calculo pid, la tarea c realiza el adc de planta, y la tarea d realiza las comunicaciones, los tiempos de ejecucion de la tarea a b y c son de 1 ms, y el de comunicacion se establecio a 50ms como el mas rapido soportado por las pruebas, probablemente limitado por la maxima velocidad de baudios disponible 115200.

A continuacion se muestra el diagrama general de control

![Diagrama PID](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/DiagramaFuncional.png)

![Diagrama PID](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/PlantaCircuitoRC.png)

![Diagrama PID](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/EDUCIAA_PLANTA.jpg)



Se realizaron mediciones de la planta con osciloscopio tomando nota del tiempo de respuesta al 10% 90% del spam maximo
![Osciloscopio 10_90](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/osc_0_100.jpg)


La consigna pedia realizar tren de pulso a 10hz osea obteniendo la siguiente respuesta en el osciloscopio
![Osciloscopio 0_100](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/respuesta_10hz.jpg)


Observamos la respuesta escalon en python
![Respuesta Escalon Python](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/respuesta%20escalon_python.png)




Se realizo una primera entonacion con L=100ms obteniendo respuesta oscilante
![Respuesta inestable](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/respuesta_oscilante.png)


Se realizo un segundo modelo con L=10ms obteniendo la respuesta escalon estable y controlando

Se realizaron los calculos de ziegle-nichols siendo los resultados iniciales de entonacion, kp ki kd los siguientes

* kp 0.033707865168539325
* ki 0.011235955056179775
* kd 0.0702247191011236

![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/respuesta_pid_python.png)

![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/Escalon_PID_python.png)

![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/Respuesta_sp50_osc_picos.jpg)

![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/Respuesta%20del%20controlador.jpg)






Se realizo un modelo ideal en python para comprobar contra el modelo y respuesta real via osciloscopio 
FALTA

Referencias Externas:

https://www.keil.com/pack/doc/CMSIS/DSP/html/group__PID.html