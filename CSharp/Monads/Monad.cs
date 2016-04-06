namespace Monads
{
    using System;

    /// <summary>
    /// A simple monad.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class Monad<T>
    {
        /// <summary>
        /// An object around which a monad is constructed.
        /// In order to defer monad execution, the inner object
        /// must be a function type.
        /// </summary>
        public Func<T> Inner { get; private set; }

        /// <summary>
        /// Constructor used for creating terminal monads.
        /// </summary>
        /// <param name="inner"></param>
        public Monad(T inner)
        {
            this.Inner = () => { return inner; };
        }

        /// <summary>
        /// Constructor used for chaining monads through a unit operator.
        /// </summary>
        /// <param name="inner"></param>
        public Monad(Func<T> inner)
        {
            this.Inner = inner;
        }

        /// <summary>
        /// This is a monadic "unit" operator.
        /// It passes the input monad's inner object to a function that should chain the inner
        /// object to an inner object of a new monad (by calling the input parameter function
        /// in the body of the returned function) and do some useful things along the way.
        /// </summary>
        /// <param name="input">Input monad.</param>
        /// <param name="func">Monadic chain function.</param>
        /// <returns></returns>
        public static Monad<T> operator |(Monad<T> input, Func<Func<T>, Monad<T>> func)
        {
            return new Monad<T>(() => func(input.Inner).Inner());
        }

        /// <summary>
        /// As operators cannot be generic in C#, we need a method to transmute a monad
        /// of type T into a monad of type Q. This allows us, for instance, to cut a chain
        /// of IEnumerable monads, count the elements and from there on, work only with
        /// int monads. Then maybe compare the int to another int and work only with
        /// bool monads from there on.
        /// </summary>
        /// <typeparam name="Q"></typeparam>
        /// <param name="func"></param>
        /// <returns></returns>
        public Monad<Q> Transmute<Q>(Func<Func<T>, Monad<Q>> func)
        {
            return new Monad<Q>(() => func(this.Inner).Inner());
        }
    }

    /// <summary>
    /// A simple extension class that allows existing types to be
    /// easily wrapped inside a comfy monad.
    /// </summary>
    public static class MonadExtension
    {
        /// <summary>
        /// Wraps any type inside a comfy monad.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="obj"></param>
        /// <returns></returns>
        public static Monad<T> ToMonad<T>(this T obj)
        {
            return new Monad<T>(obj);
        }
    }
}
