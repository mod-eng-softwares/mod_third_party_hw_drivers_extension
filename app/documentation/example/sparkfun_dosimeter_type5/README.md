# Pocket Geiger Radiation - Type 5 (Sparkfun) #

## Overview ##

This project aims to show the hardware driver that is used to interface with the Type5 Geiger Sensor using Silicon Labs platform.

The Type 5 Pocket Geiger Radiation Sensor from Radiation Watch is a highly sensitive radiation sensor designed for the embedded systems market. It is capable of detecting Gamma and Beta radiation. The sensor has a simple pulsed output that can be used with any microcontroller. It is beneficial for detecting the levels of ionizing radiation, used as a medical equipment, and as personal dosimeters. By combining this sensor with Silicon Labs wireless technology, it is possible to send the collected information to a data center or trigger a remote protection system.

> [!NOTE]
> These small Geiger sensors feature a measurement range of 0.05uSv/h to 10mSv/h at 0.01cpm to 300Kcpm with a required measurement time of two minutes.

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

- 1x Pocket Geiger Radiation Sensor - Type 5

## Hardware Connection ##

Connect the Pocket Geiger Type 5 board to the Silicon Labs Kit through GPIO.

![connection](image/connection.png)

The tables below provide an overview of the pin connections.

**Silicon Labs BLE Development Kit:**

| Description | BRD4108A | BRD4314A | BRD2601B | BRD2703A | BRD2704A | BRD2710A | ↔ | Pocket Geiger Radiation Sensor - Type 5 |
| --- | --- | --- | --- | --- | --- | --- | --- |  --- |
| DC 3V~9V  | 5V  | 5V  | 5V  | 5V  | 5V  | 5V  | ↔ | +V  |
| GND       | GND | GND | GND | GND | GND | GND | ↔ | GND |
| Radiation-detection pulse pin | PB3 | PB3 | PB2 | PB1 | PB1 | PB3 | ↔ | SIG |
| Noise-detection pulse pin     | PB2 | PB2 | PD3 | PD5 | PB0 | PB2 | ↔ | NS  |

**Silicon Labs Wi-Fi Development Kit:**

| Description | BRD4338A + BRD4002A | BRD2605A | BRD2708A | ↔ | Pocket Geiger Radiation Sensor - Type 5 |
| --- | --- | --- | --- | --- | --- |
| DC 3V~9V  | 5V  | 5V  | 5V  | ↔ | +V |
| GND       | GND | GND | GND | ↔ | GND |
| Radiation-detection pulse pin | GPIO_46 [P24] | GPIO_10 [P23] | GPIO_27 [MOSI] | ↔ | SIG |
| Noise-detection pulse pin     | GPIO_47 [P26] | GPIO_11 [P22] | GPIO_26 [MISO] | ↔ | NS  |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
>
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by *radiation*.

2. Click **Create** button on the **Third Party Hardware Drivers - Pocket Geiger Radiation Sensor - Type 5 (Sparkfun)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "BGM220 Explorer Kit Board" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/sparkfun_dosimeter_type5/app.c` into the project root folder (overwriting existing file).

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - **If the BLE Development Kit is used:**
     - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
     - [Application] → [Utility] → [Log]
     - [Third Party Hardware Drivers] → [Sensors] → [Pocket Geiger Radiation - Type 5 (Sparkfun)] → use default configuration

   - **If the Wi-Fi Development Kit is used:**
     - [Third Party Hardware Drivers] → [Sensors] → [Pocket Geiger Radiation - Type 5 (Sparkfun)] → use default configuration

4. Enable **Printf float**

   - Open Properties of the project.
   - Select C/C++ Build → Settings → Tool Settings → GNU ARM C Linker → General → Check **Printf float**.
     ![float](image/float.png)

5. Build and flash this example to the board.

## How It Works ##

### API Overview ###

`sparkfun_type5_config.h`: Used to configure the GPIO Pins for the sensor and some specific values for the radiation measurement process. You need to config 2 pins (SIGNAL and NOISE), the period process, history length and the history unit.

`sparkfun_type5.h`: Containing public APIs of the sensor that will be called by the application.

`sparkfun_type5.c`: The source file of the driver which contains the implementation of all the public functions that will be used by users.

### Testing ###

The application monitors the Pocket Geiger through interrupts, processes and allows to get back the data using a callback function.
Use Putty or other program to read the serial output. The board uses by default a baudrate of 115200. You should expect a similar output to the one below.

![logging_screen](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
