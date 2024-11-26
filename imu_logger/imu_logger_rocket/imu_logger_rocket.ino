/*



*/

#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <MPU9250_asukiaaa.h>


Adafruit_BMP280 bmp; // I2C


MPU9250_asukiaaa mySensor;


// FLASH_MAP_SETUP_CONFIG(FLASH_MAP_MAX_FS)

#include <ESP8266WebServer.h>
#include <WiFiUdp.h>

ESP8266WebServer server(80);

static bool fsOK;
// #if defined USE_SPIFFS
#include <FS.h>
const char* fsName = "SPIFFS";
FS* fileSystem = &SPIFFS;
SPIFFSConfig fileSystemConfig = SPIFFSConfig();
// #elif defined USE_LITTLEFS
// #include <LittleFS.h>
// const char* fsName = "LittleFS";
// FS* fileSystem = &LittleFS;
// LittleFSConfig fileSystemConfig = LittleFSConfig();
// #elif defined USE_SDFS
// #include <SDFS.h>
// const char* fsName = "SDFS";
// FS* fileSystem = &SDFS;
// SDFSConfig fileSystemConfig = SDFSConfig();
// fileSystemConfig.setCSPin(chipSelectPin);
// #else
// #error Please select a filesystem first by uncommenting one of the "#define USE_xxx" lines at the beginning of the sketch.
// #endif  


// #ifndef APSSID
#define APSSID "Rocket"
#define APPSK "empirica"

#define SSID "Al"
#define PSK "msug8207"
// #endif
// #define AP

#ifdef AP
/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;
#else
const char *ssid = SSID;
const char *password = PSK;
#endif


volatile bool do_experiment = false;
volatile bool experiment = false;
volatile bool to_report = false;


uint8_t *buffer;

uint32_t duration = 20000;


struct Data {
  uint32_t time;
  float wx;
  float wy;
  float wz;
  float ax;
  float ay;
  float az;
  float baro;
};

class DataSender {
public:
  void init() {
    // uploadFile = fileSystem->open(filename, "w+");

    // buffer = new uint8_t[sizeof(Data)];

  }
  void begin(){
    Udp.begin(8888);
  }
  void send_buffer(uint8_t *buffer, size_t size) {
    // memcpy(buffer, data, sizeof(Data));
    // uploadFile.write(buffer, sizeosf(Data));
    // Serial.println("send_buffer");
    if (has_ip) {
      Udp.beginPacket(send_to, 8888);
      Udp.write(buffer, size);
      Udp.endPacket();
    }
  }
  void deinit() {
    // uploadFile.close();
    // delete[] buffer;
  }
  void set_ip(IPAddress ip) {
    send_to = ip;

has_ip=true;
  Serial.print("set_ip: ");
  Serial.println(send_to);
  }
  WiFiUDP Udp;
  IPAddress send_to;
  bool has_ip = false;
  // uint8_t *buffer;

};

class DataWriter {
public:
  void init() {
    uploadFile = fileSystem->open(filename, "w+");
  }
  void write_buffer(uint8_t *buffer, size_t size) {
    // memcpy(buffer, data, sizeof(Data));
    uploadFile.write(buffer, size);
  }
  void deinit() {
    uploadFile.close();
    // delete[] buffer;
  }
  
  String filename = "data.bin";
  File uploadFile;
  // uint8_t *buffer;
  
};
DataWriter dw;
DataSender ds;


void doit(uint32_t time) {
  // Serial.println("doit");
  Data data;
  int result;
  result = mySensor.accelUpdate();
  result = mySensor.gyroUpdate();
  data.time = time;
  data.wx = mySensor.gyroX();
  data.wy = mySensor.gyroY();
  data.wz = mySensor.gyroZ();
  data.ax =  mySensor.accelX();
  data.ay =  mySensor.accelY();
  data.az =  mySensor.accelZ();
  data.baro = bmp.readPressure();
  memcpy(buffer, &data, sizeof(Data));
  dw.write_buffer(buffer, sizeof(Data));
  ds.send_buffer(buffer, sizeof(Data));
}

void handleData() {
  // server.send(200, "application/octet-stream");
  File file = fileSystem->open("data.bin", "r");
  if (server.streamFile(file, F("application/octet-stream")) != file.size()) { Serial.println("Sent less data than expected!"); }
  file.close();
}

