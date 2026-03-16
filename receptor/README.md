# Subir a Git

Manera correcta de subir mi aportación a GitHub desde terminal.

## Pasos

1. `git pull` → actualizar cambios del equipo
2. `git add .`
3. `git commit -m "mensaje"`
4. `git push`

# 📘 Guía Definitiva de Pines GPIO para ESP32

Esta guía clasifica los pines del ESP32 según su seguridad y función, y mapea exactamente qué hardware conectar en cada uno para evitar crasheos en el sistema.

## 🚫 1. Los Intocables (ZONA ROJA)
Bajo ninguna circunstancia conectes nada aquí. Tu ESP32 se reiniciará, dejará de funcionar o no podrás reprogramarlo.

* **GPIO 6 al 11:** Están conectados físicamente a la memoria Flash interna del chip. Si los tocas, el cerebro del ESP32 colapsa al instante.
* **GPIO 1 (TX0) y GPIO 3 (RX0):** Son las cuerdas vocales del chip. Se usan exclusivamente para el Monitor Serial y para subirle código por USB.

## ⚠️ 2. Los Pines de Arranque o "Strapping" (ZONA AMARILLA)
El ESP32 lee el voltaje de estos pines en el milisegundo exacto en que lo enciendes para saber en qué modo iniciar. Evita usarlos como entradas (botones) para no confundir al chip durante el arranque.

* **GPIO 0:** Si tiene voltaje bajo (GND) al encender, entra en modo programación.
* **GPIO 2:** Debe estar libre o en LOW al encender. (Suele estar conectado al LED azul de la placa).
* **GPIO 5:** Debe estar en HIGH durante el arranque. Muy usado para el pin CS de las tarjetas SD.
* **GPIO 12:** Si está en HIGH al arrancar, el ESP32 se quema internamente buscando un voltaje que no tiene. ¡Peligroso!
* **GPIO 15:** Imprime mensajes de depuración en el arranque. 

## 📥 3. Los "Solo Entrada" / Input Only (ZONA AZUL)
Estos pines no tienen hardware interno para sacar voltaje (No sirven para LEDs, motores, ni salidas). Tampoco tienen resistencias Pull-Up o Pull-Down internas. Son los pines analógicos más limpios y perfectos para leer sensores.

* **GPIO 34** * **GPIO 35** * **GPIO 36 (VP)** * **GPIO 39 (VN)**

## ✅ 4. Los Súper Seguros (ZONA VERDE)
Estos son tus lienzos en blanco. Puedes usarlos para lo que quieras: entradas, salidas, PWM, botones, luces, etc. No interfieren con el arranque ni con el WiFi.

* **Pines Mágicos:** GPIO 4, 13, 14, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33.

---

## 🎛️ Guía de Canales ADC (Lecturas Analógicas)

El ESP32 tiene dos convertidores analógico-digitales (ADC1 y ADC2). 
**REGLA DE ORO:** Si usas Wi-Fi, Bluetooth o ESP-NOW, el ADC2 se desactiva por completo. Por lo tanto, **SOLO DEBES USAR EL ADC1**.

| Canal ESP-IDF | Pin Físico (GPIO) | Ideal para... |
| :--- | :--- | :--- |
| `ADC1_CHANNEL_0` | GPIO 36 (VP) | Joystick Eje X, Potenciómetros |
| `ADC1_CHANNEL_3` | GPIO 39 (VN) | Joystick Eje Y, Potenciómetros |
| `ADC1_CHANNEL_4` | GPIO 32 | Sensores analógicos generales |
| `ADC1_CHANNEL_5` | GPIO 33 | Sensores analógicos generales |
| `ADC1_CHANNEL_6` | GPIO 34 | Micrófonos analógicos limpios |
| `ADC1_CHANNEL_7` | GPIO 35 | Fotorresistencias (LDR) |

*(Nota: Los canales 1 y 2 corresponden a los pines 37 y 38, pero casi ninguna placa comercial los expone físicamente).*

---

## 🔌 Compatibilidad por Componente (Cheat Sheet)

| Componente | Qué necesita | Pines Recomendados | Pines Prohibidos |
| :--- | :--- | :--- | :--- |
| **Joysticks y Potenciómetros** | ADC1 limpio para medir posición | 36(VP), 39(VN), 34, 35, 32, 33 | Cualquier pin de ADC2 (4, 12, 13...) si usas WiFi |
| **Botones y Switches** | Resistencia interna (Pull-up/down) | 13, 14, 16, 17, 25, 26, 27, 32, 33 | 34, 35, 36, 39 (A menos que pongas resistencia física) |
| **Puente H (Motores DC)** | Salida PWM (`ledc`) para controlar vel. | 4, 13, 14, 16, 17, 25, 26, 27 | 34, 35, 36, 39 (No sacan voltaje) |
| **Zumbadores (Buzzer)** | Salida Digital (HIGH/LOW) | 4, 13, 14, 16, 17, 25, 26, 27 | 34, 35, 36, 39 |
| **Micrófonos (Analógicos)** | ADC1 extremadamente limpio | 34, 35, 36, 39 | Pines digitales cerca de la antena WiFi |
| **Módulos I2C (OLED, MPU6050)** | Pines SDA y SCL estándar | 21 (SDA) y 22 (SCL) | Cualquier otro (Requiere config extra) |
| **Módulos SPI (SD, NRF24)** | Pines MOSI, MISO, CLK, CS | 23(MOSI), 19(MISO), 18(CLK), 5(CS) | Cualquier otro |
| **Tiras LED (WS2812B)** | Salida RMT rápida y estable | 13, 14, 25, 26, 27, 32, 33 | Pines Strapping (Pueden parpadear) |
| **Servomotores** | Salida PWM precisa a 50Hz | 4, 13, 14, 16, 17, 25, 26, 27 | 34, 35, 36, 39 |