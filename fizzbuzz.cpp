#include <iostream>

using namespace std;

/**
 * A program that prints the numbers from 1 to 100. But for multiples of three
 * prints “Fizz” instead of the number and for the multiples of five prints
 * “Buzz”. For numbers which are multiples of both three and five prints
 * “FizzBuzz”.
 *
 * herp derp really?
 */
int main(int argc, char** argv)
{
  for(int i = 1; i < 101; i++)
  {
    if(i%5 == 0 && i%3 == 0) cout << "FizzBuzz ";
    else if(i%3 == 0)        cout << "Fizz ";
    else if(i%5 == 0)        cout << "Buzz ";
    else                     cout << i << " ";
  }
  cout << endl;

  return 0;
}
