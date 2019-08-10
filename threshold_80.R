library(ggplot2)
library(ggthemr)
library(reshape2)
#palette:flat dust light greyscale
ggthemr("dust")
dat <- read.table(text="
h1 h2 h3 h4 h5 h6 h7 h8 
1602  2401  3003  3803  4303 5203 5903 6611
1801  2801  3336  4335  4801 6002 6801  7610
1903  3803  4603  5603 6502 7103 9904 11111
1701 2401   3102 4102 4801 5403 6504 7411", header=TRUE, as.is=TRUE)
for (i in 1:4) {
  dat[i,]=dat[i,]/1602
}

dat$Methods <- factor(c("Square", "ChessBoard", "GlobalCoolest","LocalCoolest"), 
                  levels=c("Square", "ChessBoard", "GlobalCoolest","LocalCoolest"))
mdat <- melt(dat, id.vars="Methods")
head(mdat)
ggplot(mdat, aes(variable, value, fill=Methods)) + 
  xlab("Tree height") +
  ylab("Normalized Execution Time") +
  #theme(axis.text.x = element_text(angle = 45, hjust = 0.5, vjust = 0.5))+
  geom_bar(stat="identity", position="dodge")

