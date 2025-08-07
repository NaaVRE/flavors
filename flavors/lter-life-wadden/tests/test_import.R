library(optparse)
library(jsonlite)

library(FME)
library(deSolve)
library(plot3D)
library(sf)
library(ncdf4)
library(tidyverse)

# Test tools4watlas
library(tools4watlas)
library(ggplot2)
# Load example data
data <- data_example
# Create base map
bm <- atl_create_bm(data, buffer = 800)
# Plot points and tracks
bm +
  geom_path(
    data = data, aes(x, y, colour = species),
    linewidth = 0.5, alpha = 0.5, show.legend = FALSE
  ) +
  geom_point(
    data = data, aes(x, y, color = species),
    size = 0.5, alpha = 0.5, show.legend = TRUE
  ) +
  scale_color_manual(
    values = atl_spec_cols(),
    labels = atl_spec_labs("multiline"),
    name = ""
  ) +
  guides(colour = guide_legend(
    nrow = 1, override.aes = list(size = 7, pch = 16, alpha = 1)
  )) +
  theme(
    legend.position = "top",
    legend.justification = "center",
    legend.key = element_blank(),
    legend.background = element_rect(fill = "transparent")
  )

# Test SIBES
library(SIBES)
get('SIBES_dataset')

# Test TempSED
require(TempSED)
example("TempSED_run1D")
