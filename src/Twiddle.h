#ifndef Twiddle_H
#define Twiddle_H

#include "PID.h"
#include <array>

class Twiddle : public PID
{
 public:
  /**
   * Constructor
   */
  Twiddle(double tolerance = 0.001);

  /**
   * Destructor.
   */
  virtual ~Twiddle() = default;

  virtual void Init(double Kp_, double Ki_, double Kd_);

  virtual void UpdateError(double cte);

  virtual bool CheckUpdate();

 private:
  bool NeedUpdate();

  enum State {
    STATE_INIT,
    STATE_WAIT_1,
    STATE_WAIT_2
  };

  double _tolerance;
  double _total_err;
  double _best_err;
  int _iter = 0;
  int _i = 0;
  State _state = STATE_INIT;
  bool _params_changed = false;

  std::array<double, 3> params;
  std::array<double, 3> dparams;
};

#endif  // Twiddle_H