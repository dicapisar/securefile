# 🔒 SecureFile - A Console-Based Secure File Encryption System

SecureFile is a **standalone, lightweight** console application designed to **encrypt and decrypt files securely, manage users, share files, ensure file integrity, and securely delete encrypted files**. It runs **without requiring additional installations** beyond its dependencies.

---

## 📌 Features

✅ **AES-256 Encryption**: Securely encrypt and decrypt files and folders.\
✅ **User Authentication**: Users must log in before using the application.\
✅ **Secure File Sharing**: Users can share encrypted files with other authorized users.\
✅ **File Integrity Check**: Prevents file tampering using metadata verification.\
✅ **Secure File Deletion**: Users can permanently delete encrypted files.\
✅ **Cross-Platform**: Works on **macOS** (with Homebrew) and **Windows** (using vcpkg).\
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

1️⃣ **Install Visual Studio (if not already installed)**

[Information Here](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false#installvs)

2️⃣ **Install CMake (if not already installed)**

[Information Here](https://cmake.org/download/)

3️⃣ **Install vcpkg (if not already installed)**

```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

4️⃣ **Install Dependencies**

```powershell
.\vcpkg install openssl sqlite3 boost-filesystem
```

5️⃣ **Clone the Repository**

```powershell
git clone https://github.com/dicapisar/securefile.git
cd securefile
```

6️⃣ **Create and Configure the Build Directory**

```powershell
mkdir build
cd build
cmake -A x64 --toolchain ../vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

7️⃣ **Build the Project**

```powershell
cmake --build . --config Debug
```

8️⃣ **Run the Program**

```powershell
.\Debug\securefile.exe
```

---

## 📂 Database

The project uses an in-memory SQLite database that loads from `data/data.sql` and saves to `data_backup.sql` on exit.

---

## ✨ Output Example

```bash
👀 Validating installation of Libraries...
✅ Database started on memory correctly 🚀
✅ Database restored from: data_backup.sql
ID: 1 | Nombre: ADMIN | Email: admin@email.com | Password: something | Admin: Yes | Crated at: 2025-04-28 09:07:33
ID: 2 | Nombre: Test of update   | Email: diego@torrens.com | Password: something | Admin: No | Crated at: 2025-04-28 09:07:33

📂 Encrypted File Found:
ID: 1 | Name: test_one | Path: test_path_one
👤 Owner: ADMIN | Owner Email: admin@email.com

📂 Encrypted File Found:
ID: 2 | Name: test_two | Path: test_path_two
👤 Owner: ADMIN | Owner Email: admin@email.com

📂 Encrypted File Found:
ID: 3 | Name: test_three | Path: test_path_three
👤 Owner: ADMIN | Owner Email: admin@email.com

📂 Shared Encrypted File Found:
ID: 2 | Name: test_two | Path: test_path_two
👤 Owner: Test of update   | Owner Email: diego@torrens.com

📂 Shared Encrypted File Found:
ID: 3 | Name: test_three | Path: test_path_three
👤 Owner: Test of update   | Owner Email: diego@torrens.com
✅ File service started correctly 🚀
✅ Encrypt service started correctly 🚀
❌ Error reading file. 

📁 File Encrypted Successfully at: ./encrypted_files/test.txt.enc
📂 File Decrypted Successfully at: ./encrypted_files/test.txt.enc
ID: 1
ID Encrypted File: 1
ID Shared File: 1
ID Metadata File: 1
🎉 All Libraries Working Successfully.
✅ Encrypt service closed correctly
✅ File service closed correctly
✅ Database saved in: data_backup.sql
🔒 Database closed successfully since destructor method.

```

