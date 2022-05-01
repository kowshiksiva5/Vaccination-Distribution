# Vaccination-Distribution
Implementation
In this I am using one mutex per table i.e n mutex totally.
in this i will be using k threads each for one student and n threads each for companies and n threads each for vaccinating zone
In my code every where stud_id is for student struct ,vaccine_input is for table and company_input for companies.
Filling vaccine by companies by r.
i will call n companies through m threads and in  thread i will iterate over n vaccinating zones and check weather the ith mutex  is locked or not i.e try_lock
if it locked implies working in some other thread if not i will lock this thread with mutex
and check for capacity in it if it is zero i will fill it through companies.
I will do above function many times in my code to update container value accordingly.
Student coming is done randomly i.e i will take a random number in while loop and that many students will come
and i will use a valid to know weather they are in queue or not based on valid value only i will print weather a student is in queue or not is
known to us.
In assinging j(random number one per vaccinating zone) students to n vaccinating zones again i am using mutex per vaccinating zones
i will create k threads each for a student and check weather he is in line i.e valid value if valid then i will iterate for n tables and check weather
they are allowed students here and use mutex try lock as above and use  synchronization because of that though first student is allocated vaccinating zone 1
2nd maynot be alloted table 1 so syncronization is there.
of students are there i will check for a vaccinating zone and its max value now allowed and based on that  i will assign student to that.
For quiting from code based on student valid value if no students are there in line code exits.
During the production of the vaccine by the company,the probability is randomly stored.During the vaccination of a student the student is checked whether the required  antibodies are acquired if not ressigned.
