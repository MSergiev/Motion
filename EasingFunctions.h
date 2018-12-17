/** --------------------------------------------------------
 * 
 *                  EASING FUNCTIONS
 * 
 * This class contains the mathematical equations, used for
 *                  the Motion class.
 * 
 *    Easing visual examples at: https://easings.net/
 * 
-------------------------------------------------------- **/

#ifndef EASING_FUNCTIONS_H
#define EASING_FUNCTIONS_H

#include <cmath>
#include <limits>
#include <functional>

namespace Motion
{

/** See https://easings.net/ to choose a correct easing type **/
enum class MotionType
{
    LINEAR,         // Linear
    POW,            // Raised to arbitrary power ( modifier is said power )
    QUAD,           // Quadratic
    CUBIC,          // Cubic
    BACK,           // Go slightly back and then forwards
    CIRCULAR,       // Circular motion
    ELASTIC,        // Elastic motion ( modifier controls number of wobbles )
    BOUNCE,         // Bouncing motion ( modifier controls number of bounces )
    SINE,           // Sinusoidal
    EXPONENTIAL,    // Exponential ( modifier controls the steepness of the curve )
};

/** Acceleration of the selected motion type */
enum class MotionAcceleration
{
    IN,     // Acceleration
    OUT,    // Deceleration
};


using Easing = std::function<double (double, double, double)>;

namespace EasingFunction
{
    static Easing Linear = []( double x, double slope, double ) -> double
    {
        return x*slope;
    };

    static Easing Pow = []( double x, double power, double ) -> double
    {
        return std::pow( x, power );
    };

    static Easing Sine = []( double x, double, double ) -> double
    {
        double h_pi = M_PI*0.5;
        return 1 + std::sin( h_pi*x - h_pi );
    };

    static Easing Back = []( double x, double, double ) -> double
    {
        return std::pow(x, 3) - x*std::sin( x*M_PI );
    };

    static Easing Circular = []( double x, double, double ) -> double
    {
        double inv = 1-x;
        return 1 - std::sqrt( (2 - inv) * inv );
    };

    static Easing Elastic = []( double x, double wobbles, double gravity ) -> double
    {
        double arg = wobbles * M_PI * (1 - x);
        return std::pow( M_E, (x - 1) * gravity ) * std::sin(arg)/arg;
    };

    static Easing Bounce = []( double x, double bounces, double gravity ) -> double
    {
        double arg = bounces * M_PI * (1 - x);
        return std::abs(std::pow( M_E, (x - 1) * gravity ) * std::sin(arg)/arg);
    };

    static Easing Exponential = []( double x, double steepness, double ) -> double
    {
        return std::pow( M_E, (x - 1) * steepness );
    };
}

class EasingFunctions 
{   
    
public:
  
    /** Get normalized function value */
    inline static Easing Normalized( double from, double to, const Easing& func, bool inverted = false )
    {
        return Easing(
            [&]( double val, double modifier = 1, double gravity = 6 )
            {
                double l = to-from;
                double f = ( from == 0.0 ? 0.0 : ( inverted ? 1-func(1-from, modifier, gravity) : func(from, modifier, gravity) ) );
                double t = ( to == 1.0 ? 1.0 : ( inverted ? 1-func(1-to, modifier, gravity) : func(to, modifier, gravity) ) );
                double c = ( inverted ? 1-func(1-(val*l+from), modifier, gravity) : func(val*l+from, modifier, gravity) );
                double d = t-f;
                const auto delta = std::numeric_limits<decltype(d)>::epsilon();
                if( d < delta ) d = delta;
                return (c - f) / d;
            }
        );
    }
    
