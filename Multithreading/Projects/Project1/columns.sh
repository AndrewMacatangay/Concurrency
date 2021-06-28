awk '{print NF}' output.txt | sort -nu | head -n 1 
awk '{print NF}' output.txt | sort -nu | tail -n 1
