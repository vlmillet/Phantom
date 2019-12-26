// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet. 
// Distributed under the MIT license. Text available here at http://www.wiwila.com/tools/phantom/license/ 
// ]

#include "RttiImpl.h"

namespace phantom
{
    namespace detail
    {
        template<class t_Ty>
        void DeleteDynHH<t_Ty, false, false>::apply(t_Ty* a_pInstance)
        {
            RttiMapData const& data = Rtti::Find(a_pInstance);
            PHANTOM_ASSERT(!data.isNull());
            data.delete_();
        }
    }
}

