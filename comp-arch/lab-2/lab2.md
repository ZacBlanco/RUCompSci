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



























