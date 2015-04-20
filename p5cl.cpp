#include <iostream>
#include <string>
#include <cmath>
#include <time.h>

using namespace std;


void guess()
{
  int rn = rand() % 100,
      g  = -1,
      at = 0;

  cout << "Lets Play A Guessing Game!" << endl
       << "I'll pick a number between 0 and 100, and you have 7 guesses"
       << "\nOK, I'm ready, what's your first guess?\n";

  while( ++at < 8 && g != rn)
  {
    cin >> g;
    if(g > rn && at < 8) cout << "Good guess but try a smaller number\n";
    else if(g < rn && at < 7) cout << "Good guess but try a larger number\n";
    else
      cout << "Congratulations, you've guessed my number!!!\n"
           << "You Win ^_^\nGame Over\n";
  }

  if(g != rn) cout << "The Number Is: " << rn
                   << "\nBetter Luck Next Time\n Game Over\n";
}

int main(int argc, char** argv)
{
  srand(time(0));

  guess();

  return 0;
}
