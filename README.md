# Automatic Multi-Machine Lubrication System

## Project Overview

This project is a temperature-based automatic lubrication system designed for industrial machines such as lathe, milling, grinding, and shaper. The system uses LM35 temperature sensors to monitor the thermal condition of each machine. When a machine exceeds a specified temperature threshold, its corresponding solenoid valve is activated to lubricate the system for a fixed duration, thereby reducing manual intervention and ensuring consistent performance.

## Author

**Thamarai Kannan M K S**  
B.E. Mechatronics Engineering  
Created: June 2025

## Key Features

- Real-time temperature monitoring using LM35 sensors
- Independent control of lubrication per machine
- Solenoid valves controlled via relay/MOSFET drivers
- Non-blocking timing control using `millis()`
- Modular and scalable design for additional machines

## System Components

- Arduino Uno
- 4 × LM35 Temperature Sensors (Analog)
- 4 × Solenoid Valves (controlled via relay or MOSFET)
- Pneumatic/Hydraulic Actuators (external)
- Hollow Shaft Lubrication Line
- External Power Supply (12V/24V for valves)

## Pin Configuration

| Machine   | Temperature Sensor Pin | Solenoid Valve Pin |
|-----------|------------------------|---------------------|
| Lathe     | A0                     | D4                  |
| Milling   | A1                     | D5                  |
| Grinding  | A2                     | D6                  |
| Shaper    | A3                     | D7                  |

## How It Works

1. The system continuously reads temperature data from LM35 sensors.
2. If a machine exceeds the temperature threshold (45°C), the solenoid valve for that machine is activated.
3. The valve remains ON for a fixed period (5 seconds), after which it automatically shuts off.
4. This logic repeats independently for each machine.

## Files in Repository

- `multi_machine_lubrication.ino` – Main Arduino sketch
- `README.md` – Project documentation
- `2d of project.jpg` – Project design diagram
- `Final product.jpg` – Actual prototype image
- `Applications.jpg` – Use-case documentation
- `Cost estimation.jpg` – Bill of materials (BOM)

## Applications

- Smart lubrication in CNC and manual machines
- Industrial automation of legacy equipment
- Condition-based maintenance in manufacturing environments

## License

This project is intended for academic and learning purposes. For commercial usage or extensions, proper attribution is recommended.
