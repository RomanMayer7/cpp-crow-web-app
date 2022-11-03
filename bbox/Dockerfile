FROM cppbox:latest

WORKDIR /usr/src


# installing the mongoc / mongocxx dependencies
RUN apt-get install -y \
    pkg-config \
    libssl-dev \
    libsasl2-dev \
    libbson-dev \
    git \
    curl

# compile and install mongoc
RUN cd /tmp \
    && curl -OL https://github.com/mongodb/mongo-c-driver/releases/download/1.17.2/mongo-c-driver-1.17.2.tar.gz \
    && tar xzf mongo-c-driver-1.17.2.tar.gz \
    && cd mongo-c-driver-1.17.2 \
    && mkdir cmake-build \
    && cd cmake-build \
    && cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. \
    && make \
    && make install && ldconfig /usr/local/lib


# compile and install mongocxx
RUN cd /tmp \
    && curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.6.1/mongo-cxx-driver-r3.6.1.tar.gz \
    && tar -xzf mongo-cxx-driver-r3.6.1.tar.gz \
    && cd mongo-cxx-driver-r3.6.1/build \
    && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr/local -DBSONCXX_POLY_USE_MNMLSTC=1 .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DLIBMONGOC_DIR=/usr/lib/x86_64-linux-gnu \
    -DLIBBSON_DIR=/usr/lib/x86_64-linux-gnu \
    -DCMAKE_MODULE_PATH=/usr/src/mongo-cxx-driver-r3.6.1/cmake .. \
    && make \
    && make install && ldconfig /usr/local/lib
