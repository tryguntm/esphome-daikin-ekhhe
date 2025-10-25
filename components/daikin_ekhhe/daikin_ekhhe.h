
#pragma once

#include <string>
#include <map>
#include <type_traits>

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/select/select.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/time/real_time_clock.h"

#include "daikin_ekhhe_const.h"

namespace esphome {
namespace daikin_ekkhe {

class DaikinEkhheComponent;  // Forward declaration


class DaikinEkhheNumber : public number::Number {
 public:
    void control(float value) override;
    void set_parent(DaikinEkhheComponent *parent) { this->parent_ = parent; }
    // Needed so we can set this from python and then reference it in the control function
    void set_internal_id(const std::string &id) { this->internal_id_ = id; }

  private:
    DaikinEkhheComponent *parent_;
    std::string internal_id_;
};

class DaikinEkhheSelect : public select::Select, public Component {
 public:
  void control(const std::string &value) override;
  void set_select_mappings(std::map<std::string, int> mappings) {
    this->select_mappings_ = std::move(mappings);
  }
  // this stores the number to read/write for each select option
  std::map<std::string, int> get_select_mappings() {
      return this->select_mappings_;
  }
  void set_parent(DaikinEkhheComponent *parent) { this->parent_ = parent; }
  void set_internal_id(const std::string &id) { this->internal_id_ = id; }

  private:
   std::map<std::string, int> select_mappings_; 
   DaikinEkhheComponent *parent_;
   std::string internal_id_;
};

class DaikinEkhheComponent : public Component, public uart::UARTDevice {
 public:

  DaikinEkhheComponent() = default;
  enum EkhheError {
    EKHHE_ERROR_NONE,
    EKHHE_ERROR_PACKET_SIZE, 
    EKHHE_ERROR_BUFFER_EMPTY,
    EKHHE_ERROR_CHECKSUM,
  };


  // ========== INTERNAL METHODS ==========
  void setup() override;
  void loop() override;
  void update();
  void dump_config() override;
  void on_shutdown();
  void set_update_interval(int interval_ms);

  // Methods to register sensors, binary sensors, and numbers
  void register_sensor(const std::string &sensor_name, esphome::sensor::Sensor *sensor);
  void register_binary_sensor(const std::string &sensor_name, esphome::binary_sensor::BinarySensor *binary_sensor);
  void register_number(const std::string &number_name, esphome::number::Number *number);
  void register_select(const std::string &select_name, select::Select *select);
  void register_timestamp_sensor(esphome::text_sensor::TextSensor *sensor);

  // Methods to update values dynamically (only for registered components)
  void set_sensor_value(const std::string &sensor_name, float value);
  void set_binary_sensor_value(const std::string &sensor_name, bool value);
  void set_number_value(const std::string &number_name, float value);
  void set_select_value(const std::string &select_name, int value);
  void update_timestamp(uint8_t hour, uint8_t minute);

  // Allow UART command sending for Number/Select control
  void send_uart_cc_command(uint8_t index, uint8_t value, uint8_t bit_position);


  enum EkkheDDPacket {
    DD_PACKET_START_IDX = 0,
    DD_PACKET_B_IDX     = 6,
    DD_PACKET_A_IDX     = 7,
    DD_PACKET_C_IDX     = 8,
    DD_PACKET_D_IDX     = 10,
    DD_PACKET_E_IDX     = 11,
    DD_PACKET_F_IDX     = 12,
    DD_PACKET_G_IDX     = 13,
    DD_PACKET_H_IDX     = 14,
    DD_PACKET_I_IDX     = 18,
    DD_PACKET_DIG_IDX   = 21,
    DD_PACKET_END       = 40,
    DD_PACKET_SIZE      = 41,
  };

