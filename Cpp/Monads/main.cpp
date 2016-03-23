#include <iostream>

#include "monad.h"
#include "control.h"

using namespace std;
using namespace monads;

void fn1() {cout << "AYE" << endl;}
void fn2() {cout << "NAY" << endl;}

class thing
{
public:
    int n{0};

    thing(int t_n)
    {
        this->n = t_n;
    }

    thing(const thing& th) : n(th.n)
    {
        //cout << "(Thing copy ctor used.)" << endl;
    }

    thing(thing&& th) noexcept : n(std::move(th.n))
    {
        //cout << "(Thing move ctor used.)" << endl;
    }
};

int main(int argc, char *argv[])
{
    cout << "Monad definition start." << endl;
    monad<bool> xx =
    monad<bool>(false)
        | control::then_if(LAM(cout << "TRUE" << endl;))
        | control::then_if_not(LAM(cout << "FALSE" << endl;));

    auto yy =
    (monad<int>(4)
        | control::add(6)
        | control::add(91)
        | control::mnd_equals(19) // Transmuted to bool monad.
        | control::then_if(LAM(cout << "EQUALS" << endl;))
       || control::then_if_not(LAM(cout << "DOESN'T EQUAL" << endl;))) // || triggers execution after the following link
        | control::then_if_not(LAM(cout << "DOESN'T EQUAL 2" << endl;)); // The rest is deferred.


    // For-each cycle.
    std::vector<string> k = {"flippin'", "string", "vector"};
    cout << "ksize: " << k.size() << endl;
    auto loop1 =
    monad<std::vector<string>>(k)
        | control::append<string>("damn")
        | control::append<string>("right!")
        | control::mnd_for<string>([](string &e)
            {
                cout << e << endl;
            })
        | control::intercept<vector<string>>([](vector<string> &vs)
            {
                cout << "size: " << vs.size() << endl;
            });

    // For cycle.
    auto loop2 =
    monad<int2tuple>(std::make_tuple(1, 4))
        | control::mnd_for([](int i)
            {
                cout << i << endl;
            });

    auto th = thing(19);
    auto mov =
    monad<thing>(th)
        | control::intercept<thing>([](thing &ic)
            {
                ;
            });

    // Mini linq. (Also inefficient as hell. :) )
    vector<thing> things = {thing(1), thing(2), thing(3), thing(4), thing(5), thing(6)};
    auto mini_linq =
    monad<vector<thing>>(things)
        | control::where<thing>([](thing &t)
            {
                return t.n > 3;
            })
        | control::select<thing, int>([](thing &t)
            {
                return t.n;
            })
        | control::mnd_for<int>([](int t)
            {
                cout << "things after where: " << t << endl;
            });

    // Monad execution deferred.
    cout << "Monad definition end." << endl;

    // Monads execute their chains.
    xx.inner();
    yy.inner();
    //loop1.inner(); // Chain can also be executed by chaining to an
                     // identity monad through ||:
    loop1 || control::identity<std::vector<string>>();

    loop2.inner();

    monad<bool>(1 + 1 == 2)||control::then_if(LAM(cout << "1 + 1 == 2" << endl;));

    // Ternary operator without a need to assign implemented using control monads.
    DECIDE(1 + 1 == 3, fn1();, fn2(););
    // ==
    monad<bool>(1 + 1 == 3)|control::then_if(CLAM(fn1();))||control::then_if_not(CLAM(fn2();));
    // CLAM is all-capturing lambda: [=](){....}

    mov.inner();

    mini_linq.inner();

    cout << "END" << endl;
}
