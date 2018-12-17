#ifndef POINT_H
#define POINT_H

#include <array>
#include <cmath>
#include <limits>

namespace Motion
{

// N-Dimentional point
template<typename ValueType, size_t Dimention>
struct PointND
{
    std::array<ValueType, Dimention> values {};

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
        for( auto i = 0; i < Dimention; ++i )
        {
            if( std::fabs(at(i) - c.at(i)) > d )
            {
                return false;
            }
        }

        return true;
    }

    void operator += ( const PointND& c ) { for( auto i = 0; i < Dimention; ++i ) at(i) += c.at(i); }
    void operator -= ( const PointND& c ) { for( auto i = 0; i < Dimention; ++i ) at(i) -= c.at(i); }
    void operator *= ( const PointND& c ) { for( auto i = 0; i < Dimention; ++i ) at(i) *= c.at(i); }
    void operator /= ( const PointND& c ) { for( auto i = 0; i < Dimention; ++i ) at(i) /= c.at(i); }
    PointND operator + ( const PointND& c ) const { auto p = *this; p += c; return p; }
    PointND operator - ( const PointND& c ) const { auto p = *this; p -= c; return p; }
    PointND operator * ( const PointND& c ) const { auto p = *this; p *= c; return p; }
    PointND operator / ( const PointND& c ) const { auto p = *this; p /= c; return p; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////


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

//template<typename ValueType>
//struct Point2D
//{
//    PointND<ValueType, 2> values {};

//    ValueType& x() { return values.at(0); }
//    ValueType& y() { return values.at(1); }
//    const ValueType& x() const { return values.at(0); }
//    const ValueType& y() const { return values.at(1); }

//    Point2D() = default;
//    Point2D( ValueType x, ValueType y )
//    {
//        x() = x;
//        y() = y;
//    }

//    bool operator== ( const Point2D& c ) const
//    {
//        return values == c.values;
//    }

//    void operator+= ( const Point2D& c )
//    {
//        values += c.values;
//    }

//    Point2D operator+ ( const Point2D& c ) const
//    {
//        return values + c.values;
//    }
//};


///////////////////////////////////////////////////////////////////////////////////////////////////


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

//template<typename ValueType>
//struct Point3D
//{
//    PointND<ValueType, 3> values {};

//    ValueType& x() { return values.at(0); }
//    ValueType& y() { return values.at(1); }
//    ValueType& z() { return values.at(2); }
//    const ValueType& x() const { return values.at(0); }
//    const ValueType& y() const { return values.at(1); }
//    const ValueType& z() const { return values.at(2); }

//    Point3D() = default;
//    Point3D( ValueType x, ValueType y, ValueType z )
//    {
//        x() = x;
//        y() = y;
//        z() = z;
//    }

//    bool operator== ( const Point3D& c ) const
//    {
//        return values == c.values;
//    }

//    void operator+= ( const Point3D& c )
//    {
//        values += c.values;
//    }

//    Point3D operator+ ( const Point3D& c )
//    {
//        return values + c.values;
//    }
//};

} // namespace Motion

#endif /** POINT_H */
