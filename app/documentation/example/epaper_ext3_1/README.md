# E-Paper EXT3-1 #

[![Type badge](https://img.shields.io/badge/Battery%20Powered%20Tools-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Battery%20Powered%20Tools)
[![Type badge](https://img.shields.io/badge/Electronic%20Shelf%20Labels-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Electronic%20Shelf%20Labels)
[![Type badge](https://img.shields.io/badge/Human%20Machine%20Interface-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Human%20Machine%20Interface)
[![Type badge](https://img.shields.io/badge/Industrial%20Wearables-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Industrial%20Wearables)
[![Type badge](https://img.shields.io/badge/Portable%20Medical%20Devices-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Portable%20Medical%20Devices)
[![Type badge](https://img.shields.io/badge/Smart%20Wearable%20Devices-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Smart%20Wearable%20Devices)

## Summary ##

Electronic paper (E-Paper) or intelligent paper, is a display device that reflects ambient light, mimicking the appearance of ordinary ink on paper. EPD Extension Kit Generation 3 (EXT3 Kit) is a new extension board that connects to E-Paper Display (EPD).
This project aims to implement a hardware driver interacting with the EXT3-1 Kit to control an E-Paper Display.

## Table Of Contents ##

- [Required Hardware](#required-hardware)
- [Hardware Connection](#hardware-connection)
- [Setup](#setup)
  - [Create a project based on an example project](#create-a-project-based-on-an-example-project)
  - [Start with an empty example project](#start-with-an-empty-example-project)
- [How It Works](#how-it-works)
- [Report Bugs & Get Support](#report-bugs--get-support)

## Required Hardware ##

- 1x [Silicon Labs BLE Explorer Kit](https://www.silabs.com/development-tools/wireless/bluetooth) based on the EFR32 SoC, such as:
  - [BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit)
  - [BG22-EK4108A](https://www.silabs.com/development-tools/wireless/bluetooth/bg22-explorer-kit?tab=overview)
  - [xG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)
  - [xG22-EK2710A](https://www.silabs.com/development-tools/wireless/efr32xg22e-explorer-kit?tab=overview)

  *or*

  1x [Silicon Labs Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917, such as:
  - [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit)
  - [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board) + [Si-MB4002A](https://www.silabs.com/development-tools/wireless/wireless-pro-kit-mainboard?tab=overview)
  - [SiW917Y-EK2708A](https://www.silabs.com/development-tools/wireless/wi-fi/siw917y-ek2708a-explorer-kit?tab=overview)

- 1x [EPD Extension Kit Generation 3 revision 1](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-3-ext3/#tab-3)
- 1x [5.81″ E-ink displays](https://www.pervasivedisplays.com/product/5-81-e-ink-displays/)

## Hardware Connection ##

The tables below provide an overview of the pin connections.

**Silicon Labs BLE Explorer Kit:**

| Pin function | BRD4108A | BRD4314A | BRD2703A | BRD2710A | ↔ | EXT3-1 Kit |
| --- | --- | --- | --- | --- | --- | --- |
| VCC 3.3V  | 3V3 | 3V3 | 3V3 | 3V3 | ↔ | 3V3 |
| GND | GND | GND | GND | GND | ↔ | GND |
| Master chip select pin of EPD | PC3 | PC3 | PC0 | PC3 | ↔ | ECSM [9] |
| SPI SCK | PC2 | PC2 | PC1 | PC2 | ↔ | SCK [2] |
| SPI MISO | PC1 | PC1 | PC2 | PC1 | ↔ | MISO [6] |
| SPI MOSI | PC0 | PC0 | PC3 | PC0 | ↔ | MOSI [7] |
| Serial bus for controlling data or command | PB4 | PB4 | PA0 | PB4 | ↔ | D/C [4] |
| Reset signal input | PB2 | PB2 | PD5 | PB2 | ↔ | RST [5] |
| Busy state output pin | PB3 | PB3 | PB1 | PB3 | ↔ | BUSY [3] |

**Silicon Labs Wi-Fi Development Kit:**

| Pin function | BRD4338A + BRD4002A | BRD2605A | BRD2708A | ↔ | EXT3-1 Kit |
| --- | --- | --- | --- | --- | --- |
| VCC 3.3V  | 3V3 | 3V3 [P20] | 3V3 | ↔ | 3V3 |
| GND | GND | GND [P1] | GND | ↔ | GND |
| Master chip select pin of EPD | GPIO_28 [P31]| GPIO_28 [P9] | GPIO_28 [CS] | ↔ | ECSM [9] |
| SPI SCK | GPIO_25 [P25] | GPIO_25 [P3] | GPIO_25 [SCK] | ↔ | SCK [2] |
| SPI MISO | GPIO_26 [P27] | GPIO_26 [P5] | GPIO_26 [MISO] | ↔ | MISO [6] |
| SPI MOSI | GPIO_27 [P29] | GPIO_27 [P7] | GPIO_27 [MOSI] | ↔ | MOSI [7] |
| Serial bus for controlling data or command | GPIO_46 [P24] | GPIO_10 [P23] | GPIO_29 [AN] | ↔ | D/C [4] |
| Reset signal input | GPIO_48 [P28] | GPIO_12 [P25] | GPIO_30 [RST] | ↔ | RST [5] |
| Busy state output pin | GPIO_47 [P26] | GPIO_11 [P22] | GPIO_12 [PWM] | ↔ | BUSY [3] |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
>
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **ext3-1**.

2. Click **Create** button on **Third Party Hardware Drivers - E-paper EXT3-1** examples. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit Board" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/epaper_ext3_1/app.c` and `app/example/epaper_ext3_1/silabs_logo.h` into the project root folder (overwriting the existing file).

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - [Third Party Hardware Drivers] → [Display & LED] → [E-Paper EXT3-1 (Pervasive Displays)]
   - [Third Party Hardware Drivers] → [Services] → [GLIB - OLED Graphics Library]

4. Build and flash the project to your device.

## How It Works ##

E-paper (also called ePaper) is an electronic display technology that mimics the appearance of paper. Using the same inks as the traditional printing industry, e-paper displays (EPDs) have tiny capsules filled with charged ink particles. When the proper charge is applied, an EPD creates highly detailed images with the contrast ratio and readability of printed material.

### Testing ###

The E-paper will look something like the picture below.

![result](image/result.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
