Secure Project
==============

General Description
-------------------
# Primary use
The program primarily serves to solve hashed passwords. The passwords are
provided by the user, in a binary file. The specific formatting of the file
can be found under: "Structure of the Program / File Formatting / Hash".
    There are four different tests that can be run on hashed passwords:
Dictionary test, Rainbow table, 1337-test and Brute force. More infromation
about these tests can be found under: "General Description / Tests". These
tests can also be toggled on and off.
    The tests will be run, until the hashed password is solved, or the tests
run out. If the hash is solved, then the plain text password is printed on the
command line. Otherwise, the program will infrom the user that the hash was not
solved.

# Secondary use
The user can also manually insert a password on the command line. The password
will then be hashed with SHA256, and all toggled tests will be run on this hash.
This serves to test the strength of the password.

# Tests
1. Dictionary test
    - One by one hashes commonly used passwords and compares the hash to the
      one we are trying to solve
    - The list of common passwords is user provided
2. Rainbow table
    - Same as the dictionary test, except the passwords come pre-hashed
    - Can only be used, when digest lengths of the hashed password and the
      rainbow table match
    - Also cannot be used, when a salt has been set
3. 1337-test
    - Takes an initial quess provided by the user, and goes through all
      permutations of 1337-replacements for characters
        - Matias -> M@7ias or M@ti@s or matias etc.
    - List of 1337-replacements under: "Classes / Decrypter / Misc / '1337' -table"
    - Can only be used, if an initial quess has been set
4. Brute force
    - Hashes all possible permutations of available characters and compares
      them to the hashed password
    - List of characters under: "Classes / Decrypter / Misc / chars_"
    - If an initial quess has been set, places characters after the quess
        - Matias -> MatiasA1 or Matias4f@ etc.
    - Only goes through as many characters as the user specifies
        - If brute depth has been set to three, then only goes through all
          permutations of a string of any three characters




Structure of the Program
-------------
# Foreword
In this chapter I will go further into detail on why I ended up doing things the way
I did and some other good-to-know information about my classes. I will discuss what
method calls what, and what dependencies there are

# Decrypter
The central class for this program. It requires the Sha-class to work, but is otherwise
independent. The name is misleading, as this class is not used to decrypt anything.
Rather, it is used to solve hashed passwords. Further details can be found under:
"General Description"
## Public
### Constructor and destructor
- No noteworthy function. They exist, because classes must have these two methods
### test_password
- The user inserted password is first salted and then hashed with SHA-256. The program
  then runs run_tests on the hashed password
- Like the name suqqests, this method can be used to test the strength of a password.
  If the program can solve the hash, it means that the password is probably not strong
  enough
- Before I had hash-files implemented in the early stages of development, I used this
  method to test the various tests (dictionary, rainbow, etc.). While not necessarily
  useful compared to solve_hash_file, it is a functional feature, so I just left it in
### solve_hash_file
- The user inserts a filename. This file is then opened and various tests are run on it
  with test_hash_file. If the tests are passed, then the program runs run_tests on the
  hashed passwords one at a time
- The is the primary feature of this program. You can read more about it under:
  "General Discussion / Primary use"
- This method wasn't always intended to be the way it is. Back when I was more
  ambitious about what I could achieve, this would take a password-locked zip-file as
  an input. Then it would extract the hash and solve it. I quickly realized that
  I could not do that, and settled for the current implementation
- The current implementation was not easy either. Reading hashed passwords, into arrays,
  from a binary file, took a lot of time to figure out. More about this under:
  "Structure of the Program / File Formatting / Hash" and
  "Secure Programming Solutions / Hash-files"
### print_options
- Prints all variables and tests. Also prints which tests are enabled
- Quick and easy way for the user to check their configuration
### set_wordlist
- The user inserts a filename. The method then tries to open this file. If it can, then
  it is set as the wordslist_
- This is just a setter with a simple validity check
### set_rainbow_table
- The user inserts a filename. The method then tries to open this file, and if it can,
  then a more thorough test is run on the file with test_rainbow_table. If it passes,
  then the filename is set as rainbow_table_
- This is a setter, with a more thorough validity check
### set_brute_depth
- Takes a user input and tries to turn it into an integer. If the integer is larger than
  zero, then set it as brute_depth_
- Solving a hashed password with brute force is exponentially more difficult for each
  additional character. In the case of my program, it takes about 70-times longer to run
  the entirety of brute_attack for each additional character. This is why I'm giving the
  user a choice to limit the depth of the brute attack
