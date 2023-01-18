#include"Encryptor.h"

/*****************************************RBinLWE256*****************************************/

bool RBinLWE256::init()
{
	return (e1.init() &&
			e2.init() &&
			e3.init() &&
			r1.init() &&
			r2.init());
	//a.random_init();
}

RBinLWE256::RBinLWE256()
{
#ifdef _DEBUG_
	std::cout << "basic constructor RBinLWE256\n";
#endif
}


// Copy constructor
RBinLWE256::RBinLWE256(const RBinLWE256& obj)
	:e1(obj.e1),
	e2(obj.e2),
	e3(obj.e3),
	r1(obj.r1),
	r2(obj.r2)
	//a(obj.a)
{
#ifdef _DEBUG_
	std::cout << "copy constructor RBinLWE256\n";
#endif
}

// Move constructor
RBinLWE256::RBinLWE256(RBinLWE256&& obj)
	:e1(std::move(obj.e1)),
	e2(std::move(obj.e2)),
	e3(std::move(obj.e3)),
	r1(std::move(obj.r1)),
	r2(std::move(obj.r2))
	//a(obj.a) 
{
#ifdef _DEBUG_
	std::cout << "move constructor RBinLWE256\n";
#endif
};

RBinLWE256::~RBinLWE256()
{
#ifdef _DEBUG_
	std::cout << "Destructor RBinLWE256\n";
#endif
}

// Copy assignment operator.
RBinLWE256& RBinLWE256::operator=(const RBinLWE256& obj)
{
	e1 = obj.e1;
	e2 = obj.e2;
	e3 = obj.e3;
	r1 = obj.r1;
	r2 = obj.r2;
	//a = obj.a;
#ifdef _DEBUG_
	std::cout << "copy asign. operator RBinLWE256\n";
#endif
	return *this;
}


// Move assignment operator.
RBinLWE256& RBinLWE256::operator=(RBinLWE256&& obj)
{
	e1 = std::move(obj.e1);
	e2 = std::move(obj.e2);
	e3 = std::move(obj.e3);
	r1 = std::move(obj.r1);
	r2 = std::move(obj.r2);
	//a = std::move(obj.a);
#ifdef _DEBUG_
	std::cout << "move asign. operator RBinLWE256\n";
#endif
	return *this;
}

std::ostream& operator<< (std::ostream& out, const RBinLWE256& obj)
{
	out << "\nRBinLWE obj.\n";
	out << "\n\te1 = " << obj.e1 << '\n';
	out << "\n\te2 = " << obj.e2 << '\n';
	out << "\n\te3 = " << obj.e3 << '\n';
	out << "\n\tr1 = " << obj.r1 << '\n';
	out << "\n\tr1 = " << obj.r2 << '\n';
	//out << "\n\ta = " << obj.a << '\n';
	return out;
}

KeyRing RBinLWE256::key_gen(const Polynomial& a) 
{
	KeyRing keys;
	e1.random_init();
	e2.random_init();
	e3.random_init();
	r1.random_init();
	r2.random_init();

	//Polynomial p = ;
	keys.public_key = r1 - (a * r2);
	keys.private_key = r2;

	return keys;
}

Polynomial RBinLWE256::encode(const BinPolynomial& m)
{
	uint16_t bit = 0;
	int i = 0;
	Polynomial Temp;
	Temp.init(0, m.poly_mod);
	for (uint32_t j = 0; j < m.mem_size; j++)
	{
		for (uint32_t n = 0; n < 8; n++)
		{
			bit = (m.buffer[j] >> n) & 1;
			Temp.buffer[i] = bit * (Q / 2); 
			i++;
		}
	}
	return Temp;
}


BinPolynomial RBinLWE256::decode(const Polynomial& c) const
{
	static int k = (c.poly_mod - 2) / 2;

	uint16_t bit = 0;
	int i = 0;
	BinPolynomial Temp;
	Temp.init(0, c.poly_mod);
	for (uint32_t j = 0; j < Temp.mem_size; j++)
	{
		for (uint32_t n = 0; n < 8; n++)
		{
			int t = (c.buffer[i] - i - k);

			t = t < 0 ? -t : t;

			if (t <= (Q / 4)) 
				bit = 1;
			else
				bit = 0;

			Temp.buffer[j] |= bit << n;
			i++;
		}
	}
	return Temp;
}

CipherText RBinLWE256::encrypt(const Polynomial& p, const BinPolynomial& m, const Polynomial& a)
{
	CipherText c;
	e1.random_init();
	e2.random_init();
	e3.random_init();
	Polynomial n = encode(m);

	c.c1 = e2 + (a * e1);
	c.c2 = n + e3 + (p * e1);

	return c;
}


