#include <iostream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Patient {
public:
    int id;
    string name;
    int age;
    string department;
    int severity; // 1-5
    bool emergency;

    Patient() {}
    Patient(int i, string n, int a = 0, string d = "", int s = 0, bool e = false) {
        id = i; name = n; age = a; department = d; severity = s; emergency = e;
    }
};
class Node {
public:
    Patient patient;
    Node* next;
    Node(Patient p){ patient=p; next=nullptr; }
};
class PatientQueue {
public:
    Node* front;
    Node* rear;
    PatientQueue(){ front=rear=nullptr; }

    bool isEmpty(){ return front==nullptr; }

    void enqueue(Patient p){
        Node* node = new Node(p);
        if(isEmpty()){ front=rear=node; }
        else if(p.emergency){ node->next = front; front = node; }
        else { rear->next=node; rear=node; }
        cout << "Patient admitted: " << p.name << endl;
    }
    Patient dequeue(){
        if(isEmpty()){ cout<<"Queue empty!\n"; return Patient(-1,""); }
        Node* temp=front;
        Patient p=temp->patient;
        front=front->next;
        if(!front) rear=nullptr;
        delete temp;
        cout<<"Patient discharged: "<<p.name<<endl;
        return p;
    }
    void display(){
        Node* temp=front;
        cout<<"Queue: ";
        while(temp!=nullptr){
            cout<<temp->patient.name<<" ";
            temp=temp->next;
        }
        cout<<endl;
    }
    Node* search(int id){
        Node* temp=front;
        while(temp!=nullptr){
            if(temp->patient.id==id) return temp;
            temp=temp->next;
        }
        return nullptr;
    }
};
class Operation {
public:
    string type;
    Patient patient;
    Operation() {}
    Operation(string t, Patient p){ type=t; patient=p; }
};
class StackNode {
public:
    Operation op;
    StackNode* next;
    StackNode(Operation o){ op=o; next=nullptr; }
};
class Stack {
public:
    StackNode* top;
    Stack(){ top=nullptr; }

    bool isEmpty(){ return top==nullptr; }

    void push(Operation o){ StackNode* n = new StackNode(o); n->next=top; top=n; }

    Operation pop(){
        if(isEmpty()) return Operation("none", Patient(-1,""));
        StackNode* temp = top;
        Operation o=temp->op;
        top=top->next;
        delete temp;
        return o;
    }
};
class BSTNode {
public:
    Patient patient;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Patient p){ patient=p; left=right=nullptr; }
};
class PatientBST {
public:
    BSTNode* root;
    PatientBST(){ root=nullptr; }

    BSTNode* insert(BSTNode* node, Patient p){
        if(!node) return new BSTNode(p);
        if(p.id < node->patient.id) node->left = insert(node->left, p);
        else node->right = insert(node->right, p);
        return node;
    }

    void insert(Patient p){ root = insert(root, p); }

    BSTNode* search(BSTNode* node, int id){
        if(!node || node->patient.id==id) return node;
        if(id<node->patient.id) return search(node->left,id);
        return search(node->right,id);
    }

    BSTNode* search(int id){ return search(root,id); }

    BSTNode* getRoot(){ return root; }
};
class Archive {
public:
    Node* head;
    Archive(){ head=nullptr; }

    void add(Patient p){
        Node* n = new Node(p);
        n->next = head;
        head=n;
        cout<<"Archived: "<<p.name<<endl;
    }

