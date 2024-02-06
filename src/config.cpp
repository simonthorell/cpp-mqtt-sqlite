//==============================================================================
// Configuration Class
//==============================================================================
#include "config.h"
#include <fstream>
#include <iostream>
#include <cstring> // For string manipulation
#include <nlohmann/json.hpp>
#include <string>

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
        // std::cout << "Loaded configuration data: " << configData.dump() << std::endl;

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
    dbUser = configData["database"]["user"].get<std::string>();
    dbPassword = configData["database"]["password"].get<std::string>();
    mqttBrokerURI = configData["mqtt"]["broker_uri"].get<std::string>();
    mqttBrokerPort = configData["mqtt"]["broker_port"].get<int>();
    mqttClientId = configData["mqtt"]["client_id"].get<std::string>();
    mqttTopic = configData["mqtt"]["topic"].get<std::string>();
    mqttUsername = configData["mqtt"]["username"].get<std::string>();
    mqttPassword = configData["mqtt"]["password"].get<std::string>();
    aesKey = configData["aes"]["key"].get<std::string>();

    //Print the configuration
    // std::cout << "Database User: " << dbUser << std::endl;
    // std::cout << "Database Password: " << dbPassword << std::endl;
    // std::cout << "MQTT Broker URI: " << mqttBrokerURI << std::endl;
    // std::cout << "MQTT Broker Port: " << mqttBrokerPort << std::endl;
    // std::cout << "MQTT Client ID: " << mqttClientId << std::endl;
    // std::cout << "MQTT Topic: " << mqttTopic << std::endl;
    // std::cout << "MQTT Username: " << mqttUsername << std::endl;
    // std::cout << "MQTT Password: " << mqttPassword << std::endl;
    // std::cout << "AES Key: " << aesKey << std::endl;
}

//==============================================================================
// Public Getters
//==============================================================================
std::string Config::getDbUser() const { return dbUser; }
std::string Config::getDbPassword() const { return dbPassword; }
const char* Config::getMqttBrokerURI() const { return mqttBrokerURI.c_str(); }
int         Config::getMqttBrokerPort() const { return mqttBrokerPort; }
const char* Config::getMqttClientId() const { return mqttClientId.c_str(); }
const char* Config::getMqttTopic() const { return mqttTopic.c_str(); }
const char* Config::getMqttUsername() const { return mqttUsername.c_str(); }
const char* Config::getMqttPassword() const { return mqttPassword.c_str(); }
std::string Config::getAesKey() const { return aesKey; }
