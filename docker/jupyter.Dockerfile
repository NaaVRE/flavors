FROM quay.io/jupyter/minimal-notebook:lab-4.3.6

COPY --chown=jovyan:jovyan ./docker/jupyter.requirements.txt requirements.txt
RUN pip install -r requirements.txt

# nb_conda_kernels for auto-discovery of kernels in other conda environments
RUN mamba install --yes "nb_conda_kernels>=2.5.0"; \
    mamba clean --all --yes

# Disable "Would you like to get notified about official Jupyter news?"
# https://jupyterlab.readthedocs.io/en/stable/user/announcements.html
RUN jupyter labextension disable "@jupyterlab/apputils-extension:announcements"

ARG CONDA_ENV_FILE
COPY --chown=jovyan:jovyan ${CONDA_ENV_FILE?} environment.yaml
RUN mamba env create --yes -f environment.yaml && \
    mamba clean --all --yes
RUN echo '{"CondaKernelSpecManager": {"env_filter": "/opt/conda$", "conda_only": true}}' >> /home/jovyan/.jupyter/jupyter_config.json
