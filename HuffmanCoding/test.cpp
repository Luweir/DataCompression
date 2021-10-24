#include <iostream>
#include <queue>
using namespace std;

typedef struct _A
{
    int l;
    int r;
    int label;
} A;

bool operator<(A a1, A a2)
{
    return a1.r < a2.r;
}

bool operator>(A a1, A a2)
{
    return a1.l > a2.l;
}

int main()
{
    priority_queue<A, vector<A>, greater<A>> que1; // 递增 - 对应>
    priority_queue<A, vector<A>, less<A>> que2;    // 递减 - 对应<
    //  l  r  label
    A a1 = {1, 2, 1};
    A a2 = {6, 7, 2};
    A a3 = {3, 5, 3};
    A a4 = {2, 3, 4};
    A a5 = {4, 10, 5};
    que1.push(a1);
    que1.push(a2);
    que1.push(a3);
    que1.push(a4);
    que1.push(a5);
    que2.push(a1);
    que2.push(a2);
    que2.push(a3);
    que2.push(a4);
    que2.push(a5);
    cout << "按照l递增：";
    while (!que1.empty())
    {
        cout << "a" << que1.top().label << "<";
        que1.pop();
    }
    cout << endl;
    cout << "按照r递减：";
    while (!que2.empty())
    {
        cout << "a" << que2.top().label << ">";
        que2.pop();
    }
    cout << endl;
    return 0;
}