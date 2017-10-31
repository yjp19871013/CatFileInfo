#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include <string>
#include <vector>
#include <sys/stat.h>

using namespace std;

class FileStatParser {
public:
	explicit FileStatParser(char *pathname);
	explicit FileStatParser(string pathname);
	virtual ~FileStatParser();

	void parse();
	string getDescription();

private:
	string getType(struct stat s);
	string getCreateTime(struct stat s);
	string getModifyTime(struct stat s);
	string getAccessTime(struct stat s);
	string transferTmToStr(struct tm t);
	string getFileSize(struct stat s);
	string getOwnerPermissions(struct stat s);
	string getGroupPermissions(struct stat s);
	string getOthersPermissions(struct stat s);
	string getSpecialPermissions(struct stat s);
	string formPermissionsString(vector<string> permissions, string noHint);

private:
	string filePathname;
	string description;
};

#endif /* FILEPARSER_H_ */
