#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include<fstream>



using std::cout;
using std::endl;
using std::vector;
using std::ifstream;
using std::ofstream;


int main()
{

    size_t N = 8;

    vector < vector<unsigned int> > identity(N,vector<unsigned int>(N,0));


    for(size_t row =0; row < N ; row++)
    {

               identity[row][row] = 1;
            //identity[row][col+1] = 1;
    }

    for(size_t row = 0; row < N; row++)
    {
        for(size_t col =0; col < N; col++)
        {
            cout << identity[row][col];
        }
        cout << endl;
    }





    return 0;
}
