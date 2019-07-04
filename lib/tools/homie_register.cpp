#include "homie_register.h"
#include <device_manager.h>
#include <functional>

using namespace gj::utils::homie;

HomieRegister* HomieRegister::instance;

HomieRegister::HomieRegister(){

    temp_setup = [](HomieNode* node){
      node->setProperty("unit").send("centigrades");
    };

    temp_loop = [this](HomieNode* node){
      if (millis() - this->lastTemperatureSent >= this->TEMPERATURE_INTERVAL * 1000UL 
            || this->lastTemperatureSent == 0) {
        double temperature = gj::atlas::DeviceManager::get_instance() \
                    ->get_device_value(gj::atlas::Device::TEMP_SENSOR);
        Homie.getLogger() << "Temperature: " << temperature << " °C" << endl;
        node->setProperty("degrees").send(String(temperature));
        this->lastTemperatureSent = millis();
      }
    };

}

HomieRegister::~HomieRegister(){

}

void HomieRegister::set_up(){
   // The underscore is not a typo! See Magic bytes
  Homie_setFirmware("liquid-box", "1.0.0");

  Homie \
    .setSetupFunction([this](){
      // go through the nodes and set them up
      for(auto& node: nodes){
        switch(node_types[node.first]){
          case gj::atlas::Device::TEMP_SENSOR:{
            temp_setup(node.second);
          }
        }
      }
    }) \
    .setLoopFunction([this](){
      this->loop_handler();
    });

  Homie.onEvent([this](const HomieEvent& event){
    this->on_homie_event(event);
  });

  Homie.setup();
}

void HomieRegister::add_node(
      gj::atlas::Device::device_type dev_type,
      const char* id,
      const char* t){

  switch(dev_type){
    case gj::atlas::Device::TEMP_SENSOR:{
      auto node = new HomieNode(id, t); 
      node->advertise("unit");
      node->advertise("degrees");    
      nodes.insert(std::make_pair(id, node));
      node_types.insert(std::make_pair(id, dev_type));
    }
  }

}

void HomieRegister::add_node(gj::atlas::Device* device){
  switch(device->get_device_type()){
    case gj::atlas::Device::TEMP_SENSOR:{
      add_node(gj::atlas::Device::TEMP_SENSOR, "temperature", "TEMP_SENSOR");
    }
  }

}

void HomieRegister::loop_handler(){
  // go through the nodes and set them up
  for(auto& node: nodes){
    switch(node_types[node.first]){
      case gj::atlas::Device::TEMP_SENSOR:{
        temp_loop(node.second);
      }
    }
  }
  
}

void HomieRegister::on_homie_event(const HomieEvent& event) {
  switch(event.type) {
    case HomieEventType::STANDALONE_MODE:
      // Do whatever you want when standalone mode is started
      Serial.println("HomieEventType::STANDALONE_MODE");
      break;
    case HomieEventType::CONFIGURATION_MODE:
      // Do whatever you want when configuration mode is started
      Serial.println("HomieEventType::CONFIGURATION_MODE");
      break;
    case HomieEventType::NORMAL_MODE:
      // Do whatever you want when normal mode is started
      Serial.println("HomieEventType::NORMAL_MODE");
      break;
    case HomieEventType::OTA_STARTED:
      // Do whatever you want when OTA is started
      Serial.println("HomieEventType::OTA_STARTED");
      break;
    case HomieEventType::OTA_PROGRESS:
      // Do whatever you want when OTA is in progress
      Serial.println("HomieEventType::OTA_PROGRESS");
      // You can use event.sizeDone and event.sizeTotal
      break;
    case HomieEventType::OTA_FAILED:
      // Do whatever you want when OTA is failed
      Serial.println("HomieEventType::OTA_FAILED");
      break;
    case HomieEventType::OTA_SUCCESSFUL:
      // Do whatever you want when OTA is successful
      Serial.println("HomieEventType::OTA_SUCCESSFUL");
      break;
    case HomieEventType::ABOUT_TO_RESET:
      // Do whatever you want when the device is about to reset
      Serial.println("HomieEventType::ABOUT_TO_RESET");
      break;
    case HomieEventType::WIFI_CONNECTED:
      // Do whatever you want when Wi-Fi is connected in normal mode
      Serial.println("HomieEventType::WIFI_CONNECTED");
      // You can use event.ip, event.gateway, event.mask
      break;
    case HomieEventType::WIFI_DISCONNECTED:
      // Do whatever you want when Wi-Fi is disconnected in normal mode
      Serial.println("HomieEventType::WIFI_DISCONNECTED");
      // You can use event.wifiReason
      break;
    case HomieEventType::MQTT_READY:
      // Do whatever you want when MQTT is connected in normal mode
      Serial.println("HomieEventType::MQTT_READY");
      break;
    case HomieEventType::MQTT_DISCONNECTED:
      // Do whatever you want when MQTT is disconnected in normal mode
      Serial.println("HomieEventType::MQTT_DISCONNECTED");
      // You can use event.mqttReason
      break;
    case HomieEventType::MQTT_PACKET_ACKNOWLEDGED:
      // Do whatever you want when an MQTT packet with QoS > 0 is acknowledged by the broker
      Serial.println("HomieEventType::MQTT_PACKET_ACKNOWLEDGED");
      // You can use event.packetId
      break;
    case HomieEventType::READY_TO_SLEEP:
      // After you've called `prepareToSleep()`, the event is triggered when MQTT is disconnected
      Serial.println("HomieEventType::READY_TO_SLEEP");
      break;
  }
}