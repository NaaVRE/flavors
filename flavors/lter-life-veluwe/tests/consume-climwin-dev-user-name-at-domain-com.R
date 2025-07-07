install.packages("zoo",repos = "http://cran.us.r-project.org")

library(optparse)
library(jsonlite)
library(zoo)


option_list = list(

make_option(c("--id"), action="store", default=NA, type="character", help="my description"),
make_option(c("--rolling_mean_temp_str"), action="store", default=NA, type="character", help="my description"),
make_option(c("--temperature_data_str"), action="store", default=NA, type="character", help="my description")

)

# set input parameters accordingly
opt = parse_args(OptionParser(option_list=option_list))


id <- gsub('"', '', opt$id)
rolling_mean_temp_str <- gsub('"', '', opt$rolling_mean_temp_str)
temperature_data_str <- gsub('"', '', opt$temperature_data_str)





cat("Original Temperature Data:\n", head(temperature_data_str), "\n\n")
cat("Rolling Mean Temperature in Moving Windows:\n", head(coredata(rolling_mean_temp_str)), "\n")



