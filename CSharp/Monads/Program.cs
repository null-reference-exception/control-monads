namespace Monads
{
    using System;

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Will declare monad.");

            var cond = true.ToMonad()
                       | Control.And(() => nine() + 1 == 10)
                       | Control.And(() => nine() + 1 == 10)
                       | Control.ThenIf(
                           () => { Console.WriteLine("TRUE"); },
                           () => { Console.WriteLine("FALSE"); })
                       | Control.And(() => nine() + 1 == 20)
                       | Control.Not()
                       | Control.ThenIf(
                           () => { Console.WriteLine("TRUE 2"); },
                           () => { Console.WriteLine("FALSE 2"); })
                       | Control.Not()
                       | Control.ThenIfNot(
                           () => { Console.WriteLine("FALSE 3"); });

            // Execution is deferred.
            Console.WriteLine("Monad declared.");
            
            // Monad is evaluated.
            cond.Inner();

            Console.ReadLine();
        }

        static int nine()
        {
            Console.WriteLine("Nine called.");
            return 9;
        }
    }
}
