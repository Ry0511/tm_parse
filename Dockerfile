FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386

RUN apt-get update && apt-get install -y --no-install-recommends \
    software-properties-common \
    wget \
    ca-certificates \
    gnupg \
    apt-transport-https \
    build-essential \
    ninja-build \
    gcc-multilib \
    g++-multilib \
    libc6:i386 libc6-dev:i386 \
    libstdc++6:i386 \
    curl

RUN wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor -o /usr/share/keyrings/llvm.gpg && \
    echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main" > /etc/apt/sources.list.d/llvm.list

RUN apt-get update && apt-get install -y --no-install-recommends \
    clang-18 \
    lld-18 \
    libc++-18-dev \
    libc++abi-18-dev \
    libclang-18-dev \
    llvm-18 \
    llvm-18-dev \
    llvm-18-runtime

RUN cd /tmp && \
    wget https://github.com/Kitware/CMake/releases/download/v3.23.5/cmake-3.23.5-linux-x86_64.sh && \
    chmod +x cmake-3.23.5-linux-x86_64.sh && \
    ./cmake-3.23.5-linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.23.5-linux-x86_64.sh

RUN apt-get clean && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-18 100 && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100

RUN gcc --version && g++ --version && clang --version && clang++ --version && cmake --version

RUN mkdir git_repos

WORKDIR /git_repos

CMD [ "bash" ]
