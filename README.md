# C++ Backend for MQTT-TLS with SQLite Storage

## Overview
This C++ backend application is designed to securely handle user credentials through a series of robust cryptographic operations. The system subscribes to an MQTT broker using TLS connections to ensure secure message transmission. Published messages, containing user credentials (email and password), are encrypted with AES256 and need to be decrypted upon receipt. The application then applies further security measures before storing these credentials into a SQLite database. The entire application is containerized using Docker to ensure consistency across different environments and ease of deployment.

## Features
- **MQTT TLS Subscription**: Securely subscribes to an MQTT broker, ensuring that all data received is encrypted during transit.
- **AES256 Decryption**: Decrypts the received MQTT messages, which contain user credentials, using AES256 encryption standards.
- **Password Salting & Hashing**: Enhances password security by appending a randomly generated salt to the decrypted password and then hashing the result using OpenSSL EVP with SHA256.
- **SQLite Database Storage**: Stores the user's email, hashed password, and the salt in a secure and efficient SQLite database.
- **Dockerized Application**: Encapsulates the application and its environment in a Docker container for easy deployment and scalability.

## Prerequisites
- Docker installed on your machine.
- OpenSSL for encryption and decryption operations.
- An MQTT broker with TLS support.
- SQLite3 for database operations.
- A C++ environment with support for standard libraries.

## Installation & Setup
1. **Clone the repository**: `git clone https://github.com/simonthorell/cpp-mqtt-sqlite`
2. **Navigate to the project directory**: `cd cpp-mqtt-sqlite`
3. **Build the Docker image**: `docker build -t cpp-mqtt-sqlite .`
4. **Run the Docker container**: `docker run -d --name cpp-mqtt-sqlite-container cpp-mqtt-sqlite`

## Usage
1. **Start the MQTT broker** with TLS support and ensure it's correctly configured.
2. **Run the Docker container**. It will automatically subscribe to the MQTT topic and wait for incoming messages.
3. **Publish encrypted user credentials** (email and password) to the MQTT broker. Ensure each message includes the user's email, encrypted password, and IV for AES256 decryption.
4. The application will:
- Decrypt the message using AES256.
- Generate a random salt and append it to the decrypted password.
- Hash the salted password using OpenSSL's EVP with SHA256.
- Store the email, hashed password, and salt in the SQLite database.

## Security Notes
- Ensure that the MQTT broker is properly secured and only authorized clients can publish or subscribe to topics.
- Regularly update the encryption and hashing libraries to their latest versions to mitigate newly discovered vulnerabilities.
- Secure the SQLite database file and ensure it's not accessible by unauthorized users or applications.

## Contribution
Contributions are welcome. Please fork the repository, make your changes, and submit a pull request.

## License
Distributed under the MIT License. See `LICENSE` for more information.