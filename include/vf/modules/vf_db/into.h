// Copyright (C) 2008 by Vinnie Falco, this file is part of VFLib.
// See the file LICENSE.txt for licensing information.
// Based on ideas from the soci wrapper sqlite back-end.

#ifndef __VF_DB_INTO_VFHEADER__
#define __VF_DB_INTO_VFHEADER__

#include "vf/modules/vf_db/detail/into_type.h"
#include "vf/modules/vf_db/detail/exchange_traits.h"
#include "vf/modules/vf_db/detail/type_conversion.h"

namespace db {

template<typename T>
detail::into_type_ptr into(T& t)
{
  return detail::do_into(t,
                         typename detail::exchange_traits<T>::type_family());
}

template<typename T>
detail::into_type_ptr into(T& t, indicator& ind)
{
  return detail::do_into(t, ind,
                         typename detail::exchange_traits<T>::type_family());
}

}

#endif