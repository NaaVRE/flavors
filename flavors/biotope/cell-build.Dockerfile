FROM continuumio/miniconda3:4.12.0 as build-trias

RUN \
  conda install conda-build git && \
  conda skeleton cran https://github.com/trias-project/trias --git-tag v2.0.7 && \
  conda build --R=4.3 r-trias

FROM mambaorg/micromamba:2.0.7

RUN micromamba install -y -n base -c conda-forge conda-pack && \
    micromamba clean --all --yes

ARG CONDA_ENV_FILE
COPY --chown=mambauser:mambauser ${CONDA_ENV_FILE?} environment.yaml
RUN micromamba create -y -n venv -f environment.yaml && \
    micromamba clean --all --yes

COPY --from=build-trias /opt/conda/conda-bld/linux-64/r-trias-2.0.7-r43_0.tar.bz2 .
#RUN conda install r-trias-2.0.7-r43_0.tar.bz2
