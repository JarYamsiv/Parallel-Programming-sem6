if [[ "$1" == "1" ]]; then
  sleep 5
  echo q


elif [[ "$1" == "2" ]]; then
  sleep 5
  echo p 0 0
  sleep 5
  echo q

elif [[ "$1" == "3" ]]; then
  sleep 0
  echo p 100 100
  sleep 3
  echo p 100 100
  sleep 3
  echo p 200 0
  sleep 2
  echo q

elif [[ "$1" == "4" ]]; then
  sleep 5
  echo p 200 200
  sleep 17
  echo q

  
fi