    void display(){
        Node* temp = head;
        cout<<"Archive: ";
        while(temp!=nullptr){ cout<<temp->patient.name<<" "; temp=temp->next; }
        cout<<endl;
    }
};
class PatientSorter {
public:
    static void mergeSort(Patient* arr, int left, int right){
        if(left>=right) return;
        int mid = left + (right-left)/2;
        mergeSort(arr,left,mid);
        mergeSort(arr,mid+1,right);
        merge(arr,left,mid,right);
    }

private:
    static void merge(Patient* arr,int left,int mid,int right){
        int n1 = mid-left+1, n2 = right-mid;
        Patient* L = new Patient[n1]; Patient* R = new Patient[n2];
        for(int i=0;i<n1;i++) L[i]=arr[left+i];
        for(int i=0;i<n2;i++) R[i]=arr[mid+1+i];

        int i=0,j=0,k=left;
        while(i<n1 && j<n2){
        if (L[i].severity > R[j].severity ||
           (L[i].severity == R[j].severity && L[i].department < R[j].department) ||
           (L[i].severity == R[j].severity && L[i].department == R[j].department &&L[i].id < R[j].id))
           { arr[k++] = L[i++];}
        else{ arr[k++] = R[j++];}
        }
        while(i<n1) arr[k++] = L[i++];
        while(j<n2) arr[k++] = R[j++];
        delete[] L; delete[] R;
    }
};
class DischargeManager {
public:
    PatientQueue* queue;
    Archive* archive;
    Stack* undoStack;
    DischargeManager(PatientQueue* q, Archive* a, Stack* undo){ queue=q; archive=a; undoStack=undo; }

