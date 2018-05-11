setwd("/home/bio23user/Documents/Projects/fig-wasps/cpp_version/simuls/")


draws <- expand.grid(ag = c(0.45),npop=c(500),nfights=c(1))
Nt <- 1e5
numrep <- 10
draws$filename <- paste("cpp_sims/file",1:nrow(draws),sep="")

for(i in 1:nrow(draws)){
  with(draws[i,],system(paste("./main",filename,npop,nfights,numrep,ag,format(Nt,scientific=FALSE))))
}

pdf("plots.pdf")
for(j in 1:nrow(draws)){
  filename <- draws[j,'filename']
  ag <- read.csv(paste(filename,'.ag',sep=""),header=FALSE)
  ag1 <- read.csv(paste(filename,'.ag1',sep=""),header=FALSE)
  ag2 <- read.csv(paste(filename,'.ag2',sep=""),header=FALSE)
  pr <- read.csv(paste(filename,'.pr',sep=""),header=FALSE)
  pr1 <- read.csv(paste(filename,'.pr1',sep=""),header=FALSE)
  pr2 <- read.csv(paste(filename,'.pr2',sep=""),header=FALSE)
  
  par(mfrow=c(1,3),oma=c(0,0,2,0))
  for(i in 1:nrow(ag)){
    plot(1:ncol(ag),ag[i,],xlim=c(1,ncol(ag)),ylim=c(0,1),type="l",main="overall")
    lines(1:ncol(ag),pr[i,],col="red")
    
    plot(1:ncol(ag),ag1[i,],xlim=c(1,ncol(ag)),ylim=c(0,1),type="l",main="site 1")
    lines(1:ncol(ag),pr1[i,],col="red")
    
    plot(1:ncol(ag),ag2[i,],xlim=c(1,ncol(ag)),ylim=c(0,1),type="l",main="site 2")
    lines(1:ncol(ag),pr2[i,],col="red")
    
    legend("bottomright",legend = c('aggression','site preference'),lty=1,col=c('black','red'))
    
  }
  #title(paste("N =",draws[j,'npop'],"ag = ",draws[j,'ag']),outer=TRUE)
}
dev.off()