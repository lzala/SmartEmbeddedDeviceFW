# SmartEmbeddedDeviceFW
SW implementation of bluetooth connectivity stack between
stm32f429i-disco and BTM222 for real time environments.
SW resources: 
-FreeRTOS (threads, queues, mutexes ) 
-emWin 
-stm32f4 LLD  
-Toolchain: Eclipse CDT + CodeSourcery + openOCD  
The challenge was to interface a real time kernel with bluetooth 
SPP (AT commands) + the APP provides the possibility to configure BT link
via graphical interface running on the MCU.
Other features of the application: 
- Paint Tool 
- GPS NMEA sentences decoder  
- Task Manager