  enum EkhheD2Packet {
    D2_PACKET_START_IDX = 0,
    D2_PACKET_MASK1_IDX = 1,
    D2_PACKET_MASK2_IDX = 2,
    D2_PACKET_MODE_IDX  = 3,
    D2_PACKET_P4_IDX    = 4,
    D2_PACKET_P7_IDX    = 5,
    D2_PACKET_P10_IDX   = 6,
    D2_PACKET_P2_IDX    = 7,
    D2_PACKET_P29_IDX   = 9,
    D2_PACKET_P31_IDX   = 10,
    D2_PACKET_P8_IDX    = 11,
    D2_PACKET_P9_IDX    = 12,
    D2_PACKET_ECO_TTARGET_IDX       = 13,
    D2_PACKET_AUTO_TTARGET_IDX      = 14,
    D2_PACKET_BOOST_TTGARGET_IDX    = 15,
    D2_PACKET_ELECTRIC_TTARGET_IDX  = 16,
    D2_PACKET_P1_IDX    = 20,
    D2_PACKET_P32_IDX   = 21,
    D2_PACKET_P3_IDX    = 22,
    D2_PACKET_P30_IDX   = 23,
    D2_PACKET_P25_IDX   = 24,
    D2_PACKET_P26_IDX   = 25,
    D2_PACKET_P27_IDX   = 26,
    D2_PACKET_P28_IDX   = 27,
    D2_PACKET_P12_IDX   = 28,
    D2_PACKET_P14_IDX   = 29,
    D2_PACKET_P24_IDX   = 30,
    D2_PACKET_P16_IDX   = 31,
    D2_PACKET_P23_IDX   = 32,
    D2_PACKET_P17_IDX   = 33, 
    D2_PACKET_P18_IDX   = 34,
    D2_PACKET_P19_IDX   = 35,
    D2_PACKET_P20_IDX   = 36,
    D2_PACKET_P21_IDX   = 37,
    D2_PACKET_P22_IDX   = 38,
    D2_PACKET_P34_IDX   = 39,
    D2_PACKET_P37_IDX   = 40,
    D2_PACKET_P38_IDX   = 41,
    D2_PACKET_P40_IDX   = 42,
    D2_PACKET_P36_IDX   = 43,
    D2_PACKET_P35_IDX   = 44,
    D2_PACKET_P41_IDX   = 45,
    D2_PACKET_P42_IDX   = 46,
    D2_PACKET_P43_IDX   = 47,
    D2_PACKET_P44_IDX   = 48,
    D2_PACKET_P45_IDX   = 49,
    D2_PACKET_P46_IDX   = 50,
    D2_PACKET_HOUR_IDX  = 56,
    D2_PACKET_MIN_IDX   = 57,
    D2_PACKET_P47_IDX   = 59,
    D2_PACKET_P48_IDX   = 60,
    D2_PACKET_P49_IDX   = 61,
    D2_PACKET_P50_IDX   = 62,
    D2_PACKET_P51_IDX   = 63,
    D2_PACKET_P52_IDX   = 64,
    D2_PACKET_END       = 70, 
    D2_PACKET_SIZE      = 71,
  };

  enum EkhheD4Packet {
    D4_PACKET_START_IDX = 0,
    D4_PACKET_END       = 50,
    D4_PACKET_SIZE      = 51,
  };

  enum EkhheC1Packet {
    C1_PACKET_START_IDX = 0,
    C1_PACKET_END       = 50,
    C1_PACKET_SIZE      = 51,
  };

