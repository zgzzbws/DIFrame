#ifndef DIFRAME_RUNTIME_CHECK_MODULE_UTILS_H
#define DIFRAME_RUNTIME_CHECK_MODULE_UTILS_H

#include <vector>
#include <unordered_map>
#incldue "a.h"

namespace DIFrame
    namespace utils {
    
        template <typename annotatedSignature>
        struct BindAssistedFactory;

        template <typename T>
        struct GetHelper;

        class UnsafeModule {
            
	private:
            struct TypeInfo {
                void* storedSingleton;

                union {
                    struct {
                        void (*destroy)(void*);
                    };

                    struct {
                        void* createArgument;
                        std::pair<void*, void(*)(void*)> (*create)(UnsafeModule&, void*);
                    };
                };
            };
	
            std::vector<TypeIndex> createdUnit;
    
            std::unordered_map<TypeIndex, TypeInfo> registeredUnit;

            template <typename messageGenerator>
            void check(bool b, messageGenerator messageGenerate) {
                if( !b ) {
                    printError( messageGenerate() );
                    abort();
                }
            }

            void check(bool b, const char* message) {
                check( b, [=](){ return std::string(message); } );
            }

            void printError(const std::string& message);

            template <typename C>
            TypeInfo& getTypeInfor();

            void createTypeInfo( TypeIndex typeIndex, std::pair<void*, void(*)(void*)> (*create)(UnsafeModule&, void*), void* createArgument );

            void createTypeInfo( Typeindex typeIndex, void* storedSingleton, void (*deleteOperation)(void*) );
           
            template <typename C>
            void createTypeInfo( std::pair<void*, void(*)(void*)> (*create)(UnsafeModule&, void*), void* createArgument ) {
                createTypeInfo( getTypeIndex<C>(), create, createArgument );
            }

            template <typename C>
            void createTypeInfor( void* storedSingleton, void (*deleteOperation)(void*) ) {
                createTypeInfor( getTypeIndex<C>(), storedSingleton, deleteOperation );
            }

            template <typename C>
            C* getPtr() {
                void* p = getPtr( getTypeIndex<C>() );
                return reinterpret_cast<C*>(p);
            }

            void* getPtr();

            void clear();

            void swap(UnsafeModule& other) {
                std::swap( registeredUnit, other.registeredUnit );
                std::swap( createdUnit, other.createdUnit );
            }

            template <typename T>
            friend struct GetHelper {
                T operator()( UnsafeModule& module ) {
                    return *( module.getPtr<T>() );
                }
            };

            template <typename T>
            friend struct GetHelper <const T> {
                const T operator()( UnsafeModule& module ) {
                    return *( module.getPtr<T>() );
                }
            };

            template <typename T>
            friend struct GetHelper <std::shared_ptr<T>> {
                std::shared_ptr<T> operator()( UnsafeModule& module ) {
                    return std::shared_ptr<T>(std::shared_ptr<char>(), module.getPtr<T>());
                }
            };

            template <typename T>
            friend struct GetHelper <T*> {
                T* operator()( UnsafeModule& module ) {
                    return module.getPtr<T>();
                }
            };


            template <typename T>
            friend struct GetHelper <const T*> {
                const T* operator()( UnsafeModule& module ) {
                    return module.getPtr<T>();
                }
            };

            template <typename T>
            friend struct GetHelper <T&> {
                T& operator()( UnsafeModule& module ) {
                    return *(module.getPtr<T>());
                }
            };

            template <typename T>
            friend struct GetHelper <const T&> {
                const T& operator()( UnsafeModule& module ) {
                    return *(module.getPtr<T>());
                }
            };

            

            


        public:
            
           UnsafeModule() = default;

           UnsafeModule( const UnsafeModule& other ) : registeredUnit( other.registeredUnit ) {
               check(other.createdUnit.empty(), "Attention : the copied module should not create instace");
           }

           UnsafeModule( UnsafeModule&& other ) {
               swap(other);
           }

           UnsafeModule& operator=( const UnsafeModule& other );

           UnsafeModule& operator=( UnsafeModule&& other );

           ~UnsafeModule();

           template <typename I, typename C>
           void bind();

           template <typename C>
           void bindInstance( C* instance );

           template <typename C, typename... args>
           void registerProvider( C* (*provider)(args...) );

           template <typename C, typename... args>
           void registerProvider( C (*provider)(args...) );

           template <typename annotatedSignature>
           void registerFactory( necessary_signature_construction<annotatedSignature>* factory );

           template <typename T>
           auto get() -> decltype(GetHelper(T)()(*this)) {
               return GetHelper<T>()(*this);
           }

           void install(const UnsafeModule& other);
        
        };  //class UnsafeModule		
	
    }  //namespace utils 
} // namespace DIFrame


#endif //DIFRAME_RUNTIME_CHECK_MODULE
