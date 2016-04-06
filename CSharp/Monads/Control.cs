namespace Monads
{
    using System;
    using System.Linq;
    using System.Collections.Generic;

    public static class Control
    {
        // Chain functions. --------------------------------------------------------------------

        /// <summary>
        /// Monadic chain function that triggers action based on the input bool monad.
        /// Unchanged input monad is sent to the output.
        /// </summary>
        /// <param name="isTrue">Action to perform upon input bool monad being true.</param>
        /// <param name="isFalse">Action to perform upon input bool monad being false.</param>
        /// <returns>The input monad.</returns>
        public static Func<Func<bool>, Monad<bool>> ThenIf(Action isTrue, Action isFalse = null)
        {
            return (func =>
            {
                var eval = func();
                if (eval)
                {
                    if (isTrue != null)
                    {
                        isTrue();
                    }
                }
                else
                {
                    if (isFalse != null)
                    {
                        isFalse();
                    }
                }

                return eval.ToMonad();
            });
        }

        /// <summary>
        /// Monadic chain function that triggers action based on the input bool monad.
        /// </summary>
        /// <param name="conseq">Action to perform upon input bool monad being false.</param>
        /// <returns>The input monad.</returns>
        public static Func<Func<bool>, Monad<bool>> ThenIfNot(Action conseq)
        {
            return (func =>
            {
                var eval = func();
                if (!eval)
                {
                    if (conseq != null)
                    {
                        conseq();
                    }
                }

                return eval.ToMonad();
            });
        }

        /// <summary>
        /// Monadic chain function that adds the expression with the input monad and
        /// sends the resulting monad to the output.
        /// </summary>
        /// <param name="expr">Expression to be added with.</param>
        /// <returns>Resulting monad.</returns>
        public static Func<Func<bool>, Monad<bool>> And(Func<bool> expr)
        {
            return (func => (func() && expr()).ToMonad());
        }

        /// <summary>
        /// Monadic chain function that ors the expression with the input monad and
        /// sends the resulting monda to the output.
        /// </summary>
        /// <param name="expr">Expression to be or'd with.</param>
        /// <returns>Resulting monad.</returns>
        public static Func<Func<bool>, Monad<bool>> Or(Func<bool> expr)
        {
            return (func => (func() || expr()).ToMonad());
        }

        /// <summary>
        /// Monadic chain function that negates the input monad and sends it on the output.
        /// </summary>
        /// <returns>Negated input monad.</returns>
        public static Func<Func<bool>, Monad<bool>> Not()
        {
            return (func => (!func()).ToMonad());
        }

        /// <summary>
        /// Monadic chain function that performs action for every element of an IEnumerable.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="action"></param>
        /// <returns></returns>
        public static Func<Func<IEnumerable<T>>, Monad<IEnumerable<T>>> For<T>(Action<T> action)
        {
            return (func =>
            {
                var eval = func();
                foreach (var obj in eval)
                {
                    action(obj);
                }

                return eval.ToMonad();
            });
        }

        public static Func<Func<int>, Monad<int>> Add(int number)
        {
            return (func => (func() + number).ToMonad());
        }

        // Transmutation functions. --------------------------------------------------------------

        /// <summary>
        /// Counts the elements inside input IEnumerable monad and return a new
        /// int monad containing the count.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public static Func<Func<IEnumerable<T>>, Monad<int>> Count<T>()
        {
            return (func =>
            {
                var eval = func();
                return eval.Count().ToMonad();
            });
        }

        /// <summary>
        /// Calls Equals(other) on input monad inner object and return a
        /// new bool monad with the result of the comparison.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="other"></param>
        /// <returns></returns>
        public static Func<Func<T>, Monad<bool>> MndEquals<T>(T other)
        {
            return (func =>
            {
                var eval = func();
                return eval.Equals(other).ToMonad();
            });
        } 
    }
}
