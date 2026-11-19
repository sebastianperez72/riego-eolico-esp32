# Sistema de riego autónomo con energía eólica

Proyecto del curso **PI6002 – Proyectos Interdisciplinarios II (UTEC)**.
Sistema de riego automático alimentado por un aerogenerador Savonius. Contribuye al **ODS 6 (Meta 6.4)**.

## Funcionamiento
1. Una hélice Savonius mueve un motor **NEMA 17**, que funciona como generador.
2. Dos puentes de diodos **1N5819** rectifican la corriente y dos capacitores de **1000 µF / 25 V** la filtran.
3. Un regulador **LM2596** fija 5.0 V y un módulo **TP4056** carga dos baterías **18650** dentro de un **Battery Shield**.
4. Un **ESP32** lee un sensor de humedad de suelo y, mediante un **relé**, enciende una minibomba de 5 V cuando el suelo está seco.

## Estructura del repositorio
```
riego-eolico-esp32/
├── firmware/
│   └── riego_eolico/
│       └── riego_eolico.ino   # Código del ESP32
├── docs/
│   ├── esquematico.png        # Esquemático eléctrico (Anexo B)
│   └── diagrama_flujo.png     # Diagrama de flujo / FSM (Anexo C)
└── README.md
```

## Conexiones del ESP32
| Señal | Pin ESP32 |
|---|---|
| Sensor de humedad (AO) | GPIO 32 |
| Relé (S) | GPIO 33 |
| Alimentación sensor y relé | 3V3 / GND |
| Alimentación ESP32 | VIN ← 5V del Battery Shield |

La bomba está conectada en **COM / NC** del relé. Por eso **LOW enciende la bomba** y **HIGH la apaga**.

## Parámetros
| Parámetro | Valor |
|---|---|
| Umbral de riego | 30 % |
| Muestras por lectura | 10 (cada 10 ms) |
| Calibración seco / mojado | 3000 / 1300 (ADC) |
| Ciclo de control | 2 s |

## Cómo cargar el código
1. Instalar el **Arduino IDE** y el soporte para placas **ESP32** (Espressif).
2. Abrir `firmware/riego_eolico/riego_eolico.ino`.
3. Seleccionar la placa **ESP32 Dev Module** y el puerto COM.
4. Subir el código y abrir el **Monitor Serie** a **115200 baudios**.

## Integrantes
- Nombre 1
- Nombre 2
- Nombre 3
