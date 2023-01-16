#include "Polynomial.h"

#include <random> // TODO: change on libsodium later

#ifdef _DEBUG_

static std::uniform_int_distribution<uint32_t> unid1(0, Q - 1);
static std::uniform_int_distribution<uint32_t> unid2(0, 3);
#else

static std::uniform_int_distribution<uint32_t> unid1(0, Q-1);
static std::uniform_int_distribution<uint32_t> unid2(0, (Q - 1)/2);
#endif
static std::random_device rd1;
static std::mt19937 prnd(rd1());

/*****************************************PolynomialBase*****************************************/

//vertex_shader& operator=(vertex_shader&& rhs)
//{
//	shader::operator=(std::move(rhs));
//	return *this;
//}

bool PolynomialBase::memory_init(uint32_t m, uint8_t c)
{
	bool flag = true;
	if (m != mem_size) {

		if (buffer != nullptr) {
			delete[] buffer;
#ifdef _DEBUG_
			std::cout << "delete\n";
#endif
		}
		buffer = nullptr;
		mem_size = m;
		try {
			buffer = new uint8_t[mem_size]();

			if (c != 0)
			{
				for (uint32_t i = 0; i < mem_size; i++)
					buffer[i] = c;
			}
		}
		catch (const std::bad_alloc& e) {
			std::cout << "Allocation failed: " << e.what() << '\n';
			buffer = nullptr;
			mem_size = 0;
			flag = false;
		}
	}
	return flag;
}

// Copy constructor
PolynomialBase::PolynomialBase(const PolynomialBase& obj)
{

	poly_mod = obj.poly_mod;
	if (memory_init(obj.mem_size, 0))
	{
		for (uint32_t i = 0; i < mem_size; i++)
			buffer[i] = obj.buffer[i];
	}

#ifdef _DEBUG_
	std::cout << "Copy constructor PolynomialBase" << std::endl;
#endif
}

// Move constructor
PolynomialBase::PolynomialBase(PolynomialBase&& obj) noexcept
{
	mem_size = obj.mem_size;
	buffer = obj.buffer;
	poly_mod = obj.poly_mod;
	obj.buffer = nullptr;
	obj.mem_size = 0;

#ifdef _DEBUG_
	std::cout << "Move constructor PolynomialBase" << std::endl;
#endif
}


// Copy assignment operator.
PolynomialBase& PolynomialBase::operator=(const PolynomialBase& obj)
{
	poly_mod = obj.poly_mod;
	if (memory_init(obj.mem_size, 0))
	{
		for (uint32_t i = 0; i < mem_size; i++)
			buffer[i] = obj.buffer[i];
	}

#ifdef _DEBUG_
	std::cout << "Copy assignment operator PolynomialBase" << std::endl;
#endif

	return *this;
}

// Move assignment operator.
PolynomialBase& PolynomialBase::operator=(PolynomialBase&& obj)
{
	if (this != &obj)
	{
		mem_size = obj.mem_size;
		poly_mod = obj.poly_mod;
		delete[]buffer;
		buffer = obj.buffer;
		obj.buffer = nullptr;
		obj.mem_size = 0;
	}
#ifdef _DEBUG_
	std::cout << "Move assignment operator PolynomialBase" << std::endl;
#endif
	return *this;
}

bool PolynomialBase::random_init(uint32_t n)
{
	bool flag = memory_init(n, 0);
	if (flag)
	{
		poly_mod = n;
		for (uint32_t i = 0; i < mem_size; i++)
			buffer[i] = unid1(prnd);
	}
	return flag;
}

bool PolynomialBase::init(uint8_t c, uint32_t n)
{
	bool flag = memory_init(n, c);
	if(flag)
		poly_mod = n;
	return flag;
}

bool PolynomialBase::init_str(const std::string& str, uint32_t n)
{

	int size = str.size();
	bool flag = memory_init(n, 0);
	if (flag)
	{
		poly_mod = n;
		for (int i = 0; i < mem_size; i++)
		{
			if (i < size)
				buffer[i] = str[i];
			else
				buffer[i] = (uint8_t)(unid1(prnd) % Q);
		}
	}
	return flag;
}

