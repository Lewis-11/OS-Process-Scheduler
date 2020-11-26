#!/bin/bash
#The proportion between asteroids and planets is 70/30
#Iterations are 50, 100, 200
#Sum of asteroids and planets is 250, 500 and 1000



echo "---ITERATIONS: 50 ---"
for i in {0..14}
do
    ./main_seq 175 50 75 6
done
echo "---"

for i in {0..14}
do
    ./main_seq 350 50 150 6
done
echo "---"

for i in {0..14}
do
    ./main_seq 700 50 300 6
done
echo "---"

echo "---ITERATIONS: 100 ---"
for i in {0..14}
do
    ./main_seq 175 100 75 6
done
echo "---"

for i in {0..14}
do
    ./main_seq 350 100 150 6
done
echo "---"

for i in {0..14}
do
    ./main_seq 700 100 300 6
done
echo "---"

echo "---ITERATIONS: 200---"
for i in {0..14}
do
    ./main_seq 175 200 75 6
done
echo "---"

for i in {0..14}
do
    ./main_seq 350 200 150 6
done
echo "---"

for i in {0..14}
do
    ./main_seq 700 200 300 6
done
echo "---"

echo "All tests done. End of execution."
