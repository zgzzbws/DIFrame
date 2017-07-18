#ifndef DIFRAME_CONTAINER_PROCESS_H
#define DIFRAME_CONTAINER_PROCESS_H

namespace DIFrame {
namespace utils{

    //AddRequirement
    template <typename Contain, typename NewDependency, bool also_in_container>
    struct AppendDependencyHelper {};

    template <typename Contain, typename NewDependency>
    struct AppendDependencyHelper <Contain, NewDependency, true> {
        using type = Contain;
    };

    template <typename Contain, typename NewDependency>
    struct AppendDependencyHelper <Contain, NewDependency, false> {
        using type = ContainerImpl <append_params<NewDependency, typename Contain::Dep>, typename Contain::Cons, 
                                    typename Contain::Sig>;
    };

    template <typename Contain, typename NewDependency>
    using AppendDependency = typename AppendDependencyHelper <Contain, 
                                                              NewDependency,
                                                              is_in_params<NewDependency, typename Contain::Cons>::value ||
                                                              is_in_params<NewDependency, typename Contain::Dep>::value> :: type;

    template <typename Contain, typename SeveralDependency>
    struct AppendSeveralDependencyHelper {};

    template <typename Contain, typename SeveralDependency>
    struct AppendSeveralDependencyHelper {
        using type = Contain;
    }

    template <typename Contain, typename CurrentDependency, typename... OtherDependency>
    struct AppendSeveralDependencyHelper <Contain, params<CurrentDependency, OtherDependency...>> {
        using tempContainer = AppendSeveralDependencyHelper <Contain, params<OtherDependency>>::type;
        using type = AppendDependency<tempContainer, CurrentDependency>;
    }

    template <typename Contain, typename SeveralDependency>
    using AppendSeveralDependency = typename AppendSeveralDependencyHelper<Contain, SeveralDependency>::type;

    //RemoveRequirement
    template <typename Contain, typename DeletedDependency>
    using RemoveDependency = typename

    //AddProvide
    template <typename Contain, typename NewConstructor, typename Paras>
    struct AppendSignatureHelper {
        using NewSignature = 
        //check
        using tempContainer = ContainerImpl<typename Contain::Dependency, >
        using type = RemoveDependency<tempContainer, NewConstructor>;
    }

    template <typename Contain, typename NewConstructor, typename Paras>
    using AppendSignature = typename AppendSignatureHelper<Contain, NewConstructor, Paras>::type;

    //ExpandInjectorsInParams

    //ConstructorProvider

    //RegisterProvider
    




}
}

#endif // DIFRAME_CONTAINER_PROCESS_H