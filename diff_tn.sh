for t in 9 10 11 12 13 14 15 16
do
    for r in 9 10 12 14 15 16 18 20 21 24 25 28 30 32 35 40
    do
        if [ $t -le $r ]  
        then
            ./lineshape 4 200 $r $t 0.2
        fi
    done
done