/*
	Starter code for assignment 6, CSC 2/454, Fall 2019

	Provides skeleton of code for a simple hierarchy of set
	abstractions.

	Everything but /main/ should be moved to a .h file, which should
	then be #included from here.
*/

#include <set>
#include <iostream>
#include <string.h>
#include <type_traits>
using std::set;
using std::cout;
using std::string;

// Naive comparator.
// Provides a default for any type that has an operator<
// and an operator==.
//
template<typename T>
class comp {
public:
	bool precedes(const T& a, const T& b) const {
		return a < b;
	}
	bool equals(const T& a, const T& b) const {
		return a == b;
	}
};

// Abstract base class from which all sets are derived.
//
template<typename T, typename C = comp<T>>

class simple_set {
public:
	virtual ~simple_set<T, C>() { }
	// destructor should be virtual so that we call the right
	// version when saying, e.g.,
	// simple_set* S = new derived_set(args);
	//  ...
	// delete S;
	virtual simple_set<T, C>& operator+=(const T item) = 0;
	// inserts item into set
	// returns a ref so you can say, e.g.
	// S += a += b += c;
	virtual simple_set<T, C>& operator-=(const T item) = 0;
	// removes item from set, if it was there (otherwise does nothing)
	virtual bool contains(const T& item) const = 0;
	// indicates whether item is in set
};

//---------------------------------------------------------------

// Example of a set that implements the simple_set interface.
// Borrows the balanced tree implementation of the standard template
// library.  Note that you are NOT to use any standard library
// collections in your code (though you may use strings and streams).
//
template<typename T>
class std_simple_set : public virtual simple_set<T>, protected set<T> {
	// 'virtual' on simple_set ensures single copy if multiply inherited
public:
	virtual ~std_simple_set<T>() { }  // will invoke std::~set<T>()
	virtual std_simple_set<T>& operator+=(const T item) {
		set<T>::insert(item);
		return *this;
	}
	virtual std_simple_set<T>& operator-=(const T item) {
		(void)set<T>::erase(item);
		return *this;
	}
	virtual bool contains(const T& item) const {
		return (set<T>::find(item) != set<T>::end());
	}
};

//---------------------------------------------------------------

// Characteristic array implementation of set.
// Requires instantiation with guaranteed low and one-more-than-high
// bounds on elements that can be placed in the set.  Should compile
// and run correctly for any element class T that can be cast to int.
// Throws out_of_bounds exception when appropriate.
//
class out_of_bounds { };    // exception
template<typename T>
class carray_simple_set : public virtual simple_set<T> {
	// 'virtual' on simple_set ensures single copy if multiply inherited
	// You'll need some data members here.
	const T L;
	const T H;
	bool* b1;
	static const out_of_bounds err;
public:
	// fill in these methods:
	carray_simple_set(const T l, const T h) : L(l), H(h) {   // constructor
		if (l > h) throw err;
		b1 = new bool[H - L];
	}
	virtual ~carray_simple_set() {              // destructor
	}
	virtual carray_simple_set<T>& operator+=(const T item) {
		if ((item >= H) || (item < L)) throw err;
		*(b1 + ((int)item - (int)L)) = true;
		return *this;
	}
	virtual carray_simple_set<T>& operator-=(const T item) {
		if ((item >= H) || (item < L)) throw err;
		*(b1 + ((int)item - (int)L)) = false;
		return *this;
	}
	virtual bool contains(const T& item) const {
		return *(b1 + ((int)item - (int)L)) == true;
	}
};

//---------------------------------------------------------------

// Naive hash function object.
// Provides a default for any type that can be cast to int.
//
template<typename T>
class cast_to_int {
public:
	int operator()(const T n) {
		return (int)n;
	}
};

// Hash table implementation of set.
// Requires instantiation with guaranteed upper bound on number of elements
// that may be placed in set.  Throws overflow if bound is exceeded.
// Can be instantiated without second generic parameter if element type
// can be cast to int; otherwise requires hash function object.
//
class overflow { };         // exception
template<typename T, typename F = cast_to_int<T>>


class hashed_simple_set : public virtual simple_set<T> {
	// 'virtual' on simple_set ensures single copy if multiply inherited
	// You'll need some data members here.
	// I recommend you pick a hash table size p that is a prime
	// number >= n, use F(e) % p as your hash function, and rehash
	// with kF(e) % p after the kth collision.  (But make sure that
	// F(e) is never 0.)
	int H;
	int P;
	T *pointer;
	static const overflow err;

public:
	hashed_simple_set(const int n) : H(n), P(primelarger(n)) {    // constructor
		pointer = new T[P];
	}
	virtual ~hashed_simple_set() { }    // destructor

