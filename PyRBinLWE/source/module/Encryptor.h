#ifndef _ENCRYPTOR_
#define _ENCRYPTOR_

#include "Polynomial.h"
#include <vector>
// 


class KeyRing {
public:
	Polynomial    public_key;
	BinPolynomial private_key;
	friend std::ostream& operator<< (std::ostream& out, const KeyRing& obj);

	KeyRing();

	// parametrized  constructor
	KeyRing(const Polynomial& pub_key, const BinPolynomial& pr_key)
		:public_key(pub_key), private_key(pr_key)
	{}

	// Copy constructor
	KeyRing(KeyRing& obj);

	// Move constructor
	KeyRing(KeyRing&& obj);

	// Copy assignment operator.
	KeyRing& operator=(const KeyRing& obj);

	// Move assignment operator.
	KeyRing& operator=(KeyRing&& obj);

	bool operator==(const KeyRing& obj) const
	{
		return (public_key == obj.public_key) && (private_key == obj.private_key);
	}

	friend class RBinLWE256;
};

class CipherText {
public:
	Polynomial c1;
	Polynomial c2;
	friend std::ostream& operator<< (std::ostream& out, const CipherText& obj);

	CipherText();


	// Copy constructor
	CipherText(const CipherText& obj);

	// Move constructor
	CipherText(CipherText&& obj);

	// Copy assignment operator.
	CipherText& operator=(const CipherText& obj);

	// Move assignment operator.
	CipherText& operator=(CipherText&& obj);

	bool operator==(const CipherText& obj) const
	{
		return (c1 == obj.c1) && (c2 == obj.c2);
	}

	friend class RBinLWE256;
};

class RBinLWE256 {
private:

	BinPolynomial e1;
	BinPolynomial e2;
	BinPolynomial e3;
	BinPolynomial r1;
	BinPolynomial r2;
	//Polynomial	a;

public:
	bool init();
	KeyRing key_gen(const Polynomial& a);
	CipherText encrypt(const Polynomial& p, const BinPolynomial& m, const Polynomial&a) ;
	BinPolynomial decrypt(const BinPolynomial& p, const CipherText& c) ;
	Polynomial encode(const BinPolynomial& m) ;
	BinPolynomial decode(const Polynomial& r) ;

	std::vector<CipherText> encrypt_file(const Polynomial& p, const std::string& buffer, const Polynomial& a) ;
	std::string decrypt_file(const BinPolynomial& r, const std::vector<CipherText>& cbuffer) ;

	RBinLWE256();

	// Copy constructor
	RBinLWE256(const RBinLWE256& obj);

	// Move constructor
	RBinLWE256(RBinLWE256&& obj);

	// Copy assignment operator.
	RBinLWE256& operator=(const RBinLWE256& obj);

	// Move assignment operator.
	RBinLWE256& operator=(RBinLWE256&& obj);

	~RBinLWE256();

	friend std::ostream& operator<< (std::ostream& out, const RBinLWE256& obj);
};


#endif