  enum EkhheCCPacket {
    CC_PACKET_START_IDX = 0,
    CC_PACKET_MASK1_IDX = 1,
    CC_PACKET_MASK2_IDX = 2,
    CC_PACKET_MODE_IDX  = 3,
    CC_PACKET_P4_IDX    = 4,
    CC_PACKET_P7_IDX    = 5,
    CC_PACKET_P10_IDX   = 6,
    CC_PACKET_P2_IDX    = 7, 
    CC_PACKET_P29_IDX   = 9,
    CC_PACKET_P31_IDX   = 10,
    CC_PACKET_P8_IDX    = 11,
    CC_PACKET_P9_IDX    = 12,
    CC_PACKET_ECO_TTARGET_IDX       = 13,
    CC_PACKET_AUTO_TTARGET_IDX      = 14,
    CC_PACKET_BOOST_TTGARGET_IDX    = 15,
    CC_PACKET_ELECTRIC_TTARGET_IDX  = 16,
    CC_PACKET_P1_IDX    = 20, 
    CC_PACKET_P32_IDX   = 21,
    CC_PACKET_P3_IDX    = 22, 
    CC_PACKET_P30_IDX   = 23,
    CC_PACKET_P25_IDX   = 24,
    CC_PACKET_P26_IDX   = 25,
    CC_PACKET_P27_IDX   = 26,
    CC_PACKET_P28_IDX   = 27,
    CC_PACKET_P12_IDX   = 28,
    CC_PACKET_P14_IDX   = 29,
    CC_PACKET_P24_IDX   = 30,
    CC_PACKET_P16_IDX   = 31,
    CC_PACKET_P23_IDX   = 32,
    CC_PACKET_P17_IDX   = 33, 
    CC_PACKET_P18_IDX   = 34,
    CC_PACKET_P19_IDX   = 35,
    CC_PACKET_P20_IDX   = 36,
    CC_PACKET_P21_IDX   = 37,
    CC_PACKET_P22_IDX   = 38,
    CC_PACKET_P34_IDX   = 39,
    CC_PACKET_P37_IDX   = 40,
    CC_PACKET_P38_IDX   = 41,
    CC_PACKET_P40_IDX   = 42,
    CC_PACKET_P36_IDX   = 43,
    CC_PACKET_P35_IDX   = 44,
    CC_PACKET_P41_IDX   = 45,
    CC_PACKET_P42_IDX   = 46,
    CC_PACKET_P43_IDX   = 47,
    CC_PACKET_P44_IDX   = 48,
    CC_PACKET_P45_IDX   = 49,
    CC_PACKET_P46_IDX   = 50,
    CC_PACKET_HOUR_IDX  = 57,
    CC_PACKET_MIN_IDX   = 58,
    CC_PACKET_P47_IDX   = 60, 
    CC_PACKET_P48_IDX   = 61, 
    CC_PACKET_P49_IDX   = 62, 
    CC_PACKET_P50_IDX   = 63, 
    CC_PACKET_P51_IDX   = 64, 
    CC_PACKET_P52_IDX   = 65,
    CC_PACKET_P54_IDX   = 60,
    CC_PACKET_END       = 70, 
    CC_PACKET_SIZE      = 71,
  };

  // This is the TX/control packet
  enum EkhheCDPacket {
    CD_PACKET_START_IDX = 0,
    CD_PACKET_END       = 70,
    CD_PACKET_SIZE      = 71,
  };

 private:
  // variables for sensors etc.
  std::map<std::string, esphome::sensor::Sensor *> sensors_;
  std::map<std::string, esphome::binary_sensor::BinarySensor *> binary_sensors_;
  std::map<std::string, esphome::number::Number *> numbers_;
  std::map<std::string, DaikinEkhheSelect *> selects_;
  text_sensor::TextSensor *timestamp_sensor_ = nullptr;
  esphome::time::RealTimeClock *clock;

  // UART Processing
  uint8_t ekhhe_checksum(const std::vector<uint8_t>& data_bytes);
  void parse_dd_packet(std::vector<uint8_t> buffer);
  void parse_d2_packet(std::vector<uint8_t> buffer);
  void parse_d4_packet(std::vector<uint8_t> buffer);
  void parse_c1_packet(std::vector<uint8_t> buffer);
  void parse_cc_packet(std::vector<uint8_t> buffer);
  void start_uart_cycle();
  void process_packet_set();
  bool packet_set_complete();
  void store_latest_packet(uint8_t byte);

  std::vector<uint8_t> last_d2_packet_;
  std::vector<uint8_t> last_dd_packet_;
  std::vector<uint8_t> last_cc_packet_;  // Always store CC for sending commands
  std::vector<uint8_t> last_c1_packet_;
  std::vector<uint8_t> last_d4_packet_;
  std::map<uint8_t, std::vector<uint8_t>> latest_packets_;