    /** Get easing function value by enum */
    inline static double GetFunctionValue( double val, 
                                           double from,
                                           double to,
                                           MotionType type,
                                           MotionAcceleration accel = MotionAcceleration::IN,
                                           double modifier = 6.0,
                                           double gravity = 6.0 ) 
    {
        // Calculate current step

        if( from == 0.0 && to == 1.0 ) 
        {
            switch( type ) 
            {
                case MotionType::POW:
                {
                    switch( accel ) 
                    {
                        case MotionAcceleration::IN:   return EasingFunction::Pow( val, modifier, gravity );
                        case MotionAcceleration::OUT:  return 1-EasingFunction::Pow( 1-val, modifier, gravity );
                    }
                }
                case MotionType::QUAD:
                {
                    switch( accel ) 
                    {
                        case MotionAcceleration::IN:   return EasingFunction::Pow( val, 2, gravity );
                        case MotionAcceleration::OUT:  return 1-EasingFunction::Pow( 1-val, 2, gravity );
                    }
                }
                case MotionType::CUBIC:
                {
                    switch( accel ) 
                    {
                        case MotionAcceleration::IN:   return EasingFunction::Pow( val, 3, gravity );
                        case MotionAcceleration::OUT:  return 1-EasingFunction::Pow( 1-val, 3, gravity );
                    }
                }
                case MotionType::SINE:
                {
                    switch( accel ) 
                    {
                        case MotionAcceleration::IN:   return EasingFunction::Sine( val, modifier, gravity );
                        case MotionAcceleration::OUT:  return 1-EasingFunction::Sine( 1-val, modifier, gravity );
                    }
                }
                case MotionType::BACK:
                {
                    switch( accel ) 
                    {
                        case MotionAcceleration::IN:   return EasingFunction::Back( val, modifier, gravity );
                        case MotionAcceleration::OUT:  return 1-EasingFunction::Back( 1-val, modifier, gravity );
                    }
                }
                case MotionType::CIRCULAR:
                {
                    switch( accel ) 
                    {
                        case MotionAcceleration::IN:   return EasingFunction::Circular( val, modifier, gravity );
                        case MotionAcceleration::OUT:  return 1-EasingFunction::Circular( 1-val, modifier, gravity );
                    }
                }                
                case MotionType::ELASTIC:
                {
                    switch( accel ) 
                    {
                        case MotionAcceleration::IN:   return EasingFunction::Elastic( val, modifier, gravity );
                        case MotionAcceleration::OUT:  return 1-EasingFunction::Elastic( 1-val, modifier, gravity );
                    }
                }
                case MotionType::BOUNCE:
                {
                    switch( accel ) 
                    {
                        case MotionAcceleration::IN:   return EasingFunction::Bounce( val, modifier, gravity );
                        case MotionAcceleration::OUT:  return 1-EasingFunction::Bounce( 1-val, modifier, gravity );
                    }
                }
                case MotionType::EXPONENTIAL:
                {
                    switch( accel ) 
                    {
                        case MotionAcceleration::IN:   return EasingFunction::Exponential( val, modifier, gravity );
                        case MotionAcceleration::OUT:  return 1-EasingFunction::Exponential( 1-val, modifier, gravity );
                    }
                }
                default: return EasingFunction::Linear( val, 1.0, 1.0 );
                
            }
            
        }
        else 
        {
            switch( type ) 
            {
                case MotionType::POW:          return Normalized( from, to, EasingFunction::Pow,           (accel == MotionAcceleration::OUT) )( val, modifier, gravity );
                case MotionType::QUAD:         return Normalized( from, to, EasingFunction::Pow,           (accel == MotionAcceleration::OUT) )( val, 2, gravity );
                case MotionType::CUBIC:        return Normalized( from, to, EasingFunction::Pow,           (accel == MotionAcceleration::OUT) )( val, 3, gravity );
                case MotionType::SINE:         return Normalized( from, to, EasingFunction::Sine,          (accel == MotionAcceleration::OUT) )( val, modifier, gravity );
                case MotionType::BACK:         return Normalized( from, to, EasingFunction::Back,          (accel == MotionAcceleration::OUT) )( val, modifier, gravity );
                case MotionType::CIRCULAR:     return Normalized( from, to, EasingFunction::Circular,      (accel == MotionAcceleration::OUT) )( val, modifier, gravity );
                case MotionType::ELASTIC:      return Normalized( from, to, EasingFunction::Elastic,       (accel == MotionAcceleration::OUT) )( val, modifier, gravity );
                case MotionType::BOUNCE:       return Normalized( from, to, EasingFunction::Bounce,        (accel == MotionAcceleration::OUT) )( val, modifier, gravity );
                case MotionType::EXPONENTIAL:  return Normalized( from, to, EasingFunction::Exponential,   (accel == MotionAcceleration::OUT) )( val, modifier, gravity );
                default:                       return EasingFunction::Linear( val, 1.0, 1.0 );
            }
        }
    }
    
};

} // namespace Motion

#endif /** EASING_FUNCTIONS_H */
