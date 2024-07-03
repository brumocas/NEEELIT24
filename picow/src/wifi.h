/*
 * File: client-servidor_neeeil-it.ino
 * Author: NEEEC_FEUP
 * Description: This code was written for the neeeil it robotic competition. It can be used freely.
 * Note: This is just an example, not the solution.
 */

#include <ESP8266WiFi.h>

/*------------------------------------------------Wifi------------------------------------------*/
const char* ssid = "bruninho";
const char* password = "123bruni";

WiFiServer server(80);

String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// Variable to access robot mode
String mode = "";

// Function to get client connections with new information
// Web Server implementation
void getClientInfo();

void initWifi() {
  // Connect to Wi-Fi network with SSID and password
  WiFi.begin(ssid, password);
 
  // Display progress on Serial monitor
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  // Print local IP address and start web server
  Serial.println("");
  Serial.print("WiFi connected at IP Address ");
  Serial.println(WiFi.localIP());
 
  // Start Server
  server.begin();
}

/**
 * The main loop function that runs repeatedly in the Arduino sketch.
 * It checks for incoming client connections and handles data communication with the client.
 */
void client_request() {
WiFiClient client = server.available();   // Listen for incoming clients
 
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
 
            // Get Inputs
            if (header.indexOf("GET /w") >= 0) {
              Serial.println("W");
              mode = 'w';
            }
            // Get Inputs
            if (header.indexOf("GET /s") >= 0) {
              Serial.println("S");
              mode = 's';
            }
            // Get Inputs
            if (header.indexOf("GET /r") >= 0) {
              Serial.println("R");
              mode = 'r';
            }
            // Get Inputs
            if (header.indexOf("GET /l") >= 0) {
              Serial.println("L");
              mode = 'l';
            }
            // Get Inputs
            if (header.indexOf("GET /b") >= 0) {
              Serial.println("B");
              mode = 'b';
            }
            // Get Inputs
            if (header.indexOf("GET /a") >= 0) {
              Serial.println("A");
              mode = 'a';
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
 
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #F23A3A;}</style></head>");
 
            // Web Page Heading
            client.println("<body><h1>Pico W Robot Control</h1>");
 
            // Display current state, and ON/OFF buttons for Onboard LED
            client.println("<p>Robot Mode is " + mode + "</p>");
            
            // Set buttons
            if (mode == "w") {
              client.println("<p><a href=\"/w\"><button class=\"button\">FORWARD</button></a></p>");
            } else {
              client.println("<p><a href=\"/w\"><button class=\"button button2\">FORWARD</button></a></p>");
            }

            if (mode == "b") {
              client.println("<p><a href=\"/b\"><button class=\"button\">BACK</button></a></p>");
            } else {
              client.println("<p><a href=\"/b\"><button class=\"button button2\">BACK</button></a></p>");
            }

            if (mode == "s") {
              client.println("<p><a href=\"/s\"><button class=\"button\">STOP</button></a></p>");
            } else {
              client.println("<p><a href=\"/s\"><button class=\"button button2\">STOP</button></a></p>");
            }

            if (mode == "r") {
              client.println("<p><a href=\"/r\"><button class=\"button\">RIGHT</button></a></p>");
            } else {
              client.println("<p><a href=\"/r\"><button class=\"button button2\">RIGHT</button></a></p>");
            }

            if (mode == "l") {
              client.println("<p><a href=\"/l\"><button class=\"button\">LEFT</button></a></p>");
            } else {
              client.println("<p><a href=\"/l\"><button class=\"button button2\">LEFT</button></a></p>");
            }

            if (mode == "a") {
              client.println("<p><a href=\"/a\"><button class=\"button\">AUTO</button></a></p>");
            } else {
              client.println("<p><a href=\"/a\"><button class=\"button button2\">AUTO</button></a></p>");
            }
 
            client.println("</body></html>");
 
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
}