BinPolynomial RBinLWE256::decrypt(const BinPolynomial& r, const CipherText& c) const
{
	Polynomial m = c.c2 + c.c1 * r;
	return decode(m);
}


//std::vector<CipherText> RBinLWE256::encrypt_string(const Polynomial& p, const std::string& buffer, const Polynomial& a)
//{
//	BinPolynomial m;
//	m.init(0, a.poly_mod);
//	int block_size = (a.poly_mod / 8);
//	int block_num = buffer.size() / block_size;
//	std::vector<CipherText> ctext(block_num+1);
//
//	for (int i = 0, j = 0; j <= block_num; i += block_size, j++) {
//		m.init_str(buffer.substr(i, block_size));
//		ctext[j] = encrypt(p, m, a);
//	}
//	return ctext;
//}
//
//std::string RBinLWE256::decrypt_string(const BinPolynomial& r, const std::vector<CipherText>& cbuffer) const
//{
//	std::string str;
//	BinPolynomial m;
//	m.init(0, r.poly_mod);
//	for (int i = 0; i < cbuffer.size(); i++)
//	{
//		m = decrypt(r, cbuffer[i]);
//		str += m.to_string();
//	}
//	return str;
//}



/*****************************************RBinLWE256*****************************************/

/*****************************************KeyRing*****************************************/


KeyRing::KeyRing()
{
#ifdef _DEBUG_
	std::cout << "basic constructor RBinLWE256\n";
#endif
}

// Copy constructor
KeyRing::KeyRing(KeyRing& obj)
	:public_key(obj.public_key), private_key(obj.private_key)
{
#ifdef _DEBUG_
	std::cout << "copy constructor RBinLWE256\n";
#endif
}

// Move constructor
KeyRing::KeyRing(KeyRing&& obj)
	:public_key(std::move(obj.public_key)),
	private_key(std::move(obj.private_key))
{
#ifdef _DEBUG_
	std::cout << "move constructor RBinLWE256\n";
#endif
}

// Copy assignment operator.
KeyRing& KeyRing::operator=(const KeyRing& obj)
{
	public_key = obj.public_key;
	private_key = obj.private_key;
#ifdef _DEBUG_
	std::cout << "copy asign. operator KeyRing\n";
#endif
	return *this;
}

// Move assignment operator.
KeyRing& KeyRing::operator=(KeyRing&& obj)
{
	public_key = std::move(obj.public_key);
	private_key = std::move(obj.private_key);
#ifdef _DEBUG_
	std::cout << "move asign. operator KeyRing\n";
#endif
	return *this;
}

std::ostream& operator<< (std::ostream& out, const KeyRing& obj)
{
	out << "\nPublic Key.\n";
	out <<  obj.public_key << '\n';
	out << "\nPrivate Key.\n";
	out << obj.private_key << '\n';
	return out;
}

/*****************************************KeyRing*****************************************/

/*****************************************CipherText*****************************************/
CipherText::CipherText()
{
#ifdef _DEBUG_
	std::cout << "basic constructor CipherText\n";
#endif
}

// Copy constructor
CipherText::CipherText(const CipherText& obj)
	:c1(obj.c1), c2(obj.c2)
{
#ifdef _DEBUG_
	std::cout << "copy constructor CipherText\n";
#endif
}

// Move constructor
CipherText::CipherText(CipherText&& obj)
	:c1(std::move(obj.c1)),
	c2(std::move(obj.c2))
{
#ifdef _DEBUG_
	std::cout << "move constructor CipherText\n";
#endif
}

// Copy assignment operator.
CipherText& CipherText::operator=(const CipherText& obj)
{
	c1 = obj.c1;
	c2 = obj.c2;
#ifdef _DEBUG_
	std::cout << "copy asign. operator CipherText\n";
#endif
	return *this;
}

// Move assignment operator.
CipherText& CipherText::operator=(CipherText&& obj)
{
	c1 = std::move(obj.c1);
	c2 = std::move(obj.c2);
#ifdef _DEBUG_
	std::cout << "move asign. operator KeyRing\n";
#endif
	return *this;
}

std::ostream& operator<< (std::ostream& out, const CipherText& obj)
{
	out << "\nCipherText\n";
	out << "\n\t c1 = " << obj.c1 << '\n';
	out << "\n\t c2 = " << obj.c2 << '\n';
	return out;
}

/*****************************************CipherText*****************************************/
