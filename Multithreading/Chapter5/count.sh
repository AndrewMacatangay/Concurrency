awk '{count[$3]++} END {for (x in count) print x, count[x]}' < output.txt
