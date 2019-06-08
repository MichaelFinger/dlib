// Copyright (C) 2008  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_OPTIMIZATIOn_STOP_STRATEGIES_H_
#define DLIB_OPTIMIZATIOn_STOP_STRATEGIES_H_

#include <cmath>
#include <limits>
#include "../matrix.h"
#include "../algs.h"
#include "optimization_stop_strategies_abstract.h"
#include <iostream>

namespace dlib
{

// ----------------------------------------------------------------------------------------

    class objective_delta_stop_strategy
    {
    public:
        explicit objective_delta_stop_strategy (
            double min_delta = 1e-7
        ) : _verbose(false), _been_used(false), _min_delta(min_delta), _max_iter(0), _cur_iter(0), _prev_funct_value(0) 
        {
            DLIB_ASSERT (
                min_delta >= 0,
                "\t objective_delta_stop_strategy(min_delta)"
                << "\n\t min_delta can't be negative"
                << "\n\t min_delta: " << min_delta
            );
        }

        objective_delta_stop_strategy (
            double min_delta,
            unsigned long max_iter
        ) : _verbose(false), _been_used(false), _min_delta(min_delta), _max_iter(max_iter), _cur_iter(0), _prev_funct_value(0) 
        {
            DLIB_ASSERT (
                min_delta >= 0 && max_iter > 0,
                "\t objective_delta_stop_strategy(min_delta, max_iter)"
                << "\n\t min_delta can't be negative and max_iter can't be 0"
                << "\n\t min_delta: " << min_delta
                << "\n\t max_iter:  " << max_iter 
            );
        }

        objective_delta_stop_strategy& be_verbose( 
        ) 
        {
            _verbose = true;
            return *this;
        }

        template <typename T>
        bool should_continue_search (
            const T& ,
            const double funct_value,
            const T& 
        ) 
        {
            if (_verbose)
            {
                using namespace std;
                cout << "iteration: " << _cur_iter << "   objective: " << funct_value << endl;
            }

            ++_cur_iter;
            if (_been_used)
            {
                // Check if we have hit the max allowable number of iterations.  (but only
                // check if _max_iter is enabled (i.e. not 0)).
                if (_max_iter != 0 && _cur_iter > _max_iter)
                    return false;

                // check if the function change was too small
                _current_change_in_function_value = funct_value - _prev_funct_value; // MF

                if (std::abs(_current_change_in_function_value) < _min_delta) // MF
                    return false;
            }

            _been_used = true;
            _prev_funct_value = funct_value;
            return true;
        }

        // MF
        double current_change_in_function_value() {
          return _current_change_in_function_value;
        }

        // MF
        int current_iteration() {
          return static_cast<int>(_cur_iter);
        }

    private:
        bool _verbose;

        bool _been_used;
        double _min_delta;
        unsigned long _max_iter;
        unsigned long _cur_iter;
        double _prev_funct_value;

        double _current_change_in_function_value; // MF
    };

// ----------------------------------------------------------------------------------------

    class gradient_norm_stop_strategy
    {
    public:
        explicit gradient_norm_stop_strategy (
            double min_norm = 1e-7
        ) : _verbose(false), _min_norm(min_norm), _max_iter(0), _cur_iter(0) 
        {
            DLIB_ASSERT (
                min_norm >= 0,
                "\t gradient_norm_stop_strategy(min_norm)"
                << "\n\t min_norm can't be negative"
                << "\n\t min_norm: " << min_norm
            );
        }

        gradient_norm_stop_strategy (
            double min_norm,
            unsigned long max_iter
        ) : _verbose(false), _min_norm(min_norm), _max_iter(max_iter), _cur_iter(0) 
        {
            DLIB_ASSERT (
                min_norm >= 0 && max_iter > 0,
                "\t gradient_norm_stop_strategy(min_norm, max_iter)"
                << "\n\t min_norm can't be negative and max_iter can't be 0"
                << "\n\t min_norm: " << min_norm
                << "\n\t max_iter:  " << max_iter 
            );
        }