### set_brute_quess
- Takes a user input and sets it as the start_quess_. If the user input is empty, resets
  the start_quess_
- The start_quess_ is used for both 1337 and brute
    - Not necessary for brute, but mandatory for 1337
- Helps the program pinpoint a password that the user might half-remember
### set_tests
- Opens a menu, where the user can toggle tests on and off
- Tests like brute and 1337 can be time consuming, and rainbow tables / dictionary attacks
  can be useles in some situations. To save time and screen space, the user can enable and
  disable these tests
- It works by setting the truth-value for each test in tests_
- This feature was also originally meant for testing in the early stages for development.
  Yet it proved to be useful enough for me to keep it in the finished product
    - It is not difficult to see from what this method looks like, that it was not meant to
      be final
### set_salt
- Takes a user inserted salt as a hexadecimal, and tries to convert it into binary with
  hex_to_bin. If it works, then the binary is set as the salt
- The hex is inserted without the 0x
- I felt that this is the most convenient way for the user to insert a salt. The other
  options I considered were a string and a binary file. The string would have
  limited possible salts significantly, and the binary file would have eaten into user
  experience
    - Also, I did not want to make another "test_x"-function
- This is the last feature I implemented. This reflects in the fact that salts are not
  mentioned all that much in the program or the documentation
    - While salts are not mentioned, they do exist in all tests

## Private
### run_tests
- Takes a hashed password and runs all toggled tests on it. Returns, if a test is
  succesful
- Runs a for-loop through tests_. Then runs the test if .toggled = true
    - More information under: "Structure of the Program / Decrypter / Misc / tests_"
- This method mainly exists to trim down on duplicate code
### rainbow_attack
- Takes a hashed password and runs a rainbow table -attack on it
    - More information under: "General Description / Tests"
- Before the table is read, test_rainbow_table is run again. This helps prevent
  errors from renaming and modifying files
- This was difficult to make work correctly. More information under:
  "Structure of the Program / File Formatting / Rainbow Table" and
  "Secure Programming Solutions / Rainbow Tables"
### dictionary_attack
- Takes a hashed password and runs a dictionary attack on it
    - More information under: "General Description / Tests"
- The passwords are read from wordslist_
- The first test I implemented. Also the simplest
### leet_attack
- Takes a hashed password and runs a "1337" -attack on it
    - More information under: "General Description / Tests"
- Calls leet_recursive to solve the hashed password
- Starts with start_quess_
  - Can't start if start_quess_ is empty
### leet_recursive
- A recursive help-method for the "1337" -attack
- Starting from the first character of start, the character is removed and leet-
  alternatives are one by one added to the end of end. Then leet_recursive is
  called with the shorter start and the longer end. When start is empty, end is
  hashed and compared to the hashed password
- Recursion is by far the easiest way to go through all possible permutations of
  anything
### brute_attack
- Takes a hashed password and runs a brute force attack on it
    - More information under: "General Description / Tests"
- Calls brute_recursive to solve the hashed password
- Starts with start_quess_
- Also checks if the hashed password is the starting quess
### brute_recursive
- A recursive help-method for the Brute force attack
- Adds a character from chars_ to the end of brute, then calls brute_recursive
  again, with depth+1. When brute_depth_ is reached, hashes brute and compares
  it to the hashed password
- Recursion is by far the easiest way to go through all possible permutations of
  anything
### test_rainbow_table
- Tests the validity of a rainbow table
- I'll go more into detail in:
  "Structure of the Program / File Formatting / Rainbow Table" and
  "Secure Programming Solutions / Rainbow Tables"
- This is run during set_rainbow_table and rainbow_attack
### test_hash_file
- Tests the validity of a hash file
- I'll go more into detail in:
  "Structure of the Program / File Formatting / Hash" and
  "Secure Programming Solutions / Hash-files"
### hex_to_bin
- Turns a hexadecimal into a binary vector
- Unlike elsewhere in my program, the binary salt is stored in a vector, not an
  array
    - The main reason for this is convenience on my part. It was easier to
      divide the salt into bytes inside a vector and then append those onto
      the password, than do the same with an array. And because this was such
      a late addition, I figured that the easier way would result in the
      program being more likely to work afterwards, as compared to me trying to
      do something fancy with arrays
