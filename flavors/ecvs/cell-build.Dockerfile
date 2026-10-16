FROM mambaorg/micromamba:2.9.0

RUN micromamba install -y -n base -c conda-forge conda-pack && \
    micromamba clean --all --yes

ARG CONDA_ENV_FILE
COPY --chown=mambauser:mambauser ${CONDA_ENV_FILE?} environment.yaml
RUN micromamba create -y -n venv -f environment.yaml
RUN mamba install -y -n venv -c conda-forge tensorflow-cpu
RUN micromamba clean --all --yes
