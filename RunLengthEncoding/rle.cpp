//游程编码
#include <iostream>
#include <assert.h>
#include <bits/stdc++.h>

using namespace std;

// Error handler: print message if any, and exit
void quit(const char *message = 0)
{
    if (message)
        cout << message << endl;
    exit(1);
}

class RLE
{
private:
    string raw;
    string comp;

public:
    long long inSize;
    long long outSize;
    RLE()
    {
    }
    string GetComp()
    {
        return comp;
    }
    // Compress pure letter string data
    void CompressPureString()
    {
        int len = raw.size();
        if (len == 0)
        {
            cout << "the size of s is 0" << endl;
        }
        comp.clear();
        int count = 1;
        for (int i = 1; i < len; i++)
        {
            if (raw[i] == raw[i - 1])
            {
                count++;
            }
            else
            {
                comp += to_string(count) + raw[i - 1];
                count = 1;
            }
        }
        comp += to_string(count) + raw[len - 1];
    }
    // Decompress pure alphabetic string data
    string DecompressPureString(string ss = "")
    {
        string curS = ss == "" ? comp : ss;
        string res = "";
        int i = 0, j = 0;
        int len = curS.size();
        while (j < len)
        {
            if (curS[j] >= '0' && curS[j] <= '9')
            {
                j++;
                continue;
            }
            else
            {
                res += string(stoi(curS.substr(i, j - i + 1)), curS[j]);
                j++;
                i = j;
            }
        }
        return res;
    }
    // General string compression
    void CompressCommonString()
    {
        int len = raw.size();
        if (len <= 1)
        {
            cout << "Illegal size" << endl;
        }
        comp.clear();
        int count = 1;
        for (int i = 1; i < len; i++)
        {
            if (raw[i] == raw[i - 1] && count < 9)
            {
                count++;
            }
            else
            {
                comp += to_string(count) + raw[i - 1];
                count = 1;
            }
        }
        comp += to_string(count) + raw[len - 1];
    }
    // common string decompression
    string DecompressCommonString(string ss = "")
    {
        string curS = ss == "" ? comp : ss;
        string res = "";
        int j = 1;
        int len = curS.size();
        while (j < len)
        {
            res += string(curS[j - 1] - '0', curS[j]);
            j += 2;
        }
        return res;
    }
    // Randomly generate RLE-friendly pure letter strings
    void RandomPureString(int length)
    {
        raw.clear();
        srand(int(time(0)));
        while (length > 0)
        {
            int cur_length = rand() % length + 1;
            char cur_char = rand() % 27 + 'a';
            raw += string(cur_length, cur_char);
            length -= cur_length;
        }
    }
    // Randomly generate RLE-friendly strings
    void RandomCommonString(int length)
    {
        raw.clear();
        srand(int(time(0)));
        while (length > 0)
        {
            int cur_length = rand() % length + 1;
            char cur_char = rand() % 95 + 32;
            raw += string(cur_length, cur_char);
            length -= cur_length;
        }
    }
    // compress the file in to out
    void CompressFile(fstream &in, fstream &out)
    {
        long long inLength = 0;
        long long outLnegth = 0;
        unsigned char c;
        unsigned char last;
        in.read((char *)&last, sizeof(c));
        unsigned char count = 1;
        while (in.read((char *)&c, sizeof(c)))
        {
            if (c == last && count < 255)
            {
                count++;
            }
            else
            {
                out.write((char *)&count, sizeof(count));
                out.write((char *)&last, sizeof(c));
                outLnegth += 2;
                inLength += count;
                count = 1;
                last = c;
            }
        }
        out.write((char *)&count, sizeof(count));
        out.write((char *)&c, sizeof(c));
        inSize = inLength + count;
        outSize = outLnegth + 2;
    }
    // Decompress the file in to out
    void DecompressFile(fstream &in, fstream &out)
    {
        long long inLength = 0;
        long long outLnegth = 0;
        unsigned char count;
        unsigned char c;
        while (in.read((char *)&count, sizeof(count)) && in.read((char *)&c, sizeof(c)))
        {
            inLength += 2;
            outLnegth += count;
            while (count--)
            {
                out.write((char *)&c, sizeof(c));
            }
        }
        outSize = outLnegth;
        inSize = inLength;
    }
    void SetRaw(string newS)
    {
        //set s[0]='\0'
        raw.clear();
        raw = newS;
    }
    string GetRaw()
    {
        return raw;
    }
};

int main(int argc, char **argv)
{
    if (argc != 4 || argv[1][0] != 'c' && argv[1][0] != 'd')
    {
        cout << "rle file compressor (C) 2021, luweir\n\n"
             << "To compress:   rle c input output  \n"
             << "To decompress: rle d input output  \n"
             << endl;
        return 1;
    }
    clock_t start = clock();
    RLE *r = new RLE();

    // begin -> test
    // r->RandomCommonString(28);
    // cout << r->GetRaw() << endl;
    // r->CompressCommonString();
    // cout << r->GetComp() << endl;
    // cout << r->DecompressCommonString() << endl;
    // FILE *in = fopen(argv[0], "rb");
    // end

    // compress file
    fstream in(argv[2], ifstream::in | ios::binary);
    fstream out(argv[3], ios::trunc | ios::binary | ofstream ::out);
    if (!in)
    {
        perror(argv[0]);
        exit(1);
    }
    if (argv[1][0] == 'c')
    {
        r->CompressFile(in, out);
    }
    // decompress file
    if (argv[1][0] == 'd')
    {
        r->DecompressFile(in, out);
    }

    printf("%ld -> %ld in %1.4f sec. \n",
           r->inSize, r->outSize, double(clock() - start) / CLOCKS_PER_SEC);

    in.close();
    out.close();
    return 0;
}