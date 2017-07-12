#ifndef DIFRAME_BUILDER_H
#define DIFRAME_BUILDER_H



#include <vector>
#include <unordered_map>
#include <set>

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
        
        //To create the BuildType
        //Method 1
        void createBuildType( TypeIndex typeIndex, void* ParamsForCreate, void* (*create)(Build&, void*), void (*destroy)(void*) );

        //To create the BuildType
        //Method 2
        void createBuildType( TypeIndex typeIndex, void* Instance, void (*destroy)(void*) );

        //To create the BuildType
        //Method 3
        template <typename T>
        void createBuildType( void* ParamsForCreate, void* (*create)(Build&, void*), void (*destroy)(void*) ) {
            createBuildType( getTypeIndex<T>(), ParamsForCreate, create, destroy );
        }

        //To create the BuildType
        //Method 4
        template <typename T>
        void createBuildType( void* Instance, void (*destroy)(void*) ) {
            createBuildType( getTypeIndex<T>(), Instance, destroy );
        }

        //To get the BuildType
        template <typename T>
        BuildType& getBuildType() {
            TypeIndex typeIndex = getTypeIndex<T>();
            auto itr = BuildTypeMap.find( typeIndex );
            if( itr == BuildTypeMap.end() )
                return NULL;
            return itr->second;
        }
 
        //This vector stores the created BuildType (Instances)
        std::vector<BuildType> BuildTypeList;

        //This hash map is the core component of this framework, it maps the typeid of a class/constructor with
        //the corresponding Instance/BuildType
        std::unordered_map<TypeIndex, BuildType> BuildTypeMap

        //The set to store the instances created by this Build
        template <typename T>
        static std::shared_ptr<char> thisInstanceSet( Build& build );

        //Obtain the instance from BuildType
        template <typename T>
        T* getInstance() {
            void* p = getInstance( getTypeIndex<T>() );
            return reinterpret_cast<T*>(p);
        }

        void* getInstance( TypeIndex typeIndex );

        //Obtain the Instance from BuildType
        void constructInstance(TypeIndex typeIndex, BuildType& buildType);

        template <typename T>
        friend struct GetHelper;

        //clear operation
        void clear();

        //
        void swap( Build& build ) {
            std::swap( BuildTypeMap, other.BuildTypeMap );
            std::swap( BuildTypeList, other.BuildTypeList );
        }

    public:
        //Constructors & Deconstructors
        Build() = default;

        Build( const Build& other ) : BuildTypeMap( other.BuildTypeMap ) {
            //FruitCheck(other.createdSingletons.empty(), 
            //             "Attempting to copy a component that has already started creating instances");
        }

        Build( Build&& other ) {
            swap( other );
        }
  
        Build& operator=( const Build& other );
        Build& operator=( Build&& other );
  
        ~Build();
        
        //Bind the Instance with its interface
        //I and T shouldn't be pointers;
        template <typename I, typename T>
        void set() {
            //TODO: add the assert to gurantee I and T are pointers
            auto createFun = []( Build& build, void* ) {
                T* thisPtr = build.getPtr<T>();
                I* ITypePtr = static_cast<I*>(thisPtr);
                return reinterpret_cast<void*>(ITypePtr);
            };
            createBuildType<I>( nullptr, createFun, nopDeleter );
        }

        template <typename T>
        void setInstance( T& instance ) {
            createBuildType<T>( &instance, nopDeleter );
        }

        template <typename T, typename... Param>
        void registerProvider( T* (*provider)(Param...), void (*deleter)(void*) );
  
        template <typename T, typename... Param>
        void registerProvider( T (*provider)(Param...), void (*deleter)(void*) );
  
        template <typename AnnotatedSignature>
        void registerFactory( RequiredSignatureForAssistedFactory<AnnotatedSignature>* factory );

        // Note: `other' must be a pure component (no singletons created yet)
        // while this doesn't have to be.
        void install( const ComponentStorage& other );

        template <typename T>
        auto get() -> decltype(GetHelper<T>()(*this)) {
            return GetHelper<T>()(*this);
        }

    }

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_BUILDER_H