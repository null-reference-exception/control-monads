namespace Monads
{
    using System;

    public static class Control
    {
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
    }
}