	virtual hashed_simple_set<T, F>& operator+=(const T item) {
		int tmp;
		if ((F()(item) % H) == 0) {
			tmp = 1;
		}
		else {
			tmp = F()(item) % H;
		}
		if ((*(pointer + tmp) != item) && (*(pointer + tmp) != ((T)0)))
		{
			//collision
			int k = 1;
			while (*(pointer + k * tmp) != item && *(pointer + k * tmp) != ((T)0) && k <= H) {
				k++;
			}
			if (k > H) {
				throw err;
			}
			else {
				*(pointer + k * tmp) = item;
			}
		}
		else {
			*(pointer + tmp) = item;
		}
		return *this;
	}

	virtual hashed_simple_set<T, F>& operator-=(const T item) {

		for (int k = 1; k <= P; k++)
		{
			if ((*(pointer + k * (F()(item) % H))) == item)
			{
				(*(pointer + k * (F()(item) % H))) = (T)0;
				return *this;
			}
		}
		return *this;
	}

	virtual bool contains(const T& item) const {
		int tmp = (F()(item) % H);
		if (tmp == 0) {
			tmp = 1;
		}
		for (int k = 1; k <= P; k++)
		{
			if ((*(pointer + k * tmp)) == item)
			{
				return true;
			}
		}
		return false;
	}

private: 
	int isPrime(int num) {//check if num is prime number
		if (num == 1) {
			return false;
		}
		if (num == 2) {
			return true;
		}
		for (int i = 2; i < num; i++) {
			if (num % i == 0) {
				return false;
			}
		}
		return true;
	}

	int primelarger(int num)
	{
		for (int i = num; i < num*num; i++)
		{
			if (isPrime(i))
			{
				return i;
			}
		}
		return 0;
	}
};

//---------------------------------------------------------------

// Sorted array implementation of set; supports binary search.
// Requires instantiation with guaranteed upper bound on number of
// elements that may be placed in set.  Throws overflow if bound is
// exceeded.
//
template<typename T, typename C = comp<T>>









class bin_search_simple_set : public virtual simple_set<T> {
protected:
	static const overflow err;
	const int max;
	int csize;

	C compare;
	T *t1;
private:
	int find_mid_index(int up, int low) const {
		return ((up - low) / 2 + low);
	}
	bool has_value(int up, int low, const T& item) const { 
		if (up <= low) return false;
		else {
			int mid = find_mid_index(up, low);
			if (compare.equals(t1[mid], item)) return true;
			else if (compare.precedes(t1[mid], item)) {
				if (mid == low) return false;
				else { return has_value(up, mid, item); }
			}
			else if (compare.precedes(item, t1[mid])) {
				if (mid == up) return false;
				else { return has_value(mid, low, item); }
			}
			return false;
		}
	}
public:
	bin_search_simple_set(const int n) : max(n), csize(0), t1(new T[max]), compare() {
	}

	virtual ~bin_search_simple_set() {} 

	virtual bin_search_simple_set<T, C>& operator+=(const T item) {
		if (csize >= max) throw err;
		if (compare.precedes(t1[csize - 1], item)) { t1[csize] = item; csize++; }
		else {
			bool shift_array = false;
			int insert_index = 0;
			for (int i = 0; i < csize; i++) {
				if (compare.equals(t1[i], item)) break; 
				else if (compare.precedes(item, t1[i])) { shift_array = true; insert_index = i; break; }
			}
			if (shift_array) {
				T val_to_enter = item;
				for (int i = insert_index; i <= csize; i++) {
					T current_a_i = t1[i];
					t1[i] = val_to_enter;
					val_to_enter = current_a_i;
				}
				csize++;
			}
		}
		return *this;
	}

	virtual bin_search_simple_set<T, C>& operator-=(const T item) {
		if (csize == 0) return *this;
		bool sa = false;
		int del = 0;
		for (int i = 0; i < csize; i++) {
			if (compare.equals(t1[i], item)) {
				sa = true;
				del = i;
				break;
			}
		}
		if (sa) {
			for (int i = del; i < csize; i++) {
				t1[i] = t1[i + 1];
			}
			csize--;
		}

		return *this;
	}

