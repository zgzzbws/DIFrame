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
        using AppendDependencyContainer = AppendDependency<Contain, Implement>;
        using 
        M2 operator()(Contain&& contain) {
            contain.reflections.template set<Interface, Implement>();
            return M2(std::move(contain.reflections));
        };
    };

    template <typename Contain, typename Signature>
    struct SetWithConstructor {
        using Dependency = decltype()
        using
        using M1
        M1 operator()(Contain&& contain) {
            return 
        }
    };

    template <typename Contain, typename Implement>
    struct SetWithInstance {
        using M1
        M1 operator()(Contain&& contain, Implement* ) {
            contain.reflections.setInstance()
            return std::move(contain.reflections);
        }
    };

    template <typename Contain, typename Signature>
    struct SetWithDependency {
        // General Case
        // This cannot be used if match failure
    };

    template <typename Contain, typename Implement, typename... Paras>
    struct SetWithDependency <Contain, Implement(Paras...)> {
        using Signature = Implement(Paras...);
        using 
        using M1
        using M2
        M2 operator()(Contain&& contain, Signature* dependency) {
            contain.reflections.setWithDependency(dependency);
            return std::move(contain.reflections);
        }
    };

    template <typename Contain, typename AssistedSignature>
    struct SetWithAssisted {
        
    };

    template <typename Contain, typename OthreContain>
    struct AppendContainer {
        using OtherDep = typename OtherContain::Dep;
        using OtherCons = typename OtherContain::Cons;
        using OtherSig = typename OtherContain::Sig;

        using NewDep
        using NewCons
        using NewSig

        using M1

        M1 operator()(Contain&& contain, const OtherContain& otherContain) {
            contain.reflections.append(otherContain.reflections);
            return std::move(contain.reflections);
        }
    };

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_CONTAINER_WRAPPER_H