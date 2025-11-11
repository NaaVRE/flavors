FROM ubuntu:24.04

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