std::string PolynomialBase::to_string() const
{
	return std::string((char*)buffer, mem_size);
}

std::string PolynomialBase::print_string() const
{
	std::stringstream out;
	out << "[";
	if (buffer != nullptr)
	{
		for (uint32_t i = 0; i < mem_size - 1; i++)
		{
			out << (uint16_t)buffer[i] << ", ";
		}
		out << (uint16_t)buffer[mem_size - 1] << ']';
	}
	else {
		out << ']';
	}

	return out.str();
}

/*****************************************PolynomialBase*****************************************/

/*****************************************Polynomial*****************************************/

////parametrized constructor
//Polynomial::Polynomial(bool mem_init)
//{
//	buffer = nullptr;
//	mem_size = 0;
//	if (mem_init == true)
//	{
//		mem_size = poly_mod;
//		try {
//			buffer = new uint8_t[mem_size]();
//		}
//		catch (const std::bad_alloc& e) {
//			std::cout << "Allocation failed: " << e.what() << '\n';
//			buffer = nullptr;
//		}
//	}
//#ifdef _DEBUG_
//	std::cout << "parametrized  constructor Polynomial" << std::endl;
//#endif
//}

Polynomial::Polynomial()
	: PolynomialBase()
{
	//mem_size = 0;
#ifdef _DEBUG_
	std::cout << "basic constructor Polynomial\n";
#endif
};


// Copy constructor
Polynomial::Polynomial(const Polynomial& obj)
	:PolynomialBase(obj)
{
#ifdef _DEBUG_
	std::cout << "Copy constructor Polynomial" << std::endl;
#endif
}

// Move constructor
Polynomial::Polynomial(Polynomial&& obj)
	:PolynomialBase(std::move(obj))
{

#ifdef _DEBUG_
	std::cout << "Move constructor Polynomial" << std::endl;
#endif
}


// Copy assignment operator.
Polynomial& Polynomial::operator=(const Polynomial& obj)
{
	PolynomialBase::operator=(obj);
#ifdef _DEBUG_
	std::cout << "Copy assignment operator Polynomial" << std::endl;
#endif

	return *this;
}

// Move assignment operator.
Polynomial& Polynomial::operator=(Polynomial&& obj)
{
	PolynomialBase::operator=(std::move(obj));
#ifdef _DEBUG_
	std::cout << "Move assignment operator Polynomial" << std::endl;
#endif
	return *this;
}


std::ostream& operator<< (std::ostream& out, const Polynomial& obj)
{
	out << '[';
	if (obj.buffer != nullptr)
	{
		for (uint32_t i = 0; i < obj.mem_size - 1; i++)
		{
			out << obj.buffer[i] << ", ";
		}
		out << obj.buffer[obj.mem_size - 1] << ']';
	}
	else {
		out << ']';
	}
	return out;
}



Polynomial Polynomial::operator+(const Polynomial& obj) const
{
	Polynomial Temp;
	Temp.init(0, obj.poly_mod);
	if ((buffer != nullptr) && (obj.buffer != nullptr))
	{
		for (uint32_t i = 0; i < mem_size; i++)
		{
			Temp.buffer[i] = (buffer[i] + obj.buffer[i]) % Q;
		}
	}
	return Temp;
}

Polynomial Polynomial::operator-(const Polynomial& obj) const
{
	Polynomial Temp;
	Temp.init(0, obj.poly_mod);
	if ((buffer != nullptr) && (obj.buffer != nullptr))
	{
		for (uint32_t i = 0; i < mem_size; i++)
		{
			Temp.buffer[i] = (buffer[i] - obj.buffer[i]) % Q;
		}
	}
	return Temp;
}

Polynomial& Polynomial::operator+=(const Polynomial& obj)
{
	if ((buffer != nullptr) && (obj.buffer != nullptr))
	{
		for (uint32_t i = 0; i < mem_size; i++)
		{
			buffer[i] = (buffer[i] + obj.buffer[i]) % Q;
		}

	}
	return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& obj)
{
	if ((buffer != nullptr) && (obj.buffer != nullptr))
	{
		for (uint32_t i = 0; i < mem_size; i++)
		{
			buffer[i] = (buffer[i] - obj.buffer[i]) % Q;
		}
	}
	return *this;
}

Polynomial Polynomial::operator*(const BinPolynomial& obj) const
{
	uint16_t bit = 0;
	uint32_t d = 0;
	Polynomial Temp;
	Temp.init(0, obj.poly_mod);
	uint32_t j = 0;
	for (uint32_t m = 0; m < obj.mem_size; m++)
	{
		for (uint32_t n = 0; n < 8; n++)
		{
			bit = (obj.buffer[m] >> n) & 1;
			if (bit) {
				for (uint32_t i = 0; i < poly_mod; i++)
				{
					j = (i + d) % poly_mod;
					if (j >= i) {
						Temp.buffer[j] = (Temp.buffer[j] + buffer[i]) % Q;
					}
					else {
						Temp.buffer[j] = (Temp.buffer[j] - buffer[i]) % Q;
					}
				}
			}
			d++;
		}
	}
	return Temp;
}

Polynomial& Polynomial::operator*=(const BinPolynomial& obj)
{
	Polynomial Temp = *this * obj;
	*this = Temp;
	return *this;
}

Polynomial Polynomial::operator+(const BinPolynomial& obj) const
{
	uint16_t bit = 0;
	uint32_t i = 0;
	Polynomial Temp;
	Temp.init(0, obj.poly_mod);
	for (uint32_t k = 0; k < obj.mem_size; k++)
	{
		for (uint32_t n = 0; n < 8; n++)
		{
			bit = (obj.buffer[k] >> n) & 1;
			Temp.buffer[i] = (buffer[i] + bit) % Q;
			i++;
		}
	}
	return Temp;
}

Polynomial Polynomial::operator-(const BinPolynomial& obj) const
{
	uint16_t bit = 0;
	uint32_t i = 0;
	Polynomial Temp;
	Temp.init(0, obj.poly_mod);
	for (uint32_t k = 0; k < obj.mem_size; k++)
	{
		for (uint32_t n = 0; n < 8; n++)
		{
			bit = (obj.buffer[k] >> n) & 1;
			Temp.buffer[i] = (buffer[i] - bit) % Q;
			i++;
		}
	}
	return Temp;
}

Polynomial& Polynomial::operator+=(const BinPolynomial& obj)
{
	uint16_t bit = 0;
	uint32_t i = 0;
	for (uint32_t k = 0; k < obj.mem_size; k++)
	{
		for (uint32_t n = 0; n < 8; n++)
		{
			bit = (obj.buffer[k] >> n) & 1;
			buffer[i] = (buffer[i] + bit) % Q;
			i++;
		}
	}
	return *this;
}

Polynomial& Polynomial::operator-=(const BinPolynomial& obj)
{
	uint16_t bit = 0;
	uint32_t i = 0;
	for (uint32_t k = 0; k < obj.mem_size; k++)
	{
		for (uint32_t n = 0; n < 8; n++)
		{
			bit = (obj.buffer[k] >> n) & 1;
			buffer[i] = (buffer[i] - bit) % Q;
			i++;
		}
	}
	return *this;
}

bool Polynomial::operator==(const Polynomial& obj) const
{
	if (mem_size == obj.mem_size)
	{
		bool flag = true;
		for (uint32_t i = 0; i < mem_size; i++)
		{
			if (buffer[i] != obj.buffer[i])
				flag = false;
		}
		return flag;
	}
	else return false;
}



/*****************************************Polynomial*****************************************/

/*****************************************BinPolynomial*****************************************/


BinPolynomial::BinPolynomial()
	: PolynomialBase()
{
	//mem_size = 0;
#ifdef _DEBUG_
	std::cout << "basic constructor BinPolynomial\n";
#endif
};


// Copy constructor
BinPolynomial::BinPolynomial(const BinPolynomial& obj)
	:PolynomialBase(obj)
{
#ifdef _DEBUG_
	std::cout << "Copy constructor BinPolynomial" << std::endl;
#endif
}

