# 🔒 SecureFile - A Console-Based Secure File Encryption System

SecureFile is a **standalone, lightweight** console application designed to **encrypt and decrypt files securely, manage users, share files, ensure file integrity, and securely delete encrypted files**. It runs **without requiring additional installations** beyond its dependencies.

---

## 📌 Features
✅ **AES-256 Encryption**: Securely encrypt and decrypt files and folders.  
✅ **User Authentication**: Users must log in before using the application.  
✅ **Secure File Sharing**: Users can share encrypted files with other authorized users.  
✅ **File Integrity Check**: Prevents file tampering using metadata verification.  
✅ **Secure File Deletion**: Users can permanently delete encrypted files.  
✅ **Cross-Platform**: Works on **macOS** (with Homebrew) and **Windows** (using vcpkg).  
✅ **No Advanced Configurations Required**: Simple console-based interface.

---

## 🛠️ Installation

### 🍏 macOS (with Homebrew)

1️⃣ **Install Homebrew (if not installed)**
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

2️⃣ **Install Dependencies**
```bash
brew install openssl sqlite boost
```

3️⃣ **Clone the Repository**
```bash
git clone git@github.com:dicapisar/securefile.git
cd securefile
```

4️⃣ **Build the Project**
```bash
mkdir build && cd build
cmake ..
cmake --build .
./securefile
```

---

### 🪟 Windows (with vcpkg)

1️⃣ **Install vcpkg (if not already installed)**
```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

2️⃣ **Install Dependencies**
```bash
.\vcpkg install openssl sqlite3 boost-filesystem
```

3️⃣ **Clone the Repository**
```bash
git clone https://github.com/dicapisar/securefile.git
cd securefile
```

4️⃣ **Configure the Project with vcpkg**
```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake  # path to vcpkg installed
```

5️⃣ **Build the Project**
```bash
cmake --build .
```

6️⃣ **Run the Program**
```bash
.\Debug\securefile.exe
```

---

## 📂 Database

The project uses an in-memory SQLite database that loads from `data/data.sql` and saves to `data_backup.sql` on exit.

---

## ✨ Output Example

```bash
✅ Database started on memory correctly 🚀
📁 File Encrypted Successfully at: ./encrypted_files/test.txt.enc
📂 File Decrypted Successfully at: ./decrypted_files/test.txt
🎉 All Libraries Working Successfully.
```

---