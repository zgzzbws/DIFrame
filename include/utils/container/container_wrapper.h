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

    //set
    template <typename Contain, typename Interface, typename Implement >
    struct Set {
        using AppendDependencyContainer = AppendDependency<Contain, Implement>;
        using AppendSignatureContainer = AppendSignature<AppendDependencyContainer, Interface, params<Implement>>
        AppendSignatureContainer operator()(Contain&& contain) {
            //check 
            //check
            //check
            contain.reflections.template set<Interface, Implement>();
            return AppendSignatureContainer(std::move(contain.reflections));
        };
    };

    template <typename Contain, typename Signature>
    struct SetWithConstructor {
        using thisInstanceType = decltype(CreateConstructor<Signature>::fun);
        using SetWithDependencyResult = SetWithDependency<Contain, thisInstanceType>;
        using ResultOfSetWithDependency = resultOf<SetWithDependencyResult, Contain&&, thisInstanceType*>;
        ResultOfSetWithDependency operator()(Contain&& contain) {
            return SetWithDependencyResult()(std::move(contain), CreateConstructor<Signature>::fun);
        }
    };

    //setWithInstance
    template <typename Contain, typename Implement>
    struct SetWithInstance {
        using AppendSignatureContainer = AppendSignature<Contain, Implement, params<>>;
        AppendSignatureContainer operator()(Contain&& contain, Implement* instance) {
            contain.reflections.setWithInstance(instance);
            return std::move(contain.reflections);
        }
    };

    //setWithDependency
    template <typename Contain, typename Signature>
    struct SetWithDependency {
        // General Case
        // This cannot be used if match failure
    };

    template <typename Contain, typename Implement, typename... Paras>
    struct SetWithDependency <Contain, Implement(Paras...)> {
        using Signature = Implement(Paras...);
        using SignatureDependency = ExpandTypeOfParams<params<getType<Paras>...>>;
        using AppendDependencyContainer = AppendDependency<Contain, SignatureDependency>;
        using AppendSignatureContainer = AppendSignature<AppendDependencyContainer, getType<Implement>, SignatureDependency>
        AppendSignatureContainer operator()(Contain&& contain, Signature* dependency) {
            contain.reflections.setWithDependency(dependency);
            return std::move(contain.reflections);
        }
    };

    /*
    template <typename Contain, typename AssistedSignature>
    struct SetWithAssisted {
    };
    */

    //append
    template <typename Contain, typename OthreContain>
    struct AppendContainer {
        using OtherDep = typename OtherContain::Dep;
        using OtherCons = typename OtherContain::Cons;
        using OtherSig = typename OtherContain::Sig;
        //check
        using NewCons = concat_params<typename Contain::Cons, OtherCons>;
        using NewDep = set_difference<merge_sets<typename Contain::Dep, OtherDep>, NewCons>;
        using NewSig = AddSigs<typename Contain::Sig, OtherSig>;
        using NewContainer = ContainerImpl<NewDep, NewCons, NewSig>;
        NewContainer operator()(Contain&& contain, const OtherContain& otherContain) {
            contain.reflections.append(otherContain.reflections);
            return std::move(contain.reflections);
        }
    };

} // namespace utils
} // namespace DIFrame

#endif // DIFRAME_CONTAINER_WRAPPER_H