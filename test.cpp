/*************************************************************************
	> File Name: test.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2017年05月14日 星期日 13时27分39秒
 ************************************************************************/

#include<iostream>
using namespace std;
//#define EX(path, img) path##img
//#define PATH(imgpath) EX(sss, imgpath)
//#define PA(img) #PATH(img)
//#define A(a) a*a
//#define B(b) b+b
#define PATH(path, img) #path###img
int main()
{
	int a = 2;
	cout << PATH(ss,dd) << endl;
	return 0;
}
