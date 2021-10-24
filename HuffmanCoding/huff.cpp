#include <iostream>
#include <assert.h>
#include <bits/stdc++.h>
using namespace std;

#define BITS_PER_CHAR 8

// Error handler: print message if any, and exit
void quit(const char *message = 0)
{
    if (message)
        printf("%s\n", message);
    exit(1);
}

class HuffmanTreeNode
{
public:
    unsigned char ch;
    int count;
    HuffmanTreeNode *left;
    HuffmanTreeNode *right;

public:
    HuffmanTreeNode(int chh, int countt) : ch(chh), count(countt), left(nullptr), right(nullptr){};
    HuffmanTreeNode(int chh, int countt, HuffmanTreeNode *l, HuffmanTreeNode *r) : ch(chh), count(countt), left(l), right(r){};
    HuffmanTreeNode() : ch(0), count(0), left(nullptr), right(nullptr){};
    // �������캯��һ��Ҫ����Ϊ���Ƿŵ��ǽڵ㣬һ��Ҫ�����ݸ��ƹ�ȥ������ٿ�����������֣�
    HuffmanTreeNode(const HuffmanTreeNode &N)
    {
        ch = N.ch;
        count = N.count;
        if (N.left != nullptr)
        {
            left = new HuffmanTreeNode();
            *left = *(N.left);
        }
        else
        {
            left = nullptr;
        }
        if (N.right != nullptr)
        {
            right = new HuffmanTreeNode();
            *right = *(N.right);
        }
        else
        {
            right = nullptr;
        }
    }
};
// ����"<"���ţ�����Ϊ��Сֵ����
bool operator<(const HuffmanTreeNode &N1, const HuffmanTreeNode &N2)
{
    return N1.count > N2.count;
}

class Huff
{
private:
    HuffmanTreeNode *root;
    map<unsigned char, int> statistic;    // Ȩ��ͳ�Ʊ�
    map<unsigned char, string> codeTable; // �����

public:
    Huff()
    {
        root = new HuffmanTreeNode();
    }
    // �ͷŶ���ռ�
    void FreeNode(HuffmanTreeNode *curNode)
    {
        if (curNode)
        {
            FreeNode(curNode->left);
            FreeNode(curNode->right);
        }
        delete (curNode);
    }
    // ���³�ʼ��
    void Init()
    {
        statistic.clear();
        codeTable.clear();
        FreeNode(root);
        root = new HuffmanTreeNode();
    }
    // ͳ�ƴ�Ƶ
    void WordFrequencyStatistics(FILE *in)
    {
        int c;
        fseek(in, 0, SEEK_END);
        int size = ftell(in);
        if (size < 0 || size >= 0x7FFFFFFF)
            quit("input file too big");
        fseek(in, 0, SEEK_SET);
        while ((c = getc(in)) != EOF)
        {
            statistic[c]++;
        }
    }
    // ������������
    void CreateHuffmanTree()
    {
        priority_queue<HuffmanTreeNode> myq;
        for (auto pair : statistic)
        {
            HuffmanTreeNode temp = HuffmanTreeNode(pair.first, pair.second);
            myq.push(temp);
        }
        // �����ȷ���濽�����������������ͻ����
        while (myq.size() > 1)
        {
            HuffmanTreeNode left = myq.top();
            myq.pop();
            HuffmanTreeNode right = myq.top();
            myq.pop();
            // �����������ĸ��ڵ㣬����ѹ�����
            HuffmanTreeNode parent = HuffmanTreeNode(-1, left.count + right.count, &left, &right);
            myq.push(parent);
        }
        *root = myq.top();
        myq.pop();
    }
    void Build(HuffmanTreeNode *curNode, string code)
    {
        // ���root��Ҷ�ӽ��
        if (curNode->left == nullptr && curNode->right == nullptr && (int)(root->ch) > 0)
        {
            // ��������
            // cout << int(curNode->ch) << " " << code << endl;
            codeTable[curNode->ch] = code;
            return;
        }
        if (curNode->left != nullptr)
            Build(curNode->left, code + '0');
        if (curNode->right != nullptr)
            Build(curNode->right, code + '1');
    }
    // ���ɱ����
    void BuildCode()
    {
        if (root == nullptr)
        {
            quit("the root is null");
        }
        string code = "";
        HuffmanTreeNode *curNode = root;
        Build(curNode, code);
    }
    // ����Ƶ��д���ļ�
    void WriteStatistic(FILE *out)
    {
        int len = statistic.size();
        // ǰ�ĸ��ֽڱ�ʾ����Ƶ�ж��ٶԣ���statistic�ж��ٶԣ�����ÿ���ַ���Ƶ��ҲҪ�ĸ��ֽڣ���Ϊ1���ֽ�ֻ�ܴ�255���£�����ʵ��
        putc(len >> 24, out);
        putc(len >> 16, out);
        putc(len >> 8, out);
        putc(len, out);
        for (auto pair : statistic)
        {
            // �ַ�0~255��һ���ֽ�ֱ��д��
            putc((int)pair.first, out);
            // �����Ļ�������һ���ļ�������������ظ��ַ��������������ĸ��ֽ�����Ƶ��
            // �൱���ԣ�1���ֽڵ��ַ���4���ֽڵ�Ƶ�ʣ���ÿ����ͬ���ַ�Ƶ�ʶ�д��ȥ��1���ֽ�*10^9=1G����������
            // ������ڴ��ļ�
            putc((int)(pair.second >> 24), out);
            putc((int)(pair.second >> 16), out);
            putc((int)(pair.second >> 8), out);
            putc((int)(pair.second), out);
        }
    }

