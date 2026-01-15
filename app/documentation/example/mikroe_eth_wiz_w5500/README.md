# W5500 - ETH WIZ Click (Mikroe) #

[![Type badge](https://img.shields.io/badge/IoT%20Gateways-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=IoT%20Gateways)
[![Type badge](https://img.shields.io/badge/WiFi%20Access%20Points-salmon)](https://siliconlabs-massmarket.github.io/repository-catalog/#applications-list?filter=Wi--Fi%20Access%20Points)

## Summary ##

This project aims to implement a hardware driver for the W5500 ethernet controller that is integrated on the ETH Wiz click board. The W5500 chip is a Hardwired TCP/IP embedded Ethernet controller that enables easier internet connection for embedded systems using SPI (Serial Peripheral Interface).

W5500 enables users to have the Internet connectivity in their applications just by using the single chip to implement TCP/IP Stack, 10/100 Ethernet MAC and PHY. Hardwired TCP/IP stack supports TCP, UDP, IPv4, ICMP, ARP, IGMP, and PPPoE. W5500 uses a 32Kbytes internal buffer as its data communication memory. For more information about the W5500, see the following [source](https://docs.wiznet.io/img/products/w5500/w5500_ds_v109e.pdf).

The board can be used for industrial automation systems, IP set-top boxes, VoIP/Video phone systems, security systems, home networks and gateways and test and measurement equipment and for many other applications.

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

- 1x [ETH WIZ Click board](https://www.mikroe.com/eth-wiz-click)
- 1x Ethernet Cable, e.g. [Ethernet Roll Cable](https://www.mikroe.com/ethernet-roll-transparent)

## Hardware Connection ##

The Silicon Labs Explorer Kit boards feature a mikroBUS™ socket, allowing the ETH WIZ Click board to connect easily via the mikroBUS header. Ensure that the 45-degree corner of the ETH WIZ Click board aligns with the 45-degree white line on the Explorer Kit. The hardware connection is illustrated in the image below.

![board](image/board.png)

For the Silicon Labs boards that do not have a mikroBUS™ socket, consider using the Wire Jumpers.

The tables below provide an overview of the pin connections.

**Silicon Labs BLE Explorer Kit:**

| Description | BRD4314A | BRD4108A | BRD2703A | BRD2710A | ↔ | ETH WIZ Click |
| --- | --- | --- | --- | --- | --- | --- |
| SPI CS PIN  | PC3 | PC3 | PC0 | PC3 | ↔ | CS  |
| SPI CLK PIN | PC2 | PC2 | PC1 | PC2 | ↔ | SCK |
| SPI RX PIN  | PC1 | PC1 | PC2 | PC1 | ↔ | SDO |
| SPI TX PIN  | PC0 | PC0 | PC3 | PC0 | ↔ | SDI |
| Reset       | PC6 | PC6 | PC8 | PC6 | ↔ | RST |

**Silicon Labs Wi-Fi Development Kit:**

| Description | BRD4338A + BRD4002A | BRD2605A | BRD2708A | ↔ | ETH WIZ Click |
| --- | --- | --- | --- | --- | --- |
| RTE_GSPI_MASTER_CLK_PIN  | GPIO_25 [P25] | GPIO_25 [P3] | GPIO_25 | ↔ | SCK |
| RTE_GSPI_MASTER_MISO_PIN | GPIO_26 [P27] | GPIO_26 [P5] | GPIO_26 | ↔ | SDO |
| RTE_GSPI_MASTER_MOSI_PIN | GPIO_27 [P29] | GPIO_27 [P7] | GPIO_27 | ↔ | SDI |
| GPIO_CS                  | GPIO_28 [P31] | GPIO_28 [P9] | GPIO_28 | ↔ | CS  |
| Reset           | GPIO_30 [P35] | GPIO_30 [P13] | GPIO_30 | ↔ | RST |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
>
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add the your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by "w5500".

2. Click **Create** button on the **Third Party Hardware Drivers - W5500 - ETH Wiz Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_eth_wiz_w5500/app.c` into the project root folder (overwriting the existing file).

3. Open the .slcp file. Select the **SOFTWARE COMPONENTS** tab and install the following components:

   - **If the BLE Explorer Kit is used:**
     - [Services] → [IO Stream] → [IO Stream: EUSART] with the default instance name: **vcom**
     - [Application] → [Utility] → [Log]
     - [Application] → [Utility] → [Assert]
     - [Platform] → [Driver] → [SPI] → [SPIDRV] → [mikroe] → change the configuration for [SPI master chip select (CS) control scheme] to "CS controlled by the application"
     - [Third Party Hardware Drivers] → [Interface] → [W5500 - ETH WIZ Click (Mikroe)] → use the default configuration

   - **If the Wi-Fi Development Kit is used:**
     - [Third Party Hardware Drivers] → [Interface] → [W5500 - ETH WIZ Click (Mikroe)] → use the default configuration
     - [Application] → [Utility] → [Assert]
     - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [GSPI] → Configure a different pin as CS0 to replace GPIO 28 (e.g. GPIO 49), since GPIO 28 is already managed by the [W5500 - ETH WIZ Click (Mikroe)] component

4. Build and flash the project to your device.

## How It Works ##

### API Overview ###

A detailed description of each function can be found in doc/doxygen.

The driver is divided into an Application layer, a TCP/IP stack and a Physical layer.
The Application layer is where applications requiring network communications live. Examples of these applications include email clients and web browsers. These applications use the TCP/IP stack to send requests to connect to remote hosts.

The TCP/IP stack establishes the connection between applications running on different hosts. It uses TCP for reliable connections and UDP for fast connections. It keeps track of the processes running in the applications above it by assigning port numbers to them and uses the Network layer to access the TCP/IP network.

The physical layer provides integration to the host microcontroller hardware-dependent codes.

![software_layers](image/software_layers.png)

`dhcp.c`: DHCP library. The dynamic host configuration protocol (DHCP) is the application responsible for requesting and offering IP addresses.

`dns.c`: DNS library. A Domain Name System (DNS) enables to browse a website by providing the website or domain name instead of the website’s IP address.

`sntp.c`: SNTP library. SNTP stands for Simple Network Time Protocol, which is a service that provides the time of day to network devices. Typical accuracy is in the range of hundreds of milliseconds.

`http_server.c`: HTTP server library. The Hypertext Transfer Protocol (HTTP) is the most commonly used TCP/IP application as it transfers web pages from a web server to a web browser.

`socket.c`: Enables applications to connect to a Transmission Control Protocol/Internet Protocol (TCP/IP) network.

`ethernet_udp.c`: Library to send/receive UDP packets.

`ethernet_server.c`: Library is for all Ethernet server based calls. It is not called directly but invoked whenever you use a function that relies on it.

`ethernet_client.c`: Library is for all Ethernet client based calls. It is not called directly but invoked whenever you use a function that relies on it.

`w5x00.c`: Implements public interfaces to interact with the W5500 chip.

`w5x00_platform.c`: Integrates the Silabs SPI driver for SPI communication.

### Testing ###

This example demonstrates the HTTP client features of the driver.
The connection diagram of this example is shown in the image below:

![diagram](image/diagram.png)

The following diagram shows the program flow as implemented in the file `app.c`:

![Work Flow](image/flow.png)

Use a terminal program, such as the Console that is integrated in Simplicity Studio or a third-party tool terminal like PuTTY to receive the logs from the virtual COM port. Note that the board uses the default baud rate of 115200. You should expect a similar output to the one below.

![log](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
