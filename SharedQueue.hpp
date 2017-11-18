#ifndef SharedQueue_h
#define SharedQueue_h

#include <mutex>

template <typename T>
class SharedQueue {
public:
  SharedQueue();
  ~SharedQueue();
  
    /*
     These declarations mean that we do NOT get the automatically
     defined copy/copy-assign functions.  If you try to call one
     by copying the queue, you'll get a compiler error.
     
     This is a common technique for things that are uncopyable (like std::thread and std::mutex, for example).
     
     We really DO want a destructor, so cheating at the
     rule of 3 here makes sense here.
     */
    SharedQueue(const SharedQueue<T>&) = delete;
    SharedQueue<T>& operator=(const SharedQueue<T>&) = delete;

    
  //Return true if the queue is empty
  bool IsEmpty() const;
  
  //Enqueue the next item at the tail of the queue.
  void Add(T value);
  
  //Dequeue the next queue element and store it in "item" variable.  The function returns false if the queue is empty and no item can be retrieved.
  bool Remove(T &item);
  void Print();
private:
  struct QueueItem {
	QueueItem(T val, QueueItem* n) :item(val), next(n) {}
    T item;
    QueueItem *next;
  };

  QueueItem *head;
  QueueItem *tail;
  std::mutex mtx;
  //Fill in the The private data members.
};

//Fill in the function definitions
template <typename T>
SharedQueue<T>::SharedQueue() {
	head = nullptr;
	tail = nullptr;
};

template <typename T>
SharedQueue<T>::~SharedQueue() {
	QueueItem *temp = head;
	while (head != NULL) {
		temp = temp->next;
		delete head;
		head = temp;
	}
	head = nullptr;
	tail = nullptr;
};

template <typename T>
bool SharedQueue<T>::IsEmpty() const {
	return (head == nullptr);
};

template <typename T>
void SharedQueue<T>::Add(T value) {
	QueueItem *temp = new QueueItem(value, nullptr);
	mtx.lock();
	if (IsEmpty()) {
		head = temp;
		tail = temp;
	}
	else {
		tail->next = temp;
		tail = temp;
	}
	mtx.unlock();
};

template <typename T>
bool SharedQueue<T>::Remove(T &item) {
	mtx.lock();
	if (tail == nullptr) {
		mtx.unlock();
		return false;
	}

	if (head == tail)
	{
		item = head->item;
		delete head;
		head = nullptr;
		tail = nullptr;
	}
	else {
		item = head->item;
		QueueItem *temp = head;
		head = head->next;
		delete temp;
	}
	
	mtx.unlock();
	return true;
};

template <typename T>
void SharedQueue<T>::Print() {
	mtx.lock();
	if (IsEmpty())
		std::cout << "\n";
	else
	{
		for (QueueItem *temp = head; temp != nullptr; temp + 1)
		{
			std::cout << "(" << this->item << ")";
		}
	}
	std::cout << "/n";
	mtx.unlock();
};


#endif // !SharedQueue
