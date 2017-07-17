#include "utils/core/reflections.h"

#include <iostream>

using std::cout;
using std::cin;

namespace DIFrame{
namespace utils{

    Reflections& Reflections::operator=( const Reflections& other ) {

    }

    Reflections& Reflections::operator=( Reflections&& other ) {
        
    }

    void Reflections::createDynamicBuild( TypeIndex typeIndex, void* ParamsForCreate, void* (*create)(Reflections&, void*), void (*destroy)(void*) ) {

    }

    void Reflections::createDynamicBuild( TypeIndex typeIndex, void* Instance, void (*destroy)(void*) ) {

    }

    void* Reflections::getInstanceHelper( TypeIndex typeIndex ) {

    }

    void Reflections::install( const Reflection& other ) {

    }

    void Reflections::clear{

    }

}
}