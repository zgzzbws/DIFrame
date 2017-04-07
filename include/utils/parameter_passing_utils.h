#ifndef DIFRAME_PARAMETER_PASSING_H
#define DIFRAME_PARAMETER_PASSING_H

#include <functional>

namespace DIFrame {
    namespace utils {
        
        //The struct to pass the parameters
        template <typename... parameters>
        struct parm { };
        
        //********************
        //To judge whether the parameter list has one or multi-parameters
        //If there is just one parameter in the parameter list
        template <typename T>
        struct has_multi_parameters : public std::false_type {};

        //If there is more than one parameter in the list
        template <typename... Ts>
        struct has_multi_parameters <parm<Ts...>> : public std::true_type {};

        //********************
        //To attach a parameter to the parameter list
        template <typename T, typename P>
        struct attach_parm_impl {};

        template <typename T, typename... Ts>
        struct attach_parm_impl <T, parm<Ts...>> {
            using type = parm<T, Ts...>;
        };

        template <typename T, typename  P>
        using attach_parm = typename attach_parm_impl<T, P>::type;

        //********************
        //To join new parameter lists to the existing one
        template <typename... P>
        struct join_parm_impl {};

        template <>
        struct join_parm_impl <> {
            using type = parm <>;
        };

        template <typename P>
        struct join_parm_impl <P> {
            using type = P;
        };

       template <typename... Ts, typename... Us, typename... Ps>
       struct join_parm_impl <parm<Ts...>, parm<Us...>, Ps...> {
           using type = typename join_parm_impl<parm<Ts..., Us...>, Ps...>::type;
       };
 
       template <typename... Ps>
       using join_parm = typename join_parm_impl<Ps...>::type;

       //********************
       //To remove the existing parameter from parameter list
       template <typename T, typename P>
       struct remove_parm_impl {};

       template <typename T, typename... Ps>
       struct remove_parm_impl <T, parm<Ps...>> {
           using type = join_parm <
               typename std::conditional<std::is_same<T, Ps>::value, parm<>, parm<Ps>>::type
           ...>;
       };

       template <typename T, typename P>
       using remove_parm = typename remove_parm_impl<T, P>::type;
      
       //********************
       //To judge whether the parameter is in the list
       template<typename T, typename P>
       struct parm_in_list {};

       template <typename T, typename... Ts>
       struct parm_in_list <T, parm<Ts...>> : public static_or<std::is_same<T, Ts>::value...> {}; 
        
       //********************
       //If the parameter list is empty
       template<typename P>
       struct empty_parm_list : public std::is_same<parm<>, P> {};

       //********************
       //If you want to add the parameter to the list using this instead of calling join_parm_impl directly
       //This struct can attach the parameters according to the proper condition
       template <bool is_in_list, typename T, typename P>
       struct add_to_parm_impl {};

       template <typename T, typename P>
       struct add_to_parm_impl <false, T, P> {
           using type = join_parm <T, P>;
       };

       template <typename T, typename P>
       struct add_to_parm_impl <true, T, P> {
           using type = P
       };

       template <typename T, typename P>
       using add_to_parm = typename add_to_parm_impl<parm_in_list<T, P>::value, T, P>::type;

       //********************
       //To judge whether there is intersection between two parameter lists
        


     

    } //namespace utils
} //namespace DIFrame

#endif
