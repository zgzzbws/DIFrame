#ifndef DIFRAME_CONTAINER_PROCESS_H
#define DIFRAME_CONTAINER_PROCESS_H

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
        using type = tempContainer <append_params<NewDependency, typename Container::Dep>, typename Container::Cons, 
                                    typename Container::Sig, typename Container::Set>;
    };

    template <typename Container, typename NewDependency>
    using AppendDependency = typename AppendDependencyHelper <Container, 
                                                              NewDependency,
                                                              is_in_list<NewDependency, typename Container::Cons>::value ||
                                                              is_in_list<NewDependency, typename Container::Dep>::value> :: type;

    


}
}

#endif // DIFRAME_CONTAINER_PROCESS_H