for value in "static" "dynamic" "guided" "auto"
do
    export OMP_SCHEDULE=$value
    echo "\n"
    echo "=====" $value "====="
    ./gaussian_elimination 42000 1
done