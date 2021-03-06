#ifndef DIFRAME_DETAIL_PARAMS_PROCESS_H
#define DIFRAME_DETAIL_PARAMS_PROCESS_H

namespace DIFrame{
namespace utils{

    template <typename... Types>
    struct params {};

    template <typename Type>
    struct is_params : std::false_type {};

    template <typename... Types>
    struct is_params <params<Types...>> : std::true_type {};

    template <typename Type, typename Parm>
    struct append_params_helper {};

    template <typename Type, typename... Parms>
    struct append_params_helper <Type, params<Parms...>> {
        using type = params<Type, Parms...>;
    }

    template <typename Type, typename Parm>
    using append_params = typename append_params_helper<Type, Parm>::type;

    //is_in_params

    //remove_from_params

    //concat_params

    //set_difference

}
}

#endif // DIFRAME_DETAIL_PARAMS_PROCESS_H