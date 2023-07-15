all=0
ct=0
flag=0
flag2=0
handler()
{
ct=`expr $ct + 1`
if test $flag -eq 0
	then echo "Please write down directory name"
fi
if test $ct -eq 2
	then if test $flag -eq 0
		then flag2=2

	     	else if  test $all -gt 0
			then
				flag2=0
				echo
				echo $all
			else echo "We cant found out need fail"
		     fi
	     fi
fi
}

trap "handler" 2

echo "Please write down directory nammme:"
read name
flag=1
names=`find $name -type f -atime -5 -size +4b | tr '\n' ' '`
for i in $names
do
c=`stat $i -c%b`
all=`expr $all + $c`
echo $i
done
if test  $flag2 -eq 2
	then
		echo
		echo $all
fi


