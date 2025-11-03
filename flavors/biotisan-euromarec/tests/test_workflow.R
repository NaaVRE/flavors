library("readxl")
library(dplyr)

# Data cleaning----
# Load data & metadata
args <- commandArgs(trailingOnly = TRUE)
md <- read_excel("Template_MBO_Example_raw.xlsx", sheet = "METADATA") #Load metadata sheet
data <- read_excel("Template_MBO_Example_raw.xlsx", sheet = "BIRDS") #Load data sheet (in this example: "BIRDS")
walhalla <-
# Create a table with sites with more that 7 sampling years
sites <- data %>%
  group_by(siteid) %>%
  summarise(nyear = n_distinct(substr(datecollected, 1, 4))) %>%
  filter(nyear > 7)

md <- merge(md,sites, by = "siteid")

# Keep sites within the study area [our boundaries are latitude (25:90), longitude (-45:70)]
md <- filter(md, decimallatitude >= 25, decimallatitude <= 90,
                 decimallongitude >= -45, decimallongitude <= 70)

data <- data %>% # Keep data from these sites
  filter(siteid %in% md$siteid)

# Check that depth is ~constant
#(in this case is not necessary, but for other taxonomic groups is possible that the sample were taken at different depths)
for (i in names(table(data$siteid))){
  x <- filter(data, siteid == i)
  print(table(x$maximumdepthinmeters))
}

# Check sampling dates
for (i in names(table(data$siteid))){
  x <- filter(data, siteid == i)
  print(table(x$datecollected))
}

# In this case, most of the sampling campaigns were conducted in winter
# One was conducted in summer and should be removed since the sampling season is not consistent

data$month <- as.numeric(format(data$datecollected, "%m")) # Create a column with the sampling month

data <- data %>%
  filter(!month %in% c(8)) #Remove those samples in non-consistent seasons (summer in this case)

# Note that some time series can have more than one sampling campaign per year and even per season (not in this case)
# For our analysis, we are only keeping one sampling campaign per year


# Update the table with sites with more that 7 sampling years
# After removing inconsistent sampling campaigns, some time series may become shorter than 8 years
sites <- data %>%
  group_by(siteid) %>%
  summarise(nyear = n_distinct(substr(datecollected, 1, 4))) %>%
  filter(nyear > 7)

data <- data %>% # Keep data from these sites
  filter(siteid %in% md$siteid)
md <- md %>% # Keep metadata from these sites
  filter(siteid %in% md$siteid)

md_final <- md[,c(1:8)]
data_final <- data[,c(1:15)]

write.csv(md_final, file = "metadata_Example.csv")
write.csv(data_final, file = "data_Example.csv")

# Trend analysis----
library(vegan)
library(dplyr)
library(ggplot2)
library(nlme)

# Load cleaned data & metadata
md <- read.csv("metadata_Example.csv", sep = ",")
data <- read.csv("data_Example.csv", sep = ",")
data$year <- as.numeric(format(as.Date(data$datecollected), "%Y"))
colnames(data)

# Calculate community metrics
data.tax <- data %>%
  group_by(siteid, year, datecollected) %>%
  summarise(richness = n_distinct(taxaname[parameter_value > 0]), # Richness
            parameter_value_tot = sum(parameter_value), # Abundance estimate
            parameter = unique(parameter),
            parameter_standardunit = unique(parameter_standardunit),
            diversity = diversity(parameter_value, index="shannon"), # Diversity
            )

# Temporal analysis. Example with Richness and these 2 time series
results.richness <- data.frame(siteid = character(0), slope = numeric(0), p = numeric(0))

for (i in names(table(data.tax$siteid))) {
  x <- subset(data.tax, siteid == i)
  # We used GLS models taking into account the temporal autocorrelation
  gls_model <- gls(log10(richness+1) ~ year, data = x, correlation = corAR1(form = ~ 1 | year))
  slope <- coef(gls_model)[2]
  p <- summary(gls_model)$tTable[2, 4]

  # Save results
  results.richness <- rbind(results.richness, data.frame(siteid = i, slope = slope, p = p))
}

print(results.richness)

# In this example the second site showed a significant decrease in Richness over time (p<0.05)

final_results <- merge(md,results.richness, by = "siteid"); final_results

# Delete files created
file.remove("metadata_Example.csv")
file.remove("data_Example.csv")