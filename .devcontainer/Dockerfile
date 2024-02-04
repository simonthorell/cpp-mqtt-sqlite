FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.28.2"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

#==============================================================================
# Install basic dependencies
#==============================================================================

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>

# Git, OpenSSL, and SSL development libraries are required to build the dependencies
RUN apt-get update && apt-get install -y \
    git \
    openssl \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

#==============================================================================
# Clone and build dependencies
#==============================================================================

# Clone and build Paho MQTT C library
RUN git clone https://github.com/eclipse/paho.mqtt.c.git \
    && cd paho.mqtt.c \
    && cmake -Bbuild -H. -DPAHO_WITH_SSL=ON -DPAHO_ENABLE_TESTING=OFF \
    && cmake --build build/ --target install -j$(nproc)

# Clone and build Paho MQTT C++
RUN git clone https://github.com/eclipse/paho.mqtt.cpp.git \
    && cd paho.mqtt.cpp \
    && cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=FALSE -DPAHO_BUILD_SAMPLES=FALSE \
    && cmake --build build/ --target install -j$(nproc)

# Clone and build SQLiteCpp
RUN git clone https://github.com/SRombauts/SQLiteCpp.git \
    && cd SQLiteCpp \
    && cmake -Bbuild -H. -DSQLITECPP_BUILD_TESTS=FALSE -DSQLITECPP_BUILD_EXAMPLES=FALSE \
    && cmake --build build/ --target install -j$(nproc)

# Note: This library is header-only, so it will be fetched using CMake's FetchContent
# Clone and install JSON for Modern C++ (nlohmann/json)
# RUN git clone https://github.com/nlohmann/json.git \
#     && cd json \
#     && cmake -Bbuild -H. \
#     && cmake --build build/ --target install -j$(nproc)