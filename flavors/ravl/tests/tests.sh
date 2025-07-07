#!/bin/sh
vol2bird --version &> version || true
if  grep -q 'vol2bird version' version ; then cat version ;  else exit 255;  fi

KNMI_vol_h5_to_ODIM_h5 &> KNMI_vol_h5_to_ODIM_h5_out || true
if  grep -q 'Usage: KNMI_vol_h5_to_ODIM_h5 ODIM_file.h5 KNMI_input_file.h5' KNMI_vol_h5_to_ODIM_h5_out ; then cat KNMI_vol_h5_to_ODIM_h5_out ;  else exit 255;  fi
