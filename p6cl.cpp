#include <iostream>
#include <climits>
#include <cmath>
#include <sstream>
#include <string>

using namespace std;

string word_rev()
{
  string ret = "";
  size_t sz  = 0;

  cout << "Please input the string you would like to reverse.\n>> ";
  cin  >> ret;
  cout << "\nreverse string: " << ret << " -> ";

  sz   = ret.size();

  ret += " ";

  for(size_t i = 0; i < sz / 2; ++i)
  {
    ret[sz] = ret[i];
    ret[i]  = ret[sz - i - 1];
    ret[sz  - i - 1] = ret[sz];
  }
  ret.erase(sz);

  cout << ret << endl;

  return ret;
}

bool prime()
{
  bool ret = true;
  int    p = 0;

  cout << "Enter an integer value to verify a prime\n>> ";
  cin  >> p;

  if(p < 2)
  {
    ret = false;
  }
  else
  {
    int rp = (int) sqrt(p);
    for(int i = 2; i <= rp && ret; ++i)
    {
      if(p%i == 0)ret = false;
    }
  }

  if(ret) cout << "The value " << p << " is a prime number\n";
  else    cout << "The vlaue " << p << " is not a prime number\n";

  return ret;
}

void eratos()
{
  int mp = 0;

  cout << "Input an integer value to bound the max prime number.\n>> ";
  cin  >> mp;

  if(mp > INT_MAX)
  {
    cout << "This program only computes prime numbers <= "
         << INT_MAX << endl;
  }
  else
  {
    bool p[mp];

    int sq,
        i;

    for(int i = 0; i < mp; ++i) p[i] = true;

    p[0] = p[1] = false;


    for(i = 2; (sq = i*i) <= mp; ++i)
    {
      if(p[i])
      {
        for(int j = sq, k = 0; j < mp; ++k, j = sq + (k*i))
        {
          if(p[j])
          {
            p[j] = false;
          }
        }
      }
    }

    cout << "All primes <= " << mp << ":\n";

    for(i = 2; i < mp; ++i)
    {
      if(p[i])
      {
        cout << i << ": is prime\n";
      }
    }
  }

}

int main(int argc, char** argv)
{
  string rs = word_rev();

  prime();

  eratos();

  return 0;
}

