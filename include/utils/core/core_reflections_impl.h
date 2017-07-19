#ifndef DIFRAME_CORE_REFLECTIONS_IMPL_H
#define DIFRAME_CORE_REFLECTIONS_IMPL_H

namespace DIFrame {
namespace utils {

    template <typename Interface, typename Implement>
    inline void set() {
        //TODO: guarantee Interface and Implement are pointers
        static_assert(!std::is_pointer<Interface>::value, "The Interface cannot be a pointer");
        static_assert(!std::is_pointer<Implement>::value, "The Implement cannot be a pointer");
        auto create = []( Reflections& reflections, void* ) {
            Implement* ImplementPtr = reflections.getInstanceHelper<Implement>();
            Interface* InterfacePtr = static_cast<Interface*>(ImplementPtr);
            return reinterpret_cast<void*>(InterfacePtr);
        };
        createDynamicBuild<Interface>( nullptr, create, nopDeleter );
    }

    template <typename Implement>
    inline void setWithInstance( Implement* instance ) {
        //check( instance != nullptr );
        createDynamicBuild<Implement>( instance, nopDeleter );
    }

    template <typename Implement, typename... Dependencies>
    inline void setWithDependency( Implement* (*funptr)(Dependencies...), void (*deleteptr)(void*) ) {

    }

    template <typename Implement, typename... Dependencies>
    inline void setWithDependency( Implement (*funptr)(Dependencies...), void (*deleteptr)(void*) ) {
        
    }



} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_CORE_REFLECTIONS_IMPL_H