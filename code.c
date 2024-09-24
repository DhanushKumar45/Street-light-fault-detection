#include <ESP8266WiFi.h>
const char* ssid = "vetri";
const char* host = "maker.ifttt.com";

// Web Server on port 80
WiFiServer server(80);

int led= D6; // connect LDR sensor to arduino 
int sensorPin=A0;
int sensorValue;
char a;
char y;
char c;

void setup() 
{
    pinMode (led, OUTPUT); 
    Serial.begin(115200);
    Serial.println("Email from Node Mcu");
    delay(100);
    delay(1000);
    a=y;
    connectWiFi();   
     // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

void loop()
{     
      WiFiClient client; 
      const int httpPort = 80;  
      if (!client.connect(host, httpPort)) 
      {  
         Serial.println("connection failed");  
         return;
      }
  

      sensorValue = analogRead(sensorPin);
      Serial.println(sensorValue);
      if (sensorValue <= 12)
      {     digitalWrite(led,HIGH);
          Serial.println("led");
          Serial.println("source light OFF");
          Serial.println("FAULT IS DETECTED");  
          delay(4000);
         
         if(a==y)
        {        
            String url = "https://maker.ifttt.com/trigger/LDR_SENSOR/json/with/key/eLXMhMmUZpierx7t8m4K7IP15H8PC8bFapluGZizG-C"; 
            Serial.print("Requesting URL: ");
            Serial.println(url);                 
            client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");                    
              delay(4000);
                } 
                a ='n';
             }  
      
      if (sensorValue > 12)
      {
         Serial.println("OK");
         Serial.println("led");
         digitalWrite(led,LOW);  
         delay(4000);
         delay(sensorValue);  
           a=y; 
               }
    // Listenning for new clients
client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        c = client.read();

            Serial.println(c);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
          
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>STREET LIGHT</h1><h3>SENSOR VALUE = ");
            client.println(sensorValue);
            if (sensorValue <= 12)
          {
            client.println("</h3><h3> LIGHT 1  ");
            client.println();
            client.println("</h3><h3>SOURCE LIGHT OFF");
            client.println("</h3><h3>FAULT IS DETECTED");
          }
          if (sensorValue > 12)
          {
            client.println("</h3><h3> LIGHT 1  ");
            client.println();
            client.println("</h3><h3>SOURCE LIGHT ON");
            client.println("</h3><h3>FAULT IS NOT DETECTED");
          }
            client.println("...</h3><h3>");
            client.println("</body></html>");     
            break;
        }
        if (c == '\n') {
      
          blank_line = true;
        }
       
      }
    }  
  
    
    Serial.println("Client disconnected.");
  
          
  
  
    while((!(WiFi.status() == WL_CONNECTED)))
    {
      connectWiFi();
    } 
}
void connectWiFi()
{ 

  int i=0;
  Serial.println("Connecting to wifi......");
  WiFi.begin(ssid);
  while ((!(WiFi.status() == WL_CONNECTED)))
  { 
      Serial.println(" - ");
      i++;
      delay(300);
      if(i>10)
      {
        connectWiFi();
      }
  }
    Serial.println("");
    Serial.println("WiFi connected");
}
