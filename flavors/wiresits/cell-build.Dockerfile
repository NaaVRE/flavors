FROM mambaorg/micromamba:2.0.7

RUN micromamba install -y -n base -c conda-forge conda-pack && \
    micromamba clean --all --yes

ARG CONDA_ENV_FILE
COPY --chown=mambauser:mambauser ${CONDA_ENV_FILE?} environment.yaml
RUN micromamba create -y -n venv -f environment.yaml && \
    micromamba clean --all --yes

COPY ./flavors/wiresits/install_packages.R .
RUN micromamba run -n venv bash -c "export PKG_LIBS=\""'$(pkg-config --libs proj) $(gsl-config --libs)'"\"; Rscript install_packages.R"
