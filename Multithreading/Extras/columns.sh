awk '{print NF}' test.txt | sort -nu | head -n 1 
awk '{print NF}' test.txt | sort -nu | tail -n 1
