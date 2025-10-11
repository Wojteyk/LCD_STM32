# Modular UI Framework for STM32 & ST7735S

This project is a demonstration of a modular, event-driven UI framework designed for resource-constrained embedded systems. It showcases best practices in software architecture, hardware abstraction, and driver development for the STM32 platform.

![Photo of the project running on a breadboard]

## Key Features & Skills Demonstrated

* **Modular Architecture**: The project is architected in distinct layers (Application, UI Core, Hardware Abstraction Layer, Drivers), demonstrating a strong understanding of **separation of concerns** and creating portable, testable code.

* **Frame Buffer for Performance**: A complete LCD driver for the ST7735S was written from scratch, utilizing an in-memory **frame buffer** to ensure smooth, flicker-free rendering and fast screen updates.

* **Event-Driven State Machine (FSM)**: User input is handled by a simple but effective Finite State Machine that processes short and long press events. This demonstrates the ability to manage application state cleanly.

* **Dynamic Configuration**: The UI supports on-the-fly customization of **color themes** and **backlight brightness**, showcasing flexible and user-centric design.

* **Professional Tooling**: The entire codebase is documented using **Doxygen**, and the project is version-controlled with **Git**, demonstrating a professional development workflow.

---

## Tech Stack

* **Hardware**: STM32F4 Series Microcontroller, ST7735S SPI LCD Display
* **Language**: C
* **Core Libraries**: STM32 HAL
* **Tools**: STM32CubeIDE, Git

---

## Architecture Overview

The framework's design enforces a clean separation between logic and hardware.

1.  **Application Layer (`main.c`)**: Initializes modules and orchestrates the main event loop.
2.  **UI Core (`ui.c`)**: Manages the high-level state, navigation logic, and decides *what* to draw.
3.  **Hardware Abstraction Layer (`ui_hw.c`)**: Provides a generic interface for hardware like buttons and PWM, decoupling the UI core from specific pins or timers.
4.  **LCD Driver (`lcd.c`)**: A low-level driver that handles all SPI communication and primitive drawing operations for the ST7735S display.

---
