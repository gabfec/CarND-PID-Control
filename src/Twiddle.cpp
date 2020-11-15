#include "Twiddle.h"
#include <iostream>
#include <limits>
#include <math.h>

Twiddle::Twiddle(double tolerance) :
  _tolerance(tolerance)
{
  _best_err = std::numeric_limits<int>::max();
}

constexpr auto N = 100;

bool Twiddle::NeedUpdate()
{
  return (fabs(dparams[0]) + fabs(dparams[1]) + fabs(dparams[2])) > _tolerance;
}

void Twiddle::UpdateError(double cte)
{
  //Ignore the first N reading
  if (not NeedUpdate())
    return;

  if (_iter > N)
    _total_err += cte * cte;

  if (_iter == 2 * N)
  {
    double err = _total_err / N;
    std::cout <<  "err=" << err << " d0=" << dparams[0] << " d1=" << dparams[1] << " d2=" << dparams[2] <<  std::endl;
    _total_err = 0;
    _iter = 0;

    do
    {
      switch (_state)
      {
      case STATE_INIT:
        params[_i] += dparams[_i];
        _state = STATE_WAIT_1;
        break;
      case STATE_WAIT_1:
        if (err < _best_err)
        {
          // There was some improvement
          _best_err = err;
          dparams[_i] *= 1.1;

          std::cout << "Good 1: " << params[0] << " " << params[1] << " " << params[2] << std::endl;
          _state = STATE_INIT;
        }
        else
        {
          // Threre was no improvement
          params[_i] -= 2 * dparams[_i];  // Go into the other direction
          _state = STATE_WAIT_2;
        }
        break;
      case STATE_WAIT_2:
        if (err < _best_err)
        {  // There was an improvement
            _best_err = err;
            dparams[_i] *= 1.1;
            std::cout << "Good 2: " << params[0] << " " << params[1] << " " << params[2] << std::endl;
        }
        else
        {  // There was no improvement
            params[_i] += dparams[_i];
            // As there was no improvement, the step size in either
            // direction, the step size might simply be too big.
            dparams[_i] *= 0.9;
        }
        _state = STATE_INIT;
        break;
      default:
        std::cout << "This should never happen\n";
        break;
      }
      if (_state == STATE_INIT)
      {
        _i = (_i + 1) % 3;
        // Skip I by now
        if (_i == 1) _i++;
      }
    }  while (_state == STATE_INIT);
    _params_changed = true;

    std::cout << "i=" << _i << " state=" << _state << " ==> " << params[0] << " " << params[1] << " " << params[2] << std::endl;
  }
  _iter++;
}

std::array<double, 3>  Twiddle::GetParams()
{
  _params_changed = false;
  return params;
}
