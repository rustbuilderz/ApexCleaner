#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>
#include <shlobj.h>
#include <cstdlib>

void slowtype(const std::string& text, int delay = 25) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

void loading_dots(const std::string& text = "Loading", int delay = 500, int cycles = 3) {
    for (int i = 0; i < cycles; ++i) {
        std::cout << "\r" << text << "...";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        std::cout << "\r" << std::string(text.length(), ' ') << "\r";
    }
}

bool isAdmin() {
    BOOL isAdmin = FALSE;
    PSID administratorsGroup;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup)) {
        CheckTokenMembership(NULL, administratorsGroup, &isAdmin);
        FreeSid(administratorsGroup);
    }

    return isAdmin;
}

bool registryKeyExists(const std::string& key) {
    std::string command = "reg query \"" + key + "\"";
    return system(command.c_str()) == 0;
}

void cleaner(bool automaticDelete) {
    const std::vector<std::string> directories = {
        "C:\\Program Files (x86)\\Steam\\userdata",
        "C:\\Program Files (x86)\\Steam\\appcache",
        "C:\\Program Files (x86)\\Steam\\config",
        "C:\\Program Files (x86)\\Steam\\logs",
        "C:\\Program Files (x86)\\Steam\\dumps",
        "C:\\Program Files (x86)\\Steam\\temp",
        "C:\\Program Files\\EA Games\\Apex Legends\\file2.log",
        "C:\\Program Files (x86)\\Origin Games\\Apex\\file1.txt",
        "C:\\Program Files (x86)\\Origin Games\\Apex",
        "C:\\Program Files\\EA Games\\Apex Legends",
        "%USERPROFILE%\\AppData\\Local\\Temp\\EA\\",
        "%USERPROFILE%\\AppData\\Roaming\\Respawn\\Apex\\",
        "C:\\Program Files (x86)\\EasyAntiCheat\\",
        "C:\\Program Files (x86)\\Common Files\\EasyAntiCheat\\",
        "%USERPROFILE%\\AppData\\Local\\EasyAntiCheat\\",
        "C:\\Windows\\System32\\EasyAntiCheat.sys",
        "C:\\Windows\\System32\\drivers\\EasyAntiCheat.sys",
        "%USERPROFILE%\\AppData\\Local\\Origin\\",
        "%USERPROFILE%\\AppData\\Roaming\\Origin\\",
        "%USERPROFILE%\\AppData\\Local\\Electronic Arts\\",
        "C:\\Program Files (x86)\\Origin Games\\",
        "%USERPROFILE%\\AppData\\Local\\Temp\\",
        "%USERPROFILE%\\AppData\\Local\\Apex\\",
        "%USERPROFILE%\\AppData\\Local\\Apex\\",
        "C:\\Program Files (x86)\\Origin\\Origin Games\\Apex",
        "%ProgramData%\\Origin",
        "HKEY_CURRENT_USER\\Software\\EA Games\\Apex",
        "%USERPROFILE%\\Saved Games\\Respawn\\Apex",
        "%systemdrive%\\Program Files (x86)\\EasyAntiCheat\\",
        "%Temp%\\",
        "%TEMP%\\",
        "%systemdrive%\\ProgramData\\Origin\\",
        "%systemdrive%\\Program Files (x86)\\Origin\\"
    };

    loading_dots("[+] Loading..");
    system("cls");

    for (const auto& dir_path : directories) {
        try {
            if (std::filesystem::exists(dir_path)) {
                for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
                    try {
                        std::string user_input;
                        if (std::filesystem::is_directory(entry)) {
                            std::cout << "Would you like to delete directory: " << entry.path() << " (y/n)? ";
                            std::cin >> user_input;
                            if (user_input == "y" || user_input == "Y") {
                                std::filesystem::remove_all(entry);
                                std::cout << "[+] Deleted directory " << entry.path() << std::endl;
                            }
                            else {
                                std::cout << "[-] Skipped directory " << entry.path() << std::endl;
                            }
                        }
                        else {
                            std::cout << "Would you like to delete file: " << entry.path() << " (y/n)? ";
                            std::cin >> user_input;
                            if (user_input == "y" || user_input == "Y") {
                                std::filesystem::remove(entry);
                                std::cout << "[+] Deleted file " << entry.path() << std::endl;
                            }
                            else {
                                std::cout << "[-] Skipped file " << entry.path() << std::endl;
                            }
                        }
                    }
                    catch (const std::filesystem::filesystem_error& e) {
                        std::cout << "[!] Permission denied for " << entry.path() << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cout << dir_path << " not found" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    // Ask for confirmation before deleting registry keys
    std::string key1 = "HKEY_CURRENT_USER\\Software\\EA Games\\Apex";
    std::string user_input;
    if (registryKeyExists(key1)) {
        std::cout << "Would you like to delete registry key: " << key1 << " (y/n)? ";
        std::cin >> user_input;
        if (user_input == "y" || user_input == "Y") {
            if (system(("reg delete \"" + key1 + "\" /f").c_str()) == 0) {
                std::cout << "[+] Deleted registry key " << key1 << std::endl;
            }
            else {
                std::cout << "[-] Failed to delete registry key " << key1 << std::endl;
            }
        }
        else {
            std::cout << "[-] Skipped registry key " << key1 << std::endl;
        }
    }
    else {
        std::cout << "[-] Registry key not found: " << key1 << std::endl;
    }

    std::string key2 = "HKEY_CURRENT_USER\\SOFTWARE\\Valve\\Steam\\Users";
    if (registryKeyExists(key2)) {
        std::cout << "Would you like to delete registry key: " << key2 << " (y/n)? ";
        std::cin >> user_input;
        if (user_input == "y" || user_input == "Y") {
            if (system(("reg delete \"" + key2 + "\" /f").c_str()) == 0) {
                std::cout << "[+] Deleted registry key " << key2 << std::endl;
            }
            else {
                std::cout << "[-] Failed to delete registry key " << key2 << std::endl;
            }
        }
        else {
            std::cout << "[-] Skipped registry key " << key2 << std::endl;
        }
    }
    else {
        std::cout << "[-] Registry key not found: " << key2 << std::endl;
    }

    std::string file_path = "C:\\Eos_seed.bin";
    if (std::filesystem::exists(file_path)) {
        try {
            if (std::filesystem::remove(file_path)) {
                std::cout << "[+] Deleted " << file_path << std::endl;
            }
            else {
                std::cout << "[-] Failed to delete " << file_path << std::endl;
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cout << "[-] Error while deleting " << file_path << ": " << e.what() << std::endl;
        }
    }
    else {
        std::cout << "[-] File not found: " << file_path << std::endl;
    }
}

void exit_program() {
    system("cls");
    slowtype("[+] Everything should be cleaned.");
    slowtype("[!] Closing :D");
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

int main() {
    if (!isAdmin()) {
        std::cout << "This script requires administrator privileges. Please run it as an administrator." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return 0;
    }

    slowtype("[!] Apex Legends cleaner by rustbuilderz.\n");

    slowtype("[!] Press Enter to continue..");
    std::cin.get();
    system("cls");

    slowtype("[!] WARNING: please log out of Steam and press Enter again.");
    std::cin.get();

    slowtype("[+] Please wait");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    system("taskkill /im steam.exe /f");
    system("cls");

    int option = 0;
    slowtype("[+] Choose a cleaning option:");
    slowtype("[1] Go through files one by one (ask y/n for every delete)");
    slowtype("[2] Delete all automatically");
    slowtype("[3] End");
    std::cin >> option;
    std::cin.ignore();  // To handle leftover newline character after user input

    if (option == 1) {
        cleaner(false); // Ask for confirmation before deletion
    }
    else if (option == 2) {
        cleaner(true);  // Delete all automatically
    }
    else if (option == 3) {
        exit_program();  // End the program
        return 0;
    }
    else {
        slowtype("[-] Invalid option. Exiting...");
    }

    return 0;
}
