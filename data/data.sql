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

INSERT INTO users (student_id, name, email, password, is_admin) VALUES ("A", "ADMIN", "admin@email.com", "something", 1);
INSERT INTO users (student_id, name, email, password, is_admin) VALUES ("A0001625", "Diego", "diego@torrens.com", "something", 0);

INSERT INTO encrypted_files (owner_id, file_name, file_path, password) VALUES (1, "test_one", "test_path_one", "test_password");
INSERT INTO encrypted_files (owner_id, file_name, file_path, password) VALUES (1, "test_two", "test_path_two", "test_password");
INSERT INTO encrypted_files (owner_id, file_name, file_path, password) VALUES (1, "test_three", "test_path_three", "test_password");

INSERT INTO shared_files (encrypted_file_id, shared_user_id) VALUES (2, 2);
INSERT INTO shared_files (encrypted_file_id, shared_user_id) VALUES (3, 2);


SELECT e.id, e.owner_id, e.file_name, e.file_path, e.last_modified, e.password, u.id, u.student_id, u.name, u.email, u.password, u.is_admin, u.created_at
FROM shared_files
JOIN encrypted_files E on E.id = shared_files.encrypted_file_id
JOIN users u on shared_files.shared_user_id = u.id
WHERE shared_user_id = 1;

