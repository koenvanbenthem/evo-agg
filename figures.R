setwd("/home/bio23user/Documents/Projects/fig-wasps/cpp_version/simuls/")


draws <- expand.grid(ag = c(0.35,0.45,0.55,0.65),npop=c(30,100),nfights=c(1,3))
Nt <- 1e4
numrep <- 100
draws$filename <- paste("cpp_sims/file",1:nrow(drawsc),".csv",sep="")

for(i in 1:nrow(drawsc)){
  with(draws[i,],system(paste("./main",filename,npop,nfights,numrep,ag)))
}


pdf("Results-R-C.pdf")
for(i in 1:nrow(draws)){
  ag = draws$ag[i]
  npop = draws$npop[i]
  nfights = draws$nfights[i]
  
  outc2 <- outc_ag[(draws$ag == ag & draws$npop==npop & draws$nfights == nfights),]
  tmp <- read.csv(draws$filename[i],header=FALSE)
  ### line graphs
  par(mfrow=c(1,2),oma=c(0,0,2,0))
   
  matplot(t(tmp),type="l",main='c++')
  matplot(t(outc2),type="l",col=rgb(0.8,0.2,0.2,alpha=0.4),main='R')
  mtext(paste("Ag0 =",ag," N = ",npop," nfights = ",nfights),outer=TRUE,cex=1.5)
  ### histograms
  
  hist(tmp[,ncol(tmp)],main='c++')
  hist(outc2[,ncol(outc2)],main='R')
  mtext(paste("Ag0 =",ag," N = ",npop," nfights = ",nfights),outer=TRUE,cex=1.5)
  
}
dev.off()

for(i in 1:nrow(drawsc)){
  ag = drawsc$ag[i]
  npop = drawsc$npop[i]
  nfights = drawsc$nfights[i]
  
  tmp <- read.csv(drawsc$filename[i],header=FALSE)
  
  if(i == 1){
  all <- data.frame(ag=ag,npop=npop,nfights=nfights,val=tmp[,ncol(tmp)])
  }else{
  all <- rbind(all,data.frame(ag=ag,npop=npop,nfights=nfights,val=tmp[,ncol(tmp)]))
  }
}

library(ggplot2)
ggplot(all,aes(x=val)) + geom_histogram() + facet_grid(ag ~ npop + nfights) + theme_bw()
