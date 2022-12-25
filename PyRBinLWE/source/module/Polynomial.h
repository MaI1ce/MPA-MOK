#ifndef _POLYNOMIAL_
#define _POLYNOMIAL_

//#define uint64_t unsigned long long
//#define uint32_t unsigned int
//#define uint16_t unsigned short
//#define uint8_t  unsigned char

//#define _DEBUG_
#define Q 256 // coeficient module

#include <new>
#include <iostream>
#include <sstream>

class RBinLWE256;

class PolynomialBase {
protected:
	uint32_t poly_mod;
	//uint32_t coef_mod;
	uint32_t mem_size;
	uint8_t* buffer;

	bool memory_init(uint32_t m, uint8_t c);

public:
	uint32_t get_poly_mod() { return poly_mod; }

	virtual bool random_init(uint32_t n = 128);

	virtual bool init(uint8_t c = 0, uint32_t n = 128);

	virtual bool init_str(const std::string& str, uint32_t n = 128);

	virtual std::string print_string();

	std::string to_string();

	PolynomialBase()
		:poly_mod(0), mem_size(0), buffer(nullptr) {
#ifdef _DEBUG_
		std::cout << "basic constructor PolynomialBase\n";
#endif
	}

	// Copy constructor
	PolynomialBase(const PolynomialBase& obj);

	// Move constructor
	PolynomialBase(PolynomialBase&& obj) noexcept;

	// Copy assignment operator.
	PolynomialBase& operator=(const PolynomialBase& obj);

	// Move assignment operator.
	PolynomialBase& operator=(PolynomialBase&& obj);

	virtual ~PolynomialBase()
	{
#ifdef _DEBUG_
		std::cout << "destructor PolynomialBase\n";
#endif
		delete[]buffer;
		buffer = nullptr;
		mem_size = 0;
		poly_mod = 0;
	}

};

class Polynomial;

class BinPolynomial: public PolynomialBase {
private:
	//uint8_t* buffer;
	
public:

	BinPolynomial();

	// Copy constructor
	BinPolynomial(const BinPolynomial& obj);

	// Move constructor
	BinPolynomial(BinPolynomial&& obj);

	// Copy assignment operator.
	BinPolynomial& operator=(const BinPolynomial& obj);

	// Move assignment operator.
	BinPolynomial& operator=(BinPolynomial&& obj);

	Polynomial operator+(const Polynomial& obj) const;
	Polynomial operator-(const Polynomial& obj) const;

	bool operator==(const BinPolynomial& obj) const;

	bool random_init(uint32_t n = 128) override;

	bool init(uint8_t c = 0, uint32_t n = 128) override;

	bool init_str(const std::string& str, uint32_t n = 128) override;

	std::string print_string() override;

	//read from left to right (smallest coef to the left - biggest to the right)
	friend std::ostream& operator<< (std::ostream& out, const BinPolynomial& obj);

	friend class Polynomial;
	friend class RBinLWE256;
};
/*
Polunomial degree of n - an array of coefficients with length of n

Arr[n] -> a0*x^0 == Arr[0]; a(n-1)*x^(n-1) == Arr[n-1]

BinPolynomial - an array of bits

Brr[n][8] -> b0*x^0 == Brr[0][0];  b(n-1)*x^(n-1) == Brr[(n-1)/8][7];  

*/
class Polynomial : public PolynomialBase {
private:
	//uint16_t* buffer;
public:
	Polynomial();

	// Copy constructor
	Polynomial(const Polynomial& obj);

	// Move constructor
	Polynomial(Polynomial&& obj);

	// Copy assignment operator.
	Polynomial& operator=(const Polynomial& obj);

	// Move assignment operator.
	Polynomial& operator=(Polynomial&& obj);

	Polynomial operator+(const Polynomial& obj) const;
	Polynomial operator-(const Polynomial& obj) const;

	Polynomial& operator+=(const Polynomial& obj);
	Polynomial& operator-=(const Polynomial& obj);

	Polynomial operator*(const BinPolynomial& obj) const;
	Polynomial operator+(const BinPolynomial& obj) const;
	Polynomial operator-(const BinPolynomial& obj) const;

	Polynomial& operator*=(const BinPolynomial& obj);
	Polynomial& operator+=(const BinPolynomial& obj);
	Polynomial& operator-=(const BinPolynomial& obj);

	bool operator==(const Polynomial& obj) const;

	//read from left to right (smallest coef to the left - biggest to the right)
	friend std::ostream& operator<< (std::ostream& out, const Polynomial& obj);

	//virtual bool random_init(uint32_t n = 128);

	//virtual bool init(uint8_t c = 0, uint32_t n = 128);

	//virtual bool init(const std::string& str, uint32_t n = 128);

	friend class BinPolynomial;
	friend class RBinLWE256;

};

#endif
