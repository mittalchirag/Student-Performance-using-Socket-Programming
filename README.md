# Student-Performance-using-Socket-Programming

The question poses a scenario of calculating the overall performance of a student in all the subjects (of his/her currently enrolled semester). A student is considered PASS or FAIL based on his marks in SEE (Semester End Examination) and his/her eligibility to write SEE according to conditions based on Component-1 marks and Attendance. The problem suggests to find the solution using Concurrent Client-Server architecture.  Below are the requirements for the system:
 * Client and Server should perform IPC through Network using Sockets
 * Client should send the student’s registration number (given by user) to the Server
 * Each subject is hosted on a separate server, that is, there are multiple servers and each server provide the result of that subject.
 * Each Server should fetch the given student’s Component-1 (TermTest-1, TermTest-2, TermTest-3, Assignment-1, Assignment-2) marks, Attendance and SEE marks from the database.
 * Each Server should return the component-1 marks, attendance and SEE marks to the client.
 * Client should compute the eligibility of student and tell whether he/she is PASS or FAIL in that subject from the received data based on following conditions:
    1.	A student is said to be eligible to write SEE if below two conditions are true else, he/she is non-eligible and is considered to be FAILED in that subject
        * The total of component-1 i.e.  best two of (TermTest-1, TermTest-2 & TermTest-3 marks), Assignment-1 and Assignment-2 marks should be greater than or equal to 40% score
        * Attendance should be greater than or equal to 80%
    2.	A student must be able to score at least 40% in SEE for passing that subject.
  Therefore, to PASS in a subject student must have 1) 40% score in Component-1, 2) Attendance should be greater than 80% and 3) 40% score in SEE
  * Client should be able to check his/her performance in all subjects by querying his registration number.
  * Multiple clients should be able to connect to the servers simultaneously (i.e. concurrency).
  
**To solve the problem, a Master-Slave (or Parent-Child) architecture is used as shown below in the figure:**
There are 4 different servers (assuming the student is enrolled in CSE 6th Semester). These servers are abstracted from client with a master (or parent) server. The master server connects to all other servers upon getting the registration number from client. These subject-wise servers connect to the DB and fetch the data from corresponding tables. The benefit of this kind of architecture is that the client only has to remember the IP and Port Number of one server. This architecture also improves speed and is highly scalable.
![architecture](https://github.com/mittalchirag/Student-Performance-using-Socket-Programming/blob/master/b2.png)

To solve the problem, TCP sockets are used for connection-oriented communication. Below is the flow diagram of system.
**Flowchart for Client-Server IPC:**
![flow-chart](https://github.com/mittalchirag/Student-Performance-using-Socket-Programming/blob/master/nps.png)
 
