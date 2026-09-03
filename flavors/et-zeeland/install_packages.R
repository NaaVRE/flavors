pak::pkg_install("ReacTran")
library("ReacTran")

devtools::install_github('TempSED/TempSED@6101ced7f79b300f19999f61f50fefe6099aab76', build_vignettes = TRUE, dependencies = FALSE)
library('TempSED')