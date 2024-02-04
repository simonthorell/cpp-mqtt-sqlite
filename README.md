# C++ Backend for MQTT-TLS with SQLite Storage

## Overview
**For Use with the ESP32 MQTT Credential Publisher:**
[ESP32 MQTT Credential Publisher](https://github.com/simonthorell/esp32-lvgl-hid).

This C++ backend application is designed to securely handle user credentials through a series of robust cryptographic operations. The system subscribes to an MQTT broker using TLS connections to ensure secure message transmission. Published messages, containing user credentials (email and password), are encrypted with AES256 and need to be decrypted upon receipt. The application then applies further security measures before storing these credentials into a SQLite database. The entire application is containerized using Docker to ensure consistency across different environments and ease of deployment.  

## Features
- **MQTT TLS Subscription**: Securely subscribes to an MQTT broker, ensuring that all data received is encrypted during transit.
- **AES256 Decryption**: Decrypts the received MQTT messages, which contain user credentials, using AES256 encryption standards.
- **Password Salting & Hashing**: Enhances password security by appending a randomly generated salt to the decrypted password and then hashing the result using OpenSSL EVP with SHA256.
- **SQLite Database Storage**: Stores the user's email, hashed password, and the salt in a secure and efficient SQLite database.
- **Dockerized Application**: Encapsulates the application and its environment in a Docker container for easy deployment and scalability.

## Prerequisites
Before you begin, ensure you have the following installed on your machine:
- **Docker**: A containerization platform. [Download Docker](https://docs.docker.com/get-docker/)
- **Visual Studio Code (VSCode)**: A source-code editor. [Download VSCode](https://code.visualstudio.com/download)
- **Dev Containers** - Extension for VSCode: An extension that lets you use a Docker container as a full-featured development environment. [Get the Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)

## Installation & Setup
1. **Clone the repository**: 
```bash
git clone https://github.com/simonthorell/cpp-mqtt-sqlite
```
2. **Navigate to the project directory**:
```bash 
cd cpp-mqtt-sqlite
```
3. **Open the project in VSCode**: 
```bash 
code .
```
- After opening the project, VSCode may automatically prompt you to reopen the project in a dev container. If not, proceed with the next steps.
4. **Reopen in Container**: [alternative]
    - Press `F1` to open the Command Palette.
    - Type `Dev Containers: Rebuild and Reopen in Container`, and select it.
    - VSCode will build the Docker container based on the .devcontainer settings. This might take some time the first time as it needs to download the base image and install all dependencies.
    - After the container is built, you can start developing within the VSCode environment, with all dependencies and extensions already set up as specified in your .devcontainer configuration.
    - Remember, after the initial setup, every time you open the project in VSCode, you can directly start working inside the containerized environment, ensuring consistency across your development team.

## Usage
1. **Start the MQTT broker** with TLS support and ensure it's correctly configured.
2. **Run the Docker container**. It will automatically subscribe to the MQTT topic and wait for incoming messages.
3. **Publish encrypted user credentials** (email and password) to the MQTT broker. Ensure each message includes the user's email, encrypted password, and IV (Initialization Vector) for AES256 decryption.
4. The application will:
- Decrypt the message using AES256.
- Generate a random salt and append it to the decrypted password.
- Hash the salted password using OpenSSL's EVP with SHA256.
- Store the email, hashed password, and salt in the SQLite database.

## Additional Information

### Preconfigured Debian 12 Environment
If you use this repository, a Debian 12 environment is already set up and ready to use. This environment is provided as a Docker container for your convenience.

### Setting Up a Development Container
Below are the basic steps to configure a new **.devcontainer** using the VSCode Extension **Dev Containers**:

1. **Create New Environment**: 
   - Press `F1` to open the Command Palette.
   - Type `Dev Containers: Add Development Container Configuration Files...` and select it.
   - Select `Add configuration to user data folder`.
   - Search for `C++` and select the C++ devcontainer template.
   - Select `debian-12` (or latest version).
   - Select the latest version in the list (currently 3.22.2).
   - In the list **Select Features**, you can just click OK to configure later.
   - Choose a predefined container configuration or your own `Dockerfile`.
   - A new `.devcontainer` folder with `devcontainer.json` and a `Dockerfile` will be created in your project root.

2. **Customize the Configuration**:
   - Modify the `devcontainer.json` and `Dockerfile` as needed to fit your project's requirements.
   - You can specify settings, port forwards, extensions, and more in `devcontainer.json`.
   - Add any necessary system packages, libraries, or tools in the `Dockerfile`.

3. **Reopen in Container**:
   - With the `.devcontainer` folder in place, you can now reopen your project in a container.
   - Press `F1`, type `Dev Containers: Reopen in Container`, and select it.
   - VS Code will build the container (if it's the first time or if there are changes) and then connect to it.

4. **Use the Devcontainer**:
   - After the container is built and VS Code has connected to it, you can start coding with your environment already set up.
   - The terminal in VS Code will be within the container, and any terminal commands you run will execute there.

### Useful Docker Commands
```bash
cd .devcontainer

# Clean cache
docker builder prune -a

# Rebuild Devcontainer
docker build -t cpp-mqtt-sqlite .

# Run Container
docker run -it --rm cpp-mqtt-sqlite

# List running containers
docker ps

# Stop a running container
docker stop [CONTAINER_ID or NAMES]

# Remove a container
docker rm [CONTAINER_ID or NAMES]

# List all images
docker images

# Remove an image
docker rmi [IMAGE_ID]
```

### Continuous Integration and Deployment (CI/CD) Configuration with GitHub Actions
- Details to be added... // ST 2024-02-04

## Security Notes
- Ensure that the MQTT broker is properly secured and only authorized clients can publish or subscribe to topics.
- Regularly update the encryption and hashing libraries to their latest versions to mitigate newly discovered vulnerabilities.
- Secure the SQLite database file and ensure it's not accessible by unauthorized users or applications.

## Contribution
Contributions are welcome. Please fork the repository, make your changes, and submit a pull request.

## License
Distributed under the MIT License. See `LICENSE` for more information.