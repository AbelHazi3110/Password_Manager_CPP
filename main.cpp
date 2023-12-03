#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

// Struct to represent a password entry
struct PasswordEntry {
    std::string name;
    std::string password;
    std::string category;
    std::string website;
    std::string login;
};

// Class for managing the password manager application
class PasswordManager {
private:
    std::string sourceFile;
    std::string mainPassword;

public:
    PasswordManager(std::string  file, std::string  password)
            : sourceFile(std::move(file)), mainPassword(std::move(password)) {}

    void launch() {
        // Load encrypted password data from the file
        std::vector<PasswordEntry> passwords = loadPasswords();
        if (passwords.empty()) {
            std::cout << "No passwords found in the file." << std::endl;
        }

        // Display the menu and handle user commands
        int choice;
        do {
            displayMenu();
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    searchPasswords(passwords);
                    break;
                case 2:
                    sortPasswords(passwords);
                    break;
                case 3:
                    addPassword(passwords);
                    break;
                case 4:
                    editPassword(passwords);
                    break;
                case 5:
                    deletePasswords(passwords);
                    break;
                case 6:
                    addCategory(passwords);
                    break;
                case 7:
                    deleteCategory(passwords);
                    break;
                case 0:
                    std::cout << "Exiting the program." << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
            }

            // Save the modified passwords back to the file
            savePasswords(passwords);

        } while (choice != 0);
    }

