#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include "Polynomial.h"
#include "Encryptor.h"

namespace py = pybind11;
using namespace pybind11::literals;

//PYBIND11_MAKE_OPAQUE(std::vector<CipherText>);

template <class Base = PolynomialBase> class PyPolynomialBase : public Base {
public:
	/* Inherit the constructors */
	using Base::Base;
	/* Trampoline (need one for each virtual function) */
	bool random_init(uint32_t n = 128) override {
		PYBIND11_OVERRIDE(
			bool, /* Return type */
			PolynomialBase, /* Parent class */
			random_init, /* Name of function in C++ (must match Python name) */
			n /* Argument(s) */
		);
	}
	bool init(uint8_t c = 0, uint32_t n = 128) override {
		PYBIND11_OVERRIDE(
			bool, /* Return type */
			PolynomialBase, /* Parent class */
			init, /* Name of function in C++ (must match Python name) */
			c, n /* Argument(s) */
		);
	}
	bool init_str(const std::string& str, uint32_t n = 128) override {
		PYBIND11_OVERRIDE(
			bool, /* Return type */
			PolynomialBase, /* Parent class */
			init_str, /* Name of function in C++ (must match Python name) */
			str, n /* Argument(s) */
		);
	}
	std::string print_string() const override {
		PYBIND11_OVERRIDE(
			std::string, /* Return type */
			PolynomialBase, /* Parent class */
			print_string, /* Name of function in C++ (must match Python name) */
			 /* Argument(s) */
		);
	}
};


PYBIND11_MODULE(mpamok, handle) {

	handle.doc() = "Module name: PyRBinLWE256 \n \
					Author:	Vladyslav Shapoval \n \
					MPA-MOK-2022 semestral project ";

	py::class_<PolynomialBase, PyPolynomialBase<>>(handle, "PolynomialBase")
		.def(py::init<>())
		.def("__copy__", [](const PolynomialBase& self) {
			return PolynomialBase(self);
		})
		.def("__deepcopy__", [](const PolynomialBase& self, py::dict) {
			return PolynomialBase(self);
		}, "memo"_a)
		.def("to_bytes", [](const PolynomialBase& self) {
			return py::bytes(self.to_string());
			})
		.def("print_bytes", [](const PolynomialBase& self) {
			return py::bytes(self.print_string());
			})
		.def("get_poly_mod", &PolynomialBase::get_poly_mod)
		.def("init", &PolynomialBase::init, py::arg("c") = 0, py::arg("n") = 128)
		.def("random_init", &PolynomialBase::random_init, py::arg("n") = 128)
		.def("init_str", &PolynomialBase::init_str, py::arg("str"), py::arg("n") = 128);

	py::class_<Polynomial, PolynomialBase, PyPolynomialBase<Polynomial>>(handle, "Polynomial")
		.def(py::init<>())
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self == py::self)
		.def(py::self + BinPolynomial())
		.def(py::self - BinPolynomial())
		.def(py::self * BinPolynomial())
		.def(py::self += BinPolynomial())
		.def(py::self -= BinPolynomial())
		.def(py::self *= BinPolynomial());

	py::class_<BinPolynomial, PolynomialBase, PyPolynomialBase<BinPolynomial>>(handle, "BinPolynomial")
		.def(py::init<>())
		.def(py::self == py::self)
		.def("__add__", [](const BinPolynomial& a, const Polynomial& b) {
			return a + b;
			}, py::is_operator())
		.def("__sub__", [](const BinPolynomial& a, const Polynomial& b) {
				return a - b;
			}, py::is_operator());

	py::class_<KeyRing>(handle, "KeyRing")
		.def(py::init<>())
		.def(py::init<const Polynomial&, const BinPolynomial&>())
		.def_readwrite("public_key", &KeyRing::public_key)
		.def_readwrite("private_key", &KeyRing::private_key)
		.def("to_bytes", [](const KeyRing& self) {
				return py::bytes(self.to_string());
			})
		.def("print_bytes", [](const KeyRing& self) {
						return py::bytes(self.print_string());
			})
		.def(py::self == py::self);

	py::class_<CipherText>(handle, "CipherText")
		.def(py::init<>())
		.def_readwrite("c1", &CipherText::c1)
		.def_readwrite("c2", &CipherText::c2)
		.def("to_bytes", [](const CipherText& self) {
			return py::bytes(self.to_string());
			})
		.def("print_bytes", [](const CipherText& self) {
			return py::bytes(self.print_string());
			})
		.def(py::self == py::self);

	py::class_<RBinLWE256>(handle, "RBinLWE")
		.def(py::init<>())
		.def("init", &RBinLWE256::init)
		.def("get_encryption_time", &RBinLWE256::get_enc_time)
		.def("get_decryption_time", &RBinLWE256::get_dec_time)
		.def("key_generator", &RBinLWE256::key_gen, py::arg("a"))
		.def("encrypt", &RBinLWE256::encrypt, py::arg("public_key"), py::arg("msg"), py::arg("a"))
		.def("decrypt", &RBinLWE256::decrypt, py::arg("private_key"), py::arg("c"));

}