directories = c("mamdani/", "mamdani/matlab/", "mamdani/octave/", "takagi-sugeno/", "takagi-sugeno/matlab/", "takagi-sugeno/octave/", "tsukamoto/")
###REMOVE
baseDirectory = '/tmp/fl/'
setwd(baseDirectory)
#####
baseDirectory = getwd()

for (directory in directories){
  message("Processing directory: ", directory)
  setwd(directory)
  rScripts = list.files(".", pattern = "*.R")
  for (i in seq(1, length(rScripts))){
    rScript = rScripts[i]
    message(sprintf("\tProcessing file %i/%i: %s", i, length(rScripts), rScript))
    source(rScript)
  }
  setwd(baseDirectory)
}

#Processing file 9/14: slcp1.R
#Saving 10.7 x 10.2 in image
#Processing file 10/14: slcpp1.R