    void dischargeById(int id=-1){
        Patient discharged(-1,"");
        if(id==-1) discharged=queue->dequeue();
        else{
            Node* curr = queue->front;
            Node* prev = nullptr;
            while(curr!=nullptr){ if(curr->patient.id==id) break; prev=curr; curr=curr->next; }
            if(!curr){ cout<<"Patient with ID "<<id<<" not found.\n"; return; }
            discharged = curr->patient;
            if(prev) prev->next = curr->next;
            else queue->front = curr->next;
            if(curr==queue->rear) queue->rear=prev;
            delete curr;
            cout<<"Patient discharged: "<<discharged.name<<endl;
        }
        if(discharged.id!=-1){
            archive->add(discharged);
            undoStack->push(Operation("discharge",discharged));
        }
    }
};
class PerformanceAnalyzer {
public:
    void analyze(int N=10000){
        cout<<"\n=== Performance Analysis ("<<N<<" patients) ===\n";
        Patient* arr = new Patient[N];
        Node* head = nullptr;
        PatientBST bst;
        for(int i=0;i<N;i++){
            arr[i] = Patient(i,"Patient",30,"Dept",i%10,false);
            Node* n = new Node(arr[i]);
            n->next = head; head = n;
        }
        auto start = high_resolution_clock::now();
        for(int i=0;i<N;i++) bst.insert(arr[i]);
        auto end = high_resolution_clock::now();
        cout<<"BST insertion: "<<duration_cast<milliseconds>(end-start).count()<<" ms\n";
        int key = N/2;

        start = high_resolution_clock::now();
        for(int i=0;i<N;i++) if(arr[i].id==key) break;
        end = high_resolution_clock::now();
        cout<<"Array linear search: "<<duration_cast<milliseconds>(end-start).count()<<" ms\n";

        start = high_resolution_clock::now();
        Node* temp=head;
        while(temp!=nullptr){ if(temp->patient.id==key) break; temp=temp->next; }
        end = high_resolution_clock::now();
        cout<<"Linked list search: "<<duration_cast<milliseconds>(end-start).count()<<" ms\n";

        start = high_resolution_clock::now();
        bst.search(key);
        end = high_resolution_clock::now();
        cout<<"BST search: "<<duration_cast<milliseconds>(end-start).count()<<" ms\n";

        start = high_resolution_clock::now();
        PatientSorter::mergeSort(arr,0,N-1);
        end = high_resolution_clock::now();
        cout<<"Merge sort by severity+dept: "<<duration_cast<milliseconds>(end-start).count()<<" ms\n";

        delete[] arr;
        cout<<"=== End of Performance Analysis ===\n";
    }
};
int main() {
    PatientQueue queue;
    PatientBST bst;
    Archive archive;
    Stack undoStack, redoStack;
    DischargeManager dis(&queue, &archive, &undoStack);

    int choice;
    do {
        cout << "===== Hospital Management Menu =====\n";
        cout << "1. Admit Patient\n2. Emergency Admit\n3. Discharge Patient\n";
        cout << "4. Search Patient by ID\n5. Undo\n6. Redo\n";
        cout << "7. Display Queue\n8. Display Archive\n9. Sort Patients\n10. Performance Analysis\n11. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        if(cin.fail()){
            cin.clear();
        cin.ignore(1000,'\n');
        continue;
        }
        if(choice == 1 || choice == 2) {
            int id, age, severity;
            string name, dept;
            cout << "Enter Name: "; cin.ignore(); getline(cin,name);
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Age: "; cin >> age;
            cout << "Enter Department: "; cin.ignore(); getline(cin,dept);
            cout << "Enter Severity (1-5): "; cin >> severity;
            Patient p(id, name, age, dept, severity, choice == 2);

            queue.enqueue(p);
            bst.insert(p);
            undoStack.push(Operation("admit", p));
        }
        else if(choice == 3) {
            int id;
            cout << "Enter ID to discharge (or -1 for front): "; cin >> id;
            if(id == -1) dis.dischargeById();
            else dis.dischargeById(id);
        }
        else if(choice == 4) {
            int id;
            cout << "Enter ID to search: "; cin >> id;
            BSTNode* f = bst.search(id);
            if(f) cout << "Found: " << f->patient.name
                       << " | Dept: " << f->patient.department
                       << " | Severity: " << f->patient.severity << endl;
            else cout << "Patient not found\n";
        }
        else if(choice == 5){ // Undo
            Operation o = undoStack.pop();
            if(o.type != "none"){
                redoStack.push(o);
                cout << "Undo: " << o.type << " " << o.patient.name << endl;
                if(o.type=="admit"){
                    Node* curr = queue.front; Node* prev=nullptr;
                    while(curr && curr->patient.id != o.patient.id){ prev=curr; curr=curr->next; }
                    if(curr){
                        if(prev) prev->next=curr->next;
                        else queue.front=curr->next;
                        if(curr==queue.rear) queue.rear=prev;
                        delete curr;
                    }
                }
                else if(o.type=="discharge"){
                    Node* curr=archive.head; Node* prev=nullptr;
                    while(curr && curr->patient.id!=o.patient.id){ prev=curr; curr=curr->next; }
                    if(curr){
                        if(prev) prev->next=curr->next;
                        else archive.head=curr->next;
                        delete curr;
                    }
                    queue.enqueue(o.patient);
                    bst.insert(o.patient);
                }
            }
        }
        else if(choice == 6){
            Operation o = redoStack.pop();
            if(o.type != "none"){
                undoStack.push(o);
                cout << "Redo: " << o.type << " " << o.patient.name << endl;
                if(o.type=="admit") queue.enqueue(o.patient);
                else if(o.type=="discharge") dis.dischargeById(o.patient.id);
            }
        }
        else if(choice == 7) queue.display();
        else if(choice == 8) archive.display();
        else if(choice == 9){
            Node* temp = queue.front;
            int n=0;
            while(temp){ n++; temp=temp->next; }
            if(n == 0){ cout<<"No patients to sort.\n"; continue; }

            Patient* arr = new Patient[n];
            temp = queue.front;
            for(int i=0;i<n;i++){ arr[i]=temp->patient; temp=temp->next; }
            PatientSorter::mergeSort(arr,0,n-1);
            cout<<"Patients sorted by Severity (desc) & Department (asc):\n";
            for(int i=0;i<n;i++)
                cout<<arr[i].id<<" | "<<arr[i].name<<" | "<<arr[i].department<<" | Severity: "<<arr[i].severity<<endl;
            delete[] arr;
        }
        else if(choice == 10){
            PerformanceAnalyzer analyzer;
            analyzer.analyze();
        }
        else if(choice == 11) break;
        else cout << "Invalid choice!\n";
    } while(choice != 11);
    return 0;
}
