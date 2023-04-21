## Se realiza la implementacion de un controlador PID y una interfaz grafica en tiempo real:

* El sistema se constituye de un controlador en EDUCIAA usando las librerias PID de cmsis arm
* Se cuenta con un pequeno programa en python para observar la respuesta en tiempo real del controlador setpoint y planta
* Se realiza calculo de ziegle-nichols usando los datos observados por el osciloscopio de la planta ante una respuesta escalon
* Se realiza simulacion en Python de la respuesta PID
* Se tomaron medida de los valores reales de los componentes de la planta para asi ser lo mas preciso posible, siendo R1=9.970Kohm, R2=974ohm C1=10.29uf C2=9.48uf
* 


La estructura basica del firmware de control PID se encuentra dentro de un RTOS con 4 tareas paralelas, donde la tarea a realiza el DAC, la tarea b realiza el calculo pid, la tarea c realiza el adc de planta, y la tarea d realiza las comunicaciones, los tiempos de ejecucion de la tarea a b y c son de 1 ms, y el de comunicacion se establecio a 50ms como el mas rapido soportado por las pruebas, probablemente limitado por la maxima velocidad de baudios disponible 115200.

* A continuacion se muestra el diagrama general de control

![Diagrama PID](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/DiagramaFuncional.png)

![Diagrama PID](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/PlantaCircuitoRC.png)

![Diagrama PID](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/EDUCIAA_PLANTA.jpg)



* Se realizaron mediciones de la planta con osciloscopio tomando nota del tiempo de respuesta al 10% 90% del spam maximo
![Osciloscopio 10_90](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/osc_0_100.jpg)


* La consigna pedia realizar tren de pulso a 10hz osea obteniendo la siguiente respuesta en el osciloscopio
![Osciloscopio 0_100](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/respuesta_10hz.jpg)


* Observamos la respuesta escalon en python
![Respuesta Escalon Python](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/respuesta%20escalon_python.png)




* Se realizo una primera entonacion con L=100ms obteniendo respuesta oscilante
![Respuesta inestable](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/respuesta_oscilante.png)


Se realizo un segundo modelo con L=10ms obteniendo la respuesta escalon estable y controlando

Se realizaron los calculos de ziegle-nichols siendo los resultados iniciales de entonacion, kp ki kd los siguientes

* kp 0.033707865168539325
* ki 0.011235955056179775
* kd 0.0702247191011236

* Respuesta grafica en python:
![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/respuesta_pid_python.png)

![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/Escalon_PID_python.png)

* Respuesta grafica en el osciloscopio calculo picos maximos iniciales
![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/Respuesta_sp50_osc_picos.jpg)
Setpoint 1.9v
Pico Maximo pid_out 2.8v
Pico Maximo Planta 2.34
spam maximo 3.3v

* Respuesta grafica en el osciloscopio calculo tiempo hasta el punto de control setpoint
![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/Respuesta_sp50_osc.jpg)
Tiempo T0 = 0.250S
Tiempo T1 = 2.280S
Tiempo de respuesta del controlador para llegar al setpoint 2.03 segundos

* Respuesta grafica en el osciloscopio calculo respuesta escalon al setpoint, respuesta escalon del controlador como reaccion
![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/Respuesta%20del%20controlador.jpg)
Tiempo T0 = 1.190S
Tiempo T1 = 2.140S
Tiempo de respuesta del controlador para llegar al setpoint 0.95 segundos


 

* Se realizo el modelo POle Placement en base al circuito y los valores reales de la planta, siendo los resultados los siguientes.

![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/PolePlacement.png)

![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/PolePlacementControl.png)

![Respuesta controlada](https://github.com/juniormonroy/CDI_TPF/blob/master/Capturas/PolePlacementControlZoom.png)
Tiempo para llegar al punto de control 

T0 = 20
T1 = 21.25
Tiempo de respuesta del control en simulacion 1.05 segundos


* Concluciones:

Los tiempos resultantes entre la simulacion y el sistem real son similares, siendo 1.05 la respuesta escalon controlador del pole placement y 
0.95s la respuesta escalon del sistema fisico real.


Siendo el setpoint 1.9, y el spam maximo 3.3, se obtuvo un % del primer sobrepico en comparacion al setpoint del controlador de:


2.8-1.9 = 0.9v diferencial entre setpoint y primer aproximacion maxima
(0.9/3.3spamMax)*100 = 27.27% de la excursion disponible como presicion


Todos los adjuntos y capturas en este repositorio adjunto estan.

Referencias Externas:

https://www.keil.com/pack/doc/CMSIS/DSP/html/group__PID.html