### add_salt
- Adds a salt to the end of a password
- One by one, pushes a byte of the salt onto the password
- Making this its own method made it easier to implement salting in the program.
  I just had to go to every place where I hash anything and call add_salt before

## Misc
### Internal variables
- wordslist_, rainbow_table_, brute_depth_, start_quess_ and salt_
- wordlist and initial quess used to be called wordslist and start quess
  - The cli and descriptions were changed, but the variables stayed
### chars_
- All characters used in the Brute force attack
- abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*_-+=?
- I played it safe with the special characters, but some of them are most
  likely useless
    - Characters like - and + are not common in passwords, and are sometimes
      even banned by whatever service the password is being made for
### Buffers
- These are used in the Sha_struct to store the hashed passwords from the SHA-
  algorithms
- Beyond test_password, these are the only non-dynamically allocated buffers
  for hashed passwords. There are two reasons for this:
    1. I know how large the hashed password is. It will always be the size of
       the digest length of the SHA-algorithm associated with the buffer.
       This means that a buffer overflow is not possible
    2. I made the Sha_struct early on in development. By the time that I
       started using dynamic memory allocation, this implementation was so
       deeply ingrained in the program, that changing it was not possible
       without major rewrites of many parts of the program
        - Also not necessary, because the possibility of a buffer overflow
          is minimal
### Sha_struct
- Contains the name of a SHA-function, the buffer for the hashed password,
  and a function pointer to the corresponding SHA-function in the Sha-class
- While function pointers are difficult to read, they proved to be extremely
  useful in making the overall code more legible and compact
    - Also modularity, because I do not have to use if-statements. I can
      just add a new SHA-function to sha_ and it works
- Also storing the hashed passwords inside the struct, means that I do not
  have to create a new array every time I hash a password. This also helps
  with legibility, compactness and modularity
- The names are mainly used for various error-messages
### Fcn_struct
- Contains a pointer to a test (dictionary, rainbow, etc.) in Decrypter, and
  a truth value, telling if it is toggled
- Once again, I find this to be more legible, compact and modular than a
  bunch of if-statements
    - I can just add tests to tests_, and it works
    - The only problem is toggle_tests, which is about as far from modular
      as possible
### "1337" -map
- Contains all conversions from a character to its 1337 -variant
    - Including the character itself in both upper- and lowercase
        - Helped with programming
- More conversions can be added
- The key for the map is a lowercase letter
### sha_ -map
- Contains a digest-length and the corresponding Sha_struct
- Digest-lengths are used to differentiate Sha_structs from each other
- Like said in Sha_struct, this makes adding new hash-functions easier
### tests_ -map
- Contains the name of a test and the corresponding Fcn_struct
- Names are used as keys. However, the map is usually just iterated through
- Like said in Fcn_struct, this makes adding new test-functions easier
### Function pointers
- sha_func_ptr and dec_func_ptr
- The actual pointers are very long to write. With using we can make these
  shortcuts


# Cli
Command line interface. This class is made to operate Decrypter from the
command line. Naturally depends on Decrypter
## Public
### Constructor and destructor
- Makes and destroys a new Decrypter -object
### run
- The user's main contact with the program. They can insert commands, which
  run will interpret and call the corresponding function in Decrypter
- q and h for quit and help
    - q returns false and h calls print_help
    - An empty input refers the user to press h
- Uses split_string and check_command to get the command from cmds_
- I usually use "always returns true, unless q, then false", when making a
  user interface

## Private
### check_command
- Goes through all available commands and checks which one corresponds to
  user input. Returns a pointer to that Cmd-struct
    - As in, checks the first word of the user's input and compares it to
      the .command attribute of all elements of cmds_
- This makes accessing available commands, and checking which commands do
  not exist, easy
### print_help
- Goes through all commands in cmds_, and prints .command and .command_exp
- Not all of the commands are intuitive at first glance. This necessitates
  the user being able to get help
### split_string
- Takes a string and a separator character as inputs, and splits the
  string at the separator character
- Because getline returns the command as a single string, there must be
  a way to split it
    - Some Decrypter -functions take input parameters as a vector. This is
      why split_string returns a vector of the split words

## Misc
### Cmd
- Contains the command, an explanation, an example parameter, and the
  pointer to a function
- The command is used in both check_command and print_help
- The explanation is used in print_help
- The example parameter is checked to see if the user input the correct
  amount of parameters
    - In the final version of this program, all the functions take either
      one or zero parameters. I prepared for some to take multiple, which
      is why the example parameter is a vector
    - This is also why all public Decrypter -functions take their
      parameters in a vector
