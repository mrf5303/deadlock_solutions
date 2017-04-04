//Matthew Field
//mrf5303
//926691697
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
using namespace std;
mutex m1, m2, m3;

void FirstDeadlock();
void SecondDeadlock();
void MutualExclusion();
void HoldandWait();
void Preemption();
void CircularWait();

int ME = 0, CW = 0, HW = 0, PR = 0;

int main(){
	//mutual exclusion
	thread mutual1(MutualExclusion);
	thread mutual2(MutualExclusion);
	cout << "mutual exlusion started" << endl;
	mutual1.join();
	mutual2.join();
	cout << "mutual exclusion finished" << endl;

	//Hold and Wait
	thread HW1(HoldandWait);
	thread HW2(HoldandWait);
	cout << "Hold and wait threads created" << endl;
	HW1.join();
	HW2.join();
	cout << "Hold and Wait threads joined" << endl;
	
	//non-Preemption
	thread pre1(Preemption);
	thread pre2(Preemption);
	cout << "Non-preemption threads created" << endl;
	pre1.join();
	pre2.join();
	cout << "Non-preemption threads joined" << endl;


	thread circle1(CircularWait);
	thread circle2(CircularWait);
	cout << "Circular threads made" << endl;
	circle1.join();
	circle2.join();
	cout << "Circular threads joined" << endl;

	thread first(FirstDeadlock);
	thread second(SecondDeadlock);
	cout << "Threads for deadlock created" << endl;

	first.join();
	second.join();

	cout << "Threads from deadlock joined" << endl;


	return 0;

}

/*
Deadlock is achieved here by requesting locks in different order
each thread attains one lock and waits for the second that is 
held by the other thread therefore no one progresses.
*/
void FirstDeadlock(){
	m1.lock();
	Sleep(500);
	m2.lock();
	cout << "Both mutexes locked by 1" << endl;
	m1.unlock();
	m2.lock();
}

void SecondDeadlock(){
	m2.lock();
	Sleep(500);
	m1.lock();
	cout << "Both mutexes locked by " << endl;
	m1.unlock();
	m2.lock();
}

/*
Mutual exlusion is prevented here because only one thread
can hold the lock at one time therefore the resource is not
shared by multiple threads at a time.
*/
void MutualExclusion(){
	Sleep(10);
	m1.lock();
	ME++;
	cout << ME << endl;
	m1.unlock();


}

/*
Hold and wait is prevented here because the locks are issued
on an all or none basis because the first lock needs to be attained
before requesting for any other locks.
*/
void HoldandWait(){
	Sleep(10);
	while (true){
		if (m1.try_lock()){
		m2.lock();
		HW++;
		cout << HW << endl;
		m1.unlock();
		m2.unlock();
		break;
		}
	}
}

/*
non-preemption is prevented here because after the first lock is attained
if the second lock is not available, it will give up the resource(lock) it
already has for other processes(threads) to use.
*/
void Preemption(){
	Sleep(10);
	bool done = false;
	while (true){
		if (m1.try_lock()){
			if (m2.try_lock()){
				PR++;
				cout << PR << endl;
				done = true;
				m2.unlock();
			}
			m1.unlock();

		}
		if (done)
			break;
	}
}

/*
circular wait is prevented here by imposing an order to how the locks can be attained.
first lock must be attained before the second lock which is before the third lock.
this way no process can attain say lock 3 without first acquiring locks 1 and 2
so when a processes tries to attain lock 3 it will always be available and not already
in use by another process
*/
void CircularWait(){
	Sleep(10);
	m1.lock();
	m2.lock();
	m3.lock();
	CW++;
	cout << CW << endl;
	m1.unlock();
	m2.unlock();
	m3.unlock();

}