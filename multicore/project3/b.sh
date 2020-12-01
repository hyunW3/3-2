for var in $(seq 5)
do
    cat output$var.txt | sort > output_serial_$var.txt
done
#mv output_serial_*.txt ./output/
#diff output_serial_1.txt output_1.txt