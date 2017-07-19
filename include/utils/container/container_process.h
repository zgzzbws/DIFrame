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
    template <typename ParasList>
    struct ExpandTypeOfParamsHelper {};

    template <>
    struct ExpandTypeOfParamsHelper <params<>> {
        using type = params<>;
    };

    template <typename Type, typename... OtherTypes>
    struct ExpandTypeOfParamsHelper<params<Type, OtherTypes...>> {
        using tempResult = ExpandTypeOfParamsHelper<params<OtherTypes...>>::type;
        using type = append_params<Type, tempResult>;
    };

    template <typename... Types, typename... OtherTypes>
    struct ExpandTypeOfParamsHelper<params<DIFrame::Get<Types...>, OtherTypes...>> {
        using tempResult = typename ExpandInjectorsInParamsHelper<params<OtherTypes...>>::type;
        using type = concat_params<params<Types...>, tempResult>;
    }

    template <typename ParasList>
    using ExpandTypeOfParams = typename ExpandTypeOfParamsHelper<ParasList>::type;

    //ConstructorProvider
    template <typename Signature>
    struct CreateConstructor {};

    template <typename Constructor, typename... Paras>
    struct CreateConstructor<Constructor(Paras...)> {
        static Constructor* fun(Paras... paras) {
            static_assert(!std::is_pointer<Constructor>::value, "DIFrame Error : Constructor cannot be a pointer");
            static_assert(std::is_constructible<Constructor, Paras...>::value, "Error, Constructor cannot be constructible with Paras");
            return new Constructor(std::forward<Paras>(paras)...);
        }
    };

    //AddDeps
}
}

#endif // DIFRAME_CONTAINER_PROCESS_H