#ifndef DIFRAME_GETTER_H
#define DIFRAME_GETTER_H

#include "container.h"

namespace DIFrame {
    
    template<typename... Types>
    class Get {
    private:

    public:
        Get() = delete;
        Get( const Get& ) = delete;
        Get( Get&& ) = default;

        Get( const Cont& cont );
        Get( Cont&& cont );

        template <typename T>
        T obtain();

        template <typename T>
        explicit operator T();
    };

}

#endif // DIFRAME_GETTER_H