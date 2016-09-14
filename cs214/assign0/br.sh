gcc -o words.out pointersorter.c -g
if [ $? == 0 ]; then
# Test Cases

# Empty/Missing Argument
./words.out

# The first assignment sheet test case
./words.out "thing stuff otherstuff blarp"

# The second assignment sheet test case
#gdb -ex=r --args ./words.out "thing1stuff3otherstuff,blarp" #DEBUG
./words.out "thing1stuff3otherstuff,blarp"

# Test characters with non-space character
./words.out "la-dee-da"

# Test with multiple non-alpha characters
./words.out "do-re-me fa =so   la ------ tee do"

# Test with capitals and lowercase
./words.out "MOM dad, hello, hEllo world mom DAD"

#Test beginning with non alphabetic
./words.out "-hello world"

#Test Ending with non-alphabetic
./words.out "hello there world-"

#Test multiple non-alpha beginnings
./words.out "-----hello there humans"

#Test multiple non-alpha endings
./words.out "we have come peacefully  ---123123"

fi
