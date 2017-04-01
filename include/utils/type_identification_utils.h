#ifndef DIFRAME_TYPE_IDENTIFICATION_UTILS_H
#define DIFRAME_TYPE_IDENTIFICATION_UTILS_H

#include <memory>

namespace DIFrame {
    namespace utils {

        template <typename T>
        struct getTypeHelper {
            using type = T;
        };

        template <typename T>
        struct getTypeHelper <const T> {
            using type = T;
        };

        template <typename T>
        struct getTypeHelper <T&> {
            using type = T; 
        };

       template <typename T>
       struct getTypeHelper <const T&> {
           using type = T;
       };

       template <typename T>
       struct getTypeHelper <T*> {
           using type = T;
       };

       template <typename T>
       struct getTypeHelper <const T*> {
           using type = T;
       };   

       template <typename T>
       struct getTypeHelper <shared_ptr<T>> {
           using type = T;
       };
        
       template <typename T>
       using getType = typename getTypeHelper<T>::type;
       
       //temp struct; 
       template <typename T>
       struct NopDeleter {
           void operator()(T*){
           }
       };
    }  //namespace utils
}  //namespace DIFrame

#endif  //DIFRAME_TYPE_IDENTIFICATION_UTILS_H
