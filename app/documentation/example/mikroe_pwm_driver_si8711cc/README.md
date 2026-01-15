# Si8711CC - PWM Driver Click (Mikroe) #

[![Type badge](https://img.shields.io/badge/Appliances-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Appliances)
[![Type badge](https://img.shields.io/badge/Entertainment%20Devices-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Entertainment%20Devices)
[![Type badge](https://img.shields.io/badge/Factory%20Automation-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Factory%20Automation)
[![Type badge](https://img.shields.io/badge/Process%20Automation-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Process%20Automation)
[![Type badge](https://img.shields.io/badge/Smart%20Agriculture-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Smart%20Agriculture)
[![Type badge](https://img.shields.io/badge/Smart%20Buildings-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Smart%20Buildings)
[![Type badge](https://img.shields.io/badge/Smart%20Hospitals-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Smart%20Hospitals)
[![Type badge](https://img.shields.io/badge/Smart%20Locks-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Smart%20Locks)

## Summary ##

This project shows the implementation of an "PWM Driver Click" driver using Si8711CC one-channel isolator from Silicon Labs.

If you need to control DC motors with loads up to 10A, PWM driver click is the perfect solution, thanks to the Silicon Labs Si8711CC one-channel isolator. It communicates with the target MCU over PWM pin, and runs on a 5V power supply. It can be used in a wide variety of ways, from controlling light intensity to serving as an ON/OFF switch to control DC load circuits.

The Si87xx isolators are pin-compatible, one-channel, drop-in replacements for popular optocouplers with data rates up to 15 Mbps. These devices isolate highspeed digital signals and offer performance, reliability, and flexibility advantages not available with optocoupler solutions. The Si87xx series is based on proprietary CMOS isolation technology for low-power and high-speed operation and are resistant to the wear-out effects found in optocouplers that degrade performance with increasing temperature, forward current, and device age. As a result, the Si87xx series offer longer service life and dramatically higher reliability compared to optocouplers. Ordering options include open collector output with and without integrated pull-up resistor and output enable options.

The click contains a P-channel DMP3010LK3 MOSFET transistor. When the click is used for PWM control it is not recommended to use it with loads of wattage over 50W because the MOSFET chip can get overheated. However, this does not apply when the click is used as an ON/OFF switch.

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

- 1x [PWM Driver Click board](https://www.mikroe.com/pwm-driver-click) based on Si8711CC one-channel isolator
- 1x [DC Motor](https://www.mikroe.com/dc-gear-motor)

## Hardware Connection ##

The Silicon Labs Explorer Kit boards feature a mikroBUS™ socket, allowing the PWM Driver Click board to connect easily via the mikroBUS header. Ensure that the 45-degree corner of the PWM Driver Click board aligns with the 45-degree white line on the Explorer Kit. The hardware connection is illustrated in the image below.

![board](image/hardware_connection.png)

For the Silicon Labs boards that do not have a mikroBUS™ socket, consider using the Wire Jumpers.

The tables below provide an overview of the pin connections.

**Silicon Labs BLE Explorer Kit:**

| Description | BRD4314A | BRD4108A | BRD2703A | BRD2710A | ↔ | PWM Driver Click Board |
| --- | --- | --- | --- | --- | --- | --- |
| PWM Control | PB4 | PB4 | PA0 | PB4 | ↔ | PWM |

**Silicon Labs Wi-Fi Development Kit:**

| Description | BRD4338A + BRD4002A | BRD2605A | BRD2708A | ↔ | PWM Driver Click Board |
| --- | --- | --- | --- | --- | --- |
| PWM Control | GPIO_7 [P20]  | GPIO_7 [P24]  | GPIO_12 | ↔ | PWM |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
>
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add the your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by "si8711cc"

2. Click **Create** button on the **Third Party Hardware Drivers - Si8711CC - PWM Driver Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_pwm_driver_si8711cc/app.c` into the project root folder (overwriting the existing file).

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - **If BLE Explorer Kit is used:**
     - [Services] → [Timers] → [Sleep Timer]
     - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
     - [Application] → [Utility] → [Log]
     - [Third-Party Hardware Drivers] → [Motor Control] → [Si8711CC - PWM Driver Click (Mikroe)]

   - **If the Wi-Fi Development Kit is used:**
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [PWM] → [channel_0] → use default configuration. **Note**: For the BRD2708A board, PWM channel 3 should be used and configured as illustrated in the figure below.
       ![pwm config](image/pwm_config.png)
     - [Third-Party Hardware Drivers] → [Motor Control] → [Si8711CC - PWM Driver Click (Mikroe)]

4. Build and flash this example to the board.

## How It Works ##

- Use a terminal program, such as the Console that is integrated in Simplicity Studio or a third-party tool terminal like PuTTY to receive the logs from the virtual COM port. Note that your board uses the default baud rate of 115200. In this example, we build a simple application that generates a PWM signal with frequency = 5000Hz, and duty level = 10% at startup. This duty level will be increased by 10% each 2000ms. If the duty level >= 100% then the duty level will be set to 10%.

- A screenshot of the console output is shown in the figure below:

  ![usb_debug](image/log.png "USB Debug Output Data")

  ![result_1](image/result_1.png "Result_1")

  ![result_2](image/result_2.png "Result_2")

  ![result_3](image/result_3.png "Result_3")

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_Drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_Drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
