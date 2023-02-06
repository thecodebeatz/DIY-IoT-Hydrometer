# DIY IoT Hydrometer

This repository provides instructions for setting up a DIY IoT Hydrometer to measure alcohol content in beer 🍺. The traditional method of using a hydrometer to measure alcohol content requires taking samples regularly, which can result in wasting precious beer. 

This solution proposes the placement of a hydrometer and a distance sensor in the fermentation tank to measure how the hydrometer sinks, proportional to changes in the wort's specific gravity and alcohol content. 

The focus of this repository is on the required software and electronic devices, **rather than the physical setup of the hydrometer and sensor**.

This repo is also a companion to [this blog post](https://codebeats.ml/blogpost/chatgpt-shocked-me-with-its-lightning-fast-understanding-of-my-beer-fermentation-thesis).

# Circuit setup

In my case, instead of placing the hydrometer in the fermentation tank, I put it in a graduated cylinder. I also added some aluminum foil on top for added surface area and to help measure how much the hydrometer sank during fermentation. I placed the hydrometer and a [HY-SRF05](https://www.amazon.com/hy-srf05/s?k=hy-srf05) distance sensor inside a box and set the sensor on top to continuously track the hydrometer's movements. 
The sensor was hooked up to an [Arduino Uno](https://store-usa.arduino.cc/products/arduino-uno-rev3), which sent all the readings straight to the internet in real-time through a [ESP8266](https://www.amazon.com/HiLetgo-Wireless-Transceiver-Development-Compatible/dp/B010N1ROQS/ref=sr_1_6?crid=1Z9JJ5TPK825Z&keywords=ESP8266&qid=1675643018&sprefix=esp8266%2Caps%2C186&sr=8-6) Wi-Fi chip.

The following is the circuit diagram.

![Digital IoT Beer Hydrometer on Arduino](https://user-images.githubusercontent.com/46902643/216880043-4759164c-d6fa-4606-a726-b20acc395f0b.svg)

The distance sensor and the Arduino Uno are wired as follows:

| HY-SRF05      | Arduino       |
| ------------- | ------------- |
| VCC           | 5V            |
| Trig          | PIN 9         |
| Echo          | PIN 10        |
| OUT           | None          |
| GND           | GND           |

The Wi-Fi chip and the Arduino Uno are wired like this:

| ESP8266       | Arduino       |
| ------------- | ------------- |
| RX            | PIN 5         |
| TX            | PIN 6         |
| GND           | GND           |
| VCC           | 5V            |
| CH_PD         | 5V            |
| GPIO 0        | None          |
| GPIO 2        | None          |
| RST           | None          |

# Data Analysis and Anomaly Detection

The data collected from the Hydrometer is analyzed to understand the behavior of the readings and detect any anomalies or outliers in the sensor readings. The data collected includes the time the readings were taken and the hydrometer's distance from the sensor. 

This data is then used to train an Isolation Forest machine learning model. The anomalies are identified as readings that deviate significantly from the normal behavior of the hydrometer, which can indicate issues with the sensor readings.

The IPython Notebook used for the data analysis and anomaly detection is available in the **Data Analysis** folder in this repo.

![Hydrometer change in distance](https://user-images.githubusercontent.com/46902643/216880194-115c794f-e5bf-4828-a05d-7ed04249ffe6.png)

