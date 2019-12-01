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
  typedef std::function<
      void (HomieNode* node, dc::atlas::Device* device)> loop_function;

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
      dc::atlas::Device::device_type dev_type,
      const char* id,
      const char* t);

  void add_node(dc::atlas::Device* device);

  void set_up();

  void on_homie_event(const HomieEvent& event);

  void loop_handler();

protected:

  std::map<std::string, HomieNode*> nodes;
  
  setup_function temp_setup;
  loop_function temp_loop;

  setup_function ph_setup;
  loop_function ph_loop;

  setup_function do_setup;
  loop_function do_loop;

  setup_function orp_setup;
  loop_function orp_loop;

  setup_function ec_setup;
  loop_function ec_loop;

  static HomieRegister* instance;

  const int TEMPERATURE_INTERVAL = 5;
  unsigned long lastTemperatureSent = 0;

  const int PH_INTERVAL = 5;
  unsigned long lastPHSent = 0;

  const int DO_INTERVAL = 5;
  unsigned long lastDOSent = 0;

  const int ORP_INTERVAL = 5;
  unsigned long lastORPSent = 0;

  const int EC_INTERVAL = 5;
  unsigned long lastECSent = 0;
};

} // namespace homie

} // namespace utils

} // namespace gj

#endif