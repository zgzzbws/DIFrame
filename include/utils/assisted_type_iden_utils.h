#ifndef DIFRAME_ASSISTED_IDEN_UTILS_H
#define DIFRAME_ASSISTED_IDEN_UTILS_H

#include "type_identification_utils.h"

namespace DIFrame {

    template <typename T>
    struct Assisted;

    namespace utils {
       
          //********************
          //If there is a parameter marked with Assisted
          //We need to avoid this mark, and obtain the necessary parms
          template <typename P>
          struct obtain_necessary_parm_avoid_assisted_mark_impl {}; 

          template <>
          struct obtain_necessary_parm_avoid_assisted_mark_impl <parm<>> {
              using type = parm<>;
          };

          template <typename T, typename Ps...>
          struct obtain_necessary_parm_avoid_assisted_mark_impl <parm<Assisted<T>, Ps...>> {
              using type = typename obtain_necessary_parm_avoid_assisted_mark_impl<parm<Ps...>>::type;
          };

          template <typename T, typename Ps...>
          struct obtain_necessary_parm_avoid_assisted_mark_impl <parm<T, Ps...>> {
              using type = attach_parm<getType<T>, typename obtain_necessary_parm_avoid_assisted_mark_impl<parm<Ps...>>::type>;
          };

          template <typename P>
          using obtain_necessary_parm_avoid_assisted_mark = typename obtain_necessary_parm_avoid_assisted_mark_impl<P>::type;
          
          //********************
          //Now we just want to get the Assisted marked type wraped in the parm<>
          template <typename P>
          struct get_assisted_parm_impl {};
      
          template <>
          struct get_assisted_parm_impl <parm<>> {
              using type = parm<>;
          };

          template <typename T, typename... Ps>
          struct get_assisted_parm_impl <parm<T, Ps...>> {
              using type = typename get_assisted_parm_impl<parm<Ps...>>::type;
          };

          template <typename T, typename... Ps>
          struct get_assisted_parm_impl <parm<Assisted<T>, Ps...>> {
              using type = attach_parm<T, typename get_assisted_parm_impl<parm<Ps...>>::type>;
          };

          template <typename P>
          using get_assisted_parm = typename get_assisted_parm_impl<P>::type;

          //********************
          //Similiar to obtain_necessary_parm_avoid_assisted_mark
          template <typename P>
          struct get_non_assisted_parm_impl {};

          template <>
          struct get_non_assisted_parm_impl <parm<>> {
              using type = parm<>;
          }; 

          template <typename T, typename... Ps>
          struct get_non_assisted_parm_impl <parm<T, Ps...>> {
              using type = attach_parm<T, typename get_non_assisted_parm_impl<parm<Ps...>>::type>;
          };

         template <typename T, typename... Ps>
         struct get_non_assisted_parm_impl <parm<Assisted<T>, Ps...>> {
             using type = typename get_non_assisted_parm_impl<parm<Ps...>>::type;
         };

         template <typename P>
         using get_non_assisted_parm = typename get_non_assisted_parm_impl<P>::type;

         //********************
         //Unwrap the parameter in Assisted struct, and add it to the normal lists
         template <typename P>
         struct flat_assisted_parm_impl {};

         template <>
         struct flat_assisted_parm_impl <parm<>> {
             using type = parm<>;
         };

         template <typename T, typename... Ps>
         struct flat_assisted_parm_impl <parm<T, Ps...>> {
             using type = attach_parm<T, typename flat_assisted_parm_impl<parm<Ps...>>::type>;
         };

         template <typename T, typename... Ps>
         struct flat_assisted_parm_impl <parm<Assisted<T>, Ps...>> {
             using type = attach_parm<T, typename flat_assisted_parm_impl<parm<Ps...>>::type>;
         };

        template <typename P>
        using flat_assisted_parm = typename flat_assisted_parm_impl<P>::type;

        //********************
        //Now we start to compose all the things together to construct needed signature
        template <typename AnnotatedSignature>
        using necessary_signature_construction = construct_signature<signatureType<AnnotatedSignature>, flat_assisted_parm<signatureArgs<AnnotatedSignature>>>;

        template <typename AnnotatedSignature>
        using function_injection_with_assisted = construct_signature<signatureType<AnnotatedSignature>, get_assisted_parm<signatureArgs<AnnotatedSignature>>>;

        //********************
        //To count the Assisted marks
        template <int index, typename P>
        struct assisted_count {};

        template <typename T, typename... Ps>
        struct assisted_count <0, parm<T, Ps...>> : public std::integral_constant<int, 0> {};

        template <typename T, typename...Ps>
        struct assisted_count <0, parm<Assisted<T>, Ps...>> : public std:integral_constant<int, 0> {};

        template <int index, typename T, typename... Ps>
        struct assisted_count <index, parm<T, Ps...>> : public assisted_count<index-1, parm<Ps...>> {};

        template <int index, typename T, typename... Ps>
        struct assisted_count <index, parm<Assisted<T>, Ps...>> :  public std:integral_constant<int, 1+assisted_count<index-1, parm<Ps...>>::value> {};
          

    } //utils
}  //DIFrame

#endif //ASSISTED_IDEN_UTILS_H
