# Computer Architecture Lab 2

## Assignment 1

## Assignment 2

Pseudo-Code

```
set x equal to 299
set temp = x
while x is > 1
  while temp is > 1
    divide x by temp
    if the remainder is greater than 0
      temp = temp - 1
    else
      loop again
  
  if(temp is 1)
    exit and print x
```

## Assignment 3

Pseudo-Code

```
input1 = x
if(input1 - 10 < 0) throw error
if(input1 - 500 > 0) throw error
input2 = y
if(input2 - 10 < 0) throw error
if(input2 - 500 > 0) throw error

if(y - x < 0)
  higher Num = x
  lower Num = y
else 
  higher = x
  lower 
  
if(lowerNum % 2) = 0
  then firstNum =lowerNum + 2
else
  then firstNum = firstNum + 1

temp1 = firstNum
sum=0
while temp1 < higherNum
  sum += temp1
  temp1 += 2
  
print sum
exit
```
## Assignment 4

Pseudo Code

```
input1 = get number of lines from user

for first n/2 lines:
  stars = 2i - 1
  spaces = (n + 1)/2 - i
  print all spaces
  print all stars


if( n % 2 == 0)
	i = n/2 + 1
	Set spaces using equation similar to above - except subtract max lines from row number
	Set stars using same equation as above except subtract n from row numbers

```

## Assignment 5

Pseudo Code

```
password = ""
while password doesn't match condition (< 6 or > 10)
	prompt a user for a password
	set password equal to user input
	store password into memory address

attempts = 3
match = false
Ask user to re enter password
	set whether whether passwords match by going character by character through each
	if password != match
		attempts--;

while the passwords don't match & have more than 0 attempts left
	prompt user user to enter again 
	check wheter password matches
	if != match
		attempts--;

if match is true
	print success
else
	print failure

exit
```

## Assignment 6



```
Read an input from the user of a hex value

Determine that every value in hex string is a valid hex char
	do this by checking whether the character appears in the string '0123456789ABCDEF'
	Fail and exit if it is not valid.

read characters from leftmost to rightmost
read the first character
while the character is not the nulll character
	from a table in the program, determine which character it is and print its binary representation.
	read the next character

```

## Assignment 7 

Pseudo-Code

```
Set up a large space (> 1000) for array storage
designate register $s2 as the size of the array
$s0 is the base address for the array

Prompt user for input

get user input by syscall
check input value
	if value == 0xF
		end input and go to sortAndPrint
	else 
		add 1 to length of the array
		jump back to user input prompt

sortAndPrint
	load address of array as argument 0
	load length of array as argument 1
	run bubble sort
	load address of array as argument 0
	load length of array as argument 1
	print Array
	load address of array as argument 0
	load length of array as argument 1
	jump and link printMedian
	load address of array as argument 0
	load length of array as argument 1
	jump and link printPositives
	load address of array as argument 0
	load length of array as argument 1
	jump and link to printNegatives 
	load address of array as argument 0
	load length of array as argument 1
	jump and link to printZeros
	exit
	
bubbleSort:
	set flag = true - # true when values have been switched
	while the flag is true
		set flag = false
		for (i = 0, i < length, i++)
			if i and i+1 are not in the right order
				swap value at location i and i+1
				set flag = true
			
			continue and loop again
		
		if flag == false, finished sorting
			exit
		else loop again
		
printArray
	start at 0 location up to (and not inluding length)
	loop until i = length - 1
		print out value at base+(4*i)

	jump to $ra

printPositives
	start at 0 location up to (and not inluding length)
	loop until i = length - 1
		if value at base+(4*i) > 0
			add to count

	print count
	jump to $ra

printNegatives
	start at 0 location up to (and not inluding length)
	loop until i = length - 1
		if value at base+(4*i) < 0
			add to count

	print count
	jump to $ra
	
printZeros
	start at 0 location up to (and not inluding length)
	loop until i = length - 1
		if value at base+(4*i) == 0
			add to count

	print count
	jump to $ra

	jump to $ra
printMedian
	if array length is odd
		median location is at index of length/2
		set value equal to base + (4*(length/2)) #integer division
	else
		median is average of length/2 and length/2 - 1
		set median = sum of values at both locations together
		divide median by 2
	
	print median
	jump to $ra
```



























