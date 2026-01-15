# HXD8357D - TFT LCD with Touchscreen (Adafruit) with LVGL #

[![Type badge](https://img.shields.io/badge/Human%20Machine%20Interface-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Human%20Machine%20Interface)

## Summary ##

This example shows an example for integrating the HXD8357D display with the [LVGL library](https://lvgl.io/).

There are several settings to adjust the number draw buffers and buffering/refreshing modes in the LVGL library. For more information, please refer to [this documemtation](https://docs.lvgl.io/8.3/porting/display.html#buffering-modes).

This example uses the source files which are generated from [SquareLine Studio](https://squareline.io/). These files are provided in the `app_ui` folder. SquareLine Studio is a professional and easy-to-use UI editor software to speed up and simplify the UI development. Visiting [this page](https://squareline.io/) for more information.

For testing, you'll need a HXD8357D display breakout, like [this large 3.5" TFT display breakout](https://www.adafruit.com/product/2050).  Make sure that the display you're using has the HXD8357D driver chip!

![HXD8357D TFT display](image/hxd8357d_display.png)

## Table Of Contents ##

- [Required Hardware](#required-hardware)
- [Hardware Connection](#hardware-connection)
- [Setup](#setup)
  - [Create a project based on an example project](#create-a-project-based-on-an-example-project)
  - [Start with an empty example project](#start-with-an-empty-example-project)
- [How It Works](#how-it-works)
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

- 1x [Adafruit HXD8357D - 3.5" TFT LCD with Touchscreen](https://www.adafruit.com/product/2050)

## Hardware Connection ##

The tables below provide an overview of the pin connections.

**Silicon Labs BLE Development Kit:**

| Description | BRD4108A | BRD4314A | BRD2601B | BRD2703A | BRD2704A | BRD2710A | ↔ | Adafruit HXD8357D |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Data / Command Selection | PC6 | PC6 | PC5 | PC8 | PC0 | PC6 | ↔ | D/C |
| Chip select | PC3 | PC3 | PA7 | PC0 | PC1 | PC3 | ↔ | ECSM [9] |
| SPI clock   | PC2 | PC2 | PC1 | PC1 | PC2 | PC2 | ↔ | SCK [2] |
| SPI MISO    | PC1 | PC1 | PC2 | PC2 | PC6 | PC1 | ↔ | MISO [6] |
| SPI MOSI    | PC0 | PC0 | PC3 | PC3 | PC3 | PC0 | ↔ | MOSI [7] |
| GPIO         | PB2 | PB2 | PD2 | PD5 | PD2 | PB2 | ↔ | XP (X+) |
| Analog input | PD2 | PD2 | PC4 | PD4 | PA0 | PD2 | ↔ | YP (Y+) |
| GPIO         | PB3 | PB3 | PB3 | PB1 | PD1 | PB3 | ↔ | YM (Y-) |
| Analog input | PB4 | PB4 | PB2 | PA0 | PD0 | PB4 | ↔ | XM (X-) |

**Silicon Labs Wi-Fi Development Kit:**

| Description | BRD4338A + BRD4002A | BRD2605A | BRD2708A | ↔ | Adafruit HXD8357D |
| --- | --- | --- | --- | --- | --- |
| Data / Command Selection | GPIO_47 [P26] | GPIO_10 [P23] | GPIO_30 [RST] | ↔ | D/C |
| Chip select | GPIO_28 [P31] | GPIO_28 [P9] | GPIO_28 [CS]   | ↔ | CS |
| SPI clock   | GPIO_25 [P25] | GPIO_25 [P3] | GPIO_25 [SCK]  | ↔ | CLK |
| SPI MISO    | GPIO_26 [P27] | GPIO_26 [P5] | GPIO_26 [MISO] | ↔ | MISO |
| SPI MISO    | GPIO_27 [P29] | GPIO_27 [P7] | GPIO_27 [MOSI] | ↔ | MOSI |
| GPIO         | GPIO_7 [P20]     | GPIO_7 [P24]    | GPIO_7 [SCL]    | ↔ | XP (X+) |
| Analog input | ULP_GPIO_1 [P16] | ULP_GPIO_1 [P4] | ULP_GPIO_7 [TX] | ↔ | YP (Y+) |
| GPIO         | GPIO_6 [P19]     | GPIO_6 [P21]    | GPIO_6 [SDA]    | ↔ | YM (Y-) |
| Analog input | ULP_GPIO_8 [P15] | ULP_GPIO_8 [P8] | ULP_GPIO_6 [RX] | ↔ | XM (X-) |

> [!IMPORTANT]
> To be able to communicate with TFT LCD using SPI mode, you need to solder closed the IM2 jumper on the back of the PCB.*

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
>
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **hxd8357d**.

2. Click **Create** button on the example:

    - **Third Party Hardware Drivers - HXD8357D - TFT LCD with Touchscreen (Adafruit) with LVGL** if using without DMA.

    - **Third Party Hardware Drivers - HXD8357D - TFT LCD with Touchscreen (Adafruit) with LVGL + DMA** if using with DMA.

    Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit Board" or "SiWx917-RB4338A Radio Board" using Simplicity Studio v5. Use the default project settings.

2. Copy source files:
   - With Gecko EFR32 SOCs:
     - Copy all files in `app/example/adafruit_tft_lcd_hxd8357d_lvgl/app_ui` except `ui_events_si91x.c` into the project root folder
     - Copy the file `app/example/adafruit_tft_lcd_hxd8357d_lvgl/gecko/app.c` into the project root folder (overwriting the existing files)
   - With SiWx917 SoCs:
     - Copy all files in `app/example/adafruit_tft_lcd_hxd8357d_lvgl/app_ui` except `ui_events_gecko.c` into the project root folder
     - Copy the file `app/example/adafruit_tft_lcd_hxd8357d_lvgl/gecko/app.c` into the project root folder (overwriting the existing files)

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - **With Gecko EFR32 SOCs:**
      - [Services] → [Timers] → [Sleep Timer]
      - [Platform] → [Driver] → [LED] → [Simple LED] → [led0, led1]
      - [Platform] → [Driver] → [Button] → [Simple Button] → [btn0, btn1]
      - [Third Party Hardware Drivers] → [Human Machine Interface] → [Touch Screen Analog Interface (Gecko)] → use the default configuration
   - **With SiWx917 SoCs:**
      - [WiSeConnect 3 SDK] → [Device] → [MCU] → [Service] → [Power Manager] → [Sleep Timer for Si91x]
      - [WiSeConnect 3 SDK] → [Device] → [MCU] → [Hardware] → [LED] → [led0, led1]
      - [WiSeConnect 3 SDK] → [Device] → [MCU] → [Hardware] → [Button] → [btn0, btn1]
      - [Third Party Hardware Drivers] → [Human Machine Interface] → [Touch Screen Analog Interface (Si91x)] → use the default configuration

   - **If using single buffer mode without DMA:**
     - [Third Party Hardware Drivers] → [Display & LED] → [HXD8357D - TFT LCD Display (Adafruit) - SPI]
   - **If using double buffered DMA mode:**
     - [Third Party Hardware Drivers] → [Display & LED] → [HXD8357D - TFT LCD Display (Adafruit) - SPI with DMA]
   - [Third Party Hardware Drivers] → [Human Machine Interface] → [Touch Screen (Analog)]
   - [Third Party Hardware Drivers] → [Services] → [LVGL - Graphic Library] → Using settings as below:
     ![Create_example](image/double_buffered_lvgl_settings.png)

4. Enable DMA support for SPI module (for SiWx917 SoCs)

   To improve SPI transfer speed, enable DMA support by changing configuration of GSPI component at: **[WiSeConnect 3 SDK] → [Device] → [Si91X] → [MCU] → [Peripheral] → [GSPI]** as the picture bellow

   | | |
   | - | - |
   | ![gspi](image/gspi.png) | ![gspi_dma](image/gspi_dma.png) |

5. Build and flash the project to your device.

## Calibration for Touch function ##

Adafruit HXD8357D uses 4 resistive touch pins (Y+ X+ Y- X-) to determine touch points. We will read the analog values from these pins to detect where the touched point is on the screen. This process will surely have uncertainties so we have to calibrate it to detect touched points properly. Please follow the steps below to calibrate the touch screen.

- Open configuration for Touch Screen (Analog) component. Enable **calibration printf** option.

  ![enable calib printf](image/enable_calib_printf.png)

- Open the console window to monitor values of the 'x' and 'y' when touching some special points then update them in CALIBRATION settings.

  ![calib point](image/calib_point.png)

- Touch X1 point then update x for **Calib X-min** and y for **Calib Y-min**.

- Touch X2 point then update x for **Calib X-max**

- Touch X3 point then update y for **Calib Y-max**

## How It Works ##

### Testing ###

The application will show you a interface to control the leds and also monitor the status of both leds and button on board.

![result](image/result.GIF)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
