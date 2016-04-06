namespace Monads
{
    using System;
    using System.Linq;

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

            var loop = ((Enumerable.Range(1, 5).ToMonad()
                         | Control.For<int>((obj) =>
                         {
                             Console.WriteLine("For loop: " + obj);
                         })).Transmute(Control.Count<int>()) // Control count cannot be chained
                                                             // because it transmutes the IEnumerable
                                                             // monad into an int monad.
                        | Control.Add(19)).Transmute(Control.MndEquals(24)) // MndEquals transmutes T monad
                                                                            // into bool monad.
                       | Control.ThenIfNot(
                           (() => { Console.WriteLine("Not equals."); }))
                       | Control.ThenIf(
                           (() => { Console.WriteLine("Equals."); }));

            // Execution is deferred.
            Console.WriteLine("Monad declared.");
            
            // Monad is evaluated.
            cond.Inner();
            loop.Inner();

            Console.ReadLine();
        }

        static int nine()
        {
            Console.WriteLine("Nine called.");
            return 9;
        }
    }
}
