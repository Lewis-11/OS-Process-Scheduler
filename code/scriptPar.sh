#!/bin/bash
#The proportion between asteroids and planets is 70/30
#Iterations are 50, 100, 200
#Sum of asteroids and planets is 250, 500 and 1000



echo "---ITERATIONS: 50 ---"
for i in {0..14}
do
    ./parallel 175 50 75 6
done
echo "---"

for i in {0..14}
do
    ./parallel 350 50 150 6
done
echo "---"

for i in {0..14}
do
    ./parallel 700 50 300 6
done
echo "---"

echo "---ITERATIONS: 100 ---"
for i in {0..14}
do
    ./parallel 175 100 75 6
done
echo "---"

for i in {0..14}
do
    ./parallel 350 100 150 6
done
echo "---"

for i in {0..14}
do
    ./parallel 700 100 300 6
done
echo "---"

echo "---ITERATIONS: 200---"
for i in {0..14}
do
    ./parallel 175 200 75 6
done
echo "---"

for i in {0..14}
do
    ./parallel 350 200 150 6
done
echo "---"

for i in {0..14}
do
    ./parallel 700 200 300 6
done
echo "---"

echo "Te quiero fernando tu eres mi parsero"
