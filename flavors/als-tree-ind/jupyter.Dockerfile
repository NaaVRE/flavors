FROM ubuntu:24.04 AS workflow_als_trees

RUN apt-get update && \
    apt-get install --no-install-recommends -y build-essential cmake && \
    apt-get install -y git && \
    apt autoclean -y && \
    apt autoremove -y

RUN cd /bin \
 && git clone --depth=1 https://github.com/Jinhu-Wang/Workflow_ALS_Trees.git \
 && cd Workflow_ALS_Trees \
 && for d in clipping retile_by_count retile_by_size; do \
      cd "$d" && mkdir -p release && cd release && cmake -DCMAKE_BUILD_TYPE=Release .. && make && cd ../..; \
    done

FROM quay.io/jupyter/minimal-notebook:lab-4.3.6

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

COPY --from=workflow_als_trees /bin/Workflow_ALS_Trees/ /bin/Workflow_ALS_Trees/