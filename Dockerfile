# Use an official C++ runtime as a parent image
FROM gcc:latest

# Set the working directory in the container
WORKDIR /usr/src/app

# Copy the current directory contents into the container at /usr/src/app
COPY . .

# Install any needed packages specified in CMakeLists.txt
RUN apt-get update && \
    apt-get install -y cmake libssl-dev

# Build the project
RUN cmake . && \
    make

# Run the application when the container launches
CMD ["./cpp-mqtt-sqlite"]