  bool uart_active_ = false;
  bool processing_updates_ = false;
  bool uart_tx_active_ = false; // used for SW "flow control" to avoid RS485 bus contention
  unsigned long last_rx_time_ = 0;

  // Cycle management
  unsigned long last_process_time_ = 0;
  unsigned long update_interval_ = 10000;
};

using namespace daikin_ekhhe;
// uint8_t variables
static const std::map<std::string, uint8_t> U_NUMBER_PARAM_INDEX = {
  {P1_LOW_WAT_PROBE_HYST,   DaikinEkhheComponent::CC_PACKET_P1_IDX},
  {P2_HEAT_ON_DELAY,        DaikinEkhheComponent::CC_PACKET_P2_IDX},
  {P3_ANTL_SET_T,           DaikinEkhheComponent::CC_PACKET_P3_IDX},
  {P4_ANTL_DURATION,        DaikinEkhheComponent::CC_PACKET_P4_IDX},
  {P7_DEFROST_CYCLE_DELAY,  DaikinEkhheComponent::CC_PACKET_P7_IDX},
  {P9_DEFR_STOP_THRES,      DaikinEkhheComponent::CC_PACKET_P9_IDX},
  {P10_DEFR_MAX_DURATION,   DaikinEkhheComponent::CC_PACKET_P10_IDX},
  {P17_HP_START_DELAY_DIG1, DaikinEkhheComponent::CC_PACKET_P17_IDX},
  {P18_LOW_WAT_T_DIG1,      DaikinEkhheComponent::CC_PACKET_P18_IDX},
  {P19_LOW_WAT_T_HYST,      DaikinEkhheComponent::CC_PACKET_P19_IDX},
  {P20_SOL_DRAIN_THRES,     DaikinEkhheComponent::CC_PACKET_P20_IDX},
  {P21_LOW_WAT_T_HP_STOP,   DaikinEkhheComponent::CC_PACKET_P21_IDX},
  {P22_UP_WAT_T_EH_STOP,    DaikinEkhheComponent::CC_PACKET_P22_IDX},
  {P29_ANTL_START_HR,       DaikinEkhheComponent::CC_PACKET_P29_IDX},
  {P30_UP_WAT_T_EH_HYST,    DaikinEkhheComponent::CC_PACKET_P30_IDX},
  {P31_HP_PERIOD_AUTO,      DaikinEkhheComponent::CC_PACKET_P31_IDX},
  {P32_EH_AUTO_TRES,        DaikinEkhheComponent::CC_PACKET_P32_IDX},
  {P34_EEV_SH_PERIOD,       DaikinEkhheComponent::CC_PACKET_P34_IDX},
  {P36_EEV_DSH_SETPOINT,    DaikinEkhheComponent::CC_PACKET_P36_IDX},
  {P37_EEV_STEP_DEFR,       DaikinEkhheComponent::CC_PACKET_P37_IDX},      
  {P38_EEV_MIN_STEP_AUTO,   DaikinEkhheComponent::CC_PACKET_P38_IDX},
  {P40_EEV_INIT_STEP,       DaikinEkhheComponent::CC_PACKET_P40_IDX},
  {P47_MAX_INLET_T_HP,      DaikinEkhheComponent::CC_PACKET_P47_IDX},
  {P49_EVA_INLET_THRES,     DaikinEkhheComponent::CC_PACKET_P49_IDX},
  {P50_ANTIFREEZE_SET,      DaikinEkhheComponent::CC_PACKET_P50_IDX},
  {P51_EVA_HIGH_SET,        DaikinEkhheComponent::CC_PACKET_P51_IDX},
  {P52_EVA_LOW_SET,         DaikinEkhheComponent::CC_PACKET_P52_IDX},
  {ECO_T_TEMPERATURE,       DaikinEkhheComponent::CC_PACKET_ECO_TTARGET_IDX},
  {AUTO_T_TEMPERATURE,      DaikinEkhheComponent::CC_PACKET_AUTO_TTARGET_IDX},
  {BOOST_T_TEMPERATURE,     DaikinEkhheComponent::CC_PACKET_BOOST_TTGARGET_IDX},
  {ELECTRIC_T_TEMPERATURE,  DaikinEkhheComponent::CC_PACKET_ELECTRIC_TTARGET_IDX},
};

// int8_t variables
static const std::map<std::string, uint8_t> I_NUMBER_PARAM_INDEX = {
  {P8_DEFR_START_THRES,       DaikinEkhheComponent::CC_PACKET_P8_IDX}, // int8_t
  {P25_UP_WAT_T_OFFSET,       DaikinEkhheComponent::CC_PACKET_P25_IDX},
  {P26_LOW_WAT_T_OFFSET,      DaikinEkhheComponent::CC_PACKET_P26_IDX},
  {P27_INLET_T_OFFSET,        DaikinEkhheComponent::CC_PACKET_P27_IDX},
  {P28_DEFR_T_OFFSET,         DaikinEkhheComponent::CC_PACKET_P28_IDX},
  {P35_EEV_SH_SETPOINT,       DaikinEkhheComponent::CC_PACKET_P35_IDX},
  {P41_AKP1_THRES,            DaikinEkhheComponent::CC_PACKET_P41_IDX},
  {P42_AKP2_THRES,            DaikinEkhheComponent::CC_PACKET_P42_IDX},
  {P43_AKP3_THRES,            DaikinEkhheComponent::CC_PACKET_P43_IDX},
  {P44_EEV_KP1_GAIN,          DaikinEkhheComponent::CC_PACKET_P44_IDX},
  {P45_EEV_KP2_GAIN,          DaikinEkhheComponent::CC_PACKET_P45_IDX},
  {P46_EEV_KP3_GAIN,          DaikinEkhheComponent::CC_PACKET_P46_IDX},
  {P48_MIN_INLET_T_HP,        DaikinEkhheComponent::CC_PACKET_P48_IDX},
};

static const std::map<std::string, uint8_t> SELECT_PARAM_INDEX = {
  {OPERATIONAL_MODE,     DaikinEkhheComponent::CC_PACKET_MODE_IDX},
  {P12_EXT_PUMP_MODE,    DaikinEkhheComponent::CC_PACKET_P12_IDX},
  {P14_EVA_BLOWER_TYPE,  DaikinEkhheComponent::CC_PACKET_P14_IDX},
  {P16_SOLAR_MODE_INT,   DaikinEkhheComponent::CC_PACKET_P16_IDX},
  {P23_PV_MODE_INT,      DaikinEkhheComponent::CC_PACKET_P23_IDX},
  {P24_OFF_PEAK_MODE,    DaikinEkhheComponent::CC_PACKET_P24_IDX}, 
};

static const std::map<std::string, std::pair<uint8_t, uint8_t>> SELECT_BITMASKS = {
  {POWER_STATUS,          {DaikinEkhheComponent::CC_PACKET_MASK1_IDX, 0}}, 
  {P39_EEV_MODE,          {DaikinEkhheComponent::CC_PACKET_MASK1_IDX, 2}},
  {P13_HW_CIRC_PUMP_MODE, {DaikinEkhheComponent::CC_PACKET_MASK1_IDX, 4}},
  {P11_DISP_WAT_T_PROBE,  {DaikinEkhheComponent::CC_PACKET_MASK2_IDX, 0}},
  {P15_SAFETY_SW_TYPE,    {DaikinEkhheComponent::CC_PACKET_MASK2_IDX, 1}},
  {P5_DEFROST_MODE,       {DaikinEkhheComponent::CC_PACKET_MASK2_IDX, 2}},
  {P6_EHEATER_DEFROSTING, {DaikinEkhheComponent::CC_PACKET_MASK2_IDX, 3}},
  {P33_EEV_CONTROL,       {DaikinEkhheComponent::CC_PACKET_MASK2_IDX, 4}},
};

static const uint8_t BIT_POSITION_NO_BITMASK = 255;
static const uint8_t PARAM_INDEX_INVALID = 255;


}  // namespace daikin_ekkhe
}  // namespace esphome
