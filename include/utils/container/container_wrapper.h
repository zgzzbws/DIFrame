#ifndef DIFRAME_CONTAINER_WRAPPER_H
#define DIFRAME_CONTAINER_WRAPPER_H

namespace DIFrame{
namespace utils{

    template <typename Contain>
    struct Identity {
        Contain operator()( Contain&& contain ) {
            return std::move(contain);
        };
    };

    template <typename Contain, typename Interface, typename Implement >
    struct Set {
        using 
        using
        M2 operator()(Contain&& contain) {
            contain.reflections.template set<Interface, Implement>();
            return M2(std::move(contain.reflections));
        };
    };

    template <typename Contain, typename Signature>
    struct SetWithConstructor {

    }

    template <typename Contain, typename Implement>
    struct SetWithInstance {

    }

    template <typename Contain, typename >
    struct SetWithDependency {

    }

    template <typename AssistedSignature>
    struct SetWithAssisted {

    }

    template <>
    struct AppendContainer {
        
    }

}
}

#endif // DIFRAME_CONTAINER_WRAPPER_H