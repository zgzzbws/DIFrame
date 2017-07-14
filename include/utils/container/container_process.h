#ifndef DIFRAME_CONTAINER_PROCESS_H
#define DIFRAME_CONTAINER_PROCESS_H

//PartialContainer

namespace DIFrame {
namespace utils{

    template <typename Container, typename NewDependency, bool also_in_container>
    struct AppendDependencyHelper {};

    template <typename Container, typename NewDependency>
    struct AppendDependencyHelper <Container, NewDependency, true> {
        using type = Container;
    };

    template <typename Container, typename NewDependency>
    struct AppendDependencyHelper <Container, NewDependency, false> {
        using type = PartialContainer <append_params<NewDependency, typename Container::Dep>, typename Container::Cons, 
                                    typename Container::Sig, typename Container::Set>;
    };

    template <typename Container, typename NewDependency>
    using AppendDependency = typename AppendDependencyHelper <Container, 
                                                              NewDependency,
                                                              is_in_params<NewDependency, typename Container::Cons>::value ||
                                                              is_in_params<NewDependency, typename Container::Dep>::value> :: type;

    template <typename Container, typename SeveralDependency>
    struct AppendSeveralDependencyHelper {};

    template <typename Container, typename SeveralDependency>
    struct AppendSeveralDependencyHelper {
        using type = Container;
    }

    template <typename Container, typename Dependency, typename... OtherDependency>
    struct AppendSeveralDependencyHelper <Container, params<Dependency, OtherDependency...>> {
        using tempContainer = AppendSeveralDependencyHelper <Container, params<OtherDependency>>::type;
        using type = AppendDependency<tempContainer, Dependency>;
    }

    template <typename Container, typename SeveralDependency>
    using AppendSeveralDependency = typename AppendSeveralDependencyHelper<Container, SeveralDependency>::type;

    template <typename Container, typename Dependency>
    using RemoveDependency = typename PartialContainer <remove_from_params<NewDependency, typename Container::Dep>, 
                                                        typename Container::Cons, 
                                                        RemoveDependencyFromSignature( NewDependency, typename Container::Sig ), 
                                                        typename Container::Set>;

    //AddProvide

    




}
}

#endif // DIFRAME_CONTAINER_PROCESS_H