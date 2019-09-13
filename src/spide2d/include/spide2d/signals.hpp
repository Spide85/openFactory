#pragma once

#include <boost/signals2.hpp>

namespace spide2d {

namespace signals = boost::signals2;
template<typename T>
using signal     = signals::signal<T>;
using connection = signals::scoped_connection;

}  // namespace spide2d