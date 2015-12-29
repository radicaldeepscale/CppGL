#!/bin/bash 
: '
this script, presuming that all executables and data have been readyed in the
configurable directories as follows, boost a run of the whole study of boxPang 
Interface.
--- this version is proprietarily written for pilot study.
'
BASEDIR=`pwd`

BINDIR=${BASEDIR}/bin
TASKDIRS="pretask task1 task2 task3 task4 task5 task6 task7 task8 task9"
EXECUTABLES="singleitr multiviewitr multiwindowitr"

DATADIR=${BASEDIR}/data
NORMALCASEDATA=normal.data
ABNORMALCASEDATA=agenesis.data
NORMALCASEDATA2=normal2.data
ABNORMALCASEDATA2=agenesis2.data

LOG=${BASEDIR}/studylog

function checkbins()
{
	if [ ! -d ${BINDIR} ];then
		echo "FATAL: directory ${BINDIR} NOT found, please set up firstly."
		return 1
	fi

	for ts in ${TASKDIRS}
	do
		for bin in ${EXECUTABLES}
		do
			if [ ! -s ${BINDIR}/${ts}/${bin} ];then
				echo "ERROR: executable ${bin} for ${ts} NOT found,"
			    echo "please set up firstly."
				return 1
			fi
		done
	done
	return 0
}

function checkdata()
{
	for dtidata in ${NORMALCASEDATA} ${ABNORMALCASEDATA} \
				${NORMALCASEDATA2} ${ABNORMALCASEDATA2}
	do
		if [ ! -s ${DATADIR}/${dtidata} ];then
			echo "ERROR: DTI data file ${DATADIR}/${dtidata} NOT found."
			return 1
		fi
	done

	for ts in task1 task2 task3 task4 task5 task6 task8 task9
	do
		if [ ! -s ${BINDIR}/${ts}/${NORMALCASEDATA} ];then
			echo "ERROR: DTI data file ${BINDIR}/${ts}/${NORMALCASEDATA} NOT found."
			return 1
		fi
	done

	for ts in task7 task9
	do
		if [ ! -s ${BINDIR}/${ts}/${ABNORMALCASEDATA} ];then
			echo "ERROR: DTI data file ${BINDIR}/${ts}/${ABNORMALCASEDATA} NOT found."
			return 1
		fi
	done

	for ts in task1 task2 task3 task4 task5
	do
		if [ ! -s ${BINDIR}/${ts}/fiberidx_${ts}.data ];then
			echo "ERROR: DTI data file ${BINDIR}/${ts}/fiberidx_${ts}.data NOT found."
			return 1
		fi
	done

	for ts in task6 task7 task8
	do
		if [ ! -s ${BINDIR}/${ts}/fiberidx_task3.data ];then
			echo "ERROR: DTI data file ${BINDIR}/${ts}/fiberidx_task3.data NOT found."
			return 1
		fi
	done

	return 0
}

function taskflag()
{
	echo -e "\n##############################################################" >> $LOG
	echo "                           TASK $1                            " >> $LOG
	echo -e "##############################################################\n" >> $LOG
}

#-----------------------------------------#
#
# pretask
#
#-----------------------------------------#
function pretask()
{
	: '
	this is a trivial, actually virutal, task. It just show the introductory
	message of the tasks to follow
	'
	${BINDIR}/pretask/singleitr \
		-f ${DATADIR}/${NORMALCASEDATA} \
		-t ${BINDIR}/pretask/tasktext \
		-V 1>> $LOG 2>&1
}

#-----------------------------------------#
#
# task 1 
#
#-----------------------------------------#
function task1()
{
	taskflag 1

	echo -e "\n##### with Normal case in Full-window single view #####" >> $LOG
	${BINDIR}/task1/singleitr \
	-f ${BINDIR}/task1/${NORMALCASEDATA} \
	-i ${BINDIR}/task1/fiberidx_task1.data \
	-t ${BINDIR}/task1/tasktext \
	-V 1>> $LOG 2>&1
}


