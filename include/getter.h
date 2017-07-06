#ifndef DIFRAME_GETTER_H
#define DIFRAME_GETTER_H

#include "container.h"

namespace DIFrame {

namespace util {

    //is obtain the instance?
    class Instance;

    template <typename T>
    struct GetHelper;

}
    
    template<typename... Types>
    class Get {
    private:
        using Parms = DIFrame::util::Parm<Types...>;
        using Cont = Container<Types...>;

        DIFrame::util::Instance* instance;

        //Get( fruit::util::Instance* instance );

        friend class DIFrame::util::Instance;

        template <typename T>
        friend struct fruit::util::GetHelper;

    public:

        Get() = delete;
        Get( const Get& ) = delete;

        Get( Get&& ) = default;

        Get( const Container<Types...>& container );
        Get( Container<Types...>&& container );

        template <typename T>
        T obtain();

        template <typename T>
        explicit operator T();
    };

} // namespace DIFrame

#endif // DIFRAME_GETTER_H