- The pointers do not point to Cli -functions. Those are handled
  separately
### cmds_
- A vector of all available commands
- Makes handling and adding commands easier
### dec_
- The Decryptor -object we use to access the Decryptor -class
- Is a pointer
### Function pointer
- The actual pointer is very long to write. With using we can make this
  shortcut



# Sha
This class exists, because I could not turn OpenSSL's SHA -functions
into function pointers
## Public
### Constructor and destructor
- Exist, because classes must have a constructor and a destructor
### SHA-224 - SHA-512
- All hashing algorithms used by Decryptor
- Like discussed previously, pointers made Decrypter better. However,
  the SHA -functions that came with OpenSSL were difficult to turn
  into function pointers. That is why I made this class. The
  function pointers were easier to make this way



# File Formatting
Both of the binary files have a very strict format that they must
follow. This is to make reading them possible. Both files rely on
a "block" -structure. This means that the hashed passwords exist
in their own "blocks" and are treated separately from each other
## Hash
| Am ||D||  Hash  ||D||  Hash  ||D||  Hash  || ... ||D||  Hash  |
- Amount
    - The first four bytes tell how many hashed passwords the file
      contains
- Block
    - Digest length
        - One byte, that tells the digest length of the following
          hash
        - Also identifies the hash -function used
    - Hash
        - The hashed password
        - Is exactly "Digest length" bytes long
## Rainbow Table
|D|| Am ||  Hash  ||P|| Pass ||  Hash  ||P|| Pass || ... ||  Hash  ||P|| Pass |
- Digest length
    - One byte, that tells the digest length of the entire table
        - All passwords are hashed with the same algorithm
- Amount
    - The next four bytes tell how many hashed passwords the file
      contains
- Block
    - Hash
        - The hashed password
        - Is exactly "Digest length" bytes long
    - Password length
        - One byte, that tells the length of the following
          password in bytes
    - Password
        - Is exactly "Password length" bytes long
        - Is plain text written in binary




Secure Programming Solutions
----------------------------
# OWASP Top 10
While I tried to follow the OWASP top 10, I quickly found out that a
large number of the outlined points do not apply to my program. This
is because the area of attack on my program is relatively small: I do
not have servers, SQL-databases, users, authentication locked features,
nor a large amount of outside dependencies. This means that while I
considered every point, I did not manage to apply all of them
## A01:2021-Broken Access Control
- Broken Access Control means that the system fails to properly enforce
  restrictions on what authenticated users can do
- There is no authentication
    - I considered that I could implement authentication to the program.
      This way you could save your settings and cracked passwords. This,
      however, would be a massive security risk compared to the slight
      inconvenience of redoing the settings every time. I would have
      to implement authentication, which would require storing user
      information, like passwords and usernames, for a slight
      convenience
    - Then again, you could also have features locked for certain users.
      This makes no sense either, because why would you want to restrict
      certain users' ability to use the program? Maybe lock out problem
      users, but then again, what is a problem user? Maybe, if in the
      future I decided to monetize this program, then I would implement
      user authentication and different levels of users. But, as it
      stands, user authentication would be unnecessary at best and a 
      security risk at worst
- The closest thing to preventing Broken Access Control would be things
  like class public-private -separation. I tried to design it in a way,
  where users could not access methods that were not explicitly necessary
  to use the program. Then again, I do not think that that really counts
  for this point

## A02:2021-Cryptographic Failures
- Cryptographic Failures means that the system fails to properly use
  cryptographic algorithms to protect sensitive data
- I do not store sensitive data
    - This is an interesting problem. Are the passwords that this program
      solves considered sensitive data? The entire point of this program
      is to undermine cryptographic security, so I would argue that no. I
      do not store the plain-text passwords anywhere, so that is not an
      issue either
    - What about the hash-files? Well they do contain the amount of
      passwords and the digest lengths for each hash, making them easy to
      decipher. Then again, the passwords themselves are hashed and
      hopefully salted, so they are relatively safe. The question here is,
      am I causing a security issue, by making someone make a file like this?
      I am going to argue that no, because the explicit purpose of this
      program is to solve those hashes, and the intention of the user is to
      undermine the cryptographic security of those passwords. I think, in
      this case, the burden of keeping the file safe is on the user
    - The last major point is displaying the passwords. When the program
      solves the hashed password, the plain text password is displayed on
      the command window. I tried to figure out other alternative ways of
      giving the user the solved password, but they all felt worse in some
      way. The core risk behind the password being displayed on the command
      window, is that someone would be:
        a. Monitoring the screen with some malware
        b. Watching the screen behind the user
      In both cases, I cannot come up with a solution, that would mitigate
      either. That's why I picked the lesser evil, and displayed it

