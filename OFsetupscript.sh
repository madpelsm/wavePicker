H=(0.189 0.38 0.8 0.728 0.9 0.9)
T=(0.86 1.19 8.44 11.94 2.67 5.3425)

arraysize=${#H[@]}


for ((i=0; i<${arraysize};i++ ))
do
    ./WaveTheoryPicker analysis=2 d=1.4 H=${H[$i]} T=${T[$i]} graph=mehaute_marked.png
    echo mkdir H${H[$i]}T=${T[$i]}
    echo cp cob_2D_basic/* H${H[$i]}T=${T[$i]}
    echo mv waveDict H${H[$i]}T=${T[$i]}/constant
    echo cd H${H[$i]}T=${T[$i]}
    echo cd ./allRun
    echo cd ..
done

