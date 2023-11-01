/* Type Traits
 * STATUS
 *  untested
 */
#pragma once

#include <type_traits>

namespace utility {
template <typename T>
using bigger_type = std::conditional_t<std::is_same_v<T, int>, long long, T>;
}
