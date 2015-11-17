# tab2abc
Bass Tab to ABC music notation converter written with Qt framework

<b>Motivation</b>

There is lot of music availabe on the internet that is notated using so called tab notation. 
Since I am a bass player most of the time I use bass tabs. But they are very hard to read
for me, I have to examine them character for character and finally the piece of music is
learned by heard and I do not need the tab notation any more. That is a time taking, exhausting
and not satisfying process for me.

I go much easier with regular music notation and notes. 

My favourite music notation program is "abcm2ps" (http://moinejf.free.fr/) which makes beautiful postscripts from a plain
ASCII music notation. Absolutely awsome!

Thats why I started to write this little Tool. Since I am working on Linux and Windows machines 
I love the Qt framework, and naturally used it for this project.

<b>User Interface</b>

The user interface is straight forward. Select an input text file with bass tabs. Select an 
output file for the ABC notated result and click start. 

As conversion options you can only choose the metrum at the moment. But there will be much more options in the future.

During conversion a lot of information is printed to a log console, should be helpful when fixing bugs/types in the
bass tab file, (usually there are a lot!)

<i>NOTE: The program is not fully implemented yet, so there will be no output file created at the moment!</i>

<b>Program logic</b>

Since there is no exact syntax definition for bass tab notation, tab2abc uses a simple parsing algorihtm
in combination with the user editing the input file until it can be parsed correctly.

The text input is read line by line. As soon as a line that looks like a string notation is encountered at least
4 more such lines are expected next.
These lines are interpreted as the 4 string from top to bottom (GDAE). If there is a tuning notated for a string, that
tuning is used as default for the next line.

Each string is parsed separately and the combined together to the note line. 

Some plausibility checks are made to ensure the interpreted strings fit together.
