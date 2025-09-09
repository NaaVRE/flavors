FROM quay.io/jupyter/minimal-notebook:lab-4.3.6

USER root
RUN apt-get update && \
    apt-get install --no-install-recommends -y nodejs npm && \
    apt autoclean -y && \
    apt autoremove -y
USER $NB_USER

COPY --chown=jovyan:jovyan ./docker/jupyter.requirements.txt requirements.txt
RUN pip install -r requirements.txt

# nb_conda_kernels for auto-discovery of kernels in other conda environments
RUN conda install "nb_conda_kernels>=2.5.0"; \
    conda clean -a

# Disable "Would you like to get notified about official Jupyter news?"
# https://jupyterlab.readthedocs.io/en/stable/user/announcements.html
RUN jupyter labextension disable "@jupyterlab/apputils-extension:announcements"

ARG CONDA_ENV_FILE
COPY --chown=jovyan:jovyan ${CONDA_ENV_FILE?} environment.yaml
RUN conda env create -f environment.yaml && \
    conda clean -a
RUN echo '{"CondaKernelSpecManager": {"env_filter": "/opt/conda$", "conda_only": true}}' >> /home/jovyan/.jupyter/jupyter_config.json

RUN pip install git+https://github.com/QCDIS/jupyter-reprolab.git