## A03:2021-Injection
- Injection means that malicious data is sent to the interpeter as part of
  a command or query, to cause unintended commands to be run
- SQL -injection
    - I do not have SQL
- Cross site scripting
    - I do not have a website
- Command injection
    - The commands are specific and only accept a specific number of parameteres
    - The parameters are sanitized

## A04:2021-Insecure Design
- A broad category, where security issues come from design oversights and not
  implementation
- It is difficult to quantify, as to how secure design was implemented. I feel
  that the rest of this document can answer that

## A05:2021-Security Misconfiguration
- Somewhat difficult to explain beyond the name, so I'm going to go down the list
  on OWASP's site, where applicable
- Unnecessary features
    - While there are features, that are not necessarily required, their existence
      is not a security risk. A prime example of this is test_password. It uses the
      exact same algorithms as solve_hash_file, and is simpler. The feature, in this
      case, poses a smaller security risk than the feature this program is built
      around. This makes its existence acceptable
- Error handling
    - Error handling does not reveal any unnecessary information
- Software is out of date or vulnerable
    - More on this in: A06:2021

## A06:2021-Vulnerable and Outdated Components
- Using components that are outdated, or with known vulnerabilities
- I am using OpenSSL 3.2.1, which is the latest release of a specific branch of
  OpenSSL
- OpenSSL is one of the most commonly used cryptographic libraries
    - I got it from openssl.org
- Otherwise, I am only using c++ and standard libraries
    - Also, I made sure to not use functions, with known security issues, like gets

## A07:2021-Identification and Authentication Failures
- Confirmation of the user's identity
- Once again, I do not have users

## A08:2021-Software and Data Integrity Failures
- "Software and data integrity failures relate to code and infrastructure that
  does not protect against integrity violations" -OWASP
- OpenSSL is one of the most commonly used cryptographic libraries
    - I got it from openssl.org
- I will discuss handling untrusted sources under Hash-files and Rainbow Tables

## A09:2021-Security Logging and Monitoring Failures
- Insufficient logging, detection and active response
- My program has faults here
    - While error cases are caught and handled appropriately, the error messages
      are not always descriptive enough. This is because some parts of my program
      test a wide variety of different issues under the same error category. This
      leads to issues being classified as the same, while their causes are different
    - I also do not keep logs of inappropriate behaviour

## A10:2021-Server-Side Request Forgery
- I do not have servers



# Hash-files
There are six tests that I run on a hash-file, to verify its integrity:
1. Does the file open
    - Self explanatory
2. Can the amount of passwords / blocks be read
    - All binary can be interpreted as a number, so this mostly checks if the first
      four bytes have anything in them. If not, the file is not accepted
3. Does the digest length match a valid one
    - We have all the valid digest lengths and their matching SHA -functions saved
      in sha_ (see: Structure of the Program / Decrypter / Misc / sha_)
    - If the digest length does not match, then the block is skipped
        - As in, the hash is not run through tests
4. Is the digest length zero
    - This is a special case, because even if the program skips the next hash due to
      an incorrect digest length, it will still use said digest length to know how far
      to skip. If that digest length is zero, then it would create an infinite loop.
      This is why, if a digest length is zero, the file is not accepted
5. Does the amount of blocks read match the amount of blocks at the start of the file
    - While reading the file, the program keeps track of how many blocks it thinks it
      has read. If, after reading has finished, the amount of blocks read does not
      match the four-byte number at the start of the file, then the file is not
      accepted
    - More under: Security Testing / Files / Hash-files
6. Is the entire file read
    - The program keeps track of where in the file it is with tellg. If, after
      reading has finished, the program is not at the end of the file, then something
      was not read correctly. This leads to the file not being accepted
    - More under: Security Testing / Files / Hash-files



# Rainbow Tables
There are five tests that I run on a rainbow table, to verify its integrity
1. Does the file open
    - Self explanatory
2. Does the digest length match a valid one
    - We have all the valid digest lengths and their matching SHA -functions saved
      in sha_ (see: Structure of the Program / Decrypter / Misc / sha_)
    - If the digest length does not match, the file is not accepted
