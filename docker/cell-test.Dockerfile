ARG BUILD_IMAGE
ARG RUNTIME_IMAGE

FROM ${BUILD_IMAGE?} AS build
# When building actual cells, extra dependencies are installed here:
# COPY environment.yaml .
# RUN micromamba install -y -n venv -f environment.yaml
ARG MAMBA_DOCKERFILE_ACTIVATE=1
USER root
RUN conda-pack -p /opt/conda/envs/venv -o /tmp/env.tar && \
    mkdir /venv && \
    cd /venv && \
    tar xf /tmp/env.tar && \
    rm /tmp/env.tar
RUN /venv/bin/conda-unpack

FROM ${RUNTIME_IMAGE?} AS runtime
COPY --from=build /venv /venv
WORKDIR /app
# When building actual cells, we copy files to the cell. In the tests, we use a
# volume mount
# COPY my-task-name.R .