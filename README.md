# Barbar-Shop-Problem
### The Barber Shop Problem

The Barber Shop Problem is a classic synchronization problem in computer science, particularly in the study of operating systems and concurrent programming. It was originally formulated by Edsger Dijkstra and serves as an excellent example for understanding semaphores, mutexes, and other synchronization mechanisms.

#### Problem Description

**Scenario:**
- A barber shop has one barber, one barber chair, and a waiting room with a finite number of chairs (say N chairs).
- When there are no customers, the barber sits in the barber chair and sleeps.
- When a customer arrives, they must check if there are any free chairs in the waiting room.
  - If there are no free chairs, the customer leaves the shop.
  - If there is at least one free chair, the customer sits in one of the free chairs and waits.
- If the barber is asleep, the customer wakes the barber.
- Once the barber finishes cutting a customer’s hair, the customer leaves, and the barber checks if there are any waiting customers.
  - If there are waiting customers, the barber wakes the next customer and cuts their hair.
  - If there are no waiting customers, the barber goes back to sleep.

#### Objectives

1. **Synchronization**: Ensure proper synchronization between the barber and the customers to avoid race conditions and deadlocks.
2. **Resource Management**: Manage the limited number of waiting chairs effectively.
3. **Concurrency**: Handle multiple customers arriving at the shop concurrently.

#### Solution Outline

To solve the Barber Shop Problem, we need to use synchronization mechanisms like semaphores and mutexes. Here’s a high-level outline of the solution:

1. **Semaphores and Mutexes**:
   - Use a semaphore to represent the number of available chairs in the waiting room.
   - Use a mutex to protect the critical section where the barber and customers interact.

2. **Customer Thread**:
   - Arrive at the shop.
   - Check if there are free chairs.
     - If no chairs are free, leave the shop.
     - If a chair is free, take a seat and wait.
   - If the barber is asleep, wake the barber.
   - Wait until the barber is ready to cut hair.
   - Get the haircut and leave the shop.

3. **Barber Thread**:
   - While the shop is open:
     - If there are no waiting customers, go to sleep.
     - If there are waiting customers, wake the next customer.
     - Cut the customer's hair.
     - Repeat.

#### Pseudo Code

Here is a simplified pseudo code to illustrate the synchronization logic:

```python
import threading
import time
import random

# Number of chairs in the waiting room
NUM_CHAIRS = 5

# Semaphores
waiting_customers = threading.Semaphore(0)  # No waiting customers initially
mutex = threading.Lock()  # Mutex for critical sections

# Number of available chairs
available_chairs = NUM_CHAIRS

def customer():
    global available_chairs

    # Arrive at the shop
    mutex.acquire()
    if available_chairs > 0:
        available_chairs -= 1  # Take a seat in the waiting room
        print(f"Customer sits. Available chairs: {available_chairs}")
        mutex.release()

        waiting_customers.release()  # Signal that a customer is waiting

        # Wait for the barber to be ready
        mutex.acquire()
        available_chairs += 1  # Leave the waiting room chair
        mutex.release()

        # Get haircut (critical section ends)
        get_haircut()
    else:
        print("Customer leaves (no available chairs).")
        mutex.release()

def barber():
    while True:
        waiting_customers.acquire()  # Wait for a customer
        mutex.acquire()

        # Cut hair (critical section)
        print("Barber is cutting hair.")
        cut_hair()

        mutex.release()

def get_haircut():
    print("Customer is getting a haircut.")
    time.sleep(random.uniform(1, 3))  # Simulate time taken for a haircut

def cut_hair():
    print("Barber is giving a haircut.")
    time.sleep(random.uniform(1, 3))  # Simulate time taken to cut hair

# Create threads
barber_thread = threading.Thread(target=barber)
barber_thread.start()

customer_threads = []
for _ in range(10):
    t = threading.Thread(target=customer)
    customer_threads.append(t)
    t.start()
    time.sleep(random.uniform(0.5, 2))  # Random arrival time for customers

for t in customer_threads:
    t.join()
```

### Key Points

- **Semaphore (`waiting_customers`)**: Keeps track of the number of customers waiting.
- **Mutex (`mutex`)**: Ensures that the critical sections (where the number of available chairs is checked and modified) are executed by only one thread at a time.
- **Available Chairs**: A shared resource indicating the number of chairs available in the waiting room.

This solution ensures that the barber and customers are properly synchronized, avoiding race conditions and ensuring that customers either get a haircut or leave if there are no available chairs.