3. Can the amount of passwords / blocks be read
    - All binary can be interpreted as a number, so this mostly checks if the
      following four bytes can be read. If not, the file is not accepted
4. Does the amount of blocks read match the amount of blocks at the start of the file
    - While reading the file, the program keeps track of how many blocks it thinks it
      has read. If, after reading has finished, the amount of blocks read does not
      match the four-byte number at the start of the file, then the file is not
      accepted
    - More under: Security Testing / Files / Rainbow tables
5. Is the entire file read
    - The program keeps track of where in the file it is with tellg. If, after
      reading has finished, the program is not at the end of the file, then something
      was not read correctly. This leads to the file not being accepted
    - More under: Security Testing / Files / Rainbow Tables
6. Does the hash match the password
    - When the program matches the hash to the hashed password, it tests if the hash
      matches its associated plain-text password. If not, an error is printed and the
      tests continue




Earlier Work
------------
This project is not based on earlier work




Security Testing
----------------
# Input testing
Input testing was done manually. This was done by inserting expected inputs, completely
wrong inputs, edgecases, and combinations of these
## Commands
- All commands work
- Commands do not accept the wrong amount of parameters
    - Commands that take zero commands, only take zero etc.
- Only accepted commands work

## test_password
- Password must have a length longer than zero
- Passwords work with special characters

## set_wordlist
- Filename must have a length longer than zero
- File can be interpreted as a wordlist, no matter what they are
    - Reading a non-wordlist file does not break the system



# Hash-files
1. Does the file open
    - Different files were created and different filenames were tested. If a file with
      a given filename existed, it could be opened
2. Can the amount of passwords / blocks be read
    - There were three different types of files created (there were multiple per type)
        1. Empty (no multiples)
        2. Small (less than four bytes)
        3. The amount was not correct
      The first two tests resulted in the program not being able to read the amount of
      hashes, which caused it to reject the file. The last test passed the amount test
      but usually caused latter tests to fail
3. Does the digest length match a valid one
    - Files were created, where the digest length of the hash would not match any
      pre-accepted digest lengths. This resulted in the block being skipped, where ever
      the error occured
4. Is the digest length zero
    - A file where any digest length is zero is always rejected
5. Does the amount of blocks read match the amount of blocks at the start of the file
    - Three different types of hash-files were created
        1. The amount was not correct
        2. There is a formatting error in a hashed password, causing it to be too long
           or too short
        3. The length of the hash is written incorrectly
      The amount not being correct always ended up with the file being rejected at the
      end, except for some combination tests. The latter two tests, however, caused
      varying behaviour, mainly dependent on how early the error occured. If the error
      happened early, the program would usually skip more blocks but also stabilize
      at some point, as in it would accidentally hit a digest length, and continue
      normally. If the error happened in the middle, then there would usually be less
      skips, but it wouldn't stabilize as often either. In the end, very few skips and
      it almost never stabilized. Overall, this method of counting blocks managed to
      catch a significant portion of faulty files
6. Is the entire file read
    - Two different types of hash-files were created
        1. There is a formatting error in a hashed password, causing it to be too long
           or too short
        2. The length of the hash is written incorrectly
      The results follow the previous tests. If the error happened early on, the entire
      file would most likely be read, where as in the middle or in the end, the program
      would usually not read the entire file, causing this test to fail
7. Combinations and special notes
    - The tests rely on wrong bytes being read usually resulting in larger numbers than
      the digest lengths that they are interpreted as. This causes the program to read
      more than it should, which means that blocks are skipped. However, this is not
      always the case, and in very rare situations the file stabilizes, before it can
      skip any blocks. This does not break the file however. Because if the file
      stabilizes before it skips a block, it means that only one or two blocks were
      corrupted. Better yet, these blocks can be handled by the program as faulty,
      because it can recognize incorrect digest lengths, so it will just skip them. In
      the exceedingly rare situation, where a deformed hash has a correct digest length,
      the program will treat it as normal, and will just not find a password for it.
      The point here is, that while the system is not immune to faulty hash-files, it
      is exceedingly resistant to them doing actual damage
    - Also, if the very last block has a formatting issue, it will lead to one of three
      possible outcomes
        1. The last block is smaller than it is supposed to be
            - The amount of blocks is correct, but the end of the file is not read
        2. The last block is larger than it is supposed to be
            - The amount of blocks is not correct
        3. The formatting errors cancel each other out
            - The program will interpret the last block as having an incorrect digest
              length


