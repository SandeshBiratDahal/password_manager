Password Manager (C++ Portable Tool)

A simple, local password manager written in C++ that allows you to generate, store, retrieve, and delete passwords.  
Fully portable — works from any folder, including OneDrive.


Features:

- Generate strong random passwords (letters, numbers, symbols)  
- Save passwords in a local file (data.bin) next to the executable  
- Retrieve passwords and copy them directly to clipboard  
- Delete individual passwords or clear all stored passwords  
- List all stored keys  
- Fully portable — works in any folder (USB, OneDrive, etc.)


Requirements:

- Windows OS  
- C++ compiler (e.g., GCC, MSVC)  
- OneDrive or other folder optional for cloud backup


Usage:

Build:
    g++ password.cpp -o password_manager.exe

Commands:
    --new <key1> <key2> ...      Generate new passwords for the given keys and save them  
    --get <key>                  Copy the password for the given key to clipboard  
    --del <key1> <key2> ...      Delete the password(s) for the given key(s)  
    --clear                       Delete all passwords after confirmation  
    --list                        List all stored keys

Examples:
    # Create passwords for Gmail and GitHub
    password_manager.exe --new Gmail GitHub
    
    # Get password for Gmail
    password_manager.exe --get Gmail

    # Delete password for GitHub
    password_manager.exe --del GitHub

    # List all keys
    password_manager.exe --list

    # Clear all passwords
    password_manager.exe --clear


How it Works:

- Data file: data.bin stored in the same folder as the executable  
- Clipboard: Copies password to clipboard using Windows API  
- Password generation: Random combination of uppercase, lowercase, numbers, and symbols


Notes / Warnings:

- This tool stores passwords locally in plain text. For real-world use, consider encrypting the data file  
- Keep the program folder in OneDrive or other backup locations to prevent data loss  
- Portable — can run from USB or external drives  
- Do not run multiple instances simultaneously — may corrupt the data file


Future Improvements:

- Encrypt stored passwords (AES or Windows CryptoAPI)  
- Cross-platform clipboard support (Linux/macOS)  
- GUI version for easier use
