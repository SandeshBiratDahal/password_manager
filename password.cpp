#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <vector>
#include <fstream>
#include <stdio.h>
using namespace std;

string get_file_path() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);
    size_t pos = fullPath.find_last_of("\\/");
    return fullPath.substr(0, pos) + "\\data.bin";
}

string get_temp_path() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);
    size_t pos = fullPath.find_last_of("\\/");
    return fullPath.substr(0, pos) + "\\temp.bin";
}

void setClipboardText(const string& text) {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        memcpy(GlobalLock(hMem), text.c_str(), text.size() + 1);
        GlobalUnlock(hMem);
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
    }
}

string generate_password_secure(int length = 20) {
    string lower = "qwertyuiopasdfghjklzxcvbnm";
    string upper = "QWERTYUIOPASDFGHJKLZXCVBNM";
    string symbols = "!@#$%^&*()_+-=~`<>?,./';:[]{}|";
    string nums = "1234567890";
    string pass = "";
    pass = pass + lower[rand() % lower.size()];
    pass = pass + upper[rand() % upper.size()];
    pass = pass + symbols[rand() % symbols.size()];
    pass = pass + nums[rand() % nums.size()];

    for (int i = 0; i < length - 4; i++) {
        int ch = rand() % 4;

        if (ch == 0) {
            pass = pass + lower[rand() % lower.size()];
        }
        else if (ch == 1) {
            pass = pass + symbols[rand() % symbols.size()];
        }
        else if (ch == 2) {
            pass = pass + nums[rand() % nums.size()];
        }
        else if (ch == 3) {
            pass = pass + upper[rand() % upper.size()];
        }
    }
    return pass;
}

bool save_password_to_file(string key, string password) {
    string line;
    int i = 0;
    fstream fout;
    fout.open(get_file_path(), ios::in);

    while (getline(fout, line)) {
        if (i % 2 == 0 && line == key) {
            cout << "Password for '" << key << "' already exists!" << endl;
            return false;
        }
        i++;
    }
    fout.close();

    fout.open(get_file_path(), ios::app);
    fout << key << endl << password << endl;
    fout.close();
    return true;
}

string get_password_from_file(string key) {
    fstream fout;
    string line;
    int i = 0;
    fout.open(get_file_path(), ios::in);
    while (getline(fout, line)) {
        if (line == key && i % 2 == 0) {
            getline(fout, line);
            return line;
        }
        i++;
    }
    return "";
}

bool delete_password_from_file(string key) {
    fstream fout, fin;
    string line;
    char ch;
    bool deleted = false;
    int i = 0;
    fin.open(get_file_path(), ios::in);
    fout.open(get_temp_path().c_str(), ios::out);

    while(getline(fin, line)) {
        if (line == key && i % 2 == 0) {
            cout << "Confirm delete '" << key << "'? [Y/N]: ";
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                getline(fin, line);
                deleted = true;
            }
            else {
                fout << line << endl;
                getline(fin , line);
                fout << line << endl;
            }
        }
        else {
            fout << line << endl;
        }
        i++;
    }
    
    fout.close();
    fin.close();

    remove(get_file_path().c_str());
    rename(get_temp_path().c_str(), get_file_path().c_str());

    return deleted;
}

int main(int argc, char* argv[]) {
    srand(time(0));
    string arg;
    vector<string> args;
    
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            arg = argv[i];
            //cout << arg << ": " << generate_password_secure() << endl;
            args.push_back(arg);
        }


        if (args[0] == "--new") {
            for (int i = 1; i < args.size(); i++) {
                arg = generate_password_secure();
                if (save_password_to_file(args[i], arg)) {
                    cout << "Created new secure password for '" << args[i] << "'!\n";
                }
            }
        }
        else if (args[0] == "--get") {
            arg = get_password_from_file(args[1]);
            if (arg != "") {
                setClipboardText(arg);
                cout << "Password for '" << args[1] << "' copied to clipboard!" << endl;
            }
            else {
                cout << "Couldn't find the password for '" << args[1] << "'!" << endl;
            }
        }
        else if (args[0] == "--del") {
            for (int i = 1; i < args.size(); i++) {
                if (delete_password_from_file(args[i])) {
                    cout << "Successfully deleted the password for '" << args[i] << "'!" << endl;
                }
                else {
                    cout << "Couldn't delete the password for '" << args[i] << "'!" << endl;
                }
            }
        }
        else if (args[0] == "--clear") {
            char ch;
            cout << "!!WARNING!!\n!!WARNING!!\n!!WARNING!!" << endl << "Pressing 'Y' means all passwords will be deleted and can not be recovered!!" << endl;
            cout << "Do you want to clear all passwords? [Y/N]: ";
            cin >> ch;

            if (ch == 'y' || ch == 'Y') {
                fstream fout;
                fout.open(get_file_path(), ios::out);
                fout.close();
                cout << "Cleared all passwords!!" << endl;
            }
            else{
                cout << "Passwords not cleared!!" << endl;
            }
        }
        else if (args[0] == "--list") {
            fstream fin(get_file_path(), ios::in);
            string line;
            int i = 0;
            while (getline(fin, line)) {
                if (i % 2 == 0) cout << line << endl;
                i++;
            }
        }
        else if (args[0] == "--rewrite") {
            char ch;

            cout << "!!WARNING!!\n!!WARNING!!\n!!WARNING!!" << endl << "Pressing 'Y' means all passwords will be rewritten and old passwords can not be recovered!!" << endl;
            cout << "Do you want to rewrite all passwords? [Y/N]: ";
            cin >> ch;
            int i = 0;
            if (ch == 'y' || ch == 'Y') {
                fstream fin, fout;
                fin.open(get_file_path(), ios::in);
                fout.open(get_temp_path(), ios::out);

                while (getline(fin, arg)) {
                    if (i % 2 == 1) {
                        fout << generate_password_secure() << endl;
                    }
                    else {
                        fout << arg << endl;
                    }
                    i++;
                }
                fin.close();
                fout.close();

                remove(get_file_path().c_str());
                rename(get_temp_path().c_str(), get_file_path().c_str());
                cout << "Rewritten all passwords!!" << endl;
            }
            else {
                cout << "Passwords not rewritten!!" << endl;
            }

        }
    }

    else {
        cout << "A neat little program to manage all your passwords locally :)" << endl;
    }
    
    return 0;
}