#include "FileStatParser.h"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sstream>

FileStatParser::FileStatParser(char *filePathname) {
	this->filePathname = string(filePathname);
}

FileStatParser::FileStatParser(string filePathname):
		filePathname(filePathname) {
}

FileStatParser::~FileStatParser() {
}

void FileStatParser::parse() {
	struct stat s;
	int ret = lstat(filePathname.c_str(), &s);
	if (ret) {
		description = "Get file info error!";
		return;
	}

	string userName = getpwuid(s.st_uid)->pw_name;
	string groupName = getgrgid(s.st_gid)->gr_name;

	string type = getType(s);
	string createTime = getCreateTime(s);
	string modifyTime = getModifyTime(s);
	string accessTime = getAccessTime(s);
	string fileSize = getFileSize(s);
	string ownerPermissions = getOwnerPermissions(s);
	string groupPermissions = getGroupPermissions(s);
	string othersPermissions = getOthersPermissions(s);
	string specialPermissions = getSpecialPermissions(s);

	description = "User: \n\t" + userName + "\n\n" \
			+ "Group: \n\t" + groupName + "\n\n" \
			+ "Type: \n\t" + type + "\n\n" \
			+ "Create Time: \n\t" + createTime + "\n\n" \
			+ "Modify Time: \n\t" + modifyTime + "\n\n" \
			+ "Access Time: \n\t" + accessTime + "\n\n" \
			+ "File Size: \n\t" + fileSize + "\n\n" \
			+ "OwnerPermissions: \n\t" + ownerPermissions + "\n\n"
			+ "GroupPermissions: \n\t" + groupPermissions + "\n\n"
			+ "OthersPermissions: \n\t" + othersPermissions + "\n\n"
			+ "SpecialPermissions: \n\t" + specialPermissions + "\n\n";
}

string FileStatParser::getType(struct stat s) {
	mode_t mode = s.st_mode;
	if (S_ISREG(mode)) {
		return "regular file";
	} else if (S_ISDIR(mode)) {
		return "directory";
	} else if (S_ISCHR(mode)) {
		return "character device";
	} else if (S_ISBLK(mode)) {
		return "block device";
	} else if (S_ISFIFO(mode)) {
		return "FIFO (named pipe)";
	} else if (S_ISLNK(mode)) {
		return "symbolic link";
	} else if (S_ISSOCK(mode)) {
		return "socket";
	}

	return "undefined type";
}

string FileStatParser::getCreateTime(struct stat s) {
	struct tm t;
	localtime_r(&s.st_ctim.tv_sec, &t);
	return transferTmToStr(t);
}

string FileStatParser::getModifyTime(struct stat s) {
	struct tm t;
	localtime_r(&s.st_mtim.tv_sec, &t);
	return transferTmToStr(t);
}

string FileStatParser::getAccessTime(struct stat s) {
	struct tm t;
	localtime_r(&s.st_atim.tv_sec, &t);
	return transferTmToStr(t);
}

string FileStatParser::transferTmToStr(struct tm t) {
	int year, month, day, hour, minute, second;
	year = t.tm_year + 1900;
	month = t.tm_mon + 1;
	day = t.tm_mday;
	hour = t.tm_hour;
	minute = t.tm_min;
	second = t.tm_sec;

	stringstream ostr;
	ostr << year << '-' << month << '-' << day \
			<< ' ' << hour << ':' << minute << ':' << second;
	return ostr.str();
}

string FileStatParser::getFileSize(struct stat s) {
	long size = s.st_blksize * s.st_blocks;
	stringstream ostr;
	ostr << size << " B";
	return ostr.str();
}

string FileStatParser::getSpecialPermissions(struct stat s) {
	vector<string> permissions;

	mode_t mode = s.st_mode;
	if (mode & S_ISUID) {
		permissions.push_back("set-user-ID");
	}

	if (mode & S_ISGID) {
		permissions.push_back("set-group-ID");
	}

	if (mode & S_ISVTX) {
		permissions.push_back("sticky");
	}

	return formPermissionsString(permissions, "no special permission");
}

string FileStatParser::getOwnerPermissions(struct stat s) {
	vector<string> permissions;

	mode_t mode = s.st_mode;
	if (mode & S_IRUSR) {
		permissions.push_back("Read Permission");
	}

	if (mode & S_IWUSR) {
		permissions.push_back("Write Permission");
	}

	if (mode & S_IXUSR) {
		permissions.push_back("Execute Permission");
	}

	return formPermissionsString(permissions, "no owner permissions");
}

string FileStatParser::getGroupPermissions(struct stat s) {
	vector<string> permissions;

	mode_t mode = s.st_mode;
	if (mode & S_IRGRP) {
		permissions.push_back("Read Permission");
	}

	if (mode & S_IWGRP) {
		permissions.push_back("Write Permission");
	}

	if (mode & S_IXGRP) {
		permissions.push_back("Execute Permission");
	}

	return formPermissionsString(permissions, "no group permissions");
}

string FileStatParser::getOthersPermissions(struct stat s) {
	vector<string> permissions;

	mode_t mode = s.st_mode;
	if (mode & S_IROTH) {
		permissions.push_back("Read Permission");
	}

	if (mode & S_IWOTH) {
		permissions.push_back("Write Permission");
	}

	if (mode & S_IXOTH) {
		permissions.push_back("Execute Permission");
	}

	return formPermissionsString(permissions, "no others permissions");
}

string FileStatParser::formPermissionsString(vector<string> permissions, string noHint) {
	if (!permissions.empty()) {
		string permissionsStr;
		vector<string>::iterator it;
		for (it = permissions.begin(); it != permissions.end(); it++) {
			if (permissionsStr.empty()) {
				permissionsStr += *it;
			} else {
				permissionsStr += "\n\t" + *it;
			}
		}
		return permissionsStr;
	}

	return noHint;
}

string FileStatParser::getDescription() {
	return description;
}


