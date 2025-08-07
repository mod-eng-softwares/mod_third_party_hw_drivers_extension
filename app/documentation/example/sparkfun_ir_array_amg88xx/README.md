# AMG8833 - Grid-EYE Infrared Array Breakout (Sparkfun) #

## Summary ##

This project shows the implementation of the [Panasonic amg88xx infrared array](https://industry.panasonic.eu/components/sensors/industrial-sensors/grid-eye/amg88xx-high-performance-type/amg8833-amg8833) driver using the [EFR32xG24 Dev Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit?tab=overview) and the [SparkFun Grid-EYE AMG8833 infrared array](https://www.sparkfun.com/products/14607). The driver includes every known functionality of the amg88xx device, such as sensor readings, power settings, interrupt setup, and more.

The Grid-EYE from Panasonic is an 8x8 thermopile array. This means you have a square array of 64 pixels each capable of independent temperature detection. It’s like having thermal camera (or Predator’s vision), just in really low resolution. It's part of SparkFun's Qwiic system, so it is easier to connect to get your low-resolution infrared image.

## Table Of Contents ##

- [Required Hardware](#required-hardware)
- [Hardware Connection](#hardware-connection)
- [Setup](#setup)
  - [Create a project based on an example project](#create-a-project-based-on-an-example-project)
  - [Start with an empty example project](#start-with-an-empty-example-project)
- [How It Works](#how-it-works)
- [Generating image with Python](#generating-image-with-python)
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

- 1x [SparkFun Grid-EYE Infrared Array Breakout - AMG8833 (Qwiic)](https://www.sparkfun.com/products/14607)

## Hardware Connection ##

For the Silicon Labs boards that feature a Qwiic connector, a [Qwiic Cable](https://www.sparkfun.com/flexible-qwiic-cable-100mm.html) is used to connect to the SparkFun Grid-EYE Infrared Array Breakout board, as illustrated in the figure below.

![connection](image/connection.png)

For the Silicon Labs boards that do not have a Qwiic connector, consider using the [Qwiic Breadboard Cable](https://www.sparkfun.com/products/14425).

The tables below provide an overview of the pin connections.

**Silicon Labs BLE Development Kit:**

| Description | BRD4108A | BRD4314A | BRD2601B | BRD2703A | BRD2704A | BRD2710A | ↔ | SparkFun Human Presence Sensor Breakout |
| --- | --- | --- | --- | --- | --- | --- | --- |  --- |
| I2C_SDA | PD3 | PD3 | PC5 | PC5 | PB4 | PD3 | ↔ | SDA |
| I2C_SCL | PD2 | PD2 | PC4 | PC4 | PB3 | PD2 | ↔ | SCL |

**Silicon Labs Wi-Fi Development Kit:**

| Description | BRD4338A + BRD4002A | BRD2605A | BRD2708A | ↔ | SparkFun Human Presence Sensor Breakout |
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

1. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by *amg88xx*.

2. Click Create button on the project **Third Party Hardware Drivers - AMG88XX - Grid-EYE Infrared Array Breakout (Sparkfun)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the your board using Simplicity Studio v5. Use the default project settings.

2. Copy the following files into the project root folder (overwriting the existing file):

   - `app/example/sparkfun_ir_array_amg88xx/app.c`
   - `app/example/sparkfun_ir_array_amg88xx/app_cli_command_table.c`
   - `app/example/sparkfun_ir_array_amg88xx/app_ir_array_cli_cmds.c`

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - **If the BLE Development Kit is used:**
     - **[Services] → [IO Stream] → [Driver] → [IO Stream: USART]** → default instance name: vcom
     - **[Services] → [IO Stream] → [IO Stream: Retarget STDIO]**
     - **[Services] → [Command Line interface] → [CLI Instance(s)]** → new instance name: vcom
     - **[Platform] → [Driver] → [I2C] → [I2CSPM]** → default instance name: qwiic
     - **[Third Party Hardware Drivers] → [Sensors] → [AMG88XX - Grid-EYE Infrared Array Breakout (Sparkfun)]** → use default configuration

   - **If the Wi-Fi Development Kit is used:**
     - **[Platform] → [Utilities] → [Atomic Operations Library]**
     - **[Services] → [IO Stream] → [Driver] → [IO Stream: Si91x UART]** → default instance name: vcom
     - **[Services] → [IO Stream] → [IO Stream: Retarget STDIO]**
     - **[Services] → [Command Line interface] → [CLI Instance(s)]** → new instance name: vcom
     - **[WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [I2C] → [i2c2]** → Select the corresponding pins according to the table provided in [Hardware Connection](#hardware-connection)
     - **[WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [IO Stream Si91x]**
     - **[Third Party Hardware Drivers] → [Sensors] → [AMG88XX - Grid-EYE Infrared Array Breakout (Sparkfun)]** → use default configuration

4. Build and flash the project to your device.

5. Launch a terminal or console, open the communication to your device.

6. The device communicates over CLI. If you type help, you will see a description how to control the device.

## How It Works ##

The following diagram presents the operation of the driver.

![Usage example](image/IR_array_struct.png)

The application only needs to call `amg88xx_init()` function once at startup. With `amg88xx_get_sensor_array_temperatures()` function, it is possible to request an array of temperatures for all 64 pixels. For more features or possibilities, please refer to the API function descriptions found in amg88xx_get_sensor_array_temperatures.h.

The amg88xx_init() requires 2 parameters: an I2CSPM instance and an I2C server address.
The I2CSPM instance is created by adding the I2CSPM software component. The I2C address for the AMG8833 module is 0x69, so it's recommended to use the AMG88XX_ADDRESS_OPEN macro. You have to select the temperature scale (either Celsius, or Fahrenheit) to display the temperature.

However, it's possible to change the server address by entering a custom address as a parameter.

## File and API Documentation ##

`ir_array_amg88xx_driver.c` - This is the top-level API implementation.

- `amg88xx_init` : Initialise the driver.
- `amg88xx_get_thermistor_temperature` : Gets the thermistor temperature.
- `amg88xx_get_sensor_array_temperatures` : Gets the temperatures of the IR sensor array.
- `amg88xx_wake` : Puts the device to normal mode from any other state.
- `amg88xx_sleep` : Puts device into sleep mode.
- `amg88xx_60_sec_standby` : Puts the device into 60 seconds update interval mode.
- `amg88xx_10_sec_standby` : Puts the device into 10 seconds update interval mode.
- `amg88xx_enable_moving_average` : Enables "Twice Moving Average".
- `amg88xx_disable_moving_average` : Disables "Twice Moving Average".

## Generating image with Python ##

There is a Python script included in the repository `temperatue_array_visualiser.py` which could control the device over serial port and print out the actual temperature array. In the script, you can setup your serial port number.
The required Python package for the app: numpy, seaborn, matplotlib.pylab, pyserial.

Below is the thermal image which is generated by the [temperatue_array_visualiser.py](temperatue_array_visualiser.py) script.

![thermal_image](image/thermal_image.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
