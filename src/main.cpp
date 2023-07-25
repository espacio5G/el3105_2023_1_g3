#include <Arduino.h>
#include <StarterKitNB.h>
#include "SparkFun_SHTC3.h"

//Definiciones (funciones, variables, etc.)

StarterKitNB sk;

//Definir tipos de variables: String, Bool, Int, Float, Uint8_t, Void

const int analogPin = WB_A1; // Pin de salida analógica del sensor conectado al pin AIN1 de la placa RAK19001
const int digitalPin = WB_IO3; // Pin de salida digital del sensor conectado al pin IO3 de la placa RAK19001
 

//Chantar el ; al final de todo

//mensajes en formato json

//Mensajes
String temperature;
String humidity;
String batteryLevelStr;
String msg; 

//APN
String apn = "m2m.entel.cl";
String user = "entelpcs";
String pw = "entelpcs";
//ThingBoard
String clientId = "grupo3";
String userName = "33333";
String password = "33333";

SHTC3 g_shtc3; // Declare an instance of the SHTC3 class

// Battery measurement parameters
#define PIN_VBAT WB_A0
#define REAL_VBAT_MV_PER_LSB (VBAT_DIVIDER_COMP * VBAT_MV_PER_LSB)
#define VBAT_DIVIDER (0.6F)
#define VBAT_DIVIDER_COMP (1.45F)
#define VBAT_MV_PER_LSB (0.73242188F)
#define NO_OF_SAMPLES 256

uint32_t vbat_pin = PIN_VBAT;


float readVBAT(void)
{
  int i;
  float raw;
  float mv;

  raw = 0;
  for (i = 0; i < NO_OF_SAMPLES; i++)
  {
    raw += analogRead(vbat_pin);
  }
  raw /= NO_OF_SAMPLES;

  mv = raw * REAL_VBAT_MV_PER_LSB;

  return mv * (1/VBAT_DIVIDER);
}

uint8_t mvToPercent(float mvolts)
{
  if (mvolts < 3300)
    return 0;

  if (mvolts < 3600)
  {
    mvolts -= 3300;
    return mvolts / 30;
  }

  mvolts -= 3600;
  uint8_t batteryPercent = 10 + (mvolts * 0.15F); // thats mvolts /6.66666666

  // Limit battery percentage to 0-100%
  if (batteryPercent > 100)
    batteryPercent = 100;

  return batteryPercent;
}
void errorDecoder(SHTC3_Status_TypeDef message)   // The errorDecoder function prints "SHTC3_Status_TypeDef" resultsin a human-friendly way
{
  switch (message)
  {
    case SHTC3_Status_Nominal:
      Serial.print("Nominal");
      break;
    case SHTC3_Status_Error:
      Serial.print("Error");
      break;
    case SHTC3_Status_CRC_Fail:
      Serial.print("CRC Fail");
      break;
    default:
      Serial.print("Unknown return code");
      break;
  }
}

void shtc3_read_data(void)
{
  g_shtc3.update();
  if (g_shtc3.lastStatus == SHTC3_Status_Nominal)
  { 
    temperature = String(g_shtc3.toDegC());
    humidity = String(g_shtc3.toPercent());

    Serial.print("RH = ");
    Serial.print(g_shtc3.toPercent()); 
    Serial.print("% (checksum: ");
    
    if (g_shtc3.passRHcrc) 
    {
      Serial.print("pass");
    }
    else
    {
      Serial.print("fail");
    }
    
    Serial.print("), T = ");
    Serial.print(g_shtc3.toDegC());
    Serial.print(" deg C (checksum: ");
    
    if (g_shtc3.passTcrc) 
    {
      Serial.print("pass");
    }
    else
    {
      Serial.print("fail");
    }
    Serial.println(")");
  }
  else
  {
    Serial.print("Update failed, error: ");
    errorDecoder(g_shtc3.lastStatus);
    Serial.println();
  }
}

void setup()
{
   sk.Setup(true);
  delay(500);
  sk.UserAPN(apn, user, pw);
  delay(500);
  sk.Connect(apn);
  delay(1000);
  sk.DeviceCheck();   // Consulta sobre las caracteristicas actuales del dispositivo
  delay(1000);
  time_t timeout = millis();
  Serial.begin(115200);
  while (!Serial)
  {
    if ((millis() - timeout) < 5000)
    {
      delay(100);
    }
    else
    {
      break;
    }
  }

  Wire.begin();
  Serial.println("shtc3 init");
  Serial.print("Beginning sensor. Result = "); 
  errorDecoder(g_shtc3.begin());              
  Wire.setClock(400000); 

  Serial.println();

  if (g_shtc3.passIDcrc)                      
  {   
    Serial.print("ID Passed Checksum. ");
    Serial.print("Device ID: 0b");
    Serial.println(g_shtc3.ID, BIN); 
  }
  else
  {
    Serial.println("ID Checksum Failed. ");
  }

  // Battery measurement setup
  adcAttachPin(vbat_pin);
  analogSetAttenuation(ADC_11db);
  analogReadResolution(12); // Can be 8, 10, 12 or 14
  delay(1);
  readVBAT(); // Get a single ADC sample and throw it away
}

void loop() {
  if (!sk.ConnectionStatus()) {
    sk.Reconnect(apn);
    delay(2000);
  }

  if (!sk.LastMessageStatus) {
    sk.ConnectBroker(clientId, userName, password);
    delay(2000);
  }

  int analogValue = analogRead(analogPin);

  // Calcular valordecloro
  int valordecloro = (2700 - analogValue) * 0.0104;
  if (valordecloro < 0) {
    valordecloro = 0;
  }

  // Leer el valor digital del sensor
  int digitalValue = digitalRead(digitalPin);

  // Imprimir los valores leídos
  Serial.print("Valor analógico: ");
  Serial.print(analogValue);
  Serial.print(", Valor digital: ");
  Serial.println(digitalValue);
  Serial.println(valordecloro);

  shtc3_read_data();
  float vbat_mv = readVBAT();
  uint8_t vbat_per = mvToPercent(vbat_mv);
  batteryLevelStr = String(vbat_per);
  msg = "{\"temperature\": " + temperature + ", \"humidity\": " + humidity + ", \"battery\": " + batteryLevelStr + ",  \"Cantidad de gramos de cloro\": " + valordecloro + "}";
  sk.SendMessage(msg);
  delay(2000);
}



