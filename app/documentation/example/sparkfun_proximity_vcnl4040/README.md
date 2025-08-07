# VCNL4040 - Proximity Sensor (Sparkfun) #

## Summary ##

This project aims to implement a hardware driver interacting with the [VCNL4040 proximity sensor](https://www.vishay.com/ppg?84274) using Silicon Labs platform.

VCNL4040 integrates a proximity sensor (PS), ambient light sensor (ALS), and a high power IRED into one small package. It incorporates photodiodes, amplifiers, and analog to digital converting circuits into a single chip by CMOS process. The 16-bit high resolution ALS offers excellent sensing capabilities with sufficient selections to fulfill most applications whether dark or high transparency lens design. High and low interrupt thresholds can be programmed for both ALS and PS, allowing the component to use a minimal amount of the microcontroller's resources. With the ability to detect objects up to 20 cm, it is often used in automatic towel and soap dispensers or automatic faucets.

The patented Filtron technology achieves ambient light spectral sensitivity closest to real human eye response and offers the best background light cancellation capability (including sunlight) without utilizing the microcontrollers’ resources. This sensor is excellent if you need a digital light sensor for your next glitter cannon.

## Table Of Contents ##

- [Required Hardware](#required-hardware)
- [Hardware Connection](#hardware-connection)
- [Setup](#setup)
  - [Create a project based on an example project](#create-a-project-based-on-an-example-project)
  - [Start with an empty example project](#start-with-an-empty-example-project)
- [How It Works](#how-it-works)
- [Report Bugs & Get Support](#report-bugs--get-support)

## Required Hardware ##

- 1x [Silicon Labs BLE Development Kit](https://www.silabs.com/development-tools/wireless/bluetooth) based on the EFR32 SoC, such as:
  - [BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit)
  - [BG22-EK4108A](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)
  - [xG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)
  - [xG22-EK2710A](https://www.silabs.com/development-tools/wireless/efr32xg22e-explorer-kit?tab=overview)
  - [XG24-DK2601B](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit)
  - [SparkFun Thing Plus Matter - MGM240P](https://www.sparkfun.com/sparkfun-thing-plus-matter-mgm240p.html)

  *or*

  1x [Silicon Labs Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917, such as:
  - [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit)
  - [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board) + [Si-MB4002A](https://www.silabs.com/development-tools/wireless/wireless-pro-kit-mainboard?tab=overview)
  - [SiW917Y-EK2708A](https://www.silabs.com/development-tools/wireless/wi-fi/siw917y-ek2708a-explorer-kit?tab=overview)

- 1x [SparkFun Proximity Sensor Breakout - 20cm, VCNL4040 (Qwiic)](https://www.sparkfun.com/products/15177)

## Hardware Connection ##

For the Silicon Labs boards that feature a Qwiic connector, a [Qwiic Cable](https://www.sparkfun.com/flexible-qwiic-cable-100mm.html) is used to connect to the SparkFun Proximity Sensor board, as illustrated in the figure below.

![connection](image/connection.png)

For the Silicon Labs boards that do not have a Qwiic connector, consider using the [Qwiic Breadboard Cable](https://www.sparkfun.com/products/14425).

The tables below provide an overview of the pin connections.

**Silicon Labs BLE Development Kit:**

| Description | BRD4108A | BRD4314A | BRD2601B | BRD2703A | BRD2704A | BRD2710A | ↔ | SparkFun Proximity Sensor |
| --- | --- | --- | --- | --- | --- | --- | --- |  --- |
| I2C_SDA | PD3 | PD3 | PC5 | PC5 | PB4 | PD3 | ↔ | SDA |
| I2C_SCL | PD2 | PD2 | PC4 | PC4 | PB3 | PD2 | ↔ | SCL |

**Silicon Labs Wi-Fi Development Kit:**

| Description | BRD4338A + BRD4002A | BRD2605A | BRD2708A | ↔ | SparkFun Proximity Sensor |
| --- | --- | --- | --- | --- | --- |
| I2C_SDA | ULP_GPIO_6 [EXP_16] | ULP_GPIO_6 | GPIO_6 | ↔ | SDA |
| I2C_SCL | ULP_GPIO_7 [EXP_15] | ULP_GPIO_7 | GPIO_7 | ↔ | SCL |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
>
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter **"VCNL4040"**.

2. Click **Create** button on the **Third Party Hardware Drivers - VCNL4040 - Proximity Sensor (Sparkfun)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/sparkfun_proximity_vcnl4040/app.c` into the project root folder (overwriting the existing file).

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - **If the BLE Development Kit is used:**
     - [Services] → [Timers] → [Sleep Timer]
     - [Services] →  [IO Stream] → [IO Stream: USART] → instance name: **vcom**
     - [Application] →  [Utility] → [Log]
     - [Platform] → [Driver] → [I2C] → [I2CSPM] → default instance name: **qwiic**
     - [Third Party Hardware Drivers] -> [Sensors] -> [VCNL4040 - Proximity Sensor (Sparkfun)] → use default configuration

   - **If the Wi-Fi Development Kit is used:**
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [I2C] → [i2c2] → Select the corresponding pins according to the table provided in [Hardware Connection](#hardware-connection)
     - [Third Party Hardware Drivers] -> [Sensors] -> [VCNL4040 - Proximity Sensor (Sparkfun)] → use default configuration

4. Build and flash the project to your device

## How It Works ##

### API Overview ###

The driver is divided into two layers: platform and interface layer. The platform layer performs the interaction between the sensor and the microcontroller by I2C protocol (I2CSPM platform service). The interface layer provides users public APIs to communicate with or control the operation of the sensor.

[sparkfun_vcnl4040_platform.c](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/tree/master/driver/public/silabs/proximity_vcnl4040/src/sparkfun_vcnl4040_platform.c) : Implementation for communication between the microcontroller and sensor board through the Silabs I2CSPM platform service.

[sparkfun_vcnl4040.c](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/tree/master/driver/public/silabs/proximity_vcnl4040/src/sparkfun_vcnl4040.c) : Implements public APIs to interface with VCNL4040 - Sparkfun Proximity Sensor.

[sparkfun_vcnl4040_config.h](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/tree/master/driver/public/silabs/proximity_vcnl4040/config/sparkfun_vcnl4040_config.h) : Defines the configuration for interrupt operation mode.

### Testing ###

The below chart represents the workflow of a simple testing program. The left chart shows the initialization steps that are needed before reading data and the right chart shows the periodic measuring process.

![flowchart](image/flowchart.png)

Use Putty or another program to read the serial output.
The BGM220P uses by default a baudrate of 115200. You should expect a similar output to the one below.

![console](image/test.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
