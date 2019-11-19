#include "canvas_register.h"

using namespace gj::utils;

Screen::canvas_func CanvasRegister::get_canvas_func(dc::atlas::Device* dev){

  switch(dev->get_device_type()){
      case dc::atlas::Device::TEMP_SENSOR:{
          return [](struct gj::utils::Screen::Screen* screen, void* param){
            dc::atlas::Device* dev = static_cast<dc::atlas::Device*>(param);
            double temp = dev->get_last_value();
            auto* d = screen->get_display();
            d->setTextSize(2);
            d->setCursor(0,18);
            d->println("TEMP");
            d->setCursor(110,18);
            d->println("C");
            d->setCursor(7,36);
            d->setTextSize(4);
            d->println(temp);
          };
      }
      case dc::atlas::Device::PH_SENSOR:{
          return [](struct gj::utils::Screen::Screen* screen, void* param){
            dc::atlas::Device* dev = static_cast<dc::atlas::Device*>(param);
            double temp = dev->get_last_value();
            auto* d = screen->get_display();
            d->setTextSize(2);
            d->setCursor(0,18);
            d->println("PH");
            d->setCursor(7,36);
            d->setTextSize(4);
            d->println(temp);
          };
      }
      case dc::atlas::Device::ORP_SENSOR:{
          return [](struct gj::utils::Screen::Screen* screen, void* param){
            dc::atlas::Device* dev = static_cast<dc::atlas::Device*>(param);
            double temp = dev->get_last_value();
            auto* d = screen->get_display();
            d->setTextSize(2);
            d->setCursor(0,18);
            d->println("ORP");
            d->setCursor(100,18);
            d->println("mV");
            d->setCursor(7,44);
            d->setTextSize(3);
            d->println(temp);
          };
      }
      case dc::atlas::Device::EC_SENSOR:{
          return [](struct gj::utils::Screen::Screen* screen, void* param){
            dc::atlas::Device* dev = static_cast<dc::atlas::Device*>(param);
            double temp = dev->get_last_value();
            auto* d = screen->get_display();
            d->setTextSize(2);
            d->setCursor(0,18);
            d->println("EC");
            d->setCursor(70,18);
            d->println("uS/cm");
            d->setCursor(7,44);
            d->setTextSize(3);
            d->println(temp);
          };
      }
      case dc::atlas::Device::DO_SENSOR:{
          return [](struct gj::utils::Screen::Screen* screen, void* param){
            dc::atlas::Device* dev = static_cast<dc::atlas::Device*>(param);
            double temp = dev->get_last_value();
            auto* d = screen->get_display();
            d->setTextSize(2);
            d->setCursor(0,18);
            d->println("DO");
            d->setCursor(70,18);
            d->println("mg/L");
            d->setCursor(7,44);
            d->setTextSize(3);
            d->println(temp);
          };
      }
  }
  
  return [](struct gj::utils::Screen::Screen* screen, void* param){
    auto* d = screen->get_display();
    d->setCursor(0,24);
    d->setTextSize(2);
    d->println("NOT FOUND");
  };
}