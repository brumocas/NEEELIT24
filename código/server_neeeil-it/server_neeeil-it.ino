
/*
 * File: client-servidor_neeeil-it.ino
 * Author: NEEEC_FEUP
 * Description: This code was written for the neeeil it robotic competition. It can be used freely.
 * Note: This is just an example, not the solution.
 */

#include <ESP8266WiFi.h>

#define SSID "neeeilito"
#define PASSWORD "NEEELITO"     
#define PORT 33000              // The port can be any number if high enough
WiFiServer server(PORT);        // Create a server object that listens on the specified port

void setup() {
    // Set Rasp as an Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID, PASSWORD);

    // Start the server
    server.begin();
    Serial.println("Server started");
}

/**
 * The main loop function that runs repeatedly in the Arduino sketch.
 * It checks for incoming client connections and handles data communication with the client.
 */
void loop() {
    // Check for incoming client connections
    WiFiClient client = server.available();
    if (client) {
        Serial.println("Client connected");
        while (client.connected()) {
            // Receive data from the client while he is available
            if (client.available()) {
                // Read the data from the client until an ending character is found
                String request = client.readStringUntil('\0');
                Serial.print("Received: ");
                Serial.print("[PC] ");
                Serial.println(request);
                // Send a response to the client
                String response = "ACK - \"" + request + "\"";
                Serial.print("[RASP] ");
                client.print(response);
                // Sends possible remaining data to the client, cleaning the buffer
                client.flush(); 
            }
        }
    }
}
