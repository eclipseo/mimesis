#include <iostream>

#include <mimesis.hpp>

using namespace std;

int main(int argc, char *argv[]) {
	Mimesis::MIMEPart msg;
	try {
		msg.load(cin);
	} catch(runtime_error &e) {
		return 1;
	}
	msg.save(cout);
}