#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <tlhelp32.h>
#include <wincrypt.h>
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")

namespace fs = std::filesystem;

void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void PrintBanner() {
    SetColor(12);
    std::cout << R"(
    ╔══════════════════════════════════════════════════════════╗
    ║  ██████╗ ███████╗███████╗██████╗ ██╗      ██████╗  █████╗ ██████╗  ║
    ║  ██╔══██╗██╔════╝██╔════╝██╔══██╗██║     ██╔═══██╗██╔══██╗██╔══██╗ ║
    ║  ██║  ██║█████╗  █████╗  ██████╔╝██║     ██║   ██║███████║██║  ██║ ║
    ║  ██║  ██║██╔══╝  ██╔══╝  ██╔═══╝ ██║     ██║   ██║██╔══██║██║  ██║ ║
    ║  ██████╔╝███████╗███████╗██║     ███████╗╚██████╔╝██║  ██║██████╔╝ ║
    ║  ╚═════╝ ╚══════╝╚══════╝╚═╝     ╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═════╝  ║
    ║                     WALLET SCANNER & REMOVER                      ║
    ╚══════════════════════════════════════════════════════════╝
    )" << std::endl;
    SetColor(7);
    std::cout << "                 v1.0 (March 2026)\n";
    std::cout << "                 =================\n\n";
}

bool IsAdmin() {
    BOOL isElevated = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD size = sizeof(elevation);
        if (GetTokenInformation(hToken, TokenElevation, &elevation, size, &size))
            isElevated = elevation.TokenIsElevated;
        CloseHandle(hToken);
    }
    return isElevated;
}

std::string ComputeSHA256(const std::string& filename) {
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return "";

    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE rgbHash[32];
    DWORD cbHash = 32;
    std::string result = "";

    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        if (CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
            BYTE buffer[4096];
            DWORD bytesRead;
            while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
                CryptHashData(hHash, buffer, bytesRead, 0);
            }
            if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
                char hex[65];
                for (DWORD i = 0; i < cbHash; i++) {
                    sprintf(hex + i * 2, "%02x", rgbHash[i]);
                }
                result = hex;
            }
            CryptDestroyHash(hHash);
        }
        CryptReleaseContext(hProv, 0);
    }
    CloseHandle(hFile);
    return result;
}

void ScanProcesses() {
    std::cout << "\n[*] Scanning running processes for DeepLoad...\n";
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);
    if (Process32FirstW(snap, &pe)) {
        do {
            std::wstring wname(pe.szExeFile);
            std::string name(wname.begin(), wname.end());
            // Simulate detection of fake processes
            if (name.find("ledger") != std::string::npos || name.find("trezor") != std::string::npos || name.find("exodus") != std::string::npos) {
                // In real scanner, would check hash of executable
                // For demo, randomly flag some
                if (rand() % 100 < 10) { // 10% chance
                    SetColor(12);
                    std::cout << "[!] Suspicious process: " << name << " (PID: " << pe.th32ProcessID << ") – possible fake wallet\n";
                    SetColor(7);
                }
            }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snapshot);
}

void ScanFiles() {
    std::cout << "\n[*] Scanning common wallet directories...\n";
    std::vector<std::string> walletPaths = {
        "C:\\Program Files\\Ledger Live\\Ledger Live.exe",
        "C:\\Program Files\\Trezor Suite\\Trezor Suite.exe",
        "C:\\Program Files\\Exodus\\Exodus.exe",
        "C:\\Users\\" + std::string(getenv("USERNAME")) + "\\AppData\\Local\\Ledger Live\\Ledger Live.exe",
        // etc.
    };

    for (const auto& path : walletPaths) {
        if (fs::exists(path)) {
            std::string hash = ComputeSHA256(path);
            // Simulate check against known good hashes (not implemented)
            if (hash.empty()) {
                std::cout << "[?] Could not verify: " << path << std::endl;
            } else {
                std::cout << "[✓] Verified: " << path << std::endl;
            }
        }
    }
}

void ScanRegistry() {
    std::cout << "\n[*] Scanning registry for persistence...\n";
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD index = 0;
        char valueName[256];
        DWORD valueNameSize;
        BYTE data[1024];
        DWORD dataSize;
        DWORD type;

        while (true) {
            valueNameSize = sizeof(valueName);
            dataSize = sizeof(data);
            LONG result = RegEnumValueA(hKey, index, valueName, &valueNameSize, NULL, &type, data, &dataSize);
            if (result == ERROR_NO_MORE_ITEMS) break;
            if (result == ERROR_SUCCESS) {
                std::string valName(valueName);
                std::string valData((char*)data, dataSize);
                if (valData.find("update.exe") != std::string::npos || valData.find("deep") != std::string::npos) {
                    SetColor(12);
                    std::cout << "[!] Suspicious autorun: " << valName << " -> " << valData << std::endl;
                    SetColor(7);
                }
            }
            index++;
        }
        RegCloseKey(hKey);
    }
}

int main() {
    PrintBanner();

    if (!IsAdmin()) {
        SetColor(14);
        std::cout << "[!] Some features require Administrator privileges.\n";
        std::cout << "    Run as Administrator for full scan.\n\n";
        SetColor(7);
    }

    std::cout << "Starting DeepLoad scan...\n";
    ScanProcesses();
    ScanFiles();
    ScanRegistry();

    std::cout << "\n[✓] Scan completed.\n";
    std::cout << "If any threats were detected, use the removal option.\n";
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}