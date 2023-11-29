/* 
 * -----------------------------------------------------------------------------
 * Example: Two way communication between ESP32 and Python using WIFI
 * -----------------------------------------------------------------------------
 * Author: Radhi SGHAIER: https://github.com/Rad-hi
 * -----------------------------------------------------------------------------
 * Date: 07-05-2023 (7th of May, 2023)
 * -----------------------------------------------------------------------------
 * License: Do whatever you want with the code ...
 *          If this was ever useful to you, and we happened to meet on 
 *          the street, I'll appreciate a cup of dark coffee, no sugar please.
 * -----------------------------------------------------------------------------
 */

#ifndef __CONFG_H___
#define __CONFG_H___

/* Constants */
#define TURNOFF_DELAY 3

/* Pin definitions */
#define FAN_PIN 5
#define DHT_PIN 15
#define SERVO_PIN 13

/* Communication params */
#define ACK                         "A" // acknowledgment packet
#define QUEUE_LEN                   5
#define MAX_BUFFER_LEN              128

/* WiFi params */
#define WIFI_SSID                   "AndroidAP"
#define WIFI_PASSWORD               "NMXPVML0507"

/* Socket */
#define SERVER_ADDRESS              "192.168.43.130"
#define SERVER_PORT                 81

#endif // __CONFG_H___
