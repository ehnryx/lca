/* Macros (maybe useful)
 * USAGE:
 *  don't
 * STATUS:
 *  untested
 */
#pragma once

// FOR_EACH macro
// max 256-342 times

#define _PARENS ()

#define _EXPAND0(...) __VA_ARGS__
#define _EXPAND1(...) _EXPAND0(_EXPAND0(_EXPAND0(_EXPAND0(__VA_ARGS__))))
#define _EXPAND2(...) _EXPAND1(_EXPAND1(_EXPAND1(_EXPAND1(__VA_ARGS__))))
#define _EXPAND3(...) _EXPAND2(_EXPAND2(_EXPAND2(_EXPAND2(__VA_ARGS__))))
#define _EXPAND4(...) _EXPAND3(_EXPAND3(_EXPAND3(_EXPAND3(__VA_ARGS__))))

#define _FOR_EACH_IMPL2() _FOR_EACH_IMPL
#define _FOR_EACH_IMPL(macro, arg1, ...) \
  macro(arg1) __VA_OPT__(_FOR_EACH_IMPL2 _PARENS(macro, __VA_ARGS__))
#define FOR_EACH(macro, ...) __VA_OPT__(_EXPAND4(_FOR_EACH_IMPL(macro, __VA_ARGS__)))

// FOR_EACH macro DONE
