#pragma once


int get_all_files_names_within_folder(string folder)
{
vector<string> names;
string search_path = folder + "/*.png*";
WIN32_FIND_DATA fd;
HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
			{
			names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
::FindClose(hFind);
				}
return names.size();
}

void load_dcm(string adress, char* argv)
{
	string x, folder(argv);
	size_t slash = folder.find_last_of("\\");
	dirPath = (slash != std::string::npos) ? folder.substr(0, slash) : folder;
	x.append(dirPath);
	x.append("\\python.exe ");
	x.append(dirPath);
	x.append("\\main.py ");
	x.append(adress);
	x.append(" ");
	x.append(dirPath);
	char *y = new char[512];
	strcpy(y, x.c_str());
	int k = system(y);
	delete[] y;
}