#include <chrono>
#include <mutex>
#include <thread>
#include "../client/Tracy.hpp"
#include "../common/TracySystem.hpp"

void TestFunction()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        ZoneScoped;
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }
}

void ResolutionCheck()
{
    for(;;)
    {
        {
            ZoneScoped;
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        }
        {
            ZoneScoped;
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        }
    }

}

void ScopeCheck()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        ZoneScoped;
    }
}

static std::mutex mutex;

void Lock1()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 4 ) );
        std::lock_guard<std::mutex> lock( mutex );
        ZoneScoped;
        std::this_thread::sleep_for( std::chrono::milliseconds( 4 ) );
    }
}

void Lock2()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 3 ) );
        std::unique_lock<std::mutex> lock( mutex );
        ZoneScoped;
        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
    }
}

int main()
{
    auto t1 = std::thread( TestFunction );
    auto t2 = std::thread( TestFunction );
    auto t3 = std::thread( ResolutionCheck );
    auto t4 = std::thread( ScopeCheck );
    auto t5 = std::thread( Lock1 );
    auto t6 = std::thread( Lock2 );

    tracy::SetThreadName( t1, "First thread" );
    tracy::SetThreadName( t2, "Second thread" );
    tracy::SetThreadName( t3, "Resolution check" );
    tracy::SetThreadName( t4, "Scope check" );
    tracy::SetThreadName( t5, "Lock 1" );
    tracy::SetThreadName( t6, "Lock 2" );

    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
        {
            ZoneScoped;
            std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
        }
        FrameMark;
    }
}
