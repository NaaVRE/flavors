pak::pkg_install("ReacTran")
library("ReacTran")

remotes::install_github("TempSED/TempSED", build_vignettes = TRUE)
library("TempSED")