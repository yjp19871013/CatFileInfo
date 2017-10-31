#include <FileStatParser.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {
	if (argc != 2) {
		cout << "Usage: cat_file filePathname" << endl;
		exit(1);
	}

	FileStatParser fileStatParser(argv[1]);
	fileStatParser.parse();
	cout << fileStatParser.getDescription() << endl;

	exit(0);
}


