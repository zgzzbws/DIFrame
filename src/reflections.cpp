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
        auto findResult = DynamicBuildMap.find( typeIndex );
        //check  if not found public a wrong message
        //if found if
        DynamicBuild& foundBuild = findResult->second;
        if( foundBuild.Instance == nullptr ) {
            //check if the foundBuild.create == null?
            //if not null, use it to create instance
            foundBuild.Instance = foundBuild.create( *this, foundBuild.ParamsForCreate );
            //check if instance is nullptr
            DynamicBuildList.push_back( typeIndex );
        }
        void* thisInstance = foundBuild.Instance;
        //check whether thisInstance is null
        return thisInstance;
    }

    void Reflections::append( const Reflection& other ) {
        //check whether the DynamicBuildList of other is empty
        //if it is not empty
        for( const auto& findResult : other.DynamicBuildMap ) {
            TypeIndex typeIndex = findResult.first;
            const DynamicBuild& foundBuild = findResult.second;
            if( foundBuild.Instance == nullptr )
                createDynamicBuild( typeIndex, foundBuild.ParamsForCreate, foundBuild.create, foundBuild.destroy );
            else
                createDynamicBuild( typeIndex, foundBuild.Instance, foundBuild.destroy );
        }
    }

    void Reflections::clear{
        
    }

} // namespace utils
} // namespace DIFrame