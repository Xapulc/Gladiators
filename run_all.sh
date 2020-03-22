g++ find_min_n_best.cpp

array1=(1.3 1)
array2=(1.2 2)
array3=(1.12 3)
array4=(1.09 4)
array5=(1.07 5)
array6=(1.057 6)
array7=(1.048 7)
array8=(1.042 8)
array9=(1.037 9)
array10=(1.034 10)
array11=(1.031 11)
array12=(1.028 12)
array=(${array1[@]}
       ${array2[@]}
       ${array3[@]}
       ${array4[@]}
       ${array5[@]}
       ${array6[@]}
       ${array7[@]}
       ${array8[@]}
       ${array9[@]}
       ${array10[@]}
       ${array11[@]}
       ${array12[@]})
let "total=${#array[@]} / 2"

for (( i=0; i < total; i++ ))
{
  let "j=2*i"
  strength=${array[j]}
  let "j=2*i+1"
  optimal=${array[j]}
  echo "s=${strength}, k=${optimal}"
  ./a.out ${strength} ${optimal} 1000 50
}

#for var in ${array[@]}
#do
#  ./a.out ${var} ${var} 100 10
#done