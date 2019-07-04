#ifndef __GJ_UTILS_HOMIE_REGISTER_H__
#define __GJ_UTILS_HOMIE_REGISTER_H__

#include <Homie.h>
#include <device.h>
#include <map>
#include <functional>


namespace gj {

namespace utils {

namespace homie {

struct HomieRegister {

  typedef std::function<void (HomieNode* node)> setup_function;
  typedef std::function<void (HomieNode* node)> loop_function;

  static HomieRegister* get_instance(HomieRegister* reg = NULL){
    if(reg){
      instance = reg;
    }else if (instance == NULL){
      instance = new HomieRegister;
    }
    return instance;
  }

  HomieRegister();

  virtual ~HomieRegister();

  void add_node(
      gj::atlas::Device::device_type dev_type,
      const char* id,
      const char* t);

  void add_node(gj::atlas::Device* device);

  void set_up();

  void on_homie_event(const HomieEvent& event);

  void loop_handler();

protected:

  std::map<std::string, HomieNode*> nodes;
  std::map<std::string, gj::atlas::Device::device_type> node_types;
  
  setup_function temp_setup;
  loop_function temp_loop;

  static HomieRegister* instance;

  const int TEMPERATURE_INTERVAL = 5;
  unsigned long lastTemperatureSent = 0;

};

} // namespace homie

} // namespace utils

} // namespace gj

#endif