	virtual bool contains(const T& item) const {
		return has_value(csize, 0, item);
	}

};

//===============================================================
// RANGE SETS

// Function object for incrementing.
// Provides a default for any integral type.
//
template<typename T>
class increment {
		static_assert(std::is_integral<T>::value, "Integral type required.");
	public:
		T operator()(T a) const {
			return ++a;
		}
	};



// Range type.  Uses comp<T> by default, but you can provide your
// own replacement if you want, e.g. for C strings.
//
class empty_range { };
class merge_fail { };
class split_fail { };


template<typename T, typename C = comp<T>>

class range {
	
	static const empty_range err;
	static const merge_fail merge_err;
	static const split_fail split_err;	

public:
	T L;      // represents all elements from L
	bool Linc;  // inclusive?
	T H;      // through H
	bool Hinc;  // inclusive?
	C cmp;     // can't be static; needs explicit instantiation



	range(const T l = (T)NULL, const bool linc = false, const T h = (T)NULL, const bool hinc = false)
		: L(l), Linc(linc), H(h), Hinc(hinc), cmp() {
	}
	T low() const { return L; }
	bool closed_low() const { return Linc; }
	T high() const { return H; }
	bool closed_high() const { return Hinc; }
	bool contains(const T& item) const {
		return ((cmp.precedes(L, item) || (Linc && cmp.equals(L, item)))
			&& (cmp.precedes(item, H) || (Hinc && cmp.equals(item, H))));
	}

	range<T, C>& operator=(const range<T, C>& ran) {
		H = ran.H;
		L = ran.L;
		Linc = ran.Linc;
		Hinc = ran.Hinc;
		return *this;
	}



	bool precedes(const range<T, C>& other) const {
		return (cmp.precedes(H, other.L) || (cmp.equals(H, other.L) && !Hinc && !other.Linc));
	}
	bool overlaps(const range<T, C>& other) const {
		if ((cmp.precedes(L, other.L) && cmp.precedes(other.L, H)) ||
			(cmp.precedes(L, other.H) && cmp.precedes(other.H, H))) {
			return true;
		}
		else {
			if (((cmp.equals(L, other.H)) && Linc && other.Hinc) ||
				((cmp.equals(H, other.L)) && Hinc && other.Linc)) {
				return true;
			}
			else return cmp.equals(L, other.L) || cmp.equals(H, other.H);
		}
	}
	range merge(const range<T, C>& other) const {
		if (!overlaps(other)) {
			throw merge_err;
		}
		else {
			T l; T h; bool lc; bool hc;
			if (cmp.precedes(L, other.L)) {
				l = L;
				lc = Linc;
			}
			else if (cmp.precedes(other.L, L)) {
				l = other.L;
				lc = other.Linc;
			}
			else { // They are equal.
				l = L;
				lc = Linc || other.Linc;
			}

			if (cmp.precedes(other.H, H)) {
				h = H;
				hc = Hinc;
			}
			else if (cmp.precedes(H, other.H)) {
				h = other.H;
				hc = other.Hinc;
			}
			else { // They are equal.
				h = H;
				hc = Hinc || other.Hinc;
			}
			return range<T, C>(l, lc, h, hc);
		}
	}
	range* split(const range<T, C>& other) const {
		//split the range into 2 by another range
		range<T, C> *ary = new range<T, C>[2];
		if (cmp.precedes(L, other.L) && cmp.precedes(other.H, H)) {
			T l1 = L;
			T h1 = other.L;
			bool lc1 = Linc;
			bool hc1 = other.Linc;
			ary[0] = range<T, C>(l1, lc1, h1, hc1);

			T l2 = other.H;
			T h2 = H;
			bool lc2 = other.Hinc;
			bool hc2 = Hinc;
			ary[1] = range<T, C>(l2, lc2, h2, hc2);
		}
		else if (includes(other.L)) {
			if (!cmp.equals(H, other.H)) {
				T l = L;
				T h = other.L;
				bool lc = Linc;
				bool hc = !other.Linc;
				ary[0] = range<T, C>(l, lc, h, hc);
				ary[1] = range<T, C>(1, false, -1, false);
			}
			else {
				T l = L;
				T h = other.L;
				bool lc = Linc;
				bool hc = !other.Linc;
				ary[0] = range<T, C>(l, lc, h, hc);

				if (Hinc && !other.Hinc) {
					ary[1] = range<T, C>(H, true, H, true);
				}
				else {
					ary[1] = range<T, C>(1, false, -1, false);
				}
			}
		}
		else if (includes(other.H)) {
			if (!cmp.equals(L, other.L)) {
				T l = other.H;
				T h = H;
				bool lc = !other.Hinc;
				bool hc = Linc;
				ary[0] = range<T, C>(l, lc, h, hc);
				ary[1] = range<T, C>(1, false, -1, false);
			}
			else {
				T l = other.H;
				T h = H;
				bool lc = !other.Hinc;
				bool hc = Linc;
				ary[0] = range<T, C>(l, lc, h, hc);

				if (Linc && !other.Linc) {
					ary[1] = range<T, C>(L, true, L, true);
				}
				else {
					ary[1] = range<T, C>(1, false, -1, false);
				}
			}
		}
		else if (other.includes(H) && other.includes(L)) {
			ary[0] = range<T, C>(1, false, -1, false);
			ary[1] = range<T, C>(1, false, -1, false);
		}
		else if (cmp.equals(H, other.H) && cmp.equals(L, other.L)) {
			if (equals(other)) {
				ary[0] = range<T, C>(1, false, -1, false);
				ary[1] = range<T, C>(1, false, -1, false);
			}
			else {
				if (Hinc == other.Hinc) {
					if (other.Linc) {
						ary[0] = range<T, C>(1, false, -1, false);
						ary[1] = range<T, C>(1, false, -1, false);
					}
					else {
						ary[0] = range<T, C>(L, true, L, true);
						ary[1] = range<T, C>(1, false, -1, false);
					}
				}
				else {
					if (other.Hinc) {
						ary[0] = range<T, C>(1, false, -1, false);
						ary[1] = range<T, C>(1, false, -1, false);
					}
					else {
						ary[0] = range<T, C>(H, true, H, true);
						ary[1] = range<T, C>(1, false, -1, false);
					}
				}
			}
		}
		return ary;
	}
	void print_range() {
		cout << L << ", " << Linc << ", " << H << ", " << Hinc << std::endl;
	}
	bool less_than(const T& item) {
		return (cmp.precedes(H, item) || (cmp.equals(H, item) && !Hinc));
	}
	bool greater_than(const T& item) {
		return (cmp.precedes(item, L) || (cmp.equals(L, item) && !Linc));
	}
	bool includes(const T& item) const {
		return ((cmp.precedes(L, item) && cmp.precedes(item, H)) || (cmp.equals(L, item) && Linc) || (cmp.equals(H, item) && Hinc));
	}
	bool equals(const range<T, C>& other) const {
		return cmp.equals(L, other.L) && cmp.equals(H, other.H) && (Linc == other.Linc) && (Hinc == other.Hinc);
	}
};

