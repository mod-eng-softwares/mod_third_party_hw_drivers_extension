# CMT-8540s-SMT - Buzz 2 Click (Mikroe) #

[![Type badge](https://img.shields.io/badge/Appliances-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Appliances)
[![Type badge](https://img.shields.io/badge/Asset%20Tracking-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Asset%20Tracking)
[![Type badge](https://img.shields.io/badge/Direction%20Finding-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Direction%20Finding)
[![Type badge](https://img.shields.io/badge/Entertainment%20Devices-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Entertainment%20Devices)
[![Type badge](https://img.shields.io/badge/Human%20Machine%20Interface-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Human%20Machine%20Interface)
[![Type badge](https://img.shields.io/badge/Loss%20Prevention-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Loss%20Prevention)
[![Type badge](https://img.shields.io/badge/Smart%20Buildings-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Smart%20Buildings)
[![Type badge](https://img.shields.io/badge/Smart%20Hospitals-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Smart%20Hospitals)

## Summary ##

This project shows the driver implementation of a magnetic buzzer using the CMT-8540S-SMT with Silicon Labs Platform. The buzzer’s resonant frequency is 4kHz. The click is designed to run on either 3.3V or 5V power supply. The buzzer is ideal for extending your prototype environment with audio signals.

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

- 1x [Buzz 2 Click board](https://www.mikroe.com/buzz-2-click) based on CMT-8540S-SMT

## Hardware Connection ##

The Silicon Labs Explorer Kit boards feature a mikroBUS™ socket, allowing the Buzz 2 Click board to connect easily via the mikroBUS header. Ensure that the 45-degree corner of the Buzz 2 Click board aligns with the 45-degree white line on the Explorer Kit. The hardware connection is illustrated in the image below.

![board](image/hardware_connection.png)

For the Silicon Labs boards that do not have a mikroBUS™ socket, consider using the Wire Jumpers.

The tables below provide an overview of the pin connections.

**Silicon Labs BLE Explorer Kit:**

| Description | BRD4314A | BRD4108A | BRD2703A | BRD2710A | ↔ | Buzz 2 Click Board |
| --- | --- | --- | --- | --- | --- | --- |
| PWM | PB4 | PB4 | PA0 | PB4 | ↔ | OE |

**Silicon Labs Wi-Fi Development Kit:**

| Description | BRD4338A + BRD4002A | BRD2605A | BRD2708A | ↔ | Buzz 2 Click Board |
| --- | --- | --- | --- | --- | --- |
| PWM | GPIO_7 [P20] | GPIO_7 [P24] | GPIO_12 | ↔ | OE |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
>
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by *buzz*.

2. Click **Create** button on the **Third Party Hardware Drivers - CMT-8540S-SMT - Buzz 2 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_buzz2_cmt_8540s_smt/app.c` into the project root folder (overwriting the existing file).

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - **If the BLE Explorer Kit is used:**
     - [Services] → [Timers] → [Sleep Timer]
     - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
     - [Application] → [Utility] → [Log]
     - [Third Party Hardware Drivers] → [Mikroe Click] → [Audio & Voice] → [CMT-8540S-SMT - Buzz 2 Click (Mikroe)]

   - **If the Wi-Fi Development Kit is used:**
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [PWM] → [channel_0] → Use default configuaration. **Note**: For the BRD2708A board, PWM channel 3 should be used and configured as illustrated in the figure below.
       ![pwm config](image/pwm_config.png)
     - [Third Party Hardware Drivers] → [Mikroe Click] → [Audio & Voice] → [CMT-8540S-SMT - Buzz 2 Click (Mikroe)]

4. Build and flash this example to the board.

## How It Works ##

The buzzer plays a pre-programmed melody. You can hear this melody when running the program.

You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like Tera Term to receive the data from the USB. A screenshot of the console output is shown in the figure below.

![logging_screen](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
