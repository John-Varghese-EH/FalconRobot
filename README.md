# Falcon 4.0.3 - Smart AI Waste Management Robot

Advanced Arduino UNO-based robot with Bluetooth control, dual ultrasonic obstacle avoidance, servo mechanism, and intelligent bin-full sensing.

## 🚀 Features
- **Dual Mode Control:** Toggle between Car Movement and Servo Control with a single button.
- **Smart Obstacle Avoidance:** Automatic front/back detection with safety retreat logic.
- **Bin Full Sensing:** Intelligent IR-based capacity monitoring with visual/auditory alerts.
- **Power Safety:** Smooth servo movements and slow power-on sweeps to prevent Arduino resets.
- **Multi-LED Feedback:** Dedicated status indicators for obstacle detection and system state.

---

## 🛠️ Hardware Requirements
- **Microcontroller:** Arduino UNO
- **Motor Shield:** Adafruit L293D Motor Shield
- **Motors:** 4x DC Motors (Front/Back Right & Left)
- **Servos:** 2x SG90 Servos (Claw/Arm)
- **Sensors:**
  - 2x HC-SR04 Ultrasonic Sensors (Front & Back)
  - 1x IR Proximity Sensor (Bin Capacity)
- **Bluetooth:** HC-05 or HC-06 Module
- **IO:** Buzzer, Headlight LED, Pin 13 Integrated LED

---

## 🔌 Wiring Pinout

| Component | Arduino Pin | Description |
| :--- | :--- | :--- |
| **Buzzer** | D2 | Pulse/Horn Output |
| **Servo 1** | D9 | Servo Signal (PWM) |
| **Servo 2** | D10 | Servo Signal (PWM) |
| **BT Module RX** | D1 (TX) | Connection via Serial |
| **BT Module TX** | D0 (RX) | Connection via Serial |
| **Indicator LED** | D13 | Built-in Status LED |
| **Front Trig** | A3 | Ultrasonic Front |
| **Front Echo** | A2 | Ultrasonic Front |
| **Back Trig** | A1 | Ultrasonic Back |
| **Back Echo** | A0 | Ultrasonic Back |
| **Bin Sensor** | A4 | IR Sensor (Bin Full) |
| **Headlight** | A5 | Main LED Output |

> **Note:** Pins D3-D8, D11-D12 are reserved by the L293D Motor Shield.

---

## 📱 App Configuration (BT Controller)
Use the **BT Controller** app or **Bluetooth Electronics** app. Map the following characters to your UI:

| Command | Character | Function |
| :--- | :---: | :--- |
| Forward / Backward | `F` / `B` | Move robot (Car Mode) or Move servos (Servo Mode) |
| Turn Left / Right | `L` / `R` | Rotate in place |
| Stop | `S` | Emergency stop |
| **Toggle Mode** | `X` | Switch between Car and Servo control |
| Horn | `Y` | Sound buzzer |
| Headlight ON/OFF | `U` / `u` | Toggle LED on A5 |
| Speed 1-4 | `1`-`4` | Set motor speed (25%-100%) |

---

## 📝 Installation

### Step 1: Install Libraries
Open Arduino IDE and install the following libraries via **Sketch > Include Library > Manage Libraries**:
1. `Adafruit Motor Shield library` (search "AFMotor")
2. `NewPing` (by Tim Eckel)
3. `Servo` (built-in, already available)

### Step 2: Upload Code
1. Connect Arduino UNO via USB.
2. **Disconnect HC-05 Bluetooth TX/RX pins** (to avoid serial conflicts).
3. Open `Robot_Project_4_0_John.ino`.
4. Select **Board: Arduino UNO** and the correct **COM Port**.
5. Click **Upload**.

### Step 3: Reconnect Bluetooth
After upload completes, reconnect the HC-05 module:
- HC-05 TX → Arduino RX (D0)
- HC-05 RX → Arduino TX (D1)

---

## ⚙️ How it Works

### Servo Power Protection
Upon startup, the robot performs a **Slow Sweep** from 0° to 90° over ~3 seconds. This prevents high current surges that can cause Arduino brownouts/resets.

### Bin Full Emergency Protocol
When the IR sensor on **A4** detects waste reaching the bin's capacity:
1. The robot **Stops** immediately.
2. The buzzer **Honks** once.
3. The **D13 LED** flashes rapidly.
4. All movement is **locked** until the bin is emptied.

Once the bin is cleared, normal operation resumes automatically.

### Dual Control Mode
- **Mode 0 (Car):** Default. F/B/L/R control robot movement.
- **Mode 1 (Servo):** Press `X` to toggle. F/B now control servos for arm/claw operation.

---

## 📦 Upload Pre-Compiled Firmware (.hex)

If you have the **pre-compiled `.hex` file** (protected binary), you can upload it without the source code:

### Method 1: Using XLoader (Easiest)
1. Download [XLoader](https://github.com/binaryupdates/xLoader) for Windows.
2. Open XLoader and configure:
   - **Hex file:** Select `Robot_Project_4_0_John_PROTECTED.hex`
   - **Device:** `Uno(ATmega328)`
   - **COM port:** Select your Arduino's port
   - **Baud rate:** `115200`
3. Click **Upload** and wait for completion.

### Method 2: Using avrdude (Command Line)
```bash
avrdude -v -patmega328p -carduino -PCOM3 -b115200 -D -Uflash:w:Robot_Project_4_0_John_PROTECTED.hex:i
```
> Replace `COM3` with your Arduino's COM port.

### Method 3: Using Arduino IDE
1. Open Arduino IDE.
2. Go to **Sketch → Upload Using Programmer** (requires ISP programmer).
3. Or use **Tools → Burn Bootloader** first if needed.

> ⚠️ **Note:** The `.hex` file is a protected binary. Source code is not included for intellectual property protection.

---

## 🔧 Troubleshooting

| Issue | Solution |
| :--- | :--- |
| Arduino restarts on servo move | Increase `delay()` in `serUp()`/`serDown()` or use external power for servos |
| Robot won't move | Check motor shield connections and ensure battery has charge |
| Bluetooth not connecting | Ensure HC-05 is in pairing mode (blinking LED) and paired to your phone |
| Bin sensor always triggers | Adjust IR sensor sensitivity potentiometer or check mounting position |

---

## 📜 License
This program is created for **Exhibition/Display purposes only**.  
Remixing or commercial use is **not allowed**.

---

## 👤 Author
**John Varghese**  
- YouTube: [@trendy_ned](https://youtube.com/@trendy_ned)  
- Instagram: [@cyber__trinity](https://www.instagram.com/cyber__trinity/) | [@john__varghese](https://www.instagram.com/john__varghese/)
