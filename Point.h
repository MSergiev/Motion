#ifndef POINT_H
#define POINT_H

#include <array>
#include <cmath>
#include <limits>

namespace Motion
{

// Two-dimensional point
template<typename ValueType>
struct Point2D
{
    ValueType x {}, y {};

    bool operator == ( const Point2D& c ) const
    {
        const auto d = std::numeric_limits<ValueType>::epsilon();
        return (std::fabs(x - c.x) < d) && (std::fabs(y - c.y) < d);
    }

    void operator += ( const Point2D& c ) { x += c.x; y += c.y; }
    void operator -= ( const Point2D& c ) { x -= c.x; y -= c.y; }
    void operator *= ( const Point2D& c ) { x *= c.x; y *= c.y; }
    void operator /= ( const Point2D& c ) { x /= c.x; y /= c.y; }
    Point2D operator + ( const Point2D& c ) const { auto p = *this; p += c; return p; }
    Point2D operator - ( const Point2D& c ) const { auto p = *this; p -= c; return p; }
    Point2D operator * ( const Point2D& c ) const { auto p = *this; p *= c; return p; }
    Point2D operator / ( const Point2D& c ) const { auto p = *this; p /= c; return p; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////


// Three-Dimensional point
template<typename ValueType>
struct Point3D
{
    ValueType x {}, y {}, z {};

    bool operator== ( const Point3D& c ) const
    {
        const auto d = std::numeric_limits<ValueType>::epsilon();
        return std::fabs(x - c.x) < d && std::fabs(y - c.y) < d && std::fabs(z - c.z) < d;
    }

    void operator+= ( const Point3D& c ) { x += c.x; y += c.y; z += c.z; }
    void operator-= ( const Point3D& c ) { x -= c.x; y -= c.y; z -= c.z; }
    void operator*= ( const Point3D& c ) { x *= c.x; y *= c.y; z *= c.z; }
    void operator/= ( const Point3D& c ) { x /= c.x; y /= c.y; z /= c.z; }
    Point3D operator + ( const Point3D& c ) const { auto p = *this; p += c; return p; }
    Point3D operator - ( const Point3D& c ) const { auto p = *this; p -= c; return p; }
    Point3D operator * ( const Point3D& c ) const { auto p = *this; p *= c; return p; }
    Point3D operator / ( const Point3D& c ) const { auto p = *this; p /= c; return p; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////


// Multi-Dimensional point
template<typename ValueType, size_t Dimension>
struct PointND
{
    std::array<ValueType, Dimension> values {};

    PointND() = default;
    PointND( const PointND& p ) : values(p.values) {}
    PointND( PointND&& p ) noexcept : values( std::move(p.values) ) {}

    ValueType& at( size_t i ) { return values.at(i); }
    const ValueType& at( size_t i ) const { return values.at(i); }

    PointND& operator= ( const PointND& p ) { values = p.values; return *this; }
    PointND& operator= ( PointND&& p ) noexcept { values = std::move(p.values); return *this; }

    bool operator== ( const PointND& c ) const
    {
        const auto d = std::numeric_limits<ValueType>::epsilon();
        for( decltype(Dimension) i = 0; i < Dimension; ++i )
        {
            if( std::fabs(at(i) - c.at(i)) > d )
            {
                return false;
            }
        }

        return true;
    }

    void operator += ( const PointND& c ) { for( auto i = 0; i < Dimension; ++i ) at(i) += c.at(i); }
    void operator -= ( const PointND& c ) { for( auto i = 0; i < Dimension; ++i ) at(i) -= c.at(i); }
    void operator *= ( const PointND& c ) { for( auto i = 0; i < Dimension; ++i ) at(i) *= c.at(i); }
    void operator /= ( const PointND& c ) { for( auto i = 0; i < Dimension; ++i ) at(i) /= c.at(i); }
    PointND operator + ( const PointND& c ) const { auto p = *this; p += c; return p; }
    PointND operator - ( const PointND& c ) const { auto p = *this; p -= c; return p; }
    PointND operator * ( const PointND& c ) const { auto p = *this; p *= c; return p; }
    PointND operator / ( const PointND& c ) const { auto p = *this; p /= c; return p; }
};

} // namespace egt

#endif /** POINT_H */
