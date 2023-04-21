#CALCULO DE KP KI KD PARA DIVERSAS FORMAS DE CONTROL P, PI, PID
#datos tomados del osciloscopio, ver capturas


#calculo de tr
tiempo_al_10_porciento = 975 # ms
tiempo_al_90_porciento = 530 # ms
tr = tiempo_al_10_porciento - tiempo_al_90_porciento #tr = 445ms
print("tr", tr)


#calculo de ts
tiempo_al_100_porciento = 110 # ms
tiempo_al_0_porciento   = 895 # ms
ts = tiempo_al_0_porciento - tiempo_al_100_porciento #ts = 785 ms  tiempo de asentamiento en segundos
print("ts:", ts)

# tiempo de retardo en segundos tomado del osciloscopio
L = 10 #ms
print("L:", L)


# Calcular Kc para P
Kc_P = 1.2 * (L / tr)
print("Kc P:", Kc_P)
# Calcular los parámetros del controlador P
Kp_p = 0.5 * Kc_P

# Calcular Kc para PI
Kc_PI = 2.0 * (L / tr)
print("Kc PI:", Kc_PI)
# Calcular los parámetros del controlador PI
Kp_pi = 0.45 * Kc_PI
Ki_pi = 1.2 * Kc_PI / L

# Calcular Kc para PID
Kc_PID = 2.5 * (L / tr)
print("Kc PID:", Kc_PID)
# Calcular los parámetros del controlador PID
Kp_pid = 0.6 * Kc_PID
Ki_pid = 2 * Kc_PID / L
Kd_pid = 0.125 * Kc_PID * L


# Imprimir los valores de los parámetros del controlador PID
print("Controlador P - Kp:", Kp_p)
print("Controlador PI - Kp:", Kp_pi, "Ki:", Ki_pi)
print("Controlador PID - Kp:", Kp_pid, "Ki:", Ki_pid, "Kd:",Kd_pid)