# Rainbow Tables
1. Does the file open
    - Different files were created and different filenames were tested. If a file with
      a given filename existed, it could be opened
2. Does the digest length match a valid one
    - Files were created, where the digest length of the hash would not match any
      pre-accepted digest lengths. This resulted in the file being rejected every time
3. Can the amount of passwords / blocks be read
    - There were three different types of files created (there were multiple per type)
        1. File empty after digest length
        2. Small digest length (less than four bytes) and empty file
        3. No amount
        4. Small digest length (less than four bytes) and full file
        5. The amount was not correct
      The first two tests resulted in the program not being able to read the amount of
      hashes, which caused it to reject the file. Tests three and four usually caused
      some further issues, similar to points 4 and 5 (next two tests). The last test
      passed the amount test but usually caused latter tests to fail
4. Does the amount of blocks read match the amount of blocks at the start of the file
    - Four different types of rainbow tables were created
        1. The amount is incorrect
        2. There is a formatting error in a hashed password, causing it to be too long
           or too short
        3. The length of the password is incorrect
        4. There is a formatting error in a password, causing it to be too long or too
           short
      The results are identical to the tests done on hash-files. If the amount was
      incorrect, then the file would be rejected, unless some other error cancelled it
      out. Then for the last three tests, the earlier on the problem was, the more
      blocks would be skipped, but the program would stabilize at some point. In the
      middle, less skips, but would not stabilize as often. In the end, usually would
      not stabilize, but also wouldn't skip as much. Also like in the case of
      hash-files, this meethod would catch the vast majority of faulty rainbow tables
5. Is the entire file read
    - Three different types of rainbow tables were created
        1. There is a formatting error in a hashed password, causing it to be too long
           or too short
        2. The length of the password is incorrect
        3. There is a formatting error in a password, causing it to be too long or too
           short
      Once again, the results are identical. If the program cannot stabilize, then it
      will almost always leave some of the file unread, causing the rainbow table to
      be rejected
6. Does the hash match the password
    - I made files, where the size of the password was correct, but some characters
      were changed. The program could catch this disparity, which led to an error being
      printed out
7. Combinations and special notes
    - Because hash-files and rainbow tables both utilize a block-structure and both
      function similarly to each other, I used similar tests for both. This results in
      most faulty files to be rejected, and the ones that aren't are so similar in
      structure, that they cannot cause harm to the system. The biggest issue these
      outliers cause, is that some blocks become corrupted, meaning that they will
      never be read as a correct password
    - Also, if the very last block has a formatting issue, it will lead to one of three
      possible outcomes
        1. The last block is smaller than it is supposed to be
            - The amount of blocks is correct, but the end of the file is not read
        2. The last block is larger than it is supposed to be
            - The amount of blocks is not correct
        3. The formatting errors cancel each other out
            - The hash or the password is deformed, leading to the hash not matching
              its associated password, leading to an error message




Missing Features
----------------
The program has every core feature that I intended it to have, and I have not
planned on adding anything more




Security Issues and Vulnerabilities
-----------------------------------
There are some security issues, mainly related to logging errors. The errors are
handled correctly, but the specific root cause of the issue can sometimes remain
unclear. The reason for this, is because it is difficult to detect where exactly
the error lies in a binary file. Usually faulty files can be read to the end,
where one of the error detection systems picks up that the file has formatting
issues. I could not figure out how to pinpoint the exact root cause of the issue,
be it a hashed password with an extra byte, or an incorrect password length. All
of these are attributed to "Formatting errors", which they are, but they could be
more specific

Also, now while I am finishing the last chapters of this documentation, I realize
that I should have spent more effort in testing and securing inputs




Suggestions and Improvements
----------------------------
There are a couple of improvements, that I would like to make
1. To completely rewrite set_tests. My schedule is very tight, and I cannot find
   the time to do bigger rewrites of the program
2. In the same vein, there are still some issues regarding legibility of the
   code. I would like to change those
3. Then when it comes to security, I would definitely focus more on logging and
   reporting errors. While I handle errors properly, it can sometimes be unclear
   as to what caused the issue, and where in the file the root cause of the issue
   is. I can't figure out how to pinpoint the error with my current error detection
   system, and I also can't figure out a better system to detect errors, that would
   also allow for more accurate logging.
4. Also, input testing should have been more thorough