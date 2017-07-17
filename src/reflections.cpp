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
        auto findResult = DynamicBuildMap.find( typeIndex );
        if( findResult == DynamicBuildMap.end() ) {
            DynamicBuild& newBuild = DynamicBuildMap[typeIndex];
            newBuild.Instance = nullptr;
            newBuild.ParamsForCreate = ParamsForCreate;
            newBuild.create = create;
            newBuild.destroy = destroy;
        } else {
            DynamicBuild& foundBuild = findResult->second;
            bool judgement = (foundBuild.ParamsForCreate == ParamsForCreate) &&
                             (foundBuild.create == create) &&
                             (foundBuild.destroy == destroy);
            //check( judgement );
        }
    }

    void Reflections::createDynamicBuild( TypeIndex typeIndex, void* Instance, void (*destroy)(void*) ) {
        auto findResult = DynamicBuildMap.find( typeIndex );
        if( findResult == DynamicBuildMap.end() ) {
            DynamicBuild& newBuild = DynamicBuildMap[ typeIndex ];
            newBuild.Instance = Instance;
            newBuild.destroy = destroy;
        } else {
            DynamicBuild& foundBuild = findResult->second;
            bool judgement = (foundBuild.Instance == Instance) &&
                             (foundBuild.destroy == destroy);
            //check( judgement );
        }
    }

    void* Reflections::getInstanceHelper( TypeIndex typeIndex ) {

    }

    void Reflections::install( const Reflection& other ) {

    }

    void Reflections::clear{

    }

}
}