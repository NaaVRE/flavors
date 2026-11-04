FROM qingzfly/getm-wad-container:fMAsink AS getm-wad

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

RUN mkdir -p /tmp/data

COPY --from=getm-wad /usr/local/bin/gotm-run /usr/local/bin/gotm-run
COPY --from=getm-wad /opt/cases/OysterGrounds /opt/cases/OysterGrounds
COPY --from=getm-wad /opt/local/gotm /opt/local/gotm
COPY --from=getm-wad /opt/netcdf-c-4.6.0 /opt/netcdf-c-4.6.0
COPY --from=getm-wad /opt/netcdf-fortran-4.4.4 /opt/netcdf-fortran-4.4.4
COPY --from=getm-wad /usr/lib/x86_64-linux-gnu/libgfortran.so.3* /opt/getm-libs/
COPY --from=getm-wad /usr/lib/x86_64-linux-gnu/libquadmath.so.0* /opt/getm-libs/
COPY --from=getm-wad /usr/lib/x86_64-linux-gnu/libhdf5_serial.so.100* /opt/getm-libs/
COPY --from=getm-wad /usr/lib/x86_64-linux-gnu/libhdf5_serial_hl.so.100* /opt/getm-libs/
COPY --from=getm-wad /usr/lib/x86_64-linux-gnu/libsz.so.2* /opt/getm-libs/
COPY --from=getm-wad /usr/lib/x86_64-linux-gnu/libaec.so.0* /opt/getm-libs/

ENV PATH="/opt/local/gotm/bin:/opt/netcdf-c-4.6.0/bin:/opt/netcdf-fortran-4.4.4/bin:${PATH}" \
    NETCDF=/opt/netcdf-fortran-4.4.4 \
    NETCDF_ROOT=/opt/netcdf-fortran-4.4.4 \
    NETCDF_LIBDIR=/opt/netcdf-fortran-4.4.4/lib \
    NETCDF_INCDIR=/opt/netcdf-fortran-4.4.4/include \
    LD_LIBRARY_PATH="/opt/getm-libs:/opt/netcdf-fortran-4.4.4/lib:/opt/netcdf-c-4.6.0/lib"