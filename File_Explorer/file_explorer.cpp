#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
using namespace std;


void listFiles(const string &path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path.c_str());
    if (!dir) {
        perror("Unable to open directory");
        return;
    }

    cout << "\nContents of: " << path << endl;
    cout << "-------------------------------------" << endl;

    while ((entry = readdir(dir)) != nullptr) {

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        struct stat fileStat;
        string fullPath = path + "/" + entry->d_name;
        stat(fullPath.c_str(), &fileStat);


        if (S_ISDIR(fileStat.st_mode))
            cout << "[DIR]  ";
        else
            cout << "[FILE] ";

        cout << entry->d_name << endl;
    }
    closedir(dir);
}

void changeDirectory(const string &newDir) {
    if (chdir(newDir.c_str()) == 0) {
        cout << "Changed into directory: " << newDir << endl;
    } else {
        perror("Error changing directory");
    }
}


void createFile(const string &filename) {
    ofstream file(filename);
    if (file) {
        cout << "File created: " << filename << endl;
        file.close();
    } else {
        cerr << "Error creating file!" << endl;
    }
}

void deleteFile(const string &filename) {
    if (remove(filename.c_str()) == 0)
        cout << "File deleted: " << filename << endl;
    else
        perror("Error deleting file");
}

void renameFile(const string &oldName, const string &newName) {
    if (rename(oldName.c_str(), newName.c_str()) == 0)
        cout << "File renamed/moved successfully!" << endl;
    else
        perror("Error renaming/moving file");
}

void copyFile(const string &source, const string &destination) {
    ifstream src(source, ios::binary);
    ofstream dest(destination, ios::binary);

    if (!src || !dest) {
        cerr << "Error opening file(s) for copy!" << endl;
        return;
    }

    dest << src.rdbuf();
    cout << "File copied successfully!" << endl;
}

void searchFile(const string &dirPath, const string &target) {
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
        return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        string fullPath = dirPath + "/" + entry->d_name;

        struct stat fileStat;
        if (stat(fullPath.c_str(), &fileStat) == -1)
            continue;

        if (S_ISDIR(fileStat.st_mode)) {
            searchFile(fullPath, target);
        } else if (target == entry->d_name) {
            cout << "Found: " << fullPath << endl;
        }
    }
    closedir(dir);
}

void managePermissions(const string &filename) {
    struct stat fileStat;
    if (stat(filename.c_str(), &fileStat) != 0) {
        perror("Error retrieving file info");
        return;
    }

    cout << "\nCurrent Permissions for " << filename << ": ";
    cout << ((fileStat.st_mode & S_IRUSR) ? "r" : "-")
         << ((fileStat.st_mode & S_IWUSR) ? "w" : "-")
         << ((fileStat.st_mode & S_IXUSR) ? "x" : "-")
         << ((fileStat.st_mode & S_IRGRP) ? "r" : "-")
         << ((fileStat.st_mode & S_IWGRP) ? "w" : "-")
         << ((fileStat.st_mode & S_IXGRP) ? "x" : "-")
         << ((fileStat.st_mode & S_IROTH) ? "r" : "-")
         << ((fileStat.st_mode & S_IWOTH) ? "w" : "-")
         << ((fileStat.st_mode & S_IXOTH) ? "x" : "-") << endl;

    char choice;
    cout << "\nDo you want to change permissions? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        string perms;
        cout << "Enter new permissions in octal (e.g., 755): ";
        cin >> perms;
        string command = "chmod " + perms + " " + filename;
        system(command.c_str());
        cout << "Permissions updated!" << endl;
    }
}

int main() {
    int choice;
    string name, newName;

    while (true) {
        cout << "\n=============================" << endl;
        cout << "      FILE EXPLORER MENU     " << endl;
        cout << "=============================" << endl;
        cout << "1. List Files" << endl;
        cout << "2. Change Directory" << endl;
        cout << "3. Create File" << endl;
        cout << "4. Delete File" << endl;
        cout << "5. Rename/Move File" << endl;
        cout << "6. Copy File" << endl;
        cout << "7. Search File" << endl;
        cout << "8. Manage Permissions" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number (1-9)." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                listFiles(".");
                break;
            case 2:
                cout << "Enter directory path: ";
                cin >> name;
                changeDirectory(name);
                break;
            case 3:
                cout << "Enter file name: ";
                cin >> name;
                createFile(name);
                break;
            case 4:
                cout << "Enter file name to delete: ";
                cin >> name;
                deleteFile(name);
                break;
            case 5:
                cout << "Enter current file name: ";
                cin >> name;
                cout << "Enter new file name/path: ";
                cin >> newName;
                renameFile(name, newName);
                break;
            case 6:
                cout << "Enter source file: ";
                cin >> name;
                cout << "Enter destination file: ";
                cin >> newName;
                copyFile(name, newName);
                break;
            case 7:
                cout << "Enter file name to search: ";
                cin >> name;
                searchFile(".", name);
                break;
            case 8:
                cout << "Enter file name: ";
                cin >> name;
                managePermissions(name);
                break;
            case 9:
                cout << "Exiting File Explorer..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Enter a number between 1–9." << endl;
        }
    }
}
