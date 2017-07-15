#ifndef DIFRAME_REFLECTIONS_H
#define DIFRAME_REFLECTIONS_H

/**
*     使用一个工厂类动态创建实例对象
*     模拟的是一个反射机制
*     这是该框架的核心
*     其效率瓶颈在于所使用的哈希函数
*/

#include <vector>
#include <unordered_map>
#include <set>

namespace DIFrame {
namespace utils {

    class Reflections {
    private:

        //This type is used for creating (if necessary), storing and deleting the instances;
        struct DynamicBuild {
            //This is the instance created for the Get method
            void* Instance;

            //Pass the necessary arguments or dependencies to the create function
            void* ParamsForCreate;

            //To create the instance
            void* (*create)( Reflections&, void* );

            //The operation to destroy the instance
            void (*destroy)(void*);

            //Override the operator '<'  to make the comparasion with other BuildType
            bool operator < ( const DynamicBuild& other ) const;
        };

        //A series of operation about DynamicBuild
        
        //To create the DynamicBuild
        //Method 1
        void createDynamicBuild( TypeIndex typeIndex, void* ParamsForCreate, void* (*create)(Reflections&, void*), void (*destroy)(void*) );

        //To create the DynamicBuild
        //Method 2
        void createDynamicBuild( TypeIndex typeIndex, void* Instance, void (*destroy)(void*) );

        //To create the DynamicBuild
        //Method 3
        template <typename T>
        void createDynamicBuild( void* ParamsForCreate, void* (*create)(Reflections&, void*), void (*destroy)(void*) ) {
            createDynamicBuild( getTypeIndex<T>(), ParamsForCreate, create, destroy );
        }

        //To create the DynamicBuild
        //Method 4
        template <typename T>
        void createDynamicBuild( void* Instance, void (*destroy)(void*) ) {
            createDynamicBuild( getTypeIndex<T>(), Instance, destroy );
        }

        //To get the DynamicBuild
        template <typename T>
        DynamicBuild& getDynamicBuild() {
            TypeIndex typeIndex = getTypeIndex<T>();
            auto itr = DynamicBuildMap.find( typeIndex );
            if( itr == DynamicBuildMap.end() )
                return NULL;
            return itr->second;
        }
 
        //This vector stores the created DynamicBuild
        std::vector<DynamicBuild> DynamicBuildList;

        //This hash map is the core component of this framework, it maps the typeid of a class/constructor with
        //the corresponding Instance/BuildType
        std::unordered_map<TypeIndex, DynamicBuild> DynamicBuildMap;

        //The set to store the instances created by this Reflections
        template <typename T>
        static std::shared_ptr<char> thisInstanceSet( Reflections& reflections );

        //Obtain the instance from BuildType
        template <typename T>
        friend struct GetInstance;

        template <typename T>
        T* getInstanceHelper() {
            void* p = getInstanceHelper( getTypeIndex<T>() );
            return reinterpret_cast<T*>(p);
        }

        void* getInstanceHelper( TypeIndex typeIndex );

        //??Obtain the Instance from DynamicBuild
        void constructInstance(TypeIndex typeIndex, DynamicBuild& dynamicBuild);

        //clear operation
        void clear();

        //
        void swap( Reflections& other ) {
            std::swap( DynamicBuildMap, other.DynamicBuildMap );
            std::swap( DynamicBuildList, other.DynamicBuildList );
        }

    public:
        //Constructors & Deconstructors
        Reflections() = default;

        Reflections( const Reflections& other ) : DynamicBuildMap( other.DynamicBuildMap ) {
            //FruitCheck(other.createdSingletons.empty(), 
            //             "Attempting to copy a component that has already started creating instances");
        }

        Reflections( Reflections&& other ) {
            swap( other );
        }
  
        Reflections& operator=( const Reflections& other );
        Reflections& operator=( Reflections&& other );
  
        ~Reflections();
        
        //Bind the Instance with its interface
        //I and T shouldn't be pointers;
        template <typename I, typename T>
        void set() {
            //TODO: add the assert to gurantee I and T are pointers
            auto createFun = []( Reflections& reflections, void* ) {
                T* thisPtr = reflections.getInstanceHelper<T>();
                I* ITypePtr = static_cast<I*>(thisPtr);
                return reinterpret_cast<void*>(ITypePtr);
            };
            createDynamicBuild<I>( nullptr, createFun, nopDeleter );
        }

        template <typename T>
        void setInstance( T& instance ) {
            createDynamicBuild<T>( &instance, nopDeleter );
        }

        template <typename T, typename... Param>
        void registerProvider( T* (*provider)(Param...), void (*deleter)(void*) );
  
        template <typename T, typename... Param>
        void registerProvider( T (*provider)(Param...), void (*deleter)(void*) );
  
        template <typename AnnotatedSignature>
        void registerFactory( RequiredSignatureForAssistedFactory<AnnotatedSignature>* factory );

        // Note: `other' must be a pure component (no singletons created yet)
        // while this doesn't have to be.
        void install( const Reflections& other );

        //fetch the instance from the DynamicBuild
        template <typename T>
        auto fetch() -> decltype(GetInstance<T>()(*this)) {
            return GetInstance<T>()(*this);
        }

    }；

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_REFLECTIONS_H