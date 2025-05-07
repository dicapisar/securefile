CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    student_id TEXT UNIQUE NOT NULL,
    name TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    is_admin INTEGER DEFAULT 0,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS encrypted_files (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    owner_id INTEGER NOT NULL,
    file_name TEXT NOT NULL,
    file_path TEXT NOT NULL,
    last_modified DATETIME DEFAULT CURRENT_TIMESTAMP,
    password TEXT NOT NULL,
    FOREIGN KEY (owner_id) REFERENCES users(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS shared_files (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    encrypted_file_id INTEGER NOT NULL,
    shared_user_id INTEGER NOT NULL,
    date DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (encrypted_file_id) REFERENCES encrypted_files(id) ON DELETE CASCADE,
    FOREIGN KEY (shared_user_id) REFERENCES users(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS metadata_files (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    encrypted_file_id INTEGER NOT NULL,
    last_update date DATETIME NOT NULL,
    FOREIGN KEY (encrypted_file_id) REFERENCES encrypted_files(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS reports (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    encrypted_file_id INTEGER NOT NULL,
    encrypted_file_name TEXT NOT NULL,
    user_id INTEGER NOT NULL,
    user_name TEXT NOT NULL,
    student_id TEXT NOT NULL,
    action TEXT NOT NULL,
    action_date date DATETIME DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO users (student_id, name, email, password, is_admin) VALUES ("A", "ADMIN", "admin@torrens.com", "100000$46d559a82b64a95621676af977bebe2b$da67dd130d230dec3a29c7a120a3225639d139f0967aa36391bea31f6d104a58", 1);
INSERT INTO users (student_id, name, email, password, is_admin) VALUES ("A0001625", "Diego", "diego@torrens.com", "100000$46d559a82b64a95621676af977bebe2b$da67dd130d230dec3a29c7a120a3225639d139f0967aa36391bea31f6d104a58", 0);