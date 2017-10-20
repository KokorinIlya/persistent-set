#include "shared_ptr.h"
#include "persistent_set.h"
#include <string>
#include <ctime>

using namespace std;

struct Base
{
	int x;

	Base(int _x) : x(_x) {};
};

struct Der : Base
{
	int y;

	Der(int _x, int _y) : Base(_x), y(_y) {};
};

struct Other
{
	int z;

	Other(int _z) : z(_z) {};
};

template cpp_course::persistent_set<string>;
template cpp_course::persistent_set<int>;

int main()
{
	srand(time(0));
	unsigned long long ts = clock();
	cpp_course::persistent_set<string> a;
	a.insert("aa");
	a.insert("bb");
	a.insert("ab");
	cpp_course::persistent_set<string> b(a);
	b.erase(b.begin());
	b.erase(b.find("bb"));
	b.insert("cccc");
	for (auto x = a.begin(); x != a.end(); ++x)
	{
		//cout << *x << " ";
	}
	//cout << endl;
	for (auto y = b.begin(); y != b.end(); y++)
	{
		//cout << *y << " ";
	}
	//cout << endl;
	swap(a, b);
	for (auto x = --a.end(); x != a.begin(); --x)
	{
		//cout << *x << " ";
	}
	//cout << endl;
	for (auto y = --b.end(); y != b.begin(); y--)
	{
		//cout << *y << " ";
	}
	//cout << endl;
	a.insert("simple");
	for (auto x = a.begin(); x != a.end(); ++x)
	{
		//cout << *x << " ";
	}
	//cout << endl;
	a.erase(a.find("ab"));
	a.insert("you");
	for (auto x = a.begin(); x != a.end(); ++x)
	{
		//cout << *x << " ";
	}
	//cout << endl;
	cpp_course::persistent_set<string> c(a);
	c.insert("Just");
	c.insert("another");
	c.erase(c.find("you"));
	c.insert("string");
	a.erase(a.find("simple"));

	for (auto x = a.begin(); x != a.end(); ++x)
	{
		//cout << *x << " ";
	}
	//cout << endl;
	for (auto x = c.begin(); x != c.end(); ++x)
	{
		//cout << *x << " ";
	}
	//cout << endl;
	c.erase(c.find("cccc"));
	for (auto x = c.begin(); x != c.end(); ++x)
	{
		//cout << *x << " ";
	}
	//cout << endl;
	cpp_course::persistent_set<string> ps(c);
	ps.insert("final_test");
	c.erase(c.find("simple"));
	for (auto x = c.begin(); x != c.end(); ++x)
	{
		//cout << *x << " ";
	}
	//cout << endl;
	for (auto x = ps.begin(); x != ps.end(); ++x)
	{
		//cout << *x << " ";
	}
	//cout << endl;

	cpp_course::persistent_set<int> d;
	for (int i = 0; i < 1000000; i++)
	{
		d.insert(rand());
	}
	for (auto i = d.begin(); i != d.end(); ++i)
	{
		//cout << *i << " ";
	}
	//cout << endl << "______________________" << endl;
	for (auto i = --d.end(); i != d.begin(); --i)
	{
		//cout << *i << " ";
	}
	//cout << endl << "______________________" << endl;
	cpp_course::persistent_set<int> e(d);
	cpp_course::persistent_set<int> f(d);
	for (int i = 0; i < 100; i++)
	{
		//cout << *d.begin() << " ";
		d.erase(d.begin());
	}
	//cout << endl << "______________________" << endl;
	for (auto i = d.begin(); i != d.end(); ++i)
	{
		//cout << *i << " ";
	}
	//cout << endl << "______________________" << endl;
	for (auto i = e.begin(); i != e.end(); ++i)
	{
		//cout << *i << " ";
	}
	//cout << endl << "______________________" << endl;
	for (auto i = f.begin(); i != f.end(); ++i)
	{
		//cout << *i << " ";
	}
	//cout << endl << "______________________" << endl;
	unsigned long long tf = clock();
	cout << static_cast<double>(tf - ts) / CLOCKS_PER_SEC;
	system("pause");

	return 0;
}

