import serial
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation

# Parametro para el ancho de la ventana de tiempo
window_width = 240


# Configuramos el puerto serial
Data_Serial = serial.Serial('/dev/ttyUSB1', 115200)

# Creamos la figura y el eje
fig, ax = plt.subplots()

# Fijamos el eje y de 0 a 1024
ax.set_ylim([-100, 1200])

# Establecemos el eje x en segundos
xlim = 6
ax.set_xlim([0, xlim])

# Creamos los arreglos para almacenar los valores de cada variable
num_samples = xlim * 100



var1_vals = np.zeros(num_samples)
var2_vals = np.zeros(num_samples)
var3_vals = np.zeros(num_samples)


x_vals = np.arange(num_samples)

# Creamos las lineas vacias para la grafica
line1, = ax.plot([], [], 'r', label='Var1')
line2, = ax.plot([], [], 'b', label='Var2')
line3, = ax.plot([], [], 'g', label='Var3')


def update(frame):
    # Leemos la linea desde el puerto serial
    line = Data_Serial.readline()

    # Decodificamos la linea y removemos el salto de linea al final
    line = line.decode().strip()

    # Verificamos si la línea no esta vacia
    if line:
        # Separamos la linea en dos valores
        values = line.split(',')

        # Verificamos si la linea contiene exactamente dos valores separados por coma
    if len(values) == 3:
        var1, var2, var3 = values
        var1 = float(var1)
        var2 = float(var2)
        var3 = float(var3)
        var1_vals[:-1] = var1_vals[1:]
        var2_vals[:-1] = var2_vals[1:]
        var3_vals[:-1] = var3_vals[1:]
        x_vals[:-1] = x_vals[1:]
        var1_vals[-1] = var1
        var2_vals[-1] = var2
        var3_vals[-1] = var3
        x_vals[-1] = x_vals[-2] + 1


        # Actualizamos los datos de las lineas de la grafica
        line1.set_data(x_vals, var1_vals)
        line2.set_data(x_vals, var2_vals)
        line3.set_data(x_vals, var3_vals)


        # Desplazamos la ventana hacia la derecha
        ax.set_xlim([x_vals[-1] - window_width, x_vals[-1]])

    return line1, line2, line3



# Creamos la animación y la ejecutamos
animacion = FuncAnimation(fig, update, frames=range(num_samples), blit=True, interval=10)

plt.show()
