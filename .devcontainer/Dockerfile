FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.28.2"
ARG BOOST_VERSION=1.84.0

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

#==============================================================================
# Install Dependencies
#==============================================================================

RUN apt-get update && apt-get install -y \
    git \
    ninja-build \
    openssl \
    libssl-dev \
    sqlite3 \
    libmosquitto-dev \
    && rm -rf /var/lib/apt/lists/*

# Clone and build SQLiteCpp (not available in vcpkg)
RUN git clone https://github.com/SRombauts/SQLiteCpp.git \
    && cd SQLiteCpp \
    && cmake -Bbuild -H. -DSQLITECPP_BUILD_TESTS=FALSE -DSQLITECPP_BUILD_EXAMPLES=FALSE \
    && cmake --build build/ --target install -j$(nproc)

# Install Boost
# https://www.boost.org/doc/libs/1_80_0/more/getting_started/unix-variants.html
RUN cd /tmp && \
    BOOST_VERSION_MOD=$(echo $BOOST_VERSION | tr . _) && \
    wget https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    tar --bzip2 -xf boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    cd boost_${BOOST_VERSION_MOD} && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    rm -rf /tmp/*