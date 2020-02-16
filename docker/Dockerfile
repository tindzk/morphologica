# Based on opencv-alpine by Juliano Petronetto (MIT Licence)

FROM alpine:3.8
ENV LANG=C.UTF-8

RUN apk update && apk upgrade && apk --no-cache add \
  bash \
  build-base \
  clang-dev \
  cmake \
  coreutils \
  freetype-dev \
  git \
  gettext \
  libc-dev \
  libffi-dev \
  libpng-dev \
  linux-headers \
  make \
  musl \
  openblas-dev \
  tiff-dev \
  unzip \
  zlib-dev \
  freeglut-dev \
  && mkdir /opt

RUN cd /opt && \
  wget https://github.com/opencv/opencv/archive/3.2.0.zip && \
  unzip 3.2.0.zip && rm 3.2.0.zip && \
  cd /opt/opencv-3.2.0 && mkdir build && cd build && \
  cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_C_COMPILER=/usr/bin/clang \
    -D CMAKE_CXX_COMPILER=/usr/bin/clang++ \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_C_EXAMPLES=OFF \
    .. && \
  make -j$(nproc) && make install && cd .. && rm -rf build

RUN cd /opt && \
  wget https://downloads.sourceforge.net/project/arma/armadillo-9.850.1.tar.xz && \
  tar xvf armadillo-9.850.1.tar.xz && \
  rm armadillo-9.850.1.tar.xz && \
  cd armadillo-9.850.1 && \
  mkdir build && \
  cd build && \
  cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && \
  make -j$(nproc) && \
  make install

RUN cd /opt && \
  wget https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.6/src/hdf5-1.10.6.tar.gz && \
  tar xvf hdf5-1.10.6.tar.gz && rm hdf5-1.10.6.tar.gz && \
  cd hdf5-1.10.6 && \
  mkdir build && \
  cd build && \
  cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && \
  make -j$(nproc) && \
  make install

RUN cd /opt && \
  git clone https://github.com/open-source-parsers/jsoncpp.git && \
  cd jsoncpp && \
  git checkout 3beb37ea14aec1bdce1a6d542dc464d00f4a6cec && \
  mkdir build && \
  cd build && \
  cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && \
  make -j$(nproc) && \
  make install && \
  cp -rv /usr/local/lib64/* /usr/local/lib/  # /usr/loca/lib/ is hard-coded in morphologica (src/CMakeLists.txt)

RUN cd /opt && \
  git clone https://github.com/ABRG-Models/morphologica.git && \
  cd morphologica && \
  mkdir build && \
  cd build && \
  cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && \
  make -j$(nproc) && \
  #ctest && \
  make install
