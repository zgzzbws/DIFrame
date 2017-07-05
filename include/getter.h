#ifndef DIFRAME_GETTER_H
#define DIFRAME_GETTER_H

#include "container.h"
#include "storage.h"

namespace DIFrame {
    
    template<typename... Types>
    class Get : public Storage<Types...> {
    public:

        Get() = delete;
        Get( const Get& ) = delete;
        
        Get( Get&& ) = default;

        Get( const Container<Types...>& container );
        Get( Container<Types...>&& container );
    };

} // namespace DIFrame

#endif // DIFRAME_GETTER_H