#ifndef DIFRAME_CORE_REFLECTIONS_IMPL_H
#define DIFRAME_CORE_REFLECTIONS_IMPL_H

namespace DIFrame {
namespace utils {

    template <typename Interface, typename Implement>
    inline void set() {
        //TODO: guarantee Interface and Implement are pointers
        static_assert( !std::is_pointer<Interface>::value, "DIFrame Error : The Interface cannot be a pointer" );
        static_assert( !std::is_pointer<Implement>::value, "DIFrame Error : The Implement cannot be a pointer" );
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
        static_assert( !std::is_pointer<Implement>::value, "DIFrame Error : The Implement cannot be a pointer" );
        //check( funptr != nullptr );
        using funType = decltype( funptr );
        auto create = []( Reflections& reflections, void* deps ) {
            funType funptr = reinterpret_cast<funType>(deps);
            Implement* ImplementPtr = funptr( reflections.fetch<Dependencies>()... );
            return reinterpret_cast<void*>(ImplementPtr);
        };
        createDynamicBuild<Implement>(reinterpret_cast<void*>(funptr), create, deleteptr);
    }

    template <typename Implement, typename... Dependencies>
    inline void setWithDependency( Implement (*funptr)(Dependencies...), void (*deleteptr)(void*) ) {
        static_assert( !std::is_pointer<Implement>::value, "DIFrame Error : The Implement cannot be a pointer" );
        static_assert(std::is_move_constructible<Implement>::value, "DIFrame Error : The Implement should be movable");
        //check( funptr != nullptr );
        using funType = decltype( funptr );
        auto create = []( Reflections& reflections, void* deps ) {
            funType funptr = reinterpret_cast<funType>(deps);
            Implement* ImplementPtr = new Implement(funptr( reflections.fetch<Dependencies>()... ));
            return reinterpret_cast<void*>(ImplementPtr);
        };
        createDynamicBuild<Implement>(reinterpret_cast<void*>(funptr), create, deleteptr);
    }

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_CORE_REFLECTIONS_IMPL_H