#  Customer Queue Management System Based on AVL Tree and Queue Data Structures

This project is a C-based application designed to manage customer data and service queues in environments such as banks or public service offices. The system utilizes two primary data structures—AVL Tree and Queue—to manage customer data efficiently and ensure proper service order based on arrival and customer status priority.

**Key Features:**

**1. Add New Customer:**
- Users can add new customers by providing their name and status ("VIP" or "REGULAR").
- The customer data is stored in an AVL Tree to enable fast name-based searches.
- The customer is also added to a Queue for sequential service based on their arrival.

**2. View Customer Queue:**
- The list of customers currently in the queue is displayed in a table format showing their name and status.
- The queue is processed using the First In First Out (FIFO) method.

**3. Process Customer Queue:**
- The system serves customers one at a time, removes their data from the queue, and updates the data file accordingly.
- After processing, details of the served customer are displayed.

**4. Search for Customer:**
- Users can search for a customer by name using the AVL Tree, which ensures logarithmic search time.
- If found, the customer's name and status are displayed.

**5. Data Persistence:**
- Customer data is saved to a file (nasabah_data.txt) to allow reloading when the program is reopened.
- The loadFromFile function ensures previously saved data is reloaded into both the AVL Tree and the Queue.
