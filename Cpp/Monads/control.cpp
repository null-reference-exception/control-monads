#include "control.h"

namespace monads
{
    control::control()
    {

    }

    auto control::then_if(std::function<void(void)> const is_true) -> MONAD_CHAIN(bool)
    {
        return [is_true](std::function<bool(void)> func) -> monad<bool>
        {
            auto eval = func();
            if (eval)
            {
                is_true();
            }

            return monad<bool>(eval);
        };
    }

    auto control::then_if_not(std::function<void(void)> const expr) -> MONAD_CHAIN(bool)
    {
        return [expr](std::function<bool(void)> func) -> monad<bool>
        {
            auto eval = func();
            if (!eval)
            {
                expr();
            }

            return monad<bool>(eval);
        };
    }

    auto control::add(int const expr) -> MONAD_CHAIN(int)
    {
        return [expr](std::function<int(void)> func) -> monad<int>
        {
            auto eval = func();

            return monad<int>(eval + expr);
        };
    }

}
