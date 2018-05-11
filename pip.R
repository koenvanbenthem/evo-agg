
meanr_mnot <- function(N,pag){
  Npair <- (N-2)/2
  p <- 1-(1-pag)^2
  as.numeric(dbinom(0:Npair,Npair,p) %*% (N/(N-0:Npair)))
}


meanr_mwins <- function(N,pag){
  Npair <- (N-2)/2
  p <- 1-(1-pag)^2
  as.numeric(dbinom(0:Npair,Npair,p) %*% (N/(N-1-0:Npair)))
}

meanr_mwins(30,0.2)
meanr_mnot(30,0.2)

MutGrowth <- function(pag,pagp,N,pwin){
  # pag < mpfr(pag,400)
  # pagp <- mpfr(pagp,400)
  # N <- mpfr(N,400)
  # pwin <- mpfr(pwin,400)
  # d <- 0.5*(N-2)*(1-(1-pag)^2)
  
  meanr_mnot(N,pag)*(1-pag)*(1-pagp) + meanr_mwins(N,pag) * (pag*pagp*0.5 + (1-pag)*pagp*pwin + pag*(1-pagp)*(1-pwin))
  # d/(N-d)*(1-pag)*(1-pagp) + (d+1)/(N-1-d) * (pag*pagp*0.5 + (1-pag)*pagp*pwin + pag*(1-pagp)*(1-pwin))
}

########### PIP ######################
tmp <- expand.grid(pag=seq(0,1,0.01),pagp=seq(0,1,0.01),N=10,pwin=0.75)
for(i in 1:nrow(tmp)){
tmp$gr[i] <- with(tmp[i,],MutGrowth(pag,pagp,N,pwin))
}
tmp$invasability <- c('+','-')[1+(tmp$gr < 1)]
library(ggplot2)

setwd("/home/bio23user/Documents/Projects/fig-wasps/cpp_version/simuls/")
cut_off <- min(tmp$pag[tmp$gr<1 & tmp$pag > tmp$pagp])
text_df <- data.frame(x=c(cut_off/2,2*cut_off/3,1-2*(1-cut_off)/3,1-(1-cut_off)/2),y=c((2+cut_off)/4,cut_off/3,1-(1-cut_off)/3,(1+cut_off)/4),label=c('-','+','+','-'),stringsAsFactors = FALSE)
ggplot() + geom_tile(data=tmp,aes(x=pag,y=pagp,fill=invasability)) + 
  geom_abline(slope=1,intercept=0) + theme_bw() + scale_fill_brewer() + 
  scale_x_continuous(expand=c(0,0)) + scale_y_continuous(expand=c(0,0)) + 
  xlab("resident aggressiveness") + ylab("mutant aggressiveness") + theme(legend.position = 'none') +
  geom_text(data=text_df,aes(x=x,y=y,label=label),size=10)


### tile leads to too many points for tikz, simplify!
library(tikzDevice)
options(tikzDocumentDeclaration = "\\documentclass[10pt]{article}\n \\renewcommand{\\familydefault}{\\sfdefault}\n") # adding non-serif font family to the options of the tikzdevice
options()
tikz("pip.tex",height=3.5,width=3.8,standAlone=TRUE)
text_df <- data.frame(x=c(cut_off/2,2*cut_off/3,1-2*(1-cut_off)/3,1-(1-cut_off)/2),y=c((2+cut_off)/4,cut_off/3,1-(1-cut_off)/3,(1+cut_off)/4),label=c('$-$','$+$','$+$','$-$'),stringsAsFactors = FALSE)
pol_df <- data.frame(x=c(0,cut_off,cut_off,0,
                         0,cut_off,cut_off,
                         cut_off,1,1,cut_off,
                         cut_off,1,cut_off),
            y=c(0,cut_off,1,1,
                0,0,cut_off,
                0,0,1,cut_off,
                cut_off,1,1),
           group=c(rep(1,4),rep(2,3),rep(3,4),rep(4,3)),fill=c(rep('1',4),rep('2',3),rep('1',4),rep('2',3)))
ggplot() + geom_polygon(data=pol_df,aes(x=x,y=y,group=group,fill=fill))+
  geom_abline(slope=1,intercept=0) + theme_bw() + scale_fill_brewer() +
  scale_x_continuous(expand=c(0,0)) + scale_y_continuous(expand=c(0,0)) + 
  xlab("resident aggressiveness") + ylab("mutant aggressiveness") + theme(legend.position = 'none') +
  geom_text(data=text_df,aes(x=x,y=y,label=label),size=10)+
  theme(plot.margin = unit(c(0.5,0.5,0.5,0.5), "cm"))
dev.off()
system("pdflatex pip.tex")

# tmp[tmp$pag==tmp$pagp,]


######### DENSITY EFFECT ON 'FLIP' POINT ##############
tmp2 <- expand.grid(pag=seq(0,1,0.005),N=seq(4,100,2),pwin=0.75)
tmp2$pagp <- tmp2$pag + 0.001
for(i in 1:nrow(tmp2)){
  tmp2$gr[i] <- with(tmp2[i,],MutGrowth(pag,pagp,N,pwin))
}

tipover <- lapply(unique(tmp2$N), function(x){
  bla <- tmp2[tmp2$N==x,]
  if(any(bla$gr > 1)){
    return(bla$pag[min(which(bla$gr>1))])
  }else{
    return(NA)
  }
})

plot(unique(tmp2$N),tipover,type='l',xlab="density",ylab="tip-over aggression")
