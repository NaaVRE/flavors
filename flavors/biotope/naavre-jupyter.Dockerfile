ARG NAAVRE_VERSION
FROM qcdis/n-a-a-vre:${NAAVRE_VERSION}

ARG CONDA_ENV_FILE

COPY ${CONDA_ENV_FILE} environment.yaml
RUN mamba env create -f environment.yaml

RUN /opt/conda/envs/biotope/bin/R -e "devtools::install_github('trias-project/trias@5d0f27f76567c0d11021a3055c32ec521622ca36')"


USER root

# OTB from the ZonalFilter dockerfile
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y python3-pip python3.8-dev software-properties-common wget && \
    add-apt-repository -y ppa:ubuntugis/ppa && \
    apt-get update && \
    apt-get install -y gdal-bin libgdal-dev cmake libglu1-mesa-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /usr/local/otb

RUN wget https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-7.3.0-Linux64.run &&  \
    chmod +x OTB-7.3.0-Linux64.run && \
    ./OTB-7.3.0-Linux64.run --target $PWD && \
    rm ./OTB-7.3.0-Linux64.run && \
    . ./otbenv.profile

WORKDIR /usr/local/lw_apps/
ADD ./flavors/biotope/OTB .
RUN  LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu/ cmake -D CMAKE_INSTALL_PREFIX=/usr/local/otb && make install

USER $NB_USER
WORKDIR /home/$NB_USER
