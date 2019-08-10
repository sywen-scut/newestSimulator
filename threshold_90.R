library(ggplot2)
library(ggthemr)
library(reshape2)
#palette:flat dust light greyscale
ggthemr("light")
dat <- read.table(text="
waittingTime  executionTime  responseTime  migrationCost
1   1   1  1
1.35   1.13   1.17  0.34
0.001   1.24   1.04   0.56
0.001  1.015   0.85  0.28", header=TRUE, as.is=TRUE)
dat$Methods <- factor(c("Square", "ChessBoard", "GlobalCoolest","LocalCoolest"), 
                      levels=c("Square", "ChessBoard", "GlobalCoolest","LocalCoolest"))
mdat <- melt(dat, id.vars="Methods")
head(mdat)
ggplot(mdat, aes(variable, value, fill=Methods)) + 
  xlab("Perfomance category") +
  ylab("Normalized Time") +
  theme(axis.text.x = element_text(angle = 45, hjust = 0.5, vjust = 0.5))+
  geom_bar(stat="identity", position="dodge")