// Move constructor
BinPolynomial::BinPolynomial(BinPolynomial&& obj)
	:PolynomialBase(std::move(obj))
{

#ifdef _DEBUG_
	std::cout << "Move constructor BinPolynomial" << std::endl;
#endif
}


// Copy assignment operator.
BinPolynomial& BinPolynomial::operator=(const BinPolynomial& obj)
{
	PolynomialBase::operator=(obj);
#ifdef _DEBUG_
	std::cout << "Copy assignment operator BinPolynomial" << std::endl;
#endif

	return *this;
}

// Move assignment operator.
BinPolynomial& BinPolynomial::operator=(BinPolynomial&& obj)
{
	PolynomialBase::operator=(std::move(obj));
#ifdef _DEBUG_
	std::cout << "Move assignment operator BinPolynomial" << std::endl;
#endif
	return *this;
}

bool BinPolynomial::random_init(uint32_t n )
{
	bool flag = memory_init(n / 8, 0);
	if (flag)
	{
		poly_mod = n;
		for (uint32_t i = 0; i < mem_size; i++)
			buffer[i] = unid1(prnd);
	}
	return flag;
}

std::ostream& operator<< (std::ostream& out, const BinPolynomial& obj)
{
	uint16_t bit = 0;
	out << '[';
	if (obj.buffer != nullptr)
	{
		for (uint32_t i = 0; i < obj.mem_size; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				bit = (obj.buffer[i] >> j) & 1;
				out << bit << ", ";
			}
		}
		out << ']';
	}
	return out;
}

std::string BinPolynomial::print_string() const
{
	uint16_t bit = 0;
	std::stringstream out;
	out << '[';
	if (buffer != nullptr)
	{
		for (uint32_t i = 0; i < mem_size; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				bit = (buffer[i] >> j) & 1;
				out << bit << ", ";
			}
		}
		out << ']';
	}
	return out.str();
}

Polynomial BinPolynomial::operator+(const Polynomial& obj) const
{
	uint16_t bit = 0;
	uint32_t i = 0;
	Polynomial Temp;
	Temp.init(0, obj.poly_mod);
	for (uint32_t k = 0; k < mem_size; k++)
	{
		for (uint32_t n = 0; n < 8; n++)
		{
			bit = (buffer[k] >> n) & 1;
			Temp.buffer[i] = (bit + obj.buffer[i]) % Q;
			i++;
		}
	}
	return Temp;
}
Polynomial BinPolynomial::operator-(const Polynomial& obj) const
{
	uint16_t bit = 0;
	uint32_t i = 0;
	Polynomial Temp;
	Temp.init(0, obj.poly_mod);
	for (uint32_t k = 0; k < mem_size; k++)
	{
		for (uint32_t n = 0; n < 8; n++)
		{
			bit = (buffer[k] >> n) & 1;
			Temp.buffer[i] = (bit - obj.buffer[i]) % Q;
			i++;
		}
	}
	return Temp;
}

bool BinPolynomial::operator==(const BinPolynomial& obj) const
{
	if (mem_size == obj.mem_size)
	{
		bool flag = true;
		for (uint32_t i = 0; i < mem_size; i++)
		{
			if (buffer[i] != obj.buffer[i])
				flag = false;
		}
		return flag;
	}
	else return false;
}

bool BinPolynomial::init(uint8_t c, uint32_t n)
{
	bool flag = memory_init(n / 8, c);
	if (flag)
		poly_mod = n;
	return flag;
}

//string substr (size_t pos, size_t len) const;
bool BinPolynomial::init_str(const std::string& str, uint32_t n )
{
	int size = str.size();
	bool flag = memory_init(n / 8, 0);
	if (flag)
	{
		poly_mod = n;
		for (int i = 0; i < mem_size; i++)
		{
			if (i < size)
				buffer[i] = str[i];
			else
				buffer[i] = (uint8_t)(unid1(prnd) % Q);
		}
	}
	return flag;
}



/*****************************************BinPolynomial*****************************************/
