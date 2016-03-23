#ifndef CONTROL_H
#define CONTROL_H

#include <functional>
#include <iostream>
#include <vector>
#include <tuple>

#include "monad.h"

namespace monads
{
    typedef std::tuple<int, int> int2tuple;

    class control
    {
    public:
        control();

        // Identity.
        template <typename T>
        auto static identity() -> MONAD_CHAIN(T)
        {
            return [](std::function<T(void)> func) -> monad<T>
            {
                auto eval = func();
                return monad<T>(std::move(eval));
            };
        }

        // Intercept any monad chain and tamper with its current inner object.
        template <typename T>
        auto static intercept(std::function<void(T &)> with) -> MONAD_CHAIN(T)
        {
            return [with](std::function<T(void)> func) -> monad<T>
            {
                auto eval = func();
                with(eval);

                return monad<T>(std::move(eval));
            };
        }

        // Conditionals with bool.
        auto static then_if(std::function<void(void)> const is_true) -> MONAD_CHAIN(bool);
        auto static then_if_not(std::function<void(void)> const expr) -> MONAD_CHAIN(bool);
        #define DECIDE(cond, yes, no) monad<bool>(cond)|control::then_if(CLAM(yes))||control::then_if_not(CLAM(no));

        // int
        auto static add(int const expr) -> MONAD_CHAIN(int);

        // For-each with std::vector.
        template <typename T>
        auto static mnd_for(std::function<void(T &)> expr) -> MONAD_CHAIN(std::vector<T>)
        {
            return [expr](std::function<std::vector<T>(void)> func) -> monad<std::vector<T>>
            {
                auto eval = func();

                std::for_each(std::begin(eval), std::end(eval), [expr](T element)
                {
                    expr(element);
                });

                return monad<std::vector<T>>(std::move(eval));
            };
        }

        // Append with std::vector.
        template <typename T>
        auto static append(T thing) -> MONAD_CHAIN(std::vector<T>)
        {
            return [thing](std::function<std::vector<T>(void)> func) -> monad<std::vector<T>>
            {
                auto eval = func();

                eval.push_back(thing);

                return monad<std::vector<T>>(std::move(eval));
            };
        }

        // Where.
        template <typename T>
        auto static where(std::function<bool(T &)> expr) -> MONAD_CHAIN(std::vector<T>)
        {
            return [expr](std::function<std::vector<T>(void)> func) -> monad<std::vector<T>>
            {
                auto eval = func();
                auto ret = std::vector<T>();

                std::for_each(std::begin(eval), std::end(eval), [expr, &ret](T element)
                {
                    if (expr(element))
                    {
                        ret.push_back(element);
                    }
                });

                return monad<std::vector<T>>(std::move(ret));
            };
        }

        // Select.
        template <typename T, typename Q>
        auto static select(std::function<Q(T &)> expr) -> MONAD_TRANSMUTE(std::vector<T>, std::vector<Q>)
        {
            return [expr](std::function<std::vector<T>(void)> func) -> monad<std::vector<Q>>
            {
                auto eval = func();
                auto ret = std::vector<Q>();

                std::for_each(std::begin(eval), std::end(eval), [expr, &ret](T element)
                {
                    ret.push_back(expr(element));
                });

                return monad<std::vector<Q>>(std::move(ret));
            };
        }

        // For cycle with int tuple.
        auto static mnd_for(std::function<void(int)> expr) -> MONAD_CHAIN(int2tuple)
        {
            return [expr](std::function<int2tuple(void)> func) -> monad<int2tuple>
            {
                auto eval = func();

                int start = std::get<0>(eval);
                int end = std::get<1>(eval);

                for(auto i = start; i < end; ++i)
                {
                    expr(i);
                }

                return monad<int2tuple>(std::move(eval));
            };
        }

        // Equals (transmute).
        template <typename T>
        auto static mnd_equals(T const expr) -> MONAD_TRANSMUTE(T, bool)
        {
            return [expr](std::function<T(void)> func) -> monad<bool>
            {
                auto eval = func();

                return monad<bool>(eval == expr);
            };
        }

        // Manual trasmute.
        template <typename A, typename B>
        auto static transmute(MONAD_TRANSMUTE(A, B) const expr) -> MONAD_TRANSMUTE(A, B)
        {
            return [expr](std::function<A(void)> func) -> monad<B>
            {
                auto eval = func();
                auto ret = expr(eval);

                return monad<B>(std::move(ret));
            };
        }
    };
}

#endif // CONTROL_H
