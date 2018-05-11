N1 <- 500
N2 <- 500

pop1 <- data.frame(ag=rep(0.8,N1))
pop2 <- data.frame(ag=rep(0.2,N2))

fighting <- function(pop){
  if(nrow(pop)<2){return(rep(TRUE,nrow(pop)))}
  # select which individuals will initiate a fight
  fighters <- pop$ag > runif(nrow(pop))
  
  # assign every individual to another individual, by generating a random order
  ord <- sample(1:nrow(pop))
  if(nrow(pop)%%2 == 1){
    ord <- ord[-1]
  }
  # and taking the first half of this random order as the first 'fighting' partner for every fight
  a <- fighters[ord[1:(length(ord)/2)]]
  # and the last half a the second 'fighting' partner
  b <- fighters[ord[(0.5*length(ord)+1):length(ord)]]
  
  # determine who wins if fight i (in p_eq[i]) is between two agressors
  p_eq <- runif(length(a)) > 0.5
  # and who wins if the fight is between one agressor and one none agressor
  p_uneq <- runif(length(a)) > 0.2
  
  # vectors for final values
  av <- rep(TRUE,length(a))
  bv <- rep(TRUE,length(b))
  
  # both initiate: 50/50, one initiates: 75/25
  # putting the right winning probabilities in
  av[a & !b] <- p_uneq[a & !b]
  av[!a & b] <- !p_uneq[!a & b]
  av[a & b] <- p_eq[a & b]
  
  bv[a & !b] <- !p_uneq[a & !b]
  bv[!a & b] <- p_uneq[!a & b]
  bv[a & b] <- !p_eq[a & b]
  
  # restructuring to resemble the original structure
  surv <- c(av,bv)
  realsurv <- rep(TRUE,nrow(pop))
  realsurv[ord] <- surv
  realsurv
}

next_pop <- function(pop,Nt,nfights){
  mean_ag <- numeric(Nt)
  mean_ag[1] <- mean(pop$ag)
  mean_prefa <- numeric(Nt)
  mean_prefa[1] <- mean(pop$prefa)
  cov_prefa_ag <- numeric(Nt)
  cov_prefa_ag[1] <- cov(pop$prefa,pop$ag)
  for(t in 2:Nt){
    ## divide population
    go_to_a <- runif(nrow(pop)) < pop$prefa
    popa <- pop[go_to_a,]
    popb <- pop[!go_to_a,]
    
    ###### FIGHT (multiple times?!)
    
    if(nrow(popa)>0){
      popa$surv <- TRUE
      for(i in 1:nfights){
        popa$surv[popa$surv] <- fighting(popa[popa$surv,])
      }
      popa[!popa$surv,] <- popa[sample(which(popa$surv),sum(!popa$surv),replace=TRUE),]
    }
    
    if(nrow(popb)>0){
      popb$surv <- TRUE
      for(i in 1:nfights){
        popb$surv[popb$surv] <- fighting(popb[popb$surv,])
      }
      popb[!popb$surv,] <- popb[sample(which(popb$surv),sum(!popb$surv),replace=TRUE),]
    }
    # re-mixing
    pop <- rbind(popa,popb)
    
    ####### mutate
    pop$ag <- pop$ag + as.numeric(runif(nrow(pop)) < 0.01) * rnorm(nrow(pop),0,0.05)
    #pop$prefa <- pop$prefa + as.numeric(runif(nrow(pop)) < 0.01) * rnorm(nrow(pop),0,0.01)
    pop$ag[pop$ag < 0] <- 0
    pop$ag[pop$ag > 1] <- 1
    pop$prefa[pop$prefa < 0] <- 0
    pop$prefa[pop$prefa > 1] <- 1
    
    mean_ag[t] <- mean(pop$ag)
    mean_prefa[t] <- mean(pop$prefa)
    cov_prefa_ag [t] <- cov(pop$ag,pop$prefa)
  }
  
  return(list(pop=pop,mean_ag=mean_ag,mean_prefa=mean_prefa,cov_prefa_ag=cov_prefa_ag))
}

draws <- expand.grid(ag = c(0.35,0.45,0.55,0.65),repl = 1:100,npop=c(30,100),nfights=c(1,3))
Nt <- 1e4
outc_ag <- matrix(NA,nrow=nrow(draws),ncol=Nt)
outc_pref <- matrix(NA,nrow=nrow(draws),ncol=Nt)
outc_cov <- matrix(NA,nrow=nrow(draws),ncol=Nt)


for(i in 1:nrow(draws)){
  pop <- data.frame(ag=rnorm(draws$npop[i],draws$ag[i],0.05),prefa=rep(1,draws$npop[i]))
  tmp <- next_pop(pop,Nt,draws$nfights[i])
  outc_ag[i,] <- tmp$mean_ag
  outc_pref[i,] <- tmp$mean_prefa
  outc_cov[i,] <- tmp$cov_prefa_ag
  cat(i,"\t",round(100*i/nrow(draws)),"%\n")
}

drawsc <- draws[draws$repl==1,]
drawsc$filename <- paste("cpp_sims/file",1:nrow(drawsc),".csv",sep="")

for(i in 1:nrow(drawsc)){
  with(drawsc[i,],system(paste("./main",filename,npop,nfights,max(draws$repl),ag)))
}

par(mfrow=c(1,3))
for(i in unique(draws$npop)){
  cols <- sample(rainbow(length(unique(draws$ag[draws$npop==i])),v = 0.8))[as.numeric(as.factor(draws$ag[draws$npop==i]))]
matplot(t(outc_ag[draws$npop==i,]),type="l",col=cols,main=i)
matplot(t(outc_pref[draws$npop==i,]),type="l",col=cols,main=i)
matplot(t(outc_cov[draws$npop==i,]),type="l",col=cols,main=i)
}
