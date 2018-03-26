#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h> 
#include <DHT.h>
const char* ssid = "DamXuamChieu";
const char* password = "doimatkhau";

ESP8266WebServer server(80);

// Pin
#define DHTPIN 5

// Su dung cam bien DHT11 
#define DHTTYPE DHT11

// Thiet lap DHT
DHT dht(DHTPIN, DHTTYPE, 15);

// Thong so WiFi nha ban
const char* ssid = "hoangviet";
const char* password = "tan7/7/95";

// Tao server
WiFiServer server(80);

void handleRoot() 
{
  char temp[400];

  snprintf(temp, 400,
    "<html>\
  <head>\
  </head>\
    <body>\
    <h1>How to html page really good</h1>\
    <a href =\"/0\">Turn OFF</a>\
    <a href =\"/1\">Turn ON</a>\
    <a href =\"/1\"> <button> Turn ON</button> </a>\
    <a href =\"/0\"> <button> Turn OFF</button> </a>\
  </body>\
</html>");
  server.send(200, "text/html", temp);
//  server.send(200, "text/plain", "I'm an awasome webServer!!!");

}


void ledOn() {
  server.send(200, "text/plain", "turn on the LED!!!");
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
}

void ledOff() {
  server.send(200, "text/plain", "Turn off the LED");
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
}

void Flash() {

  server.send(200, "text/plain", "LED flashing");

  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  delay(500);
  digitalWrite(16, LOW);
  delay(500);
  digitalWrite(16, HIGH);
  delay(500);
  digitalWrite(16, LOW);
  delay(500);
  digitalWrite(16, HIGH);
  delay(500);
  digitalWrite(16, LOW);
  delay(500);
  digitalWrite(16, HIGH);
  delay(500);
  digitalWrite(16, LOW);
  delay(500);
}

void LEDdim()
{
  pinMode(16, OUTPUT);
  analogWrite(16, 200);
}



int ADC_Sum=0;
int Battery_Value=0;
int ADC_Count=0;
int Round_OFF=0;

void Get_Anolog_Process(void)
{
  server.send(200, "text/plain", "Turn on the adc");
 
       ADC_Sum+=analogRead(A0);ADC_Count++;
        if(Battery_Value==0){
            Battery_Value=(int)(3000*((ADC_Sum/ADC_Count)/1024.0));
         }
        if(ADC_Count>=10)
        {                
            Battery_Value=(int)(3000*((ADC_Sum/ADC_Count)/1024.0));
            ADC_Count=0;ADC_Sum=0;
             Serial.print("Voltage: ");
           Serial.print(Battery_Value);
           Serial.println(" mV");
        } 
  
  pinMode(5, INPUT);

}



void setup(void) {

  Serial.begin(115200);
  delay(10);
  
  // Khoi tao DHT 
  dht.begin();
  
  // Ket noi toi mang WiFi
  Serial.println();
  Serial.println();
  Serial.print("Ket noi toi mang ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Da ket noi WiFi");
  
  Serial.print("Connecting to network");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);   // Connect to WiFi network

  while (WiFi.status() != WL_CONNECTED) {    // Wait for connection
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/0", ledOff);
  server.on("/1", ledOn);
  server.on("/2", Flash);
  server.on("/3", LEDdim);
  server.on("/4", Get_Anolog_Process);
  delay(100);

  server.begin();
  Serial.println("HTTP server started");
  
  // In ra dia chi IP
  Serial.println(WiFi.localIP());
}



void loop(void)
{
  server.handleClient();
  
	// Kiem tra khi co client ket noi
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Doi client gui ket noi
  Serial.println("Co mot client moi ket noi xem du lieu");
  while(!client.available()){
    delay(1);
  }
  
  // Doc do am
  float h = dht.readHumidity();
  // Doc nhiet do o do C
  float t = dht.readTemperature();
  
  // Doc dong dau tien cua yeu cau gui len.
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Chuan bi tao web de phan hoi
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<head>";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<meta http-equiv=\"refresh\" content=\"60\" />";
  s += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
  s += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
  s += "<style>body{font-size: 24px;} .voffset {margin-top: 30px;}</style>";
  s += "</head>";
  
  s += "<div class=\"container\">";
  s += "<h1>Theo doi nhiet do va do am</h1>";
  s += "<div class=\"row voffset\">";
  s += "<div class=\"col-md-3\">Nhiet do: </div><div class=\"col-md-3\">" + String(t) + "</div>";
  s += "<div class=\"col-md-3\">Do am: </div><div class=\"col-md-3\">" + String(h) + "</div>";
  s += "</div>";
   
  // Gui phan hoi toi client (o day la giao dien web)
  client.print(s);
  delay(1);
  Serial.println("Client da thoat");

  // Sau khi nhan duoc thong tin thi se tu dong ngat ket noi 
}
