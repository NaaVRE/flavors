FROM quay.io/jupyter/minimal-notebook:lab-4.3.6

COPY --chown=jovyan:jovyan ./docker/jupyter.requirements.txt requirements.txt
RUN pip install -r requirements.txt

# nb_conda_kernels for auto-discovery of kernels in other conda environments
RUN mamba install --yes "nb_conda_kernels>=2.5.0" && \
    mamba clean --all --yes

# Disable "Would you like to get notified about official Jupyter news?"
# https://jupyterlab.readthedocs.io/en/stable/user/announcements.html
RUN jupyter labextension disable "@jupyterlab/apputils-extension:announcements"

ARG CONDA_ENV_FILE
COPY --chown=jovyan:jovyan ${CONDA_ENV_FILE?} environment.yaml
RUN mamba env create --yes -f environment.yaml && \
    mamba clean --all --yes
RUN echo '{"CondaKernelSpecManager": {"env_filter": "/opt/conda$", "conda_only": true}}' >> /home/jovyan/.jupyter/jupyter_config.json

USER root

# OTB from the ZonalFilter dockerfile
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
      python3-pip \
      python3-dev \
      software-properties-common \
      wget \
      cmake \
      libglu1-mesa-dev \
      gdal-bin \
      libgdal-dev

WORKDIR /usr/local/otb

RUN wget https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-7.3.0-Linux64.run &&  \
    chmod +x OTB-7.3.0-Linux64.run && \
    ./OTB-7.3.0-Linux64.run --target $PWD && \
    rm ./OTB-7.3.0-Linux64.run && \
    . ./otbenv.profile

WORKDIR /usr/local/lw_apps/
ADD ./flavors/biotope/OTB .
RUN  LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu/ cmake -D CMAKE_INSTALL_PREFIX=/usr/local/otb && make install

COPY ./flavors/biotope/install_packages.R .
RUN mamba run -n biotope bash -c "Rscript install_packages.R"

USER $NB_USER
WORKDIR /home/$NB_USER
