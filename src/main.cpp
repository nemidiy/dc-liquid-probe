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
  #define SW 14
#endif

#ifdef BOARD_ESPDUINO32
  #define SW 12
#endif

// Create display
Adafruit_SSD1306 display(128, 64);
// create the screen
gj::utils::Screen screen(&display, SW);

// WIFI IP
std::string ip;

// Device manager
dc::atlas::DeviceManager device_manager;
dc::atlas::DeviceManager* dm = &device_manager;
dc::atlas::DeviceManager* t = dc::atlas::DeviceManager::get_instance(dm);

int loop_count = 0;
uint8_t signal_level = 0;

void setup(){

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

// homie register
auto reg = gj::utils::homie::HomieRegister::get_instance();

dc::atlas::Device* dev = NULL;

#ifdef I2C_TEMP_ADDR
  Homie.getLogger() << "TEMP_SENSOR @ " << I2C_TEMP_ADDR << endl;
  //create the Device
  dev = new dc::atlas::Device(
        I2C_TEMP_ADDR, dc::atlas::Device::TEMP_SENSOR);
  //add to the device manager
  device_manager.add_device(dev, "temp");
  // add the canvas to the screen
  screen.add_canvas(
        gj::utils::CanvasRegister::get_canvas_func(dev), dev);
  reg->add_node(
      dc::atlas::Device::TEMP_SENSOR,
      "temp",
      "TEMP_SENSOR");
  // schedule the read command every 2 seconds.
  device_manager.schedule_command(
        "R",
        "temp",
        dc::atlas::DeviceManager::read_double_callback,
        5000,
        900,
        2000);
#endif

#ifdef I2C_PH_ADDR
  Homie.getLogger() << "PH_SENSOR @ " << I2C_PH_ADDR << endl;
  //create the Device
  dev = new dc::atlas::Device(
        I2C_PH_ADDR, dc::atlas::Device::PH_SENSOR);
  //add to the device manager
  device_manager.add_device(dev, "ph");
  // add the canvas to the screen
  screen.add_canvas(
        gj::utils::CanvasRegister::get_canvas_func(dev), dev);
  reg->add_node(
      dc::atlas::Device::PH_SENSOR,
      "ph",
      "PH_SENSOR");
  // schedule the read command every 2 seconds.
  device_manager.schedule_command(
        "R",
        "ph",
        dc::atlas::DeviceManager::read_double_callback,
        5000,
        900,
        2000);
#endif

#ifdef I2C_ORP_ADDR
  Homie.getLogger() << "ORP_SENSOR @ " << I2C_ORP_ADDR << endl;
  //create the Device
  dev = new dc::atlas::Device(
        I2C_ORP_ADDR, dc::atlas::Device::ORP_SENSOR);
  //add to the device manager
  device_manager.add_device(dev, "orp");
  // add the canvas to the screen
  screen.add_canvas(
        gj::utils::CanvasRegister::get_canvas_func(dev), dev);
  reg->add_node(
    dc::atlas::Device::ORP_SENSOR,
    "orp",
    "ORP_SENSOR");
  // schedule the read command every 2 seconds.
  device_manager.schedule_command(
        "R",
        "orp",
        dc::atlas::DeviceManager::read_double_callback,
        5000,
        900,
        2000);
#endif

#ifdef I2C_EC_ADDR
  Homie.getLogger() << "EC_SENSOR @ " << I2C_EC_ADDR << endl;
  //create the Device
  dev = new dc::atlas::Device(
        I2C_EC_ADDR, dc::atlas::Device::EC_SENSOR);
  //add to the device manager
  device_manager.add_device(dev, "ec");
  // add the canvas to the screen
  screen.add_canvas(
        gj::utils::CanvasRegister::get_canvas_func(dev), dev);
  reg->add_node(
    dc::atlas::Device::EC_SENSOR,
    "ec",
    "EC_SENSOR");
  // schedule the read command every 2 seconds.
  device_manager.schedule_command(
        "R",
        "ec",
        dc::atlas::DeviceManager::read_double_callback,
        5000,
        900,
        2000);
#endif

#ifdef I2C_DO_ADDR
  Homie.getLogger() << "DO_SENSOR @ " << I2C_DO_ADDR << endl;
  //create the Device
  dev = new dc::atlas::Device(
        I2C_DO_ADDR, dc::atlas::Device::DO_SENSOR);
  //add to the device manager
  device_manager.add_device(dev, "do");
  // add the canvas to the screen
  screen.add_canvas(
        gj::utils::CanvasRegister::get_canvas_func(dev), dev);
  reg->add_node(
    dc::atlas::Device::DO_SENSOR,
    "do",
    "DO_SENSOR");
  // schedule the read command every 2 seconds.
  device_manager.schedule_command(
        "R",
        "do",
        dc::atlas::DeviceManager::read_double_callback,
        5000,
        900,
        2000);
#endif
  // homie initialization
  Homie_setFirmware("liquid-box", "1.0.0");
  reg->set_up();
  // get the ip
  ip = WiFi.localIP().toString().c_str();
}

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
}  // End of loop

