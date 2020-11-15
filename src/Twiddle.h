#ifndef Twiddle_H
#define Twiddle_H

#include <array>

class Twiddle {
 public:
  /**
   * Constructor
   */
  explicit Twiddle(double tolerance);

  /**
   * Destructor.
   */
  virtual ~Twiddle() = default;

  bool HasNewParams() {return _params_changed;};

  void UpdateError(double cte);

  std::array<double, 3>  GetParams();

  //void NeedRerun() {}

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


  std::array<double, 3> params{.1, 0, 1};
  std::array<double, 3> dparams{.01, 0, .1};
};

#endif  // Twiddle_H