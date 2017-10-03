for file in * 
do 
if grep -q print $file then 
	echo $file
fi
done 
exit 0