// You may find it useful to define derived types with two-argument
// constructors that embody the four possible combinations of open and
// close-ended:
//
// template<typename T, typename C = comp<T>>
// class CCrange : public range<T, C> { ...
// 
// template<typename T, typename C = comp<T>>
// class COrange : public range<T, C> { ...
// 
// template<typename T, typename C = comp<T>>
// class OCrange : public range<T, C> { ...
// 
// template<typename T, typename C = comp<T>>
// class OOrange : public range<T, C> { ...

// This is the abstract class from which all range-supporting sets are derived.
//
template<typename T, typename C = comp<T>>
class range_set : public virtual simple_set<T> {
	// 'virtual' on simple_set ensures single copy if multiply inherited
public:
	virtual range_set<T, C>& operator+=(const range<T, C> r) = 0;
	virtual range_set<T, C>& operator-=(const range<T, C> r) = 0;
};

//---------------------------------------------------------------

// As implemented in the standard library, sets contain individual
// elements, not ranges.  (There are range insert and erase operators, but
// (a) they use iterators, (b) they take time proportional to the number of
// elements in the range, and (c) they require, for deletion, that the
// endpoints of the range actually be in the set.  An std_range_set, as
// defined here, avoids shortcomings (a) and (c), but not (b).  Your
// bin_search_range_set should avoid (b), though it will have slow insert
// and remove operations.  A tree_range_set (search tree -- extra credit)
// would have amortized log-time insert and remove for individual elements
// _and_ ranges.
//
template<typename T, typename C = comp<T>, typename I = increment<T>>
class std_range_set : public virtual range_set<T, C>,
	public std_simple_set<T> {
	// 'virtual' on range_set ensures single copy if multiply inherited
	static_assert(std::is_integral<T>::value, "Integral type required.");
	I inc;
public:
	// The first three methods below tell the compiler to use the
	// versions of the simple_set methods already found in std_simple_set
	// (given true multiple inheritance it can't be sure it should do that
	// unless we tell it).
	virtual std_simple_set<T>& operator+=(const T item) {
		return std_simple_set<T>::operator+=(item);
	}
	virtual std_simple_set<T>& operator-=(const T item) {
		return std_simple_set<T>::operator-=(item);
	}
	virtual bool contains(const T& item) const {
		return std_simple_set<T>::contains(item);
	}
	virtual range_set<T>& operator+=(const range<T, C> r) {
		for (T i = (r.closed_low() ? r.low() : inc(r.low()));
			r.contains(i); i = inc(i)) {
			*this += i;
		}
		return *this;
	}
	virtual range_set<T>& operator-=(const range<T, C> r) {
		for (T i = (r.closed_low() ? r.low() : inc(r.low()));
			r.contains(i); i = inc(i)) {
			*this -= i;
		}
		return *this;
	}
};


