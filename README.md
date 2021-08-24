# STM32F407-with-HC-05-bluetooth-using-UART
This repository contains the code to interface HC-05 Bluetooth module with STM32F4 discovery board using register level UART
PC10 corresponds to TX and PC11 corresponds to RX pin. UART4 has been used for this purpose at 9600 baud rate.
The timer generates an interrupt once every 0.5 second and the value is sent to HC-05 module through UART. In this code, the numbers from 0-99 are sent in their ASCII values.
