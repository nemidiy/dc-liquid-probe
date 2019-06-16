#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>

#include <sstream>
#include <string>

#include <screen.h>
#include <device.h>
#include <canvas_register.h>

// Joystick pins
#define X  D6
#define Y  D5
#define SW D7

// Create display
Adafruit_SSD1306 display(128, 64);
// create the screen
gj::utils::Screen screen(&display, X, Y, SW);

// WIFI IP
std::string ip;
// I2C atlas devices
std::map<gj::atlas::Device::device_type, gj::atlas::Device*> devices;

void setup(){
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);

  // WIFI credentials
  const char ssid[]     = "Fibertel WiFi992 2.4GHz";
  const char password[] = "0043182796";
  // This delay is needed to let the display to initialize
  Serial.begin(9600);
  screen.set_alert();
  screen.set_bluetooth();

  //enable I2C port.
  Wire.begin();

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
  ip = WiFi.localIP().toString().c_str();
  Serial.println(WiFi.localIP());

  /* splash screen */
  display.setCursor(0, 32);
  display.setTextSize(2);
  display.println("  BOOTING");
  display.display();

  //set screen canvases
  screen.add_canvas(
      [ip](struct gj::utils::Screen::Screen* screen, void* param){
        screen->set_scrolling_message("SUPER MEGA ULTRA DEVICE");
        screen->draw_scrolling_message(0, 32, 2);
        auto d = screen->get_display();
        d->setCursor(0,55);
        d->setTextSize(1);
        d->println(ip.c_str());
      },
      NULL);

  //auto discovery
  gj::atlas::Device::auto_discovery(devices);

  for (auto& kv : devices) {
    screen.add_canvas(
      gj::utils::CanvasRegister::get_canvas_func(kv.second),
      kv.second);
  }

}

int loop_count = 0;
uint8_t signal_level = 0;

void loop(){

  if (loop_count == 15){
    // calculate signal strength, seems to go from 0 to 120
    int8_t signal = WiFi.RSSI();
    signal = (120 - signal) / 120 * 100;
    if(signal < 10){
      signal_level = 0;
    }else if (signal >= 10 && signal < 20){
      signal_level = 1;
    }else if (signal >= 20 && signal < 40){
        signal_level = 2;
    }else if (signal >= 40 && signal < 60){
        signal_level = 3;
    }else if (signal >= 60 && signal < 80){
        signal_level = 4;
    }else if (signal >= 80){
        signal_level = 5;
    }
    screen.set_signal_level(signal_level);
  }

  screen.render();
  ++loop_count;
}  // End of loop