template<typename T, typename C = comp<T>, typename I = increment<T>>
class carray_range_set : public virtual range_set<T, C>, protected carray_simple_set<T> {
	// 'virtual' on range_set ensures single copy if multiply inherited
	//static_assert(std::is_integral<T>::value, "Integral type required.");
	I increment;
	C compare;
public:
	carray_range_set(const T l, const T h) : carray_simple_set<T>(l, h), compare(), increment() {}

	virtual carray_simple_set<T>& operator+=(const T item) {
		return carray_simple_set<T>::operator+=(item);
	}

	virtual carray_simple_set<T>& operator-=(const T item) {
		return carray_simple_set<T>::operator-=(item);
	}

	virtual range_set<T, C>& operator+=(const range<T, C> r) { // add each number in the range to the set
		for (T i = (r.closed_low() ? r.low() : increment(r.low())); r.contains(i); i = increment(i)) {
			*this += i;
		}
		return *this;
	}

	virtual range_set<T, C>& operator-=(const range<T, C> r) { //remove from the set
		for (T i = (r.closed_low() ? r.low() : increment(r.low()));
			r.contains(i); i = increment(i)) {
			*this -= i;
		}
		return *this;
	}

	virtual bool contains(const T& item) const { //use the same contains routine
		return carray_simple_set<T>::contains(item);
	}
};


template<typename T, typename F = cast_to_int<T>, typename C = comp<T>, typename I = increment<T>>
class hashed_range_set : public virtual range_set<T, C>, public hashed_simple_set<T, F> {
	// 'virtual' on range_set ensures single copy if multiply inherited
	  //static_assert(std::is_integral<T>::value, "Integral type required.");
	I inc;
public:

	hashed_range_set(int size) : hashed_simple_set<T>(size) {   // constructor

	}
	virtual ~hashed_range_set() {              // destructor

	}
	virtual hashed_simple_set<T>& operator+=(const T item) {
		return hashed_simple_set<T>::operator+=(item);
	}
	virtual hashed_simple_set<T>& operator-=(const T item) {
		return hashed_simple_set<T>::operator-=(item);
	}
	virtual bool contains(const T& item) const {
		return hashed_simple_set<T>::contains(item);
	}
	virtual range_set<T, C>& operator+=(const range<T, C> r) {
		for (T i = (r.closed_low() ? r.low() : inc(r.low()));
			r.contains(i); i = inc(i)) {
			*this += i;
		}
		return *this;
	}
	virtual range_set<T, C>& operator-=(const range<T, C> r) {
		for (T i = (r.closed_low() ? r.low() : inc(r.low()));
			r.contains(i); i = inc(i)) {
			*this -= i;
		}
		return *this;
	}
};


template<typename T, typename C = comp<T>>
class bin_search_range_set : public virtual range_set<T, C>, public bin_search_simple_set<T, C> {
	const int max;
	int csize; // basically current_size from above
public:
	range<T, C>* ranges; // an array keep track of all the ranges added to the set

	bin_search_range_set(const int num) : bin_search_simple_set<T, C>(num), max(num), csize(0) {
		ranges = new range<T, C>[num];
	}
	virtual bin_search_simple_set<T, C>& operator+=(const T item) {
		return bin_search_simple_set<T, C>::operator+=(item);
	}
	virtual bin_search_simple_set<T, C>& operator-=(const T item) {
		return bin_search_simple_set<T, C>::operator-=(item);
	}
	// Need to change this.
	virtual bool contains(const T& item) const {
		//return bin_search_simple_set<T>::contains(item);
		return has_value(csize, 0, item) != -1;
	}

