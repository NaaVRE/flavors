FROM mambaorg/micromamba:2.0.7

RUN micromamba install -y -n base -c conda-forge conda-pack && \
    micromamba clean --all --yes

ARG CONDA_ENV_FILE
COPY --chown=mambauser:mambauser ${CONDA_ENV_FILE?} environment.yaml
RUN micromamba create -y -n venv -f environment.yaml && \
    micromamba clean --all --yes

RUN /opt/conda/envs/venv/bin/R -e "devtools::install_github('trias-project/trias@v2.0.7')"
#RUN mamba install --yes r-trias-2.0.7-r43_0.tar.bz2
