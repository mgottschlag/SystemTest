
#include "TestSuite.hpp"
#include "TestConfig.hpp"

#include <QCoreApplication>
#include <QProcess>

#include <iostream>

int main(int argc, char **argv) {
	QCoreApplication application(argc, argv);

	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <path> <class> <testsuite>" << std::endl;
		return -1;
	}
	// Load test config
	TestConfig config(argv[1], argv[2]);

	// Load test suite
	TestSuite suite;
	if (!suite.load(argv[3])) {
		std::cerr << "Could not load the test suite." << std::endl;
		return -1;
	}
	// Execute all tests
	return suite.execute(config);
}

