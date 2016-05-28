#ifndef MAKEUNIQUE
#define MAKEUNIQUE

#include <memory>

namespace linquitto {

/*
 * make_unique as proposed by Herb Sutter on https://herbsutter.com/gotw/_102/
 * make_unique was missing in the C++ 11 standard.
 */
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

}

#endif // MAKEUNIQUE

