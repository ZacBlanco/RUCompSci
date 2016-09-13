gcc -o words.out assign0.c -g
if [ $? == 0 ]; then
# Test Cases!!!

# Empty/Missing Argument
#gdb -ex=r --args ./words.out

# The first assignment sheet test case
#./words.out "thing stuff otherstuff blarp"


# The second assignment sheet test case
#gdb -ex=r --args ./words.out "thing1stuff3otherstuff,blarp"
./words.out "thing1stuff3otherstuff,blarp"

# Some more cases
#./words.out "la-dee-da"

#./words.out "do-re-me fa =so   la ------ tee do"
fi
