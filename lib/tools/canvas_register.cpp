#include "canvas_register.h"

using namespace gj::utils;

Screen::canvas_func CanvasRegister::get_canvas_func(gj::atlas::Device* dev){

  switch(dev->get_device_type()){
      case gj::atlas::Device::TEMP_SENSOR:{
          return [](struct gj::utils::Screen::Screen* screen, void* param){
          	gj::atlas::Device* dev = static_cast<gj::atlas::Device*>(param);
          	double temp = dev->read();
          	auto* d = screen->get_display();
	        d->setTextSize(2);
	        d->setCursor(0,18);
	        d->println("TEMP");
	        d->setCursor(120,18);
	        d->println("C");
	        d->setCursor(7,36);
	        d->setTextSize(4);
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