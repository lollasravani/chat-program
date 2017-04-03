#include <iostream>
using namespace std;


float div(int a, int b)
{
    if (b == 0)
        throw "Division by zeno is not possible";
    return (float) a/b;
}

int main()
{
    try
    {
        float v = div(10,0);
        cout << "Value is : " << v;
    }
    catch ( ... )
    {
        cout << "Error while executing division";
    }
    return 0;

}