#-----------------------------------------#
#
# task 2 
#
#-----------------------------------------#
function task2()
{
	taskflag 2

	echo -e "\n##### with Normal case in Full-window single view #####" >> $LOG
	${BINDIR}/task2/singleitr \
	-f ${BINDIR}/task2/${NORMALCASEDATA} \
	-i ${BINDIR}/task2/fiberidx_task2.data \
	-t ${BINDIR}/task2/tasktext \
	-V 1>> $LOG 2>&1
}
#-----------------------------------------#
#
# task 3
#
#-----------------------------------------#
function task3()
{
	taskflag 3

	echo -e "\n##### with Normal case in Full-window single view #####" >> $LOG
	${BINDIR}/task3/singleitr \
	-f ${BINDIR}/task3/${NORMALCASEDATA} \
	-i ${BINDIR}/task3/fiberidx_task3.data \
	-t ${BINDIR}/task3/tasktext \
	-V 1>> $LOG 2>&1
}
#-----------------------------------------#
#
# task 4
#
#-----------------------------------------#
function task4()
{
	taskflag 4

	echo -e "\n##### with Normal case in Full-window single view #####" >> $LOG
	${BINDIR}/task4/singleitr \
	-f ${BINDIR}/task4/${NORMALCASEDATA} \
	-i ${BINDIR}/task4/fiberidx_task4.data \
	-t ${BINDIR}/task4/tasktext \
	-V 1>> $LOG 2>&1
}
#-----------------------------------------#
#
# task 5
#
#-----------------------------------------#
function task5()
{
	taskflag 5

	echo -e "\n##### with Normal case in Full-window single view #####" >> $LOG
	${BINDIR}/task5/singleitr \
	-f ${BINDIR}/task5/${NORMALCASEDATA} \
	-i ${BINDIR}/task5/fiberidx_task5.data \
	-t ${BINDIR}/task5/tasktext \
	-V 1>> $LOG 2>&1
}
#-----------------------------------------#
#
# task 6 
#
#-----------------------------------------#
function task6()
{
	taskflag 6

	echo -e "\n##### with Normal case in Half-sized-window single view #####" >> $LOG
	${BINDIR}/task6/singleitr \
	-f ${BINDIR}/task6/${NORMALCASEDATA} \
	-i ${BINDIR}/task6/fiberidx_task3.data \
	-t ${BINDIR}/task6/tasktext \
	-V 1>> $LOG 2>&1
}
#-----------------------------------------#
#
# task 7
#
#-----------------------------------------#
function task7()
{
	taskflag 7
	echo -e "\n##### with Abnormal case in Half-sized-window single view #####" >> $LOG
	${BINDIR}/task7/singleitr \
	-f ${BINDIR}/task7/${ABNORMALCASEDATA} \
	-i ${BINDIR}/task7/fiberidx_task3.data \
	-t ${BINDIR}/task7/tasktext \
	-V 1>> $LOG 2>&1
}

#-----------------------------------------#
#
# task 8 
#
#-----------------------------------------#
function task8()
{
	taskflag 8
	cp -f ${BINDIR}/task8/${NORMALCASEDATA} ${BINDIR}/task8/${NORMALCASEDATA}.left 
	cp -f ${BINDIR}/task8/${NORMALCASEDATA} ${BINDIR}/task8/${NORMALCASEDATA}.right
	echo -e "\n##### with two normal cases in dual view #####" >> $LOG
	${BINDIR}/task8/multiviewitr \
	-f ${BINDIR}/task8/${NORMALCASEDATA}.left \
	-f ${BINDIR}/task8/${NORMALCASEDATA}.right \
	-i ${BINDIR}/task8/fiberidx_task3.data \
	-t ${BINDIR}/task8/tasktext \
	-r 0.25 \
	-V 1>> $LOG 2>&1
}

#-----------------------------------------#
#
# task 9
#
#-----------------------------------------#
function task9()
{
	taskflag 9
	echo -e "\n##### with normal(left) and abnormal(right) cases in dual view #####" >> $LOG
	${BINDIR}/task9/multiviewitr \
	-f ${BINDIR}/task9/${NORMALCASEDATA} \
	-f ${BINDIR}/task9/${ABNORMALCASEDATA} \
	-i ${BINDIR}/task9/fiberidx_task3l.data \
	-i ${BINDIR}/task9/fiberidx_task3r.data \
	-t ${BINDIR}/task9/tasktext \
	-r 0.25 \
	-V 1>> $LOG 2>&1
}

function costOfThisTask()
{
	s=$1
	e=$2
	((d=e-s))
	((h=d/3600))
	((m=d%3600/60))
	((s=d%3600%60))
	echo " Time cost of this task: $h hours $m minutes $s seconds." >> $LOG
}

#####################################################################################
###    Task ordering and main flow control
###
#####################################################################################

checkbins
if [ $? -ne 0 ];then
	exit 1
fi

checkdata
if [ $? -ne 0 ];then
	exit 1
fi

> $LOG

bstart=`date +%s`

curstart=`date +%s`
pretask
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

curstart=`date +%s`
task1
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

curstart=`date +%s`
task2
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

curstart=`date +%s`
task3
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

curstart=`date +%s`
task4
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

curstart=`date +%s`
task5
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

curstart=`date +%s`
task6
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

curstart=`date +%s`
task7
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

curstart=`date +%s`
task8
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

curstart=`date +%s`
task9
curend=`date +%s`
costOfThisTask ${curstart} ${curend}

end=`date +%s`
echo -e "\n################## ALL FINISHED #######################" >> $LOG
((d=end-bstart))
((h=d/3600))
((m=d%3600/60))
((s=d%3600%60))
echo " Time cost: $h hours $m minutes $s seconds." >> $LOG

echo "All finished, thank you!"
echo

exit 0

