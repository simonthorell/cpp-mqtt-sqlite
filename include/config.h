#ifndef CONFIG_H
#define CONFIG_H

#include <nlohmann/json.hpp>
#include <string>

class Config {
private:
    // Configuration variables
    std::string dbUser;
    std::string dbPassword;
    std::string mqttBrokerURI;
    std::string mqttBrokerPort;
    std::string mqttTopic;
    std::string mqttUsername;
    std::string mqttClientId;
    std::string mqttPassword;
    std::string aesKey;
    
    // Helper function to load configuration
    void loadConfig(const std::string& defaultConfigPath, const std::string& userConfigPath);

public:
    // Constructor
    Config(const std::string& defaultConfigPath, const std::string& userConfigPath);

    // Accessor methods
    std::string getDbUser() const;
    std::string getDbPassword() const;
    const char* getMqttBrokerURI() const;
    int         getMqttBrokerPort() const;
    const char* getMqttClientId() const;
    const char* getMqttTopic() const;
    const char* getMqttUsername() const;
    const char* getMqttPassword() const;
    std::string getAesKey() const;
};

#endif // CONFIG_H
