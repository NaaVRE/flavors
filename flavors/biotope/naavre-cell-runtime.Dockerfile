FROM ubuntu:22.04 as build

# OTB from the ZonalFilter dockerfile
RUN apt-get update && \
    apt-get install -y python3-pip python3.8-dev software-properties-common wget cmake libglu1-mesa-dev && \
    add-apt-repository -y ppa:ubuntugis/ppa && \
    apt-get update && \
    apt-get install -y gdal-bin libgdal-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /usr/local/otb

RUN wget https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-7.3.0-Linux64.run && \
    chmod +x OTB-7.3.0-Linux64.run && \
    ./OTB-7.3.0-Linux64.run --target $PWD && \
    rm ./OTB-7.3.0-Linux64.run && \
    . ./otbenv.profile

WORKDIR /usr/local/lw_apps/
ADD ./flavors/biotope/OTB .
RUN  LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu/ cmake -D CMAKE_INSTALL_PREFIX=/usr/local/otb && make install
