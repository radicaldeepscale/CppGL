#!/bin/bash 
: '
this script is written to check the results after the user finished all tasks
and left the results in the directory belonging to corresponding task
--- this version is proprietarily written for pilot study.
'
BASEDIR=`pwd`

BINDIR=${BASEDIR}/bin
TASKDIRS="task1 task2 task3 task4 task5 task6 task7 task8 task9"
echo
for ts in ${TASKDIRS}
do
	echo "Check result associated with $ts..."
	for eachdata in `ls ${BINDIR}/${ts}/region_*`
	do
		echo -e "\t current data - $eachdata"
		${BINDIR}/${ts}/singleitr -f $eachdata -r 0.25 2>/dev/null
	done
done

exit 0

