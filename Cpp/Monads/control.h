#ifndef CONTROL_H
#define CONTROL_H

#include <functional>
#include <iostream>

#include "monad.h"

namespace monads
{
    class control
    {
    public:
        control();

        auto static then_if(std::function<void(void)> const &is_true) -> MONAD_CHAIN(bool);
        auto static then_if_not(std::function<void(void)> const &expr) -> MONAD_CHAIN(bool);
        #define DECIDE(cond, yes, no) monad<bool>(cond)|control::then_if(CLAM(yes))||control::then_if_not(CLAM(no));

        auto static add(int const expr) -> MONAD_CHAIN(int);

        template <typename T>
        auto static mnd_equals(T const &expr) -> MONAD_TRANSMUTE(T, bool)
        {
            return [&expr](std::function<T(void)> func) -> monad<bool>
            {
                auto eval = func();

                return monad<bool>(eval == expr);
            };
        }
    };
}

#endif // CONTROL_H
