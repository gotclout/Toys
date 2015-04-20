#include <string>
#include <sstream>
#include <cmath>
#include <iostream>

using namespace std;

void multiply()
{
  for(int i = 2; i < 11; ++i)
  {
    for(int j = 1; j < 13; ++j)
    {
      cout << i*j;
      if(j + 1 < 13) cout << " ";
    }
    cout << endl;
  }
}

void quadratic()
{
  int a, b, c;
  cout << "Input value for a\n >> ";
  cin >> a;
  cout << "Input value for b\n >> ";
  cin >> b;
  cout << "Input value for c\n >> ";
  cin >> c;

  if(a == 0) cout << "discrimant 2(" << a << ") is invalid division by 0\n";
  else
  {
    float v = (float) ((b*b) - ((4 * a) * c));

    float rt = sqrt(abs(v));

    float qneg = ((.0-b) - rt) / (2*a),
          qpos = ((.0-b) + rt) / (2*a);

    cout << endl;
    cout << "quadratic pos root solution: " << qpos << endl
         << "          neg root solution: " << qneg << endl;
  }
}

void printdigits()
{
  string s = "123456789";

  cout << s << endl;
  for(int i = 0; i < 5; ++i)
  {
    cout << s;
    if(i < 4) cout << "\t";
  }
  cout << endl;
}

void printtriangle()
{
  cout << endl;
  cout << "      *      " << endl
       << "     * *     " << endl
       << "    *   *    " << endl
       << "   *     *   " << endl
       << "  *       *  " << endl
       << " *********** " << endl
       << endl;
}

int main(int argc, char** argv)
{

  multiply();
  cout << endl;
  quadratic();
  cout << endl;
  printdigits();
  printtriangle();
  return 0;
}

