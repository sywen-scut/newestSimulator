library("ggplot2")
library(reshape2)
library(ggthemr)
#palette: flat dark, dust, light, 
#earth, fresh, chalk, lilac, carrot, pale, copper, grape, greyscale, 
#sky, solarized, grass, sea, camoflauge
ggthemr("fresh")
dat <- read.table(text="
3641    4462    5880    6288    7312
4038    4644    5025    6449    6351 
4267    5239    6015    6663    7851
4309    5745    6582    7153    7816
", header= FALSE , as.is=TRUE)

names(dat) <- c("50","100","150","200","250")

dat$method <- factor(c("coolest-edge","coolest-center", "neighbor-edge","neighbor-center"), 
                      levels=c("coolest-edge","coolest-center", "neighbor-edge","neighbor-center"))
mdat <- melt(dat, id.vars="method")
dev.off()
ggplot(mdat, aes(variable, value, group=method, colour= method)) + geom_point()+ geom_line() + 
  ggtitle("14 Tasks with 2 bubbles") +
  xlab("Average comm vol") +
  ylab("ExecutionTIme") +theme(plot.title = element_text(hjust = 0.5))





