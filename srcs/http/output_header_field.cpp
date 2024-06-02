#include "iostream"

void OutputHeaderField(void) {
	std::cout << "field-name" << ":" << "OWS" << "field-value" << "OWS";
}

 int main(void) {
	OutputHeaderField();
	return 0;
 }
