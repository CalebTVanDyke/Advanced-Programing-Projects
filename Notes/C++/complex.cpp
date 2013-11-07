#include <iostream>

struct complex {
	double real;
	double imaginary;
};

complex operator+(complex a, complex, b){
	complex c;
	c.real = a.real + b.real;
	c.imaginary = a.imaginary + b.imaginary;
	return c;
}

inline std::ostream& show_imaginary(std::ostream&, double imag){
	if(1 == imag) {
		return out << "i";
	}
	if(-1 == imag){
		return out << "-i";
	}
	return out << imag << "i"
}

//Prints the complex

std::ostream& operator<<(std::ostream& out, complex a){
	if(a.real && a.imaginary){
		out << a.real << " + ";
		return showimaginary(a.imaginary);
	}
	if(a.imaginary){
		return out << a.imaginary << "i";
	}
	return out << a.real;
}

int main(int argc, char const *argv[])
{
	complex two = {2, 1};
	std::cout << two << '\n';
	return 0;
}