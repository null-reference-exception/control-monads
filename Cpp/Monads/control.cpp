#include "control.h"

namespace monads
{
    control::control()
    {

    }

    auto control::then_if(std::function<void(void)> const &is_true) -> std::function<monad<bool>(std::function<bool(void)>)>
    {
        return [&is_true](std::function<bool(void)> func) -> monad<bool>
        {
            auto eval = func();
            if (eval)
            {
                is_true();
            }

            return monad<bool>(eval);
        };
    }

    auto control::then_if_not(std::function<void(void)> const &expr) -> std::function<monad<bool>(std::function<bool(void)>)>
    {
        return [&expr](std::function<bool(void)> func) -> monad<bool>
        {
            auto eval = func();
            if (!eval)
            {
                expr();
            }

            return monad<bool>(eval);
        };
    }

    auto control::add(int const expr) -> std::function<monad<int>(std::function<int(void)>)>
    {
        return [expr](std::function<int(void)> func) -> monad<int>
        {
            auto eval = func();

            return monad<int>(eval + expr);
        };
    }

}
