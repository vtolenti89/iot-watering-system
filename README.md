# iot-watering-system
Automated low power watering system 

# Overview
This project aims to automate a watering system for a small garden.
Due to the lack of an outside spigot / outdoor faucet, a large 
reservoir and an intermediate container were used to store the water,
which then was pumped to the plants at a regular interval using
an arduino controlled pump.  

![Block diagram](https://github.com/vtolenti89/iot-watering-system/blob/master/Docs/block_diagram.png)

# Description
The larger reservoir ensures that there is enough water for a resonable time.
Pumping the water to an intermediate container, alleviates the requirement
for the water pump, since it does not have to force the water through the end
nozzles. In this arrangement, gravity will ensure that the water is evenly 
distributed through all of them.
A homemade ruler sensor is used to measure the water level of the main tank,
while the smaller container's sensor prevents any overflow. 

# TODO
* Implement the display
* Implement again wifi with low power option

# History
* Implement a second code version, where the wifi is disabled and the power consumption is drastically
reduced by hardware modification and use of deep sleep routines
* Implement first version with wifi


