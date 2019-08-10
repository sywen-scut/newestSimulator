library(ggplot2)
names(dat2) <- c("neighbourCoolest","globalCoolest", "dynamic patterning")
a= rep("localMigration", times=3)
b= rep("globalCoolest", times=3)
c= rep("neighbourCoolest", times=3)
d= rep("initialMap", times=3)
method = cbind(a,b,c,d)
dc<- data.frame(dc)
rt =as.numeric(dc[,6])
for (j in 1:3) {
  for (i in seq(j+3,12,3)) {
    rt[i] = rt[i]/rt[j]
  }
  rt[j]=1
}
d = cbind(dc[,7], rt[1:12],dc[,2],method[1:12])


d<- data.frame(d)
names(d) = c("netsize", "totalrunningtime","type","method")
d$totalrunningtime=as.numeric(levels(d$totalrunningtime))[d$totalrunningtime]
d$netsize = factor(d$netsize, levels=c("10","12", "14"))
d$method = factor(d$method, levels=c("localMigration","neighbourCoolest","globalCoolest", "initialMap"))
ggplot(d, aes(netsize, totalrunningtime, fill=method)) + 
  ylim(0,2.2) +
  #xlim(c(50,100,150,200))+
  xlab("Network Size") +
  ylab("Normalized total running time") +
  labs(title = "Real benchmark") +
  geom_bar(stat="identity", position="dodge")

write.table (d, file ="/home/chris/simulator/IPS/realbench/realbench.txt", sep ="\t", row.names =FALSE, col.names =TRUE, quote =FALSE)
#save(d,file="/home/chris/simulator/IPS/i500/diffcommvol.txt")
