###input argv: migrationPattern, interval,commvol(50,100,150,200), networksize
##workload: 12 random applications
# case 1:squareMigration
# case 2:chessMigration
# case 3:globalCoolest
# case 4:localCoolest
# case 5:dvfs only
# case 6:neighborCoolest
for type in 6
do
    for commvol in 50 100 150 200
    do
        nohup ./mixture $type 500 $commvol 8 &
    done
done
# r 1 200 18 9 0.1
