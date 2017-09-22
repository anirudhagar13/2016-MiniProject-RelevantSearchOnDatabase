dRELEVANT SEARCHES ON STRUCTURED DATA
====================================

Provide fast, relevant results to your queries on structured data.
------------------------------------

- The software provides a generic search for structured data(a database, a csv file etc) which does not require knowledge of any database languages(such as MySQL).
- However after the query has been entered, comes the most interesting part which is the ranked nature of the tuples returned in the result. Thus based on the query, the user gets a ranked list of tuples according to a relevancy score attached to each tuple.
- The relevancy score calculations are based on the characteristics (frequency, locality of occurence) of the query terms in the database and the a workload.
- The workload is a list of the queries made by users and keeps on growing( and improving the correctness of the ranked list of tuples ) as the software is used.
- What makes this project a bit different is that it provides ranked based results and does relevancy calculations on structured data, ideas which are mostly employed on unstructured data sets( such as the web pages on the internet ).

How to Run:
------------------------------------

- Get g++ in your system - 'https://cs.calvin.edu/courses/cs/112/resources/installingEclipse/cygwin/'
- Traverse into the repo folder and run 'run.sh' file
- This will create a.out OR a.exe (depending upon ur system)
- Place the .csv file in data_files folder
- Run the executable and enter the name of the above file when prompted
- The system will perform indexing so wait for two minutes
- After finished, system would be ready to accept any form of free language query and will return tuples from csv file in decreasing order of their relevance.
- The query can be in any order and case, but make sure that individual words are spelled correctly.
- In data_files folder, workload file for your csv will automatically get created and populated, to assist in relevancy search.
- With time workload file will increase and relevancy of results will become more accurate, as per workoad trend.