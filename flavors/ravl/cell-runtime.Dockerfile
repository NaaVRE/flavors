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

FROM ubuntu:24.04

COPY --from=vol2bird /opt/radar/ /opt/radar/
COPY --from=vol2bird /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu
ENV LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/opt/radar/lib:/opt/radar/rave/lib:/opt/radar/rsl/lib:/opt/radar/vol2bird/lib:/usr/lib/x86_64-linux-gnu
ENV PATH=${PATH}:/opt/radar/vol2bird/bin:/opt/radar/rsl/bin
