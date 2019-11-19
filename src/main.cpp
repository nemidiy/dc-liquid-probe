#include <Adafruit_SSD1306.h>
#include <Homie.h>
#include <Homie/Datatypes/Interface.hpp>

#include <sstream>
#include <string>

#include <screen.h>
#include <device.h>
#include <device_manager.h>
#include <canvas_register.h>
#include <homie_register.h>

// Joystick pins
#ifdef BOARD_WEMOSD1
  #define X  D7
  #define Y  D6
  #define SW 14
#endif

#ifdef BOARD_ESPDUINO32
  #define X  2
  #define Y  4
  #define SW 12
#endif

// Create display
Adafruit_SSD1306 display(128, 64);
// create the screen
gj::utils::Screen screen(&display, X, Y, SW);

// WIFI IP
std::string ip;

// Device manager
dc::atlas::DeviceManager device_manager;
dc::atlas::DeviceManager* dm = &device_manager;
dc::atlas::DeviceManager* t = dc::atlas::DeviceManager::get_instance(dm);

void setup(){

  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);

  Serial.begin(115200);
  //screen.set_alert();
  //screen.set_bluetooth();
  
  //enable I2C port.
  Wire.begin();

  /* splash screen */  
  display.setCursor(0, 32);
  display.setTextSize(2);
  display.println("  BOOTING");
  display.display();

  //set screen canvases
  screen.add_canvas(
      [ip](struct gj::utils::Screen::Screen* screen, void* param){
        screen->set_scrolling_message("LIQUID BOX");
        screen->draw_scrolling_message(0, 32, 2);
        auto d = screen->get_display();
        d->setCursor(0,55);
        d->setTextSize(1);
        d->println(ip.c_str());
      },
      NULL);

  //auto discovery
  device_manager.auto_discovery();
  // homie register
  auto reg = gj::utils::homie::HomieRegister::get_instance();

  for (auto& kv : device_manager.get_all_devs()) {
    //add the canvas fir the device
    screen.add_canvas(
        gj::utils::CanvasRegister::get_canvas_func(kv.second),
        kv.second);
    //add the homie node for that device type
    reg->add_node(kv.second);
  }

  // homie initialization
  reg->set_up();
  
  // for some reason Homie need this to trigger the on_wifi callback
  WiFi.begin(
      HomieInternals::Interface::get().getConfig().get().wifi.ssid,
      HomieInternals::Interface::get().getConfig().get().wifi.password);
  
  // get the ip
  ip = WiFi.localIP().toString().c_str();
}

int loop_count = 0;
uint8_t signal_level = 0;

void loop(){
  
  if (loop_count == 15){
    uint8_t signal = HomieInternals::Helpers::rssiToPercentage(WiFi.RSSI());
    signal_level = screen.signal_to_level(signal);
    screen.set_signal_level(signal_level);
  }

  screen.render();
  device_manager.loop();
  Homie.loop();
  ++loop_count;
  delay(10);
}  // End of loop

