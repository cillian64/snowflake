# Snowflake: David's "Yuletronics" project.

A christmas decoration shaped like a snowflake, with beautiful white LEDs.
Also the most ridiculously shaped PCB I've ever had made.

![snowflake animation](/snowflake.gif)

Uses the TI TLV61220 boost SMPS controller to generate 5V from a single
alkaline cell.  Alternatively powered via MicroUSB-B.  Controlled by STM32F042,
programmed using TagConnect SWD or USB DFU.

![PCB front](/pcb_front.png)

![PCB back](/pcb_back.png)

Based on [Adam Greig's Christmas
Tree](https://github.com/adamgreig/yuletronics/)

Made in [KiCAD 4.0.4](http://kicad-pcb.org/).