        gradient_norm_stop_strategy& be_verbose( 
        ) 
        {
            _verbose = true;
            return *this;
        }

        template <typename T>
        bool should_continue_search (
            const T& ,
            const double funct_value,
            const T& funct_derivative
        ) 
        {
            if (_verbose)
            {
                using namespace std;
                cout << "iteration: " << _cur_iter << "   objective: " << funct_value << "   gradient norm: " << length(funct_derivative) << endl;
            }

            ++_cur_iter;

            // Check if we have hit the max allowable number of iterations.  (but only
            // check if _max_iter is enabled (i.e. not 0)).
            if (_max_iter != 0 && _cur_iter > _max_iter)
                return false;

            // check if the gradient norm is too small 
            _current_gradient_norm = length(funct_derivative); // MF
            if (_current_gradient_norm < _min_norm)  // MF
                return false;

            return true;
        }

        double current_gradient_norm() {
          return _current_gradient_norm;
        }

        int current_iteration() {
          return static_cast<int>(_cur_iter);
        }

    private:
        bool _verbose;

        double _min_norm;
        unsigned long _max_iter;
        unsigned long _cur_iter;

        double _current_gradient_norm; // MF
    };

// ----------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------

    class gradient_max_abs_val_stop_strategy
    {
    public:
        explicit gradient_max_abs_val_stop_strategy (
            double min_gradient_abs_val = 1e-4
        ) : _verbose(false), _min_gradient_abs_val(min_gradient_abs_val), _max_iter(0), _cur_iter(0) 
        {
            DLIB_ASSERT (
                min_norm >= 0,
                "\t gradient_max_abs_val_stop_strategy(min_norm)"
                << "\n\t min_norm can't be negative"
                << "\n\t min_norm: " << min_norm
            );
        }

        gradient_max_abs_val_stop_strategy (
            double min_gradient_abs_val,
            unsigned long max_iter
        ) : _verbose(false), _min_gradient_abs_val(min_gradient_abs_val), _max_iter(max_iter), _cur_iter(0) 
        {
            DLIB_ASSERT (
                min_norm >= 0 && max_iter > 0,
                "\t gradient_max_abs_val_stop_strategy(min_norm, max_iter)"
                << "\n\t min_gradient_abs_val can't be negative and max_iter can't be 0"
                << "\n\t min_gradient_abs_val: " << min_norm
                << "\n\t max_iter:  " << max_iter 
            );
        }

        gradient_max_abs_val_stop_strategy& be_verbose( 
        ) 
        {
            _verbose = true;
            return *this;
        }

        template <typename T>
        bool should_continue_search (
            const T& ,
            const double funct_value,
            const T& funct_derivative
        ) 
        {
            if (_verbose)
            {
                using namespace std;
                cout << "iteration: " << _cur_iter << "   objective: " << funct_value << "   max abs gradient: " << max(abs(funct_derivative)) << endl;
            }

            ++_cur_iter;

            // Check if we have hit the max allowable number of iterations.  (but only
            // check if _max_iter is enabled (i.e. not 0)).
            if (_max_iter != 0 && _cur_iter > _max_iter)
                return false;

            // check if the max abs gradient is too small 
            _current_gradient_max_abs_val = max(abs(funct_derivative)); // MF
            if (_current_gradient_max_abs_val < _min_gradient_abs_val)  // MF
                return false;

            return true;
        }

        double current_gradient_max_abs_val() {
          return _current_gradient_max_abs_val;
        }

        int current_iteration() {
          return static_cast<int>(_cur_iter);
        }

    private:
        bool _verbose;

        double _min_gradient_abs_val;
        unsigned long _max_iter;
        unsigned long _cur_iter;

        double _current_gradient_max_abs_val; // MF
    };

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_OPTIMIZATIOn_STOP_STRATEGIES_H_