private:
    static void displayMenu() {
        std::cout << "========== Password Manager By Abel Hazi ==========" << std::endl;
        std::cout << "1. Search passwords" << std::endl;
        std::cout << "2. Sort passwords" << std::endl;
        std::cout << "3. Add password" << std::endl;
        std::cout << "4. Edit password" << std::endl;
        std::cout << "5. Delete password(s)" << std::endl;
        std::cout << "6. Add category" << std::endl;
        std::cout << "7. Delete category" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "====================================================" << std::endl;
    }

    std::vector<PasswordEntry> loadPasswords() {
        std::ifstream file(sourceFile);
        std::vector<PasswordEntry> passwords;

        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                PasswordEntry entry;
                entry.name = decrypt(line);
                std::getline(file, line);
                entry.password = decrypt(line);
                std::getline(file, line);
                entry.category = decrypt(line);
                std::getline(file, line);
                entry.website = decrypt(line);
                std::getline(file, line);
                entry.login = decrypt(line);

                passwords.push_back(entry);
            }
            file.close();
        }

        return passwords;
    }

    void savePasswords(const std::vector<PasswordEntry>& passwords) {
        std::ofstream file(sourceFile);

        if (file.is_open()) {
            for (const auto& entry : passwords) {
                file << encrypt(entry.name) << "\n";
                file << encrypt(entry.password) << "\n";
                file << encrypt(entry.category) << "\n";
                file << encrypt(entry.website) << "\n";
                file << encrypt(entry.login) << "\n";
            }
            file.close();
        }
    }

    static std::string encrypt(const std::string& data) {
        std::string encryptedData = data;
        // Example encryption logic: Caesar cipher with a shift of 3
        for (char& c : encryptedData) {
            if (std::isalpha(c)) {
                c = std::isupper(c) ? 'A' + (c - 'A' + 3) % 26 : 'a' + (c - 'a' + 3) % 26;
            }
        }
        return encryptedData;
    }

    static std::string decrypt(const std::string& data) {
        std::string decryptedData = data;
        // Example decryption logic: Caesar cipher with a shift of 3
        for (char& c : decryptedData) {
            if (std::isalpha(c)) {
                c = std::isupper(c) ? 'A' + (c - 'A' + 23) % 26 : 'a' + (c - 'a' + 23) % 26;
            }
        }
        return decryptedData;
    }


    void searchPasswords(const std::vector<PasswordEntry>& passwords) {
        std::string searchTerm;
        std::cout << "Enter the search term: ";
        std::cin.ignore();
        std::getline(std::cin, searchTerm);

        std::vector<PasswordEntry> results;
        for (const auto& entry : passwords) {
            if (entry.name.find(searchTerm) != std::string::npos ||
                entry.category.find(searchTerm) != std::string::npos ||
                entry.website.find(searchTerm) != std::string::npos ||
                entry.login.find(searchTerm) != std::string::npos) {
                results.push_back(entry);
            }
        }

        if (results.empty()) {
            std::cout << "No passwords found matching the search term." << std::endl;
        } else {
            std::cout << "Search Results:" << std::endl;
            displayPasswords(results);
        }
    }

    void sortPasswords(std::vector<PasswordEntry>& passwords) {
        int sortOption;
        std::cout << "Select sort option:" << std::endl;
        std::cout << "1. Sort by name" << std::endl;
        std::cout << "2. Sort by category" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> sortOption;

        if (sortOption == 1) {
            std::sort(passwords.begin(), passwords.end(),
                      [](const PasswordEntry &a, const PasswordEntry &b) {
                          return a.name < b.name;
                      });
        } else if (sortOption == 2) {
            std::sort(passwords.begin(), passwords.end(),
                      [](const PasswordEntry &a, const PasswordEntry &b) {
                          return a.category < b.category;
                      });
        } else {
            std::cout << "Invalid sort option." << std::endl;
            return;
        }

        std::cout << "Sorted Passwords:" << std::endl;
        displayPasswords(passwords);
    }

    static void addPassword(std::vector<PasswordEntry>& passwords) {
        PasswordEntry entry;
        std::cout << "Enter the name: ";
        std::cin.ignore();
        std::getline(std::cin, entry.name);
        std::cout << "Enter the password: ";
        std::getline(std::cin, entry.password);
        std::cout << "Enter the category: ";
        std::getline(std::cin, entry.category);
        std::cout << "Enter the website (optional): ";
        std::getline(std::cin, entry.website);
        std::cout << "Enter the login (optional): ";
        std::getline(std::cin, entry.login);

        for (const auto &existingEntry: passwords) {
            if (existingEntry.name == entry.name && existingEntry.category == entry.category &&
                existingEntry.password == entry.password) {
                std::cout << "Password already exists." << std::endl;
                return;
            }
        }

        passwords.push_back(entry);
        std::cout << "Password added successfully." << std::endl;
    }

    static void editPassword(std::vector<PasswordEntry>& passwords) {
        std::string searchTerm;
        std::cout << "Enter the name of the password to edit: ";
        std::cin.ignore();
        std::getline(std::cin, searchTerm);

        for (auto& entry : passwords) {
            if (entry.name == searchTerm) {
                std::cout << "Enter the new password: ";
                std::getline(std::cin, entry.password);
                std::cout << "Password updated successfully." << std::endl;
                return;
            }
        }

        std::cout << "No password found with the given name." << std::endl;
    }

    void deletePasswords(std::vector<PasswordEntry>& passwords) {
        std::string searchTerm;
        std::cout << "Enter the name of the password(s) to delete: ";
        std::cin.ignore();
        std::getline(std::cin, searchTerm);

        std::vector<PasswordEntry> deletedPasswords;
        for (auto it = passwords.begin(); it != passwords.end();) {
            if (it->name == searchTerm) {
                deletedPasswords.push_back(*it);
                it = passwords.erase(it);
            } else {
                ++it;
            }
        }

        if (deletedPasswords.empty()) {
            std::cout << "No password found with the given name." << std::endl;
        } else {
            std::cout << "Deleted Password(s):" << std::endl;
            displayPasswords(deletedPasswords);
        }
    }

    static void addCategory(std::vector<PasswordEntry>& passwords) {
        std::string category;
        std::cout << "Enter the name of the category: ";
        std::cin.ignore();
        std::getline(std::cin, category);

        // Check if the category already exists
        for (const auto& entry : passwords) {
            if (entry.category == category) {
                std::cout << "Category already exists." << std::endl;
                return;
            }
        }

        PasswordEntry entry;
        entry.category = category;
        passwords.push_back(entry);
        std::cout << "Category added successfully." << std::endl;
    }

    void deleteCategory(std::vector<PasswordEntry>& passwords) {
        std::string category;
        std::cout << "Enter the name of the category to delete: ";
        std::cin.ignore();
        std::getline(std::cin, category);

        std::vector<PasswordEntry> deletedPasswords;
        for (auto it = passwords.begin(); it != passwords.end();) {
            if (it->category == category) {
                deletedPasswords.push_back(*it);
                it = passwords.erase(it);
            } else {
                ++it;
            }
        }

        if (deletedPasswords.empty()) {
            std::cout << "No passwords found in the given category." << std::endl;
        } else {
            std::cout << "Deleted Passwords in the Category:" << std::endl;
            displayPasswords(deletedPasswords);
        }
    }

    static void displayPasswords(const std::vector<PasswordEntry>& passwords) {
        for (const auto& entry : passwords) {
            std::cout << "Name: " << entry.name << std::endl;
            std::cout << "Password: " << entry.password << std::endl;
            std::cout << "Category: " << entry.category << std::endl;
            std::cout << "Website: " << entry.website << std::endl;
            std::cout << "Login: " << entry.login << std::endl;
            std::cout << "==============================" << std::endl;
        }
    }
};

int main() {
    std::string sourceFile = "passwords.txt";
    std::string mainPassword = "Abel332211";

    PasswordManager manager(sourceFile, mainPassword);
    manager.launch();

    return 0;
}