/*==================[inclusiones]============================================*/
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include "queue.h"
#include "semphr.h"
#include "arm_math.h"
#include "main.h"


/*==================[definiciones y macros]==================================*/
#define USED_UART UART_USB
#define UART_RATE 115200

#define RATE_TASK_1 1 //DAC DuttyCycle 50% Periodo T 1ms
#define RATE_TASK_2 1 //Processing PID DuttyCycle 50% Periodo T 1ms
#define RATE_TASK_3 1 //ADC DuttyCycle 50% Periodo T  1ms
#define RATE_TASK_4 50 //Comm DuttyCycle 50% Periodo T  50ms

#define RATE1 pdMS_TO_TICKS(RATE_TASK_1)
#define RATE2 pdMS_TO_TICKS(RATE_TASK_2)
#define RATE3 pdMS_TO_TICKS(RATE_TASK_3)
#define RATE4 pdMS_TO_TICKS(RATE_TASK_4)


#define SEPARACION_MS 1000
#define T_SEPARACION pdMS_TO_TICKS(SEPARACION_MS)
#define PRINT_RATE_MS 500
#define PRINT_RATE pdMS_TO_TICKS(PRINT_RATE_MS)

#define MALLOC_ERROR "Malloc Failed Hook!\n"
#define MSG_ERROR_QUE "Error al crear la cola.\r\n"
#define MSG_ERROR_MTX "Error al crear el mutex.\r\n"
#define LED_ERROR LEDR
#define N_QUEUE 	5

#define N_SEM 5

//LIMITES DE OPERACION OPERATIVA
#define DAC_HIGH 750 //SETPOINT HIGH
#define DAC_LOW 120     //SETPOINT LOW

//COEFICIENTES DE CONTROL PID
#define Kp_f 0.033707f 		//calculo por ziegle nichols
#define Ki_f 0.011235f 	//calculo por ziegle nichols
#define Kd_f 0.070224f 	//calculo por ziegle nichols

//LIMITES MAXIMOS POSIBLES DEL CALCULO EN FUNCION DEL ADC Y DAC
#define MIN_OUTPUT 0 // Mínimo valor de salida permitido
#define MAX_OUTPUT 1023 // Máximo valor de salida permitido

// Definir la estructura del controlador PID
arm_pid_instance_q15 PID;

// Inicializar controlador PID
void init_PID_controller() {
  PID.Kp = (q15_t)(Kp_f * 32768);
  PID.Ki = (q15_t)(Ki_f * 32768);
  PID.Kd = (q15_t)(Kd_f * 32768);

  arm_pid_init_q15(&PID, 1);
}


//FUNCIO DE CONTROL PID DE TEMPERATURA
int16_t temperature_control_loop(int16_t setpoint, int16_t measured_temperature)
{
	  int16_t error = setpoint - measured_temperature;//CALCULO DEL ERROR

	  /*Convierte el error y la temperatura_medida del rango 0-1023 al rango Q15*/
	  int32_t error_Q15 = ((int32_t) error) << 5; // 5 bits para la parte fraccionaria Q15

	  int32_t measured_Q15 = ((int32_t) measured_temperature) << 5; // 5 bits para la parte fraccionaria Q15

	  int16_t pid_output_Q15 = arm_pid_q15(&PID, error_Q15);

	  // Convierte pid_output_Q15 del rango Q15 al rango 0-1023
	  int16_t pid_output = (int16_t) (pid_output_Q15 >> 5); // 5 bits para la parte fraccionaria Q15

	  // Saturar la salida para asegurarse de que esté dentro del rango permitido
	  if (pid_output > MAX_OUTPUT) {
	    pid_output = MAX_OUTPUT;
	    PID.Ki = 0;
	  } else if (pid_output < MIN_OUTPUT) {
	    pid_output = MIN_OUTPUT;
	    PID.Ki = 0;
	  }
	  else
		  PID.Ki = (q15_t)(Ki_f * 32768);

	  return pid_output  ; //accion directa
}


/*==================[definiciones de datos internos]=========================*/
int16_t adc_read, adc_write, dac_write, sp_pid = 0, kp_pid = 200, ti_pid = 50, td_pid = 20, pid_out = 0;


/*==========================[Inicializacion]=================================*/


/*==================[definiciones de datos externos]=========================*/
DEBUG_PRINT_ENABLE;
/*==================[definiciones de datos internos]=========================*/


/*==================[declaraciones de funciones internas]====================*/
void gpio_init( void );
/*==================[declaraciones de funciones externas]====================*/
TaskHandle_t task_handle_a;
TaskHandle_t task_handle_b;
TaskHandle_t task_handle_c;
TaskHandle_t task_handle_d;

// Prototipo de funcion de la tarea
void tarea_a( void* taskParmPtr );
void tarea_b( void* taskParmPtr );
void tarea_c( void* taskParmPtr );
void tarea_d( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
    boardConfig();									// Inicializar y configurar la plataforma

    gpio_init();

    debugPrintConfigUart( USED_UART, UART_RATE );		// UART for debug messages

    adcConfig(ADC_ENABLE);

    dacConfig(DAC_ENABLE);


    BaseType_t res;

    // Crear tarea en freeRTOS
    res = xTaskCreate(
              tarea_a,                    // Funcion de la tarea a ejecutar
              ( const char * )"tarea_a",  // Nombre de la tarea como String amigable para el usuario
              configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
              0,                          // Parametros de tarea
              tskIDLE_PRIORITY+1,         // Prioridad de la tarea
			  &task_handle_a              // Puntero a la tarea creada en el sistema
          );

    // Gestion de errores
    configASSERT( res == pdPASS );


    res = xTaskCreate(
              tarea_b,                     // Funcion de la tarea a ejecutar
              ( const char * )"tarea_b",   // Nombre de la tarea como String amigable para el usuario
              configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
              0,                          // Parametros de tarea
              tskIDLE_PRIORITY+1,         // Prioridad de la tarea
			  &task_handle_b                           // Puntero a la tarea creada en el sistema
          );

    // Gestion de errores
    configASSERT( res == pdPASS );

    // Creo tarea unica de impresion
    res = xTaskCreate(
              tarea_c,                     // Funcion de la tarea a ejecutar
              ( const char * )"tarea_c",   // Nombre de la tarea como String amigable para el usuario
              configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
              0,                          // Parametros de tarea
              tskIDLE_PRIORITY+1,         // Prioridad de la tarea
			  &task_handle_c                           // Puntero a la tarea creada en el sistema
          );

    // Gestion de errores
    configASSERT( res == pdPASS );




    // TAREA CLIENTE PARTE 2
    res = xTaskCreate(
              tarea_d,                     // Funcion de la tarea a ejecutar
              ( const char * )"tarea_d",   // Nombre de la tarea como String amigable para el usuario
              configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
              0,                          // Parametros de tarea
              tskIDLE_PRIORITY+1,         // Prioridad de la tarea
			  &task_handle_d                           // Puntero a la tarea creada en el sistema
          );

    // Gestion de errores
    configASSERT( res == pdPASS );


    vTaskStartScheduler();					// Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

    // ---------- REPETIR POR SIEMPRE --------------------------
    configASSERT( 0 );

    // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
    // directamenteno sobre un microcontroladore y no es llamado por ningun
    // Sistema Operativo, como en el caso de un programa para PC.
    return TRUE;
}

/*==================[definiciones de funciones internas]=====================*/
void gpio_init( void )
{
	gpioInit(GPIO0, GPIO_INPUT);
	gpioInit(GPIO1, GPIO_INPUT);
	gpioInit(GPIO2, GPIO_OUTPUT);
	gpioInit(GPIO3, GPIO_OUTPUT);
}
/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea

//######### TAREA DE ESCRITURA DEL DAC DEL VALOR SALIDA DEL PID
void tarea_a( void* taskParmPtr)
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  RATE1;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    //vTaskSuspend(0);
    while( TRUE )
    {
    	gpioWrite( LEDB, ON );

    	//ESCRITURA DAC
    	dacWrite(DAC,pid_out);
    	dac_write = pid_out;

        vTaskDelay( RATE1/2 );
        gpioWrite( LEDB, OFF );

        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
		//vTaskResume(task_handle_b);
		//vTaskSuspend(0);
    }
}


//######### TAREA PID DE CONTROL
void tarea_b( void* taskParmPtr )
{

	int16_t setpoint = 0; // Temperatura deseada setpoint
	int16_t measured_temperature = 0; // temperatura inicial
	init_PID_controller(); // Inicializamos el pid

	// ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  RATE2;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    //vTaskSuspend(0);
    while( TRUE )
    {
    	gpioWrite( LED1, ON );
        //adc_write = adc_read;

    	setpoint = adcRead(CH3);//lectura del setpoin por potenciometro

    	sp_pid = setpoint;

        measured_temperature = adc_read; // lectura sensor por ch1 tarea c

        pid_out = temperature_control_loop(setpoint, measured_temperature); // calculo PID


        vTaskDelay( RATE2/2 );

        gpioWrite( LED1, OFF );

        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
		//vTaskResume(task_handle_c);
		//vTaskSuspend(0);
    }
}


//######### TAREA PARA LECTURA DEL ADC COMO RESPUESTA DE LA PLANTA
void tarea_c( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  RATE3;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    //vTaskSuspend(0);
    while( TRUE )
    {
        gpioWrite( LED2, ON );

        //LECTURA DEL ADC CH1 SALIDA PLANTA
        adc_read = adcRead(CH1);

        vTaskDelay( RATE3/2 );

        gpioWrite( LED2, OFF );

        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
		//vTaskResume(task_handle_d);
		//vTaskSuspend(0);
    }
}

//######### COMUNICACIONES
void tarea_d( void* taskParmPtr )
{
	char buffer1[10], buffer2[10], buffer3[10];

	// ---------- CONFIGURACIONES ------------------------------
    TickType_t xPeriodicity =  RATE4;		// Tarea periodica cada 1000 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    //vTaskSuspend(0);
    while( TRUE )
    {

    	//vTaskEnterCritical();
    	gpioWrite( LED3, ON );

        //SALIDA DATOS SERIAL

    	//ENVIO DATO SETPOINT
    	itoa (sp_pid, buffer1, 10);
        uartWriteString(UART_USB, buffer1);
        uartWriteString(UART_USB, ",");

        //ENVIO DATO ADC PLANTA
        itoa (adc_read, buffer2, 10);
        uartWriteString(UART_USB, buffer2);
        uartWriteString(UART_USB, ",");

        //ENVIO DATO PID SALIDA CONTROL
        itoa (pid_out, buffer3, 10);
        uartWriteString(UART_USB, buffer3);
        uartWriteString(UART_USB, "\n");
       // vTaskExitCritical();

        vTaskDelay( RATE4/2 );

        gpioWrite( LED3, OFF );

        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
		//vTaskResume(task_handle_a);
		//vTaskSuspend(0);

    }

}

/* hook que se ejecuta si al necesitar un objeto dinamico, no hay memoria disponible */
void vApplicationMallocFailedHook()
{
    printf( "Malloc Failed Hook!\n" );
    configASSERT( 0 );
}


/*==================[fin del archivo]========================================*/
