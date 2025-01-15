CREATE TABLE IF NOT EXISTS schema_migrations (
                                                 id SERIAL PRIMARY KEY,
                                                 version VARCHAR(255) NOT NULL UNIQUE,
    applied_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
