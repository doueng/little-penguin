insmod main.ko
DDIR=/sys/kernel/debug/fortytwo
ID=$DDIR/id
JIFF=$DDIR/jiffies
FOO=$DDIR/foo
cat $ID
echo rofl > $ID
echo -n dengstra > $ID
cat $JIFF
cat $JIFF
echo hello > $FOO
cat $FOO
python -c "print('a'*0x4000)" > $FOO
rmmod main.ko
exit
