#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>

#include "../Motion.h"

namespace
{
    Motion::Motion<double> motion;
    Motion::MotionQueue<double> queue;
    constexpr auto filename = "motion_in.txt";
}

void ContinuityCheck()
{
    double threshold = 2000.0/motion.GetInterpolatedValues().size();
    
    for( uint32_t i = 1; i < motion.GetInterpolatedValues().size()-1; ++i )
    {
        double l = std::abs( motion.GetInterpolatedValues()[i]-motion.GetInterpolatedValues()[i-1] );
        double r = std::abs( motion.GetInterpolatedValues()[i]-motion.GetInterpolatedValues()[i+1] );
        if( std::abs(l-r) > threshold )
        {
            std::cout << "- WARNING: Visual discontinuity on frame " << +i << std::endl;
        }
    }
}

void AnalyzeMotion()
{
    double total = 0;
    for( const auto& m : queue )
    {
        total += m.duration;
    }
    
    if( std::fabs( 1.0-total ) > std::numeric_limits<double>::epsilon() )
    {
        std::cout << "- WARNING: Total duration != 1.0. Check first parameters!" << std::endl;
        std::cout << "- Remaining value: " << std::setprecision(5) << std::fixed << (1-total) << std::endl;
        std::cout << "- Suggested new parameters: " << std::endl;
        for( uint8_t i = 0; i < queue.size(); ++i )
        {
            std::cout << "- " << +i << ": " << std::setprecision(4) << std::fixed << queue[i].duration << " -> " << (queue[i].duration + (1.0-total)/queue.size()) << std::endl;
        }
    }
    
    total = 0;
    for( uint8_t i = 0; i < queue.size(); ++i )
    {
        total += queue[i].length;
    }
    
    if( std::fabs( 1-total ) > std::numeric_limits<double>::epsilon() )
    {
        std::cout << "- WARNING: Total path != 1.0. Check second parameters!" << std::endl;
        std::cout << "- Remaining value: " << std::setprecision(10) << std::fixed << (1-total) << std::endl;
        std::cout << "- Suggested new parameters: " << std::endl;
        for( uint8_t i = 0; i < queue.size(); ++i )
        {
            std::cout << "- " << +i << ": " << std::setprecision(4) << std::fixed << queue[i].length << " -> " << (queue[i].length + (1-total)/queue.size()) << std::endl;
        }
    }
    
}


void AddToQueue( std::string line )
{
    Motion::MotionParameters<double> param {};
    
    std::istringstream str( line );
    std::string s;
    uint8_t count = 0;
    while( std::getline( str, s, ',' ) )
    {
        s.erase( remove_if(s.begin(), s.end(), isspace), s.end());

        const auto set_value = [&s]( double& val )
        {
            if( s == "INFINITESIMAL" ) val = std::numeric_limits<double>::epsilon();
            else val = std::stod(s);
        };

        switch( count )
        {
            case 0:
            {
                if( s == "POW" ) param.motion_type = Motion::MotionType::POW;
                else if( s == "QUAD" ) param.motion_type = Motion::MotionType::QUAD;
                else if( s == "CUBIC" ) param.motion_type = Motion::MotionType::CUBIC;
                else if( s == "SINE" ) param.motion_type = Motion::MotionType::SINE;
                else if( s == "BACK" ) param.motion_type = Motion::MotionType::BACK;
                else if( s == "CIRCULAR" ) param.motion_type = Motion::MotionType::CIRCULAR;
                else if( s == "ELASTIC" ) param.motion_type = Motion::MotionType::ELASTIC;
                else if( s == "BOUNCE" ) param.motion_type = Motion::MotionType::BOUNCE;
                else if( s == "EXPONENTIAL" ) param.motion_type = Motion::MotionType::EXPONENTIAL;
                else param.motion_type = Motion::MotionType::LINEAR;
            }
            break;
            case 1:
            {
                if( s == "OUT" ) param.accel_type = Motion::MotionAcceleration::OUT;
                else param.accel_type = Motion::MotionAcceleration::IN;
            }
            break;
            case 2: set_value( param.duration );        break;
            case 3: set_value( param.length );          break;
            case 4: set_value( param.start_value );     break;
            case 5: set_value( param.end_value );       break;
            case 6: set_value( param.modifier );        break;
            case 7: set_value( param.gravity );         break;
            default: break;
        }

        count++;
    }

    queue.emplace_back( std::move(param) );
}

std::string FilterComments( std::ifstream& file )
{
    char tmp[1024];
    std::string str;
    while(1)
    {
        file.getline( tmp, 256 );
        str = tmp;
        str.erase( remove_if(str.begin(), str.end(), isspace), str.end());
        if( str.size() > 0 && str.at(0) != '#' && str.at(0) != '\0' ) return str;
        else if( file.eof() ) return "";
    }
    
}

void ReadFile( const char* filename )
{
    std::ifstream file( filename );
    
    if( !file )
    {
        std::cerr << "- ERROR: " << filename << " does not exist!" << std::endl;
        exit(1);
    }
    
    double init[] =
    {
        0,  // Start
        1,  // End
        1   // Duration
    };
    
    
    std::string str = FilterComments( file ); 
    
    std::istringstream ss( str );
    std::string param;
    unsigned char count = 0;
    while( std::getline( ss, param, ',' ) )
    {
        if( param == "INFINITESIMAL" )
        {
            init[count] = std::numeric_limits<double>::epsilon();
        }
        else
        {
            init[count] = std::stod( param );
        }
        count++;
    }
    
    str = FilterComments( file ); 
    
    while(1)
    {
        if( file.eof() ) break;
        AddToQueue( str );
        str = FilterComments( file ); 
    }
    
    motion.SetParameters( init[0], init[1], static_cast<Motion::TimeType>(init[2]), queue );
    
    file.close();
    
    std::cout << "File parsed successfully" << std::endl;
    
}

int main( int argc, const char* argv[] )
{
    if( argc < 2 )
    {
        ReadFile( filename );
    }
    else
    {
        ReadFile( argv[1] );
    }

    motion.DumpToFile( "motion_plot.xls" );
    AnalyzeMotion();
    ContinuityCheck();
    std::cout << "Open motion_plot.xls in MS Excel or LibreOffice Calc to chart the data." << std::endl;
    
    return 0;
}
