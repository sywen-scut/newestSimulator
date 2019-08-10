data<-read.table(text="
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	",header=FALSE, as.is=TRUE)
data$V1 <- 0:8
data$V8 <-0
data$V2 <- 1000
c<- data[,3:11]
for (i in 1) {
  c
}

write.table(data, file="~/simulator/app/single/temp.txt", sep ="   ", row.names =FALSE,col.names =FALSE, quote =TRUE)

  