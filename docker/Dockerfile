# Use a base image based on Debian Buster
FROM debian:buster

# Update the package repository and install necessary libraries and tools
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y build-essential \
                       clang \
                       make \
                       cmake \
                       libc6-dev-i386\
                       libssl-dev \
                       libreadline-dev \
                       libncurses-dev \
                       libffi-dev \
                       libxml2-dev \
                       libxslt-dev \
                       zlib1g-dev \
                       git \
                       curl \
                       vim \
                       file \
                       binutils \
                       bsdmainutils\
                       valgrind 
                    #    gdb \

# Set the working directory
WORKDIR /ft_nm

# Copy the project files to the container
COPY . /ft_nm

# Set the default command to run when the container is started
CMD ["tail", "-f", "/dev/null"]