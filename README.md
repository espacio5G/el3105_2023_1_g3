# el3105_2023_1_g3
# Proyecto-IOT-uch

# Contacto:

 Benito Fuentes Silva, mail: benitofuentessilva@gmail.com
 
 
 Fernando Urzúa, mail: fernando.urzua.v@ug.uchile.cl

 
 José Espinoza, mail: jose.espinoza.v@ug.uchile.cl


# Modulos usados post montaje:

 Se ocuparon los sensores de tempertatura y humedad para tener un uso referencial y con la
 intencion de que se pueda visualizar la entrega de datos.
 Ademas se ocupo un sensor de turbiedad, que no necesita libreria externa para su uso
 Solo se necesitaba ocupar los pines asociados al uso del sensor.

# Los modulos enlistados quedan asi:

[1] RAK19001 WisBlock Dual IO Base Board + cable USB.
[2] RAK5860 WisBlock NB-IoT Interface Module + Antena LTE + Antena GPS.
[3] RAK11200 WisBlock WiFi Module, ESP32 + 2 jumpers + 3 pares pinout
[4] Batería de Litio LiPo 2500mAh 3.7V
[5] RAK1901 WisBlock Temperature and Humidity Sensor.
[6] LGZD-V1.1 Sensor de Turbidez para Líquidos

# El sensor de turbidez tiene los siguentes datos:

Voltaje de Operación: 5VDC
Consumo de corriente : 30mA (Max.)
Tiempo de Respuesta : <500ms
Resistencia de aislación : 100M [Min];
Salida Análoga:  0-4.5V
Salida Digital: (  High : 5V    Low : 0V)
Temperatura de Operación : 5 °C a 90 °C;
Tempertura de Almacenamiento : -10 a 80 °C.
Dimensiones : 30mm x 20mm x 12mm

Conexionado:

Entre el sensor y el módulo acondicionador de señal van tres cables (incluidos) de color rojo, azul y amarillo.
El cable rojo va conectado en el pin 1 del módulo acondicionador de señal.
El cable azul va conectado en el pin 2 del módulo acondicionador de señal.
El cable amarillo va conectado en el pin 3 del módulo acondicionador de señal.
El módulo cuenta con cuatro pines para conectarse a una placa Arduino o a cualquier sistema basado en microcontrolador.
El pin G, corresponde al negativo de la fuente de alimentación (5VDC).
El pin A, corresponde a la salida análoga de voltaje (0 a 4.5V).
El pin D, corresponde a la salida digital.
El pin V se debe conectar al positivo de la fuente de alimentación (5VDC).

 Para su lectura, esencialmente se creo una variable que identifique el pin analogo.
 Despues en el loop, se hizo la lectura del pin, para despues crear una funcion que entraga gramos de cloro por litro.
 Esta conversion se hizo ya que 10ppm de cloro es usada para aguas turbias pero no en exceso (nivel intermedio),
 donde con transformaciones matematicas se llego a la contante que multiplica nuestra variable.
 Ademas se estimo un valor promedio del agua clara (2700).
 Se hacia un print de los valores para tener una lectura mas rapida en temas visuales.
 Despues simplemente se agrega al mensaje a enviar.

 El calculo de la bateria no es preciso, ya que solo muestra un 100% y da una caida brusca cuando llega al 0%.

# Hay una pagina en portugues que ayuda a entender mejor el sensor: 

https://www.usinainfo.com.br/blog/sensor-de-turbidez-projeto-de-leitura-da-qualidade-da-agua/
