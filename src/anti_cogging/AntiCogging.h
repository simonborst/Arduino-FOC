#ifndef AntiCogging_h
#define AntiCogging_h

enum AntiCoggingType {
    None,
    Position,
    Velocity,
    Parameterized,
    HandTuned,
};


class AntiCogging {
public:
  AntiCogging() 
    : type(AntiCoggingType::None)
    , freq(0)
    , amp(0)
    , offset(0)
    {}
  virtual ~AntiCogging() {}

  virtual float holding_torque_at_angle(float angle) = 0;

  virtual void update_params() = 0;

  AntiCoggingType type;

  float freq;
  float amp;
  float offset;
};

#endif