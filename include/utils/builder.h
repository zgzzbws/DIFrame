#ifndef DIFRAME_BUILDER_H
#define DIFRAME_BUILDER_H

namespace DIFrame {
namespace utils {

    class Build {
    private:
        struct BuildType {
            //This is the instance created for the Get method
            void* Instance;

            //Pass the necessary arguments or dependencies to the create function
            void* ParamsForCreate;

            //To create the instance
            void* (*create)( Build&, void* );

            //The operation to destroy the instance
            void* (*destroy)(void*);

            //Override the operator <  to make the comparasion with other BuildType
            bool operator < ( const BuildType& other ) const;
        };
    }

}
}

#endif // DIFRAME_BUILDER_H