# Neotrellis RGB 4x4 keypad (Adafruit) #

[![Type badge](https://img.shields.io/badge/Access%20Control-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Access%20Control)
[![Type badge](https://img.shields.io/badge/Appliances-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Appliances)
[![Type badge](https://img.shields.io/badge/Factory%20Automation-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Factory%20Automation)
[![Type badge](https://img.shields.io/badge/Human%20Machine%20Interface-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Human%20Machine%20Interface)
[![Type badge](https://img.shields.io/badge/Process%20Automation-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Process%20Automation)
[![Type badge](https://img.shields.io/badge/Smart%20HVAC-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Smart%20HVAC)
[![Type badge](https://img.shields.io/badge/Smart%20Locks-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Smart%20Locks)

## Summary ##

This project aims to implement a hardware driver interacting with the [Adafruit NeoTrellis RGB for 4x4 Keypad](https://www.adafruit.com/product/3954) using Silicon Labs platform.

Adafruit NeoTrellis 4x4 keypad is a trellis 4x4 elastomer-button keypad kit with a PCB with full-color NeoPixel support. These 4x4 button pad boards are fully tile-able and communicate over I2C. With 5 address pins, you get the ability to connect up to 32 together in any arrangement by soldering them edge-to-edge and soldering closed the I2C address jumpers, then use one I2C connection for all tiled NeoTrellis boards. Each NeoTrellis board comes with a seesaw chip and 16 NeoPixels already soldered in place.

## Table Of Contents ##

- [Required Hardware](#required-hardware)
- [Hardware Connection](#hardware-connection)
- [Setup](#setup)
  - [Create a project based on an example project](#create-a-project-based-on-an-example-project)
  - [Start with an empty example project](#start-with-an-empty-example-project)
- [How It Works](#how-it-works)
  - [API Overview](#api-overview)
  - [Testing](#testing)
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

- 1x [Adafruit NeoTrellis RGB for 4x4 Keypad](https://www.adafruit.com/product/3954)

## Hardware Connection ##

For the Silicon Labs boards that feature a Qwiic connector, a [JST PH to JST SH (qwiic) Cable](https://www.adafruit.com/product/4424) is used to connect to the Adafruit NeoTrellis board, as illustrated in the figure below.

![connection](image/connection.png)

For the Silicon Labs boards that do not have a Qwiic connector, consider using the [Qwiic Breadboard Cable](https://www.sparkfun.com/products/14425).

The tables below provide an overview of the pin connections.

**Silicon Labs BLE Development Kit:**

| Description | BRD4108A | BRD4314A | BRD2601B | BRD2703A | BRD2704A | BRD2710A | ↔ | Adafruit NeoTrellis |
| --- | --- | --- | --- | --- | --- | --- | --- |  --- |
| I2C_SDA | PD3 | PD3 | PC5 | PC5 | PB4 | PD3 | ↔ | SDA |
| I2C_SCL | PD2 | PD2 | PC4 | PC4 | PB3 | PD2 | ↔ | SCL |

**Silicon Labs Wi-Fi Development Kit:**

| Description | BRD4338A + BRD4002A | BRD2605A | BRD2708A | ↔ | Adafruit NeoTrellis |
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

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by "NeoTrellis".

2. Click **Create** button on the **Third Party Hardware Drivers - Neotrellis RGB 4x4 keypad (Adafruit)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/adafruit_neotrellis_keypad/app.c` into the project root folder (overwriting the existing file).

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - **If the BLE Development Kit is used**
     - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
     - [Application] → [Utility] → [Log]
     - [Platform] → [Driver] → [I2C] → [I2CSPM] → default instance name: qwiic
     - [Third Party Hardware Drivers] → [Human Machine Interface] → [NeoTrellis 4x4 Keypad (Adafruit)]

   - **If the Wi-Fi Development Kit is used:**
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [I2C] → [i2c2]  → Select the corresponding pins according to the table provided in [Hardware Connection](#hardware-connection)
     - [Third Party Hardware Drivers] → [Human Machine Interface] → [NeoTrellis 4x4 Keypad (Adafruit)]

4. Build and flash the project to your device.

## How It Works ##

### API Overview ###

```c
 ---------------------------------------------
|                  application                |
|---------------------------------------------|
|           adafruit_neotrellis.c             |
|---------------------------------------------|
|         adafruit_seesawneopixel.c           |
|---------------------------------------------|
|             adafruit_seesaw.c               |
|---------------------------------------------|
```

The Adafruit NeoTrellis driver is implemented based on the Adafruit NeoPixel driver for the seesaw chip and the Adafruit seesaw driver.

- `adafruit_neotrellis.c` : Use to control the keypad functions for interacting with multiple neotrellis boards
- `adafruit_seesawneopixel.c`: This file is part of the Adafruit NeoPixel library used for compatibility with the seesaw chip. It helps control a wide variety of WS2811- and WS2812-based RGB LED devices such as Adafruit FLORA RGB Smart Pixels and NeoPixel strips
- `adafruit_seesaw.c`: Use to communicate with the Microcontroller.

### Testing ###

- The application implements a colorful keypad. Each time a button on the keypad is pressed, the corresponding RGB LED lights.

- The RGB color can be customized on the application.

   ![result](image/result_test.gif)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
