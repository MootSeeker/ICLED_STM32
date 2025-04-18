# ✨ ICLED STM32 Library

A **lightweight driver library for controlling Würth eiSos ICLED matrices using STM32 timers and DMA**.  
This project enables precise PWM-based LED control, ideal for **visual effects**, **matrix animations**, and **embedded lighting systems** on STM32 microcontrollers.

[![forthebadge](https://forthebadge.com/images/badges/powered-by-coffee.svg)](https://forthebadge.com)  
[![forthebadge](https://forthebadge.com/images/badges/made-with-c.svg)](https://forthebadge.com)  
[![forthebadge](https://forthebadge.com/images/badges/open-source.svg)](https://forthebadge.com)  
[![forthebadge](https://forthebadge.com/images/badges/license-mit.svg)](https://forthebadge.com)  

---

## 📌 Features

- ✅ **DMA-based PWM output** using STM32 TIM1
- 🎨 **24-bit GRB color control** per LED
- 💡 **105 LEDs supported** out-of-the-box (configurable)
- 🌀 Built-in **animations**:
  - Knight Rider  
  - Color Fade  
  - Starfield  
  - Snake pattern
- ↻ **Effect switching** via GPIO interrupt
- 💻 Fully documented with **Doxygen**
- ⚙️ Works with STM32CubeIDE and HAL

---

# 🚀 Getting Started

## 1️⃣ Clone the repository

```bash
git clone https://github.com/MootSeeker/ICLED_STM32.git
```

## 2️⃣ Open in STM32CubeIDE

Import the project as an existing STM32Cube project.

## 3️⃣ Configure the hardware

- Use Timer 1, Channel 1 (TIM1 CH1), mapped to PA8.
- Make sure DMA is enabled for this timer channel in STM32CubeMX.
- Ensure system clock is **32 MHz**

## 4️⃣ Build and flash

Build the project and flash it to your STM32 board via ST-Link or DFU.

## 5️⃣ Watch it glow! 🌈

Button `S2` (on `GPIOB | GPIO_PIN_5`) cycles through the built-in effects.

---

## 📦 File Structure

```text
Core/
├── Src/
│   ├── icled.c             # LED driver logic
├── Inc/
│   ├── icled.h             # Public driver API

Examples/
├── example_app.c       # Demo effects & main animation handler
├── example_app.h       # Effect function prototypes
```

---

## 🔧 Hardware Requirements

| Component         | Purpose                         |
|------------------|----------------------------------|
| STM32 MCU        | With Timer & DMA support         |
| ICLED Matrix     | Würth eiSos 7x15 or similar      |
| Button (e.g. S2) | Used to switch LED effects       |
| 5V Power Supply  | Powers the LED matrix            |

---

## 🎨 Included Effects

- `ICLED_NightRideDemo()` – Classic red dot scanner  
- `ICLED_KnightRiderColorFade()` – Warm glowing red/orange trail  
- `ICLED_StarfieldEffect()` – Cyan background with blinking stars  
- `ICLED_SnakePattern()` – Snake movement with direction and length logic

---

## 📘️ Documentation

The entire library is documented with [**Doxygen**](https://www.doxygen.nl/).  
Generate docs using:

```bash
doxygen Doxyfile
```

---

## 🏆 License

This project is licensed under the **MIT License** – Use it, share it, improve it! ✌️  
See [LICENSE](https://github.com/MootSeeker/ICLED_STM32/blob/main/LICENSE) for details.

---

📢 **Questions or ideas?** Open an issue on  
👉 [github.com/MootSeeker/ICLED_STM32](https://github.com/MootSeeker/ICLED_STM32)

Let it shine! 💡
