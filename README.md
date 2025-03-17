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

## 🛠️ **Installation on macOS**
Follow these steps to install the required dependencies and run the application.

### 1️⃣ **Install Homebrew (if not installed)**
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2️⃣ **Install Required Libraries**

```bash
brew install openssl sqlite boost
```

### 3️⃣ **Clone the Repository**

```bash
git clone git@github.com:dicapisar/securefile.git
cd securefile
```

### 4️⃣ **Compile and Run the Application**

#### 1. Create the build directory:
```bash
mkdir -p build && cd build
```

#### 2. Run CMake:
```bash
cmake ..
```

#### 3. Compile the application:
```bash
cmake --build .
```

#### 4.	Run the program:
```bash
./securefile
```