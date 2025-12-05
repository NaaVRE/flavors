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


RUN /opt/conda/envs/lter-life-wadden/bin/git clone --depth 1 --branch 20250114.0 https://github.com/acolite/acolite.git && \
    site_package_dir="$(/opt/conda/envs/lter-life-wadden/bin/python -c  "import site; print(''.join(site.getsitepackages()))")" && \
    cp ./acolite/acolite -r "$site_package_dir" && \
    cp ./acolite/config -r "$site_package_dir" && \
    cp ./acolite/data -r "$site_package_dir"

COPY ./flavors/lter-life-wadden/install_packages.R .
RUN mamba run -n lter-life-wadden bash -c "Rscript install_packages.R"