    // ѹ���ļ�
    void CompressFile(FILE *in, FILE *out)
    {
        Init();
        WordFrequencyStatistics(in);
        CreateHuffmanTree();
        BuildCode();
        // �Ѵ�Ƶ��д���ļ�
        WriteStatistic(out);
        int c = 0;
        string res = "";
        // ��Ϊ����Ƶ��ͳ�Ƶ�ʱ��in��ָ�뵽�ļ�β���ˣ���������Ҫ����
        fseek(in, 0, SEEK_SET);
        while ((c = getc(in)) != EOF)
        {
            res += codeTable[c];
        }
        int len = res.size();
        // cout << len / 8 << endl;
        // ֮��ĵ�һ���ֽڱ�ʾ�����Ǹ��ֽ��м�λ����ʱ�벻�����õİ취��
        putc(len % 8, out);
        for (int i = 0; i < len;)
        {
            int sum = 0;
            for (int j = i; j < len && j < i + 8; j++)
            {
                sum = sum * 2 + res[j] - '0';
            }
            i += 8;
            putc(sum, out);
        }
        FreeNode(root);
    }
    // ��ѹ��ʱ�������ļ���ȡ��Ƶ��
    void ReadStatistic(FILE *in)
    {
        int c = 0;
        // ���õ�ǰ�ĸ��ֽڣ�ȷ���ж��ٸ���Ƶ��
        int numberFrequency = 0;
        for (int i = 0; i < 4; i++)
        {
            c = getc(in);
            numberFrequency = numberFrequency << 8;
            numberFrequency += c;
        }
        // ȡ��numberFrequency����Ƶ��
        while (numberFrequency-- && ((c = getc(in)) != EOF))
        {
            int i = 0;
            int frequency = 0;
            int cc = 0;
            while (i < 4 && (cc = getc(in)) != EOF)
            {
                frequency = frequency << 8;
                frequency += cc;
                i++;
            }
            statistic[c] = frequency;
        }
    }
    // ����ת������
    string IntegerToBinary(int tar)
    {
        int bitNumber = 0;
        string ret = "";
        while (bitNumber < 8)
        {
            // ע�⣬λ��������ȼ��ܵͣ�һ��Ҫ�ǵ�����
            ret += ((tar & 1) + '0');
            tar >>= 1;
            bitNumber++;
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }
    // ��ѹ���ļ�
    void DecompressFile(FILE *in, FILE *out)
    {
        Init();
        // �ȶ���Ƶ��������������
        ReadStatistic(in);
        CreateHuffmanTree();
        BuildCode();
        // ����Ƶ����Ĳ��ֽ��н�ѹ������Щ��huffman�����ʾ��
        // ��β���Ǹ��ֽھ����ж���λ
        int AtEndDigits = getc(in);

        int last = getc(in);
        int c = 0;
        string res = "";
        while ((c = getc(in)) != EOF)
        {
            res += IntegerToBinary(last);
            last = c;
        }
        // �����һλ����Ĵ�������ض�
        res += IntegerToBinary(last).substr(8 - AtEndDigits, AtEndDigits);

        HuffmanTreeNode *roott = root;
        int index = 0;
        int len = res.size();
        while (index < len)
        {
            if (res[index] == '0')
            {
                roott = roott->left;
                index++;
                // roott��Ҷ�ӽڵ�
                if (roott != nullptr && roott->left == nullptr && roott->right == nullptr)
                {
                    putc(int(roott->ch), out);
                    roott = root;
                }
                continue;
            }
            if (res[index] == '1')
            {
                roott = roott->right;
                index++;
                // roott��Ҷ�ӽڵ�
                if (roott != nullptr && roott->left == nullptr && roott->right == nullptr)
                {
                    putc(int(roott->ch), out);
                    roott = root;
                }
                continue;
            }
        }
        FreeNode(root);
    }
};

int main(int argc, char **argv)
{
    if (argc != 4 || argv[1][0] != 'c' && argv[1][0] != 'd')
    {
        cout << "Huffman file compressor (C) 2021, luweir\n\n"
             << "To compress:   huff c input output  \n"
             << "To decompress: huff d input output  \n"
             << endl;
        return 1;
    }
    clock_t start = clock();

    FILE *in = fopen(argv[2], "rb");
    if (!in)
    {
        perror(argv[0]);
        exit(1);
    }

    FILE *out = fopen(argv[3], "wb");
    Huff *h = new Huff();
    if (argv[1][0] == 'c')
    {
        h->CompressFile(in, out);
    }
    if (argv[1][0] == 'd')
    {
        h->DecompressFile(in, out);
    }

    printf("%ld -> %ld in %1.4f sec. \n",
           ftell(in), ftell(out), double(clock() - start) / CLOCKS_PER_SEC);
    fclose(in);
    fclose(out);

    return 0;
}