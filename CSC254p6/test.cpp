#include "Source1.h"
#include <iostream>
using namespace std;

class dc {
public:
	bool precedes(double a, double b) const {
		return a < b;
	}
	bool equals(double a, double b) const {
		return a == b;
	}
};

int main() {

	// Some miscellaneous code to get you started on testing your sets.
	// The following should work:

	std_simple_set<int> R;
	R += 3;
	cout << "3 is " << (R.contains(3) ? "" : "not ") << "in R\n";
	cout << "5 is " << (R.contains(5) ? "" : "not ") << "in R\n";



	simple_set<double>* S = new std_simple_set<double>();
	*S += 3.14;
	cout << "pi is " << (S->contains(3.14) ? "" : "not ") << "in S\n";
	cout << "e is " << (S->contains(2.718) ? "" : "not ") << "in S\n";

	std_simple_set<string> U;
	U += "hello";
	cout << "\"hello\" is " << (U.contains("hello") ? "" : "not ") << "in U\n";
	cout << "\"foo\" is " << (U.contains("foo") ? "" : "not ") << "in U\n";

	range<string> r1("a", true, "f", true);
	cout << "\"b\" is " << (r1.contains("b") ? "" : "not ") << "in r1\n";
	cout << "\"aaa\" is " << (r1.contains("aaa") ? "" : "not ") << "in r1\n";
	cout << "\"faa\" is " << (r1.contains("faa") ? "" : "not ") << "in r1\n";

	range<const char*, lexico_less> r2("a", true, "f", true);
	cout << "\"b\" is " << (r2.contains("b") ? "" : "not ") << "in r2\n";
	cout << "\"aaa\" is " << (r2.contains("aaa") ? "" : "not ") << "in r2\n";
	cout << "\"faa\" is " << (r2.contains("faa") ? "" : "not ") << "in r2\n";

	// The following will not work correctly yet:

	range_set<int>* X = new std_range_set<int>();
	*X += range<int>(5, true, 8, false);
	if (X->contains(4)) cout << "4 is in X\n";
	if (X->contains(5)) cout << "5 is in X\n";      // should print
	if (X->contains(6)) cout << "6 is in X\n";      // should print
	if (X->contains(7)) cout << "7 is in X\n";      // should print
	if (X->contains(8)) cout << "8 is in X\n";
	if (X->contains(9)) cout << "9 is in X\n";
	*X -= range<int>(6, true, 10, false);
	if (X->contains(4)) cout << "4 is now in X\n";
	if (X->contains(5)) cout << "5 is now in X\n";      // should print
	if (X->contains(6)) cout << "6 is now in X\n";
	if (X->contains(7)) cout << "7 is now in X\n";
	if (X->contains(8)) cout << "8 is now in X\n";
	if (X->contains(9)) cout << "9 is now in X\n";

	simple_set<weekday>* V = new carray_simple_set<weekday>(mon, (weekday)5);

	hashed_simple_set<int, cast_to_int<int>> H(100);

	bin_search_simple_set<double> J(100);
	carray_range_set<int> s(0, 100);
	s += range<int>(4, true, 9, false);
	if (s.contains(5)) cout << "5 is in range set s\n";//should print
	else cout << "5 is not in range set s1\n";


	range_set<char> * d = new carray_range_set<char>('a', 'z');
	*d += range<char>('a', true, 'd', false);
	if(d->contains('a')) cout << "a is in range set d\n";   //should print
	if (d->contains('e')) cout << "e is in range set d\n";
	else cout << "e is not in range set d\n";  //should print



	hashed_simple_set<int> s4(100);
	s4 += 1; 
	s4 += 2; 
	if (s4.contains(1)) cout << "1 is in simple set s4\n";//should print


	hashed_simple_set<char> d4(30);
	d4 += 'd';
	d4 += 'z';
	if (d4.contains('z')) cout << "z is in simple set d4\n";//should print
	d4 -= 'd';
	if (d4.contains('z')) cout << "z is in simple set d4\n";
	else cout << "z is not in range set d4 after\n";  //should print


	range_set<char> * d3 = new hashed_range_set<char>(30);
	*d3 += range<char>('a', true, 'd', false);
	*d3 += range<char>('g', true, 'i', false);
	*d3 -= range<char>('b', true, 'c', false);
	if (d3->contains('e')) cout << "e is in range set d3\n";
	else cout << "e is not in range set d3\n";  //should print



	bin_search_range_set<double, dc> s1(10);
	s1 += range<double, dc>(2.0, true, 4.0, true);
	if(s1.contains(9.0)) cout << "9.0 is in range set s1\n";
	else cout << "9.0 is not in range set s1\n"; 
	if (s1.contains(2.8)) cout << "2.8 is in range set s1\n";
	s1 += range<double, dc>(1.0, false, 10.0, false);
	if (s1.contains(9.0)) cout << "9.0 is in range set s1 after\n"; //should print
	else cout << "9.0 is not in range set s1 after\n";

}