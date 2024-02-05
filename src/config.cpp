//==============================================================================
// Configuration Class
//==============================================================================
#include "config.h"
#include <fstream>
#include <iostream>

//==============================================================================
// Constructor
//==============================================================================
Config::Config(const std::string& defaultConfigPath, const std::string& userConfigPath) {
    loadConfig(defaultConfigPath, userConfigPath);
}

//==============================================================================
// Private Method: loadConfig
// Description: Check if the user configuration file exists. If it does, load the
//              configuration from the user configuration file. If it doesn't,
//              load the configuration from the default configuration file.
//==============================================================================
void Config::loadConfig(const std::string& defaultConfigPath, const std::string& userConfigPath) {
    nlohmann::json configData;
    std::ifstream userConfigFile(userConfigPath);
    if (userConfigFile.is_open()) {
        userConfigFile >> configData;
        std::cout << "Using configuration from " << userConfigPath << std::endl;
    } else {
        std::ifstream defaultConfigFile(defaultConfigPath);
        if (defaultConfigFile.is_open()) {
            defaultConfigFile >> configData;
            std::cout << "Using default configuration from " << defaultConfigPath << std::endl;
        } else {
            std::cerr << "Could not open the configuration file." << std::endl;
            // Handle this error
        }
    }
    
    // Extract configuration data
    dbUser = configData.value("database.user", "");
    dbPassword = configData.value("database.password", "");
    mqttBrokerURI = configData.value("mqtt.broker_uri", "");
    mqttBrokerPort = configData.value("mqtt.broker_port", "");
    mqttClientId = configData.value("mqtt.client_id", "");
    mqttPassword = configData.value("mqtt.password", "");
    aesKey = configData.value("aes.key", "");
}

//==============================================================================
// Public Getters
//==============================================================================
std::string Config::getDbUser() const { return dbUser; }
std::string Config::getDbPassword() const { return dbPassword; }
std::string Config::getMqttBrokerURI() const { return mqttBrokerURI; }
std::string Config::getMqttBrokerPort() const { return mqttBrokerPort; }
std::string Config::getMqttClientId() const { return mqttClientId; }
std::string Config::getMqttPassword() const { return mqttPassword; }
std::string Config::getAesKey() const { return aesKey; }
