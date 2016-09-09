gcc -o words.out assign0.c

# Test Cases!!!

# Empty/Missing Argument
./words.out

# The first assignment sheet test case
./words.out "thing stuff otherstuff blarp"


# The second assignment sheet test case
./words.out "thing1stuff3otherstuff,blarp"


# Some more cases
./words.out "la-dee-da"

./words.out "do-re-me fa =so   la ------ tee do"