	virtual bin_search_range_set<T, C>& operator+=(const range<T, C> r) {
		if (csize == max) throw bin_search_simple_set<T, C>::err;
		if (csize == 0 || ranges[csize - 1].precedes(r)) {
			ranges[csize] = r;
			csize++;
			return *this;
		}
		range<T, C> aprange = r;
		for (int i = 0; i < csize; i++) {
			if (aprange.precedes(ranges[i])) {
				// insert range at i
				range<T, C> buffer = ranges[i];
				ranges[i] = r;
				for (int j = i + 1; j < csize; j++) {
					range<T, C> tmp = ranges[j];
					ranges[j] = buffer;
					buffer = tmp;
				}
				ranges[csize] = buffer;
				// shift array up by 1
				csize++;

				//cleanup the array
				range<T, C>* copy = new range<T, C>[max];
				int counter = 0;
				for (int i = 0; i < max; ++i) {
					if (ranges[i].H > ranges[i].L) {
						copy[counter] = ranges[i];
						counter++;
					}
				}
				ranges = copy;
				csize = counter;

				return *this;
			}
			else if (aprange.equals(ranges[i])) {
				// ignore range_of_interest because it is a repeat.
				return *this;
			}
			else if (aprange.overlaps(ranges[i])) {
				// merge ranges
				aprange = aprange.merge(ranges[i]);
				// remove old range.
				ranges[i] = range<T, C>(1, false, -1, false);
			}

		}
		*this += aprange;
		range<T, C>* copy = new range<T, C>[max];
		int counter = 0;
		for (int i = 0; i < max; ++i) {
			if (ranges[i].H > ranges[i].L) {
				copy[counter] = ranges[i];
				counter++;
			}
		}
		ranges = copy;
		csize = counter;
		return *this;
	}


	virtual bin_search_range_set<T, C>& operator-=(const range<T, C> r) {

		for (int i = 0; i < csize; i++) {
			if (ranges[i].overlaps(r)) {
				range<T, C>* p = ranges[i].split(r);

				if (p[1].equals(range<T, C>(1, false, -1, false)) && !p[0].equals(range<T, C>(1, false, -1, false))) { // returned one by split
					ranges[i] = range<T, C>(1, false, -1, false);
					*this += p[0];
				}
				else if (!p[1].equals(range<T, C>(1, false, -1, false)) && !p[0].equals(range<T, C>(1, false, -1, false))) { // returned two by split
					ranges[i] = range<T, C>(1, false, -1, false);
					*this += p[0];
					*this += p[1];
				}
				else {
					ranges[i] = range<T, C>(1, false, -1, false); 
				}
			}
		}
		range<T, C>* copy = new range<T, C>[max];
		int counter = 0;
		for (int k = 0; k < max; ++k) {
			if (ranges[k].H > ranges[k].L) {
				copy[counter] = ranges[k];
				counter++;
			}
		}
		ranges = copy;
		csize = counter;

		return *this;
	}


	//print routine only for debugging
	void print_ranges() {
		for (int i = 0; i < csize; i++) {
			ranges[i].print_range();
		}
	}

private: // the same as in bin_search_simple_set
	int find_mid_index(int upper_bound, int lower_bound) const {
		return ((upper_bound - lower_bound) / 2 + lower_bound);
	}

	int has_value(int upper, int lower, const T& item) const {
		if (upper <= lower) return -1;
		else {
			int mid = find_mid_index(upper, lower);
			if (ranges[mid].includes(item)) return mid;
			else if (ranges[mid].less_than(item)) {
				if (mid == lower) return -1;
				else { return has_value(upper, mid, item); }
			}
			else if (ranges[mid].greater_than(item)) {
				if (mid == upper) return -1;
				else { return has_value(mid, lower, item); }
			}
			return -1;
		}
	}
};



// comparator for C strings
//
class lexico_less {
public:
	bool precedes(const char *a, const char *b) const {
		return strcmp(a, b) < 0;
	}
	bool equals(const char *a, const char *b) const {
		return strcmp(a, b) == 0;
	}
};

typedef enum { mon, tue, wed, thu, fri } weekday;


