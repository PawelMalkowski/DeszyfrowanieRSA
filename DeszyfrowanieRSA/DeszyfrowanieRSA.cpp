#include "pch.h"
#include <iostream>
#include <limits.h>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <boost/multiprecision/cpp_int.hpp>
#include<fstream>
#include <stdlib.h> 
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>
#include <boost/mpl/aux_/config/eti.hpp>
#include <boost/mpl/aux_/config/integral.hpp>
#include <boost/mpl/aux_/config/static_constant.hpp>


using namespace std;
using namespace boost::multiprecision;

/* Function to calculate (base^exponent)%modulus */
int128_t gcd(int128_t a, int128_t b)
{
	int128_t temp;
	while (b != 0)
	{
		temp = a % b;

		a = b;
		b = temp;
	}
	return a;
}
int128_t modular_pow(int128_t base, int128_t exponent,
	 int128_t modulus)
{
	/* initialize result */
	 int128_t result = 1;

	while (exponent > 0)
	{
		/* if y is odd, multiply base with result */
		if (exponent & 1)
			result = (result * base) % modulus;

		/* exponent = exponent/2 */
		exponent = exponent >> 1;

		/* base = base * base */
		base = (base * base) % modulus;
	}
	return result;
}
int128_t mult(int128_t a, int128_t b, int128_t mod) {
	int128_t result = 0;
	while (b) {
		if (b & 1)
			result = (result + a) % mod;
		a = (a + a) % mod;
		b >>= 1;
	}
	return result;
}

int128_t f(int128_t x, int128_t c, int128_t mod) {
	return (mult(x, x, mod) + c) % mod;
}

int128_t abs1(int128_t x) {
	// value to abs()
	int128_t mask = x >> sizeof(int128_t) * CHAR_BIT - 1;

	 int128_t result = (x + mask) ^ mask;
	 return result;
}

/* method to return prime divisor for n */
int128_t brent(int128_t n, int128_t x0 = 2, int128_t c = 1) {
	int128_t x = x0;
	int128_t g = 1;
	int128_t q = 1;
	int128_t xs, y;

	int m = 128;
	int l = 1;
	while (g == 1) {
		y = x;
		for (int i = 1; i < l; i++)
			x = f(x, c, n);
		int k = 0;
		while (k < l && g == 1) {
			xs = x;
			for (int i = 0; i < m && i < l - k; i++) {
				x = f(x, c, n);
				
				q = mult(q, abs(y - x), n);
			}
			g = gcd(q, n);
			k += m;
		}
		l *= 2;
	}
	if (g == n) {
		do {
			xs = f(xs, c, n);
			g = gcd(abs(xs - y), n);
		} while (g == 1);
	}
	return g;
}

/* driver function */
int main()
{
	int128_t myArray[59];

	ifstream file("RSAKey.txt");
	ofstream file1("RSAPiQ.txt");
	if (file.is_open())
	{
		

		for (int i = 0; i < 59; ++i)
		{
			file >> myArray[i];
		}
	}
	for (int i = 0; i < 59; ++i)
	{
		clock_t start = clock();
		int128_t temp = brent(myArray[i]);
		clock_t stop = clock();
		long long abc = temp.convert_to<long long>();
		long long abcd = (myArray[i]/temp).convert_to<long long>();
		cout << i + 4 << " " << abc << " " << abcd << " " << stop - start << endl;
		file1 <<i+4<<" "<< abc << " "<< abcd <<" "<<stop-start<< endl;
		
	}
	return 0;
}