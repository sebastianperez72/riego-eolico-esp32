/*
 * ============================================================
 *  Sistema de riego autónomo con energía eólica
 *  Curso: PI6002 - Proyectos Interdisciplinarios II (UTEC)
 *  Microcontrolador: ESP32 DevKit
 *  Versión: Release v1.0
 * ============================================================
 *
 *  Conexiones:
 *    - Sensor de humedad (salida AO)  -> GPIO 32 (ADC)
 *    - Relé HW-482 (pin S)            -> GPIO 33
 *    - Bomba conectada en COM / NC del relé:
 *        LOW  en GPIO 33 -> relé en reposo -> bomba ENCENDIDA
 *        HIGH en GPIO 33 -> relé activado  -> bomba APAGADA
 *
 *  Máquina de estados (FSM):
 *    1. INICIO       -> configura serial, ADC y relé (bomba apagada)
 *    2. MONITOREO    -> lee el sensor (10 muestras) y calcula la humedad
 *    3. EVALUACION   -> compara la humedad con el umbral
 *    4. RIEGO        -> enciende o apaga la bomba y espera 2 s
 */

// ---------------- Pines ----------------
const int PIN_SENSOR_HUMEDAD = 32;   // Entrada analógica (ADC1)
const int PIN_RELE_BOMBA     = 33;   // Señal de control del relé

// ---------------- Calibración del sensor ----------------
const int VALOR_SECO   = 3000;       // Lectura ADC con el sensor en el aire / tierra seca
const int VALOR_MOJADO = 1300;       // Lectura ADC con el sensor en agua
const int UMBRAL_RIEGO = 30;         // Regar si la humedad es menor a 30 %

// ---------------- Tiempos ----------------
const int NUM_MUESTRAS            = 10;    // Lecturas para promediar
const unsigned long T_MUESTRA_MS  = 10;    // Espera entre lecturas
const unsigned long T_CICLO_MS    = 2000;  // Espera entre ciclos de control

// ---------------- Lógica del relé ----------------
const int BOMBA_ON  = LOW;           // Bomba en NC: relé en reposo = bomba encendida
const int BOMBA_OFF = HIGH;          // Relé activado = bomba apagada

// ---------------- Estados de la FSM ----------------
enum Estado { INICIO, MONITOREO, EVALUACION, RIEGO };
Estado estado = INICIO;

int lecturaPromedio = 0;
int humedad = 0;
bool regar = false;

// Lee el sensor varias veces y devuelve el promedio
int leerSensorPromedio() {
  long suma = 0;
  for (int i = 0; i < NUM_MUESTRAS; i++) {
    suma += analogRead(PIN_SENSOR_HUMEDAD);
    delay(T_MUESTRA_MS);
  }
  return suma / NUM_MUESTRAS;
}

// Convierte la lectura ADC a porcentaje de humedad (0 a 100 %)
int calcularHumedad(int lectura) {
  int porcentaje = map(lectura, VALOR_SECO, VALOR_MOJADO, 0, 100);
  return constrain(porcentaje, 0, 100);
}

void setup() {
  // ESTADO 1: INICIO
  Serial.begin(115200);
  analogReadResolution(12);                 // ADC de 12 bits (0 - 4095)
  pinMode(PIN_RELE_BOMBA, OUTPUT);
  digitalWrite(PIN_RELE_BOMBA, BOMBA_OFF);  // Bomba apagada por seguridad
  Serial.println("Sistema iniciado - PI6002 Release v1.0");
  estado = MONITOREO;
}

void loop() {
  switch (estado) {

    case MONITOREO:   // ESTADO 2: lectura y filtrado del sensor
      lecturaPromedio = leerSensorPromedio();
      humedad = calcularHumedad(lecturaPromedio);
      Serial.print("Lectura: ");
      Serial.print(lecturaPromedio);
      Serial.print(" | Humedad: ");
      Serial.print(humedad);
      Serial.println(" %");
      estado = EVALUACION;
      break;

    case EVALUACION:  // ESTADO 3: comparación con el umbral
      regar = (humedad < UMBRAL_RIEGO);
      estado = RIEGO;
      break;

    case RIEGO:       // ESTADO 4: control de la bomba
      if (regar) {
        digitalWrite(PIN_RELE_BOMBA, BOMBA_ON);
        Serial.println("ESTADO: SECO | RIEGO ACTIVADO");
      } else {
        digitalWrite(PIN_RELE_BOMBA, BOMBA_OFF);
        Serial.println("ESTADO: HUMEDO | RIEGO DESACTIVADO");
      }
      Serial.println("----------------------------------------");
      delay(T_CICLO_MS);
      estado = MONITOREO;
      break;

    default:
      estado = MONITOREO;
      break;
  }
}
