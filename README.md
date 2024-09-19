# graphite-scheduler
Graphite deliverable project for meeting scheduling

This program runs in C++ and takes input from the stdin stream.
The input order is specified as follows.
First member details are inputted:
The first line of input contains a single integer N which is the number of members in the team
The next 4N lines contain the details of each member
The line 4i-3 for 1<=i<=N is the ith member's name as a single-word string
The line 4i-2 for 1<=i<=N is the ith member's U.S. state as a two-letter abbreviated string
The line 4i-1 for 1<=i<=N is the ith member's available times. The line starts with an integer M, the number of distinct intervals. The next 2M numbers on the line represent the intervals as pairs of military times, such that number at position 2k and 2k+1 form an interval for all 1<=k<=M on the line.
The line 4i for 1<=i<=N is the ith member's seniority represented by a 1 for a junior or a 2 for a senior.

Then, on a question-prompt system, the user enters specifications for a meeting query, including the length of the meeting in minutes, the hard start and end times in military Eastern Time, the minimum required junior and senior members or 0 if not applicable, and a scoring matrix system.

The output will be all possible meeting times and their junior and senior attendees, based on the highest scoring meeting times.



There is a python file in the repository designed to generate randomised test data. The number N of participants can be changed as well as the range of number of distinct time intervals allowed. The python file will generate the member details in input format and they can be pasted to the stdin of the main.cpp file. Then the user must answer the meeting query specifications.
