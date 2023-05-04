#include"S2T.h"
#include<string>

using namespace std;

int main(int argc, char** argv){
    string i = argv[1];
    string o = argv[2];
    S2T s2t(i,o);
    s2t.Run();
    return 0;
}