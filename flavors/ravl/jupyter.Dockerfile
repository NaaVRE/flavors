FROM adokter/vol2bird:0.5.0 AS vol2bird

RUN apt-get update && \
    apt-get install --no-install-recommends -y libconfuse-dev libhdf5-dev gcc g++ wget unzip make cmake zlib1g-dev python-dev python-numpy libproj-dev flex-old file && \
    apt-get install -y git git-lfs && \
    apt-get install -y libgsl-dev && \
    apt-get install -y libbz2-dev bison byacc && \
    apt autoclean -y && \
    apt autoremove -y

COPY flavors/ravl/KNMI_vol_h5_to_ODIM_h5.c .
RUN gcc -Wall -L/usr/lib/x86_64-linux-gnu/hdf5/serial/ -I/usr/include/hdf5/serial KNMI_vol_h5_to_ODIM_h5.c -lhdf5 -lhdf5_hl -o KNMI_vol_h5_to_ODIM_h5
RUN mv KNMI_vol_h5_to_ODIM_h5 /opt/radar/vol2bird/bin

COPY flavors/ravl/tests/tests.sh /test_ravl.sh
RUN bash /test_ravl.sh
RUN rm /test_ravl.sh
RUN rm version KNMI_vol_h5_to_ODIM_h5_out
CMD vol2bird

FROM quay.io/jupyter/minimal-notebook:lab-4.3.6

COPY --chown=jovyan:jovyan ./docker/jupyter.requirements.txt requirements.txt
RUN pip install -r requirements.txt

# nb_conda_kernels for auto-discovery of kernels in other conda environments
RUN mamba install --y "nb_conda_kernels>=2.5.0"; \
    mamba clean --all --yes

# Disable "Would you like to get notified about official Jupyter news?"
# https://jupyterlab.readthedocs.io/en/stable/user/announcements.html
RUN jupyter labextension disable "@jupyterlab/apputils-extension:announcements"

ARG CONDA_ENV_FILE
COPY --chown=jovyan:jovyan ${CONDA_ENV_FILE?} environment.yaml
RUN mamba env create --yes -f environment.yaml && \
    mamba clean --all --yes
RUN echo '{"CondaKernelSpecManager": {"env_filter": "/opt/conda$", "conda_only": true}}' >> /home/jovyan/.jupyter/jupyter_config.json

RUN /opt/conda/envs/ravl/bin/R -e "install.packages('suntools', repos='https://cran.r-project.org')" && \
    /opt/conda/envs/ravl/bin/R -e "install.packages('bioRad', repos='https://cran.r-project.org')" && \
    /opt/conda/envs/ravl/bin/R -e "library('bioRad')"

COPY --from=vol2bird /opt/radar/ /opt/radar/
COPY --from=vol2bird /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu
ENV LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/opt/radar/lib:/opt/radar/rave/lib:/opt/radar/rsl/lib:/opt/radar/vol2bird/lib:/usr/lib/x86_64-linux-gnu
ENV PATH=${PATH}:/opt/radar/vol2bird/bin:/opt/radar/rsl/bin