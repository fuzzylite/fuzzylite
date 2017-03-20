directories = c("mamdani/", "mamdani/matlab/", "mamdani/octave/", "takagi-sugeno/", "takagi-sugeno/matlab/", "takagi-sugeno/octave/", "tsukamoto/", "hybrid")
baseDirectory = getwd()

for (directory in directories){
  message("Processing directory: ", directory)
  setwd(directory)
  rScripts = list.files(".", pattern = ".R$")
  for (i in seq(1, length(rScripts))){
    rScript = rScripts[i]
    message(sprintf("\tProcessing file %i/%i: %s", i, length(rScripts), rScript))
    scriptEnvironment = new.env()
    sys.source(rScript, scriptEnvironment)
    rm(scriptEnvironment)
  }
  setwd(baseDirectory)
}
