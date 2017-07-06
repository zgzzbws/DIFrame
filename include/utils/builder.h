#ifndef DIFRAME_BUILDER_H
#define DIFRAME_BUILDER_H

namespace DIFrame {
namespace utils {

    class Build {
    private:

        //This type is used for creating (if necessary), storing and deleting the instances;
        struct BuildType {
            //This is the instance created for the Get method
            void* Instance;

            //Pass the necessary arguments or dependencies to the create function
            void* ParamsForCreate;

            //To create the instance
            void* (*create)( Build&, void* );

            //The operation to destroy the instance
            void (*destroy)(void*);

            //Override the operator '<'  to make the comparasion with other BuildType
            bool operator < ( const BuildType& other ) const;
        };

        //A series of operation about BuildType
        //To get the BuildType
        template <typename T>
        BuildType& getBuildType();

        //To create the BuildType
        //Method 1
        void createBuildType( TypeIndex typeIndex, void* ParamsForCreate, void* (*create)(Build&, void*), void (*destroy)(void*) );

        //To create the BuildType
        //Method 2
        void createBuildType( TypeIndex typeIndex, void* Instance, void (*destroy)(void*) );

        //To create the BuildType
        //Method 3
        template <typename T>
        void createBuildType( void* ParamsForCreate, void* (*create)(Build&, void*), void (*destroy)(void*) );

        //To create the BuildType
        //Method 4
        template <typename T>
        void createBuildType( void* Instance, void (*destroy)(void*) );
 
        //This vector stores the created BuildType (Instances)
        std::vector<BuildType> BuildTypeList;

        //This hash map is the core component of this framework, it maps the typeid of a class/constructor with
        //the corresponding Instance/BuildType
        std::unordered_map<TypeIndex, BuildType> BuildTypeMap


    }

}
}

#endif // DIFRAME_BUILDER_H