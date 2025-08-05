FROM adokter/vol2bird:0.5.0

RUN apt-get update && apt-get install --no-install-recommends -y libconfuse-dev \
    libhdf5-dev gcc g++ wget unzip make cmake zlib1g-dev python-dev python-numpy libproj-dev flex-old file \
    && apt-get install -y git git-lfs && apt-get install -y libgsl-dev && apt-get install -y libbz2-dev bison byacc


# Install miniconda base utilities
RUN apt-get update && \
    apt-get install -y wget && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install miniconda
ENV CONDA_DIR=/opt/conda
RUN wget --quiet https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O ~/miniconda.sh && \
     /bin/bash ~/miniconda.sh -b -p /opt/conda

# Put conda in path so we can use conda activate
ENV PATH=$CONDA_DIR/bin:$PATH
ENV CONDA_PLUGINS_AUTO_ACCEPT_TOS=true
RUN conda install -c conda-forge mamba conda-merge conda-pack

RUN ls /opt/radar/vol2bird/bin

COPY flavors/ravl/KNMI_vol_h5_to_ODIM_h5.c .
RUN gcc -Wall -L/usr/lib/x86_64-linux-gnu/hdf5/serial/ -I/usr/include/hdf5/serial KNMI_vol_h5_to_ODIM_h5.c -lhdf5 -lhdf5_hl -o KNMI_vol_h5_to_ODIM_h5
RUN mv KNMI_vol_h5_to_ODIM_h5 /opt/radar/vol2bird/bin

RUN apt autoclean -y && apt autoremove -y

CMD ["sh","-c","vol2bird"]
