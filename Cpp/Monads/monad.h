#ifndef MONAD_H
#define MONAD_H

#include <functional>
#include <iostream>

namespace monads
{
    #define MONAD_CHAIN(type) std::function<monad<type>(std::function<type(void)>)>
    #define MONAD_TRANSMUTE(from, to) std::function<monad<to>(std::function<from(void)>)>
    #define LAM(x) [](){x}
    #define CLAM(x) [=](){x}

    template <typename T>
    class monad
    {
    public:
        std::function<T(void)> inner;

        monad(std::function<T(void)> inner)
        {
            this->inner = inner;
        }

        monad(T inner)
        {
            this->inner = [inner]() -> T { return inner; };
        }

        ///
        /// C++ operator overloading can be generic (contrary to C#).
        /// This allows us to make the monadic "unit" operator able to
        /// transmute monads.
        ///
        /// When A is the same as B, func is a monadic chain function.
        /// When A differs from B, func is a transmutation function.
        ///
        /// For instance, "and" is bool -> bool -- it chains monads,
        /// but "equals" is T -> bool -- it transmutes the T monad into
        /// a bool monad.
        ///
        template <typename A, typename B>
        friend auto operator|(monad<A> const input, MONAD_TRANSMUTE(A, B) func) -> monad<B>
        {
            //std::cout << "M unit | outer" << std::endl;
            return monad<B>([input, func]() -> B
            {
                //std::cout << "M unit | inner." << std::endl;
                return func(input.inner).inner();
            });
        }

        ///
        /// Monadic unit operator with immediate execution.
        ///
        template <typename A, typename B>
        friend auto operator||(monad<A> const input, MONAD_TRANSMUTE(A, B) const func) -> monad<B>
        {
            //std::cout << "M unit || outer" << std::endl;
            auto mnd = monad<B>([input, func]() -> B
            {
                //std::cout << "M unit || inner" << std::endl;
                return func(input.inner).inner();
            });

            mnd.inner();

            return mnd;
        }
    };
}

#endif // MONAD_H
