gcc -Wall -O -o pointersorter pointersorter.c
if [ $? == 0 ]; then
  ########################
  #      Test Cases      #
  ########################

  # Empty/Missing Argument
  ./pointersorter

  # The first assignment sheet test case
  ./pointersorter "thing stuff otherstuff blarp"

  # The second assignment sheet test case
  #gdb -ex=r --args ./pointersorter "thing1stuff3otherstuff,blarp" #DEBUG
  ./pointersorter "thing1stuff3otherstuff,blarp"

  # Test characters with non-space character
  ./pointersorter "la-dee-da"

  # Test with multiple non-alpha characters
  ./pointersorter "do-re-me fa =so   la ------ tee do"

  # Test with capitals and lowercase
  ./pointersorter "- WORLD hello world--HELLO 123WoRlD"

  #Test beginning with non alphabetic
  ./pointersorter "- hello world"

  #Test Ending with non-alphabetic
  ./pointersorter "hello there world--"

  #Test multiple non-alpha beginnings
  ./pointersorter "-----hello there humans"

  #Test multiple non-alpha endings
  ./pointersorter "we have come peacefully  ---123123"

  # Test Empty input String
  ./pointersorter ""
  
  # Test all non-alphabetic standard keyboard input
  ./pointersorter "\`1234567890-=~!@#$%$^&*()_+,./\\][';<>?|}{\":"



  which valgrind
  if [ $? == 0 ]; then
    
    #########################################################
    # Test for memeory leaks among just a couple test cases #
    #########################################################
    echo "Testing for memory leaks with valgrind"
    
    #Test multiple non-alpha endings
    valgrind --leak-check=yes ./pointersorter "we have come peacefully  ---123123"

    # Some lorem ipsum
    valgrind --leak-check=yes ./pointersorter "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
    
    # Test all non-alphabetic standard keyboard input
    valgrind --leak-check=yes ./pointersorter "\`1234567890-=~!@#$%$^&*()_+,./\\][';<>?|}{\":"
    
  fi
  
fi
