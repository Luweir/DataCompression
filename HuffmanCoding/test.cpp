#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int main()
{
    FILE *in = fopen("project/DataCompression/RunLengthEncoding/data.txt", "rb");
    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    cout << size << endl;
    cout << (getc(in) == EOF) << endl;
    return 0;
}