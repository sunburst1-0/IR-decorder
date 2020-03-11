
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>

const char* ssid = "I-D-R";
const char* password = "MEHRARH50NG";

uint16_t *irSignal;
int arrsize=0;

MDNSResponder mdns;

ESP8266WebServer server(80);

IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4 (D2)

void handleRoot() {
  server.send(200, "text/html",
              "<html>" \
                "<head><title>ESP8266 Demo</title></head>" \
                "<body>" \
                  "<h1>Hello from ESP8266, you can send NEC encoded IR" \
                      "signals from here!</h1>" \
                  "<p><a href=\"ir?code=977816508\">Send 20DF10EF</a></p>" \
                  "<p><a href=\"ir?code=596988863\">Send 0xFAB123</a></p>" \
                  "<p><a href=\"ir?code=3251431957\">Send 0xFFE896</a></p>" \
                "</body>" \
              "</html>");
}

void handleIr() {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "code") {
      uint32_t code = strtoul(server.arg(i).c_str(), NULL, 10);
     // uint32_t code = strtoul(server.arg(i).c_str(), NULL, 16);
      Serial.print("code");
      Serial.print(code);
     if(code==1){
        uint16_t   irSignal1[]= {9002,4422,624,1644,602,1650,600,524,598,542,580,528,598,544,580,528,596,1670,578,1648,600,1622,626,540,582,524,600,1648,602,1650,596,540,582,528,596,522,600,1672,580,1646,600,524,598,1648,602,524,598,1648,602,522,598,1672,580,524,596,546,578,1672,580,542,578,1648,602,542,580,1626,624};
    irSignal=irSignal1;
    arrsize=sizeof(irSignal1)/sizeof irSignal1[0];
       Serial.println("1");
      
      }
      else if(code==2){
       uint16_t    irSignal2[]= {9004,4444,602,1644,606,1622,624,498,624,522,604,524,598,542,584,540,582,1652,594,1646,602,1644,604,528,596,520,606,1644,602,1624,624,540,584,522,604,1648,600,1642,606,1644,604,524,598,1626,624,522,602,1646,604,518,604,526,600,520,602,540,584,1628,618,520,604,1644,604,518,606,1618,630};
       irSignal=irSignal2;
    arrsize=sizeof(irSignal2)/sizeof irSignal2[0];
     Serial.println("2");
        }
  

  
              
//        Serial.print("element");
//      Serial.print(irSignal[0]);
//      Serial.print(" ");
//      Serial.print(irSignal[1]);
//          Serial.print(" ");
//      Serial.print(irSignal[2]);
//          Serial.print(" ");
//      Serial.print(irSignal[3]);
//           Serial.print(" ");
//      Serial.print(irSignal[arrsize-2]);
//              Serial.print(" ");
//      Serial.print(irSignal[arrsize-1]);

      //irsend.sendNEC(code, 32);
        irsend.sendRaw(irSignal, arrsize, 38);
      //irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), 38);
    }
  }
  handleRoot();
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
}

void setup(void) {
  irsend.begin();

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/ir", handleIr);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
