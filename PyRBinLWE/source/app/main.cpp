#include <iostream>
#include <pybind11/embed.h>

namespace py = pybind11;

int main()
{
    py::scoped_interpreter guard{}; // start the interpreter and keep it alive

    py::print("this python code is compiled from C++"); // use the Python API

    auto pymodule = py::module::import("pymodule");

    py::exec(R"(
        import sys
        print(sys.path)
        text = input("press enter")
    )");

    return 0;
}