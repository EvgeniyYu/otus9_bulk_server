# otus9_bulk_server
async bulk server

Asynchronous server using Boost.Asio  
  
Example:  
Run server: bulk_server 9000 3  
Send data to server: seq 0 9 | nc localhost 9000  
The result will be displayed in console:  
bulk: 0, 1, 2   
bulk: 3, 4, 5   
bulk: 6, 7, 8   
bulk: 9  
and 4 files will be created with these records.   
