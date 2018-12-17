/** --------------------------------------------------------
 * 
 *                      MOTION
 * 
 * This class is intended for use with various parameters,
 *   animation coordinates for example, to create smooth 
 *    movements based on easing mathematical functions.
 * 
 * The class is capable of modifying all numerical types,
 *   be it integer coordinates or floating point values.
 * 
 *    Easing visual examples at: https://easings.net/
 * 
-------------------------------------------------------- **/

#ifndef MOTION_H
#define MOTION_H

#include "MotionCore.h"
#include "Point.h"

namespace Motion
{

// Single motion object
template<typename ValueType>
struct Motion
{
    MotionCore<ValueType> motion {};

    explicit Motion( bool runtime_calculation = true )
        : motion(runtime_calculation)
    {}

    // Set simple parameters
    void SetParameters(
        ValueType start_value,
        ValueType end_value,
        TimeType frame_duration,
        Type type = Type::SINE,
        double duration_split = 0.5,
        double modifier = 4,
        double gravity = 2 )
    {
        motion.SetParameters( start_value, end_value, frame_duration, type, duration_split, modifier, gravity );
    }

    // Set complex parameters
    void SetParameters(
        ValueType start_value,
        ValueType end_value,
        TimeType frame_duration,
        MotionQueue<ValueType> params )
    {
        motion.SetParameters( start_value, end_value, frame_duration, params );
    }

    // Advance to next frame
    void AdvanceToNext()
    {
        motion.AdvanceToNext();
    }

    // Check if interpolation is finished
    bool HasFinished()
    {
        return motion.HasFinished();
    }

    // Get current interpolated value
    ValueType GetCurrentValue()
    {
        return motion.GetCurrentValue();
    }

    // Reset all interpolation data
    void Reset()
    {
        motion.Reset();
    }

};


///////////////////////////////////////////////////////////////////////////////////////////////////


// Two-dimensional motion object
template<typename ValueType>
struct Motion2D
{
    MotionCore<ValueType> x {}, y {};

    explicit Motion2D( bool runtime_calculation = true )
        : x(runtime_calculation),
          y(runtime_calculation)
    {}

    // Set simple parameters
    void SetParameters(
        Point2D<ValueType> start_value,
        Point2D<ValueType> end_value,
        TimeType frame_duration,
        Type type = Type::SINE,
        double duration_split = 0.5,
        double modifier = 4,
        double gravity = 2 )
    {
        x.SetParameters( start_value.x, end_value.x, frame_duration, type, duration_split, modifier, gravity );
        y.SetParameters( start_value.y, end_value.y, frame_duration, type, duration_split, modifier, gravity );
    }

    // Set complex parameters
    void SetParameters(
        Point2D<ValueType> start_value,
        Point2D<ValueType> end_value,
        TimeType frame_duration,
        MotionQueue<ValueType> params )
    {
        x.SetParameters( start_value.x, end_value.x, frame_duration, params );
        y.SetParameters( start_value.y, end_value.y, frame_duration, params );
    }

    // Advance to next frame
    void AdvanceToNext()
    {
        x.AdvanceToNext();
        y.AdvanceToNext();
    }

    // Check if interpolation is finished
    bool HasFinished()
    {
        return x.HasFinished() && y.HasFinished();
    }

    // Get current interpolated value
    Point2D<ValueType> GetCurrentValue()
    {
        return { x.GetCurrentValue(), y.GetCurrentValue() };
    }

    // Reset all interpolation data
    void Reset()
    {
        x.Reset();
        y.Reset();
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////


// Three-dimensional motion object
template<typename ValueType>
struct Motion3D
{
    MotionCore<ValueType> x {}, y {}, z {};

    explicit Motion3D( bool runtime_calculation = true )
        : x(runtime_calculation),
          y(runtime_calculation),
          z(runtime_calculation)
    {}

    // Set simple parameters
    void SetParameters(
        Point3D<ValueType> start_value,
        Point3D<ValueType> end_value,
        TimeType frame_duration,
        Type type = Type::SINE,
        double duration_split = 0.5,
        double modifier = 4,
        double gravity = 2 )
    {
        x.SetParameters( start_value.x, end_value.x, frame_duration, type, duration_split, modifier, gravity );
        y.SetParameters( start_value.y, end_value.y, frame_duration, type, duration_split, modifier, gravity );
        z.SetParameters( start_value.z, end_value.z, frame_duration, type, duration_split, modifier, gravity );
    }

    // Set complex parameters
    void SetParameters(
        Point3D<ValueType> start_value,
        Point3D<ValueType> end_value,
        TimeType frame_duration,
        MotionQueue<ValueType> params )
    {
        x.SetParameters( start_value.x, end_value.x, frame_duration, params );
        y.SetParameters( start_value.y, end_value.y, frame_duration, params );
        z.SetParameters( start_value.z, end_value.z, frame_duration, params );
    }

    // Advance to next frame
    void AdvanceToNext()
    {
        x.AdvanceToNext();
        y.AdvanceToNext();
        z.AdvanceToNext();
    }

    // Check if interpolation is finished
    bool HasFinished()
    {
        return x.HasFinished() && y.HasFinished() && z.HasFinished();
    }

    // Get current interpolated value
    Point3D<ValueType> GetCurrentValue()
    {
        return { x.GetCurrentValue(), y.GetCurrentValue(), z.GetCurrentValue() };
    }

    // Reset all interpolation data
    void Reset()
    {
        x.Reset();
        y.Reset();
        z.Reset();
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////


// Multi-dimensional motion object
template<typename ValueType, size_t Dimension>
struct MotionND
{
    std::array<MotionCore<ValueType>, Dimension> motion {};

    explicit MotionND( bool runtime_calculation = true )
        : motion(runtime_calculation)
    {}

    MotionCore<ValueType>& at( size_t i ) { return motion.at(i); }
    const MotionCore<ValueType>& at( size_t i ) const { return motion.at(i); }

    // Set simple parameters
    void SetParameters(
        PointND<ValueType, Dimension> start_value,
        PointND<ValueType, Dimension> end_value,
        TimeType frame_duration,
        Type type = Type::SINE,
        double duration_split = 0.5,
        double modifier = 4,
        double gravity = 2 )
    {
        for( decltype(Dimension) i = 0; i < Dimension; ++i )
        {
            motion.at(i).SetParameters( start_value.at(i), end_value.at(i), frame_duration, type, duration_split, modifier, gravity );
        }
    }

    // Set complex parameters
    void SetParameters(
        PointND<ValueType, Dimension> start_value,
        PointND<ValueType, Dimension> end_value,
        TimeType frame_duration,
        MotionQueue<ValueType> params )
    {
        for( decltype(Dimension) i = 0; i < Dimension; ++i )
        {
            motion.at(i).SetParameters( start_value.at(i), end_value.at(i), frame_duration, params );
        }
    }

    // Advance to next frame
    void AdvanceToNext()
    {
        for( auto& m : motion )
        {
            m.AdvanceToNext();
        }
    }

    // Check if interpolation is finished
    bool HasFinished() const
    {
        for( const auto& m : motion )
        {
            if( !m.HasFinished() )
            {
                return false;
            }
        }
        return true;
    }

    // Get current interpolated value
    PointND<ValueType, Dimension> GetCurrentValue() const
    {
        PointND<ValueType, Dimension> p {};
        for( decltype(Dimension) i = 0; i < Dimension; ++i )
        {
            p.at(i) = motion.at(i).GetCurrentValue();
        }
        return p;
    }

    // Reset all interpolation data
    void Reset()
    {
        for( auto& m : motion )
        {
            m.Reset();
        }
    }
};

} // namespace egt

#endif /** MOTION_H */
