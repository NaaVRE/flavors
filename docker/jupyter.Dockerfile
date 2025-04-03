FROM quay.io/jupyter/minimal-notebook:lab-4.3.6

COPY --chown=jovyan:jovyan ./docker/jupyter.requirements.txt requirements.txt
RUN pip install -r requirements.txt

# nb_conda_kernels for auto-discovery of kernels in other conda environments
RUN conda install "nb_conda_kernels>=2.5.0"; \
    conda clean -a

ARG CONDA_ENV_FILE
COPY --chown=jovyan:jovyan ${CONDA_ENV_FILE?} environment.yaml
RUN conda env create -f environment.yaml && \
    conda clean -a
RUN echo '{"CondaKernelSpecManager": {"env_filter": "/opt/conda$", "conda_only": true}}' >> /home/jovyan/.jupyter/jupyter_config.json
