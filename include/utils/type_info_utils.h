#ifndef DIFRAME_TYPE_INFO
#define DIFRAME_TYPE_INFO

#include <typeinfo>

namespace DIFrame {
    namespace utils {
        
        struct TypeIndex {

            constexpr TypeIndex( const std::type_info& impl ) noexcept : impl( &impl ) {}
                        
            bool operator==( const TypeIndex& __rhs ) const noexcept {
                return *impl == *__rhs.impl;
            }            

            bool operator!=( const TypeIndex& __rhs ) const noexcept {
                return *impl != *__rhs.impl;
            }

            std::size_t hash_code() noexcept {
                return impl->hash_code();
            }

            const char* name() const {
                return impl->name();
            }

        private:
         
            const std::type_info* impl;

        };

        template <typename T>
        TypeIndex getTypeIndex() noexcept {
            static constexpr TypeIndex index = typeid(T);
            return index;
        }

    } //namespace utils
} //namespace DIFrame

namespace std {

    template <typename _Tp>
    struct hash;

    template <>
    struct hash<DIFrame::utils::TypeIndex> {
        typedef std::size_t result_type;
        typedef DIFrame::utils::TypeIndex argument_type;

        std::size_t operator()( const DIFrame::utils::TypeIndex& typeindex ) const noexcept {
            return typeindex.hash_code();
        }
    };

} //namespace std

#endif //DIFRAME_TYPE_INFO
