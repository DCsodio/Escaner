# 🚗 Vehículo con Mapeo 2D

## 📝 Descripción del Sistema
Este proyecto implementa un **vehículo** que utiliza un sensor láser **VL53L1X** para generar mapas 2D en tiempo real. El **LPC845** procesa los datos de los sensores y los envía vía UART al **ESP32**, que actúa como puente WiFi hacia una aplicación **Qt** para la visualización del mapa y control opcional del vehículo.

---

## 🏗️ Arquitectura del Sistema

**Flujo de datos:**  
LPC845 → UART → ESP32 → WiFi → Aplicación Qt

**Componentes:**
- 🖥️ **LPC845:** Procesamiento principal y lectura de sensores.
- 🌐 **ESP32:** Bridge WiFi-UART.
- 💻 **Aplicación Qt:** Interfaz gráfica y visualización del mapa 2D.

---

## 🔧 Hardware y Sensores

- Microcontrolador principal: **LPC845**
- Sensores integrados:
  - 📏 **VL53L1X:** Sensor láser de tiempo de vuelo para medición de distancias.
  - 🧭 **HMC5883L:** Magnetómetro para orientación y navegación.

**Comunicaciones:**
- UART a **115200 bps** entre LPC845 y ESP32.
- WiFi desde ESP32 hacia aplicación Qt.

---

## 📡 Protocolo de Comunicación

**Estructura del Paquete UART:**
```c
typedef struct {
    uint8_t header[2];      // Identificación "{{"
    uint16_t distancia_mm;  // Datos del láser en mm
    float heading_grados;   // Orientación en grados
    int32_t pos_x;          // Posición X del vehículo
    int32_t pos_y;          // Posición Y del vehículo
    uint8_t checksum;       // Verificación de integridad
} PaqueteSensor;
```

---

## 🔬 Configuración del Láser VL53L1X

- **Modos de operación:**
  - ⚡ Corto alcance (1): 15-33ms, mayor velocidad.
  - 🎯 Largo alcance (2): 50-500ms, mayor precisión.
- **Configuración por defecto:**
  - Rango: 2 (largo alcance)
  - Timing budget: 50ms
  - Período entre mediciones: 100ms

---

## ⚡ Funcionamiento del Sistema

**En el LPC845:**
1. Inicialización del sistema PLL y periféricos.
2. Configuración de UART e I2C.
3. Inicialización de sensores (láser y magnetómetro).
4. Lectura periódica de sensores.
5. Empaquetado y envío de datos por UART al ESP32.

**En el ESP32:**
1. Conexión a red WiFi.
2. Configuración como servidor TCP.
3. Recepción de datos UART desde LPC845.
4. Reenvío de datos por WiFi a la aplicación Qt.

**En la Aplicación Qt:**
1. Conexión vía WiFi al ESP32.
2. Recepción y procesamiento de datos.
3. Visualización del mapa 2D en tiempo real.
4. Mostrar posición, orientación y distancias del vehículo.

---

## 🚙 Características del Vehículo

**Control de movimiento:**
- Adelante/atrás con distancia especificada.
- Giro izquierda/derecha con ángulo especificado.
- Seguimiento automático de posición (coordenadas X,Y).

**Parámetros configurables:**
- Diámetro de ruedas.
- Dimensiones del vehículo (largo, ancho, alto).
- Posición inicial.

---

## 🛠️ Configuración Técnica

**Comunicación UART:**
- Baudrate: 115200
- Puerto: UART0
- Formato: 8 bits de datos, sin paridad, 1 bit de stop

**Conexiones I2C:**
- VL53L1X: Dirección 0x29
- HMC5883L: Dirección 0x1E

**WiFi (ESP32):**
- Protocolo: TCP
- Puerto configurable (ej: 8080)

---

## ✅ Ventajas de la Arquitectura

- LPC845: Procesamiento en tiempo real sin overhead de WiFi.
- ESP32: Manejo eficiente de conectividad WiFi.
- Separación de responsabilidades: Cada componente se especializa en su función.
- Escalabilidad: Fácil agregado de nuevos sensores o funcionalidades.

---

## 🖥️ Aplicación Qt

La aplicación Qt proporciona:
- Interfaz gráfica para visualización del mapa 2D.
- Representación en tiempo real de la posición del vehículo.
- Visualización de mediciones de distancia.
- Indicadores de orientación y rumbo.
- Posibilidad de enviar comandos de control al vehículo.

---

## 🚀 Uso del Sistema

1. Encender **LPC845**, **ESP32** y la aplicación Qt.
2. La aplicación Qt se conecta al ESP32 vía WiFi.
3. El vehículo comienza a enviar datos de sensores.
4. La aplicación muestra el mapa y datos en tiempo real.
5. Opcionalmente, enviar comandos de movimiento.

> El sistema permite el **mapeo autónomo de entornos 2D** mediante el sensor láser, con visualización remota en tiempo real a través de la interfaz Qt.

---

## 📂 Estructura del Proyecto (Ejemplo)

VehiculoAutonomo2D/
- src/          Código fuente LPC845 y ESP32
- qt_app/       Proyecto de la aplicación Qt
- docs/         Documentación y diagramas
- README.md     Este archivo
- LICENSE       Licencia del proyecto
