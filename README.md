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
✅ Data loaded successfully from: data.sql
✅ File service started correctly 🚀
✅ Encrypt service started correctly 🚀
🎉 All Libraries Working Successfully.

Welcome to:
    █▀▀ █▀▀ █▀▀ █ █ █▀▄ █▀▀   █▀▀ ▀█▀ █   █▀▀
    ▀▀█ █▀▀ █   █ █ █▀▄ █▀▀   █▀▀  █  █   █▀▀
    ▀▀▀ ▀▀▀ ▀▀▀ ▀▀▀ ▀ ▀ ▀▀▀   ▀   ▀▀▀ ▀▀▀ ▀▀▀
Created by:
    Name: Christophe Nicolas Bourgeonnier, Diego Camilo Pinto Sarmiento, Mayerli Almario Arevalo
    Student ID: A00173434 , A00151853, A00143310


ℹ️  [INFO] Select an option:
   1) Login
   2) Quit

```

## 🚀 Release Creation

### 🍏 macOS 

1️⃣ **Move to root folder**
```bash
cd securefile
```

2️⃣ **Create the release folder**
```bash
mkdir -p mac_build-release && cd mac_build-release
```

3️⃣ **Run cmake**
```bash
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=$(pwd)/package \
      ..
```

4️⃣ **Build the project**
```bash
cmake --build . --config Release -- -j4
```

5️⃣ **Create the package**
```bash
cmake --install . --config Release
```

This will generate an optimized securefile executable in mac_build-release/package/bin/ and copy data.sql there as well.

---

### 🪟 Windows (with vcpkg)

1️⃣ **Move to root folder**
```powershell
cd securefile
```

2️⃣ **Create the release folder**
```powershell
mkdir win-build-release; win-build-release
```

3️⃣ **Run cmake**
```powershell
cmake -A x64 `
      -DCMAKE_BUILD_TYPE=Release `
      -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake `
      -DCMAKE_INSTALL_PREFIX=$(pwd)\package `
      ..
```

4️⃣ **Build the project**
```powershell
cmake --build . --config Release --target INSTALL
```