void handleStart() {
  Serial.println("at " + String(millis()) + "ms");
  Serial.println(""); // Add an empty line
  if (server.args()> 0) {
    duration = server.arg(0).toInt()*1000;

    Serial.print("set duration to ");
    Serial.println(duration);
  }
  Serial.println("handle start");
  // duration = server.
  ds.set_ip(server.client().remoteIP());
  do_experiment = true;
}

void handleRoot() {

//   float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;

//   uint8_t sensorId;
//   int result;

//   result = mySensor.readId(&sensorId);
//   if (result == 0) {
//     Serial.println("sensorId: " + String(sensorId));
//   } else {
//     Serial.println("Cannot read sensorId " + String(result));
//   }

//   result = mySensor.accelUpdate();
//   if (result == 0) {
//     aX = mySensor.accelX();
//     aY = mySensor.accelY();
//     aZ = mySensor.accelZ();
//     aSqrt = mySensor.accelSqrt();
//     Serial.println("accelX: " + String(aX));
//     Serial.println("accelY: " + String(aY));
//     Serial.println("accelZ: " + String(aZ));
//     Serial.println("accelSqrt: " + String(aSqrt));
//   } else {
//     Serial.println("Cannod read accel values " + String(result));
//   }

//   result = mySensor.gyroUpdate();
//   if (result == 0) {
//     gX = mySensor.gyroX();
//     gY = mySensor.gyroY();
//     gZ = mySensor.gyroZ();
//     Serial.println("gyroX: " + String(gX));
//     Serial.println("gyroY: " + String(gY));
//     Serial.println("gyroZ: " + String(gZ));
//   } else {
//     Serial.println("Cannot read gyro values " + String(result));
//   }

//   result = mySensor.magUpdate();
//   if (result != 0) {
//     Serial.println("cannot read mag so call begin again");
//     mySensor.beginMag();
//     result = mySensor.magUpdate();
//   }
//   if (result == 0) {
//     mX = mySensor.magX();
//     mY = mySensor.magY();
//     mZ = mySensor.magZ();
//     mDirection = mySensor.magHorizDirection();
//     Serial.println("magX: " + String(mX));
//     Serial.println("maxY: " + String(mY));
//     Serial.println("magZ: " + String(mZ));
//     Serial.println("horizontal direction: " + String(mDirection));
//   } else {
//     Serial.println("Cannot read mag values " + String(result));
//   }

  

  server.send(200, "text/html", 
  "<h1>Rocket</h1>"
  // "<form action=\"/ctrl\" method=\"post\">"
  "<input id=\"duration\" type=\"number\" value=\"20\" />"
  "<button onclick=\"start()\">Start</button>"
  "<a href=\"/data\">data</a>"
  "<script >function start(){const xhr = new XMLHttpRequest(); xhr.open(\"POST\", \"/start?duration=\" + document.getElementById('duration').value, true);xhr.send(null);}</script>"
  // "<button name=\"stop\" value=\"stop\">Stop</button>"
  // "</form>"
  );
}




void setup() {
  delay(1000);

  buffer = new uint8_t[sizeof(Data)];
  
  Serial.begin(115200);
  Serial.println();

  fileSystemConfig.setAutoFormat(false);
  fileSystem->setConfig(fileSystemConfig);
  fileSystem->format();
  fsOK = fileSystem->begin();
  Serial.println(fsOK ? F("Filesystem initialized.") : F("Filesystem init failed!"));

#ifdef AP
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
#else
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif

// ds.begin();

  Wire.begin(0, 2);
  // delay(100);

  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  unsigned status;
  status = bmp.begin(0x76, BMP280_CHIPID);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    // while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X1,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X4,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_OFF,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1); /* Standby time. */


  // delay(100);

  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();


  
  server.on("/", handleRoot);
  server.on("/start", handleStart);
  server.on("/data", handleData);
  server.begin();
  Serial.println("HTTP server started");
  
  
  // doit();
}

uint32_t start_time = 0;
uint32_t prev_time = 0;

void loop() {
  if (do_experiment) {
    experiment = true;
    start_time = millis();
    Serial.println("start experiment");
    dw.init();
    ds.init();
    do_experiment = false;
  }
  if (experiment) {
    uint32_t time = millis();
    if ((time - start_time) > duration) {
      experiment = false;
      dw.deinit();
      ds.deinit();
      Serial.println("stop experiment");
    }
    if ((time - prev_time) >= 10) {
      doit(time);
      prev_time = time;
      // delay(500);
    }
  }
  if (!experiment) {
    server.handleClient();
  }
}
