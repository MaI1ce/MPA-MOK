#include "Polynomial.h"
#include "Encryptor.h"
#include <iostream>

RBinLWE256 foo()
{
    return RBinLWE256(true);
}

void encryption_test1() {
    RBinLWE256 test2(true);

    Polynomial a;

    a.random_init();

    KeyRing keys = test2.keyGen(a);

    std::cout << keys;

    std::cout << "\n--------------------------------\n";

    BinPolynomial m;

    m.random_init();

    auto c = test2.encrypt(keys.public_key, m, a);

    auto n = test2.decrypt(keys.private_key, c);

    std::cout << m << '\n';
    std::cout << n << '\n';

    if (m == n) {
        std::cout << "Encryption / Decryption is OK \n";
    }
    else {
        std::cout << "Encryption / Decryption is NOK \n";
    }
}

void encryption_test2()
{
	RBinLWE256 encryptor(true);

	Polynomial a;

	a.random_init();

	KeyRing keys = encryptor.keyGen(a);

	std::string file = "Some text which needs to be encrypted by RBinLWE256 algorithm ";

	auto cfile = encryptor.encrypt_file(keys.public_key,file,a);

    std::vector<CipherText>::iterator ptr;
    for (ptr = cfile.begin(); ptr < cfile.end(); ptr++)
        std::cout << *ptr << '\n';

	auto dfile = encryptor.decrypt_file(keys.private_key,cfile); 

	std::cout << dfile << '\n';
}

int main()
{
    //encryption_test1();

    encryption_test